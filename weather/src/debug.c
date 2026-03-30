#include "debug.h"
#include <unistd.h>
#include <sys/sysinfo.h>
#include "tfx_debug.h"
#include "tfx_tool.h"
#include "tfx_keyboard.h"
#include "tfx_scr.h"
#include "tfx_clock.h"
#include "tfx_wifi.h"

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define SCR_BUF_MAX_ROW 8
#define SCR_BUF_MAX_COL 1024

static int iDebugMode = DEBUG_MODE_FILE;



void DebugIncCount() 
{ 
	int iCount = 0;
	FILE * pfCount = fopen(DEBUG_REBOOT_FILE, "a+");  
		
	fseek(pfCount, 0, SEEK_SET);
	fscanf(pfCount, "%d", &iCount);
	fclose(pfCount); 

	iCount++;

	pfCount = fopen(DEBUG_REBOOT_FILE, "w");
	fseek(pfCount, 0, SEEK_SET);	
	fprintf(pfCount,"%d",iCount);	
		
	fclose(pfCount); 
		
	TfxScrClearAll();
    TfxScrDisplayStrInline(0, ALIGN_LEFT, "%d times", iCount);
   	TfxScrUpdate();
    TfxMsDelay(1200);
}

void DebugClearCount() 
{ 
	remove(DEBUG_REBOOT_FILE);
}

void setDebugMode(int eMode)
{
	iDebugMode = eMode;
}

void DebugPrintf(char * fmt, ...)
{
    if(fmt == NULL){};
#if 1

	char szTime[25] = { 0 };
	FILE* fp = NULL;
	va_list arg;
	char szSprintBuf[256] = {0};  
	
	if (DEBUG_MODE_FILE == iDebugMode)
	{
	    DebugFileSizeCheck();
	    
		fp = fopen(DEBUG_FILE, "a+");
		if (fp == NULL)
		{
			return;
		}

		TfxGetCurrentDatetime(szTime);
		fprintf(fp, "[%s] ", szTime);

		va_start(arg, fmt);
		vfprintf(fp, fmt, arg);

		va_end(arg);
		
		fclose(fp);
		
		sync();
	}
	
	else if (DEBUG_MODE_CONSOLE == iDebugMode)
	{
		va_start(arg, fmt);
		vsprintf(szSprintBuf, fmt, arg);
		printf(szSprintBuf);
		va_end(arg);
	}
#endif
}


void DebugMemory(void * data, int iLen)
{
	FILE* pFile = NULL;
	int j = 0;
	int i = 0;

	if (DEBUG_MODE_FILE == iDebugMode)
	{
	    DebugFileSizeCheck();
	    
		pFile = fopen(DEBUG_FILE, "a");
		if (pFile == NULL)
		{
			return;
		}

		//fprintf(pFile, "\nIn Memory: %#x->[\n", (unsigned int*)data);
		fprintf(pFile, "\n[\n");

		for (; i < iLen; i++)
		{
			fprintf(pFile, " %02X ", ((unsigned char*)data)[i]);

			if ((i + 1) % 10 == 0 || i == iLen - 1)
			{
				fprintf(pFile, "%s", "    | ");
				for (; j <= i; j++)
				{
					if (((char*)data)[j] == '\0')
					{
						fprintf(pFile, " %2c ", '.');
					}
					else if (((char*)data)[j] >= 127 || ((char*)data)[j] <= 31)
					{
						fprintf(pFile, " %2c ", '?');
					}
					else
					{
						fprintf(pFile, " %2c ", ((char*)data)[j]);
					}
				}
				fprintf(pFile, "%s", "\n");
			}
		}
		fprintf(pFile, "] len:%d\n", iLen);
		fclose(pFile);
	}
	return;
}

int DebugCmd(char * szCmd)
{
    if(szCmd == NULL){};
#if 0
	FILE *pPipe = NULL;
	FILE *pFile = NULL;
	char szBuff[256] = { 0 };

	pPipe = popen(szCmd, "r");
	if (NULL == pPipe)
	{
		TfxDebug("popen failed");
		return -1;
	}

	if (DEBUG_MODE_FILE == iDebugMode)
	{
		pFile = fopen(DEBUG_FILE, "a+");
		if (NULL == pFile)
		{
		    pclose(pPipe);
			return -1;
		}
		
		DebugFileSizeCheck();

		while (fread(szBuff, 1, sizeof(szBuff), pPipe) > 0)
		{
			fwrite(szBuff, 1, sizeof(szBuff), pFile);
		}
		
		fclose(pFile);
		pclose(pPipe);
		return 0;
	}

	else if (DEBUG_MODE_CONSOLE == iDebugMode)
	{
		printf("CMD: %s\n", szCmd);
		while (fread(szBuff, 1, sizeof(szBuff), pPipe) > 0)
		{
			printf("%s ", szBuff);
		}
		printf("\n ");

		pclose(pPipe);
		return 0;
	}
#endif
	
	return 0;
}

int DebugSystemCmd(char * szCmd)
{
	
	char szTime[22] = { 0 }; 
	char *szBuffer = NULL;
    FILE* fp = NULL;

    DebugFileSizeCheck();
	fp = fopen(DEBUG_FILE, "a");
	if (fp == NULL)
	{
		return 0;
	}
	
	TfxGetCurrentDatetime(szTime);
	fprintf(fp, "[%s] DebugSystemCmd:%s\n", szTime, szCmd);
	fclose(fp);
	
	szBuffer = (char*)calloc(strlen(szCmd) + strlen(DEBUG_FILE) + 6, sizeof(char));
	sprintf(szBuffer, "%s >> %s", szCmd, DEBUG_FILE);
	system(szBuffer);
	free(szBuffer);
	return 1;
}


int DebugFileSizeCheck()
{
	long nFileSize = 0;
	FILE* pFile = fopen(DEBUG_FILE, "r+");
	if (pFile == NULL)
	{
		return 0;
	}
	fseek(pFile, 0, SEEK_END);
	nFileSize = ftell(pFile);
    fclose(pFile);
    
	if (nFileSize > DEBUG_FILE_MAXSIZE * 1024 *1024)
	{
		remove(DEBUG_FILE_BACK);
		rename(DEBUG_FILE, DEBUG_FILE_BACK);
		return 1;
	}
	return 1;
}

int DebugRamInfo()
{
#if 0
	struct sysinfo s_info;
	static long lLastfree = 0;

	if (sysinfo(&s_info) == 0)
	{
		DebugPrintf("totalram: %d bytes, freeram: %d bytes, used:%d, Gap(%d) \n",
			s_info.totalram, s_info.freeram, s_info.totalram - s_info.freeram, lLastfree - s_info.freeram);
	
		lLastfree = s_info.freeram;
	}
	
#endif
	return 0;

}

unsigned long GetFreeRam()
{
	struct sysinfo s_info;
	unsigned long iMin = 0;
    int i = 0;
    
	for(i = 0; i<100 ; i++)
	{
	    if (sysinfo(&s_info) == 0)
	    {
	        if(iMin > s_info.freeram || iMin == 0)
	        {
	            iMin = s_info.freeram;
	        }
	    }
	}
	
	return iMin;
}


//丢弃:存在打开文件异常，用改DebugCmd更安�?
/*
int DebugPorcInfo()
{

	DIR *pstDir = NULL;
	FILE *pFile = NULL;
	char szFile[1024] = {0};
	char szBuff[2048] = {0};
	struct dirent *pstDirent = NULL;
	int iNum = 0;
	
	// Open /proc directory.
	pstDir = opendir("/proc");
	if (pstDir == NULL) {
		DebugPrintf("opendir failed");
		return -1;
	}

	DebugPrintf("/proc \tNo \tpid \tinfo\n");
	while(1)
	{
		pstDirent = readdir(pstDir);
		if(pstDirent == NULL)
		{	
			break;
		}
	    
		if( strspn(pstDirent->d_name, "0123456789") == strlen(pstDirent->d_name) && pstDirent->d_type == DT_DIR)
		{
			iNum++;
			sprintf(szFile, "/proc/%s/status", pstDirent->d_name);
			pFile = fopen(szFile,"r");
			if(pFile)
			{
			    DebugPrintf("proc file = %s\n",szFile);
				if( fgets(szBuff, sizeof(szBuff)/sizeof(char)-1, pFile) )
				{
					//sscanf(szBuff, "%*s %s", szName);

					DebugPrintf("proc file buf = %s\n",szBuff);
					
					DebugPrintf("\t%d \t%s \t%s ", iNum, pstDirent->d_name,szBuff);
				}
				else
				{
					DebugPrintf("\t%d \t%s \t--- \n", iNum, pstDirent->d_name);
				}
			}	
			fclose(pFile);
		}
	}
	
	closedir(pstDir);

	return 0;
}*/

