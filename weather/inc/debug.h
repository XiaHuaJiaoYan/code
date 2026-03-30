/**
 * @file debug.h
 * @brief µ÷ÊÔÄ£¿é
 * @version  1.0
 * @author ÕÅ½Ý
 * @date 2007-01-22
 */
#ifndef _DEBUG_H_
#define _DEBUG_H_

#define _CRT_SECURE_NO_WARNINGS
#define DEBUG_FILE "/appfs/debug.txt"
#define DEBUG_REBOOT_FILE "/appfs/count.txt"

#define DEBUG_FILE_BACK "/appfs/debug_back.txt"
#define DEBUG_FILE_MAXSIZE 2 // Mb

#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define DebugExplain(type, fmt, ...)                                                   \
	do                                                                                 \
	{                                                                                  \
		FILE *fp = fopen(DEBUG_FILE, "a");                                             \
		fprintf(fp, "[%s ]%s %s(Line %d): ", #type, __FILE__, __FUNCTION__, __LINE__); \
		fprintf(fp, fmt, ##__VA_ARGS__);                                               \
		fclose(fp);                                                                    \
	} while (0)

enum E_DEBUG_EXPLAIN_TYPE
{
	DEBUG_INFO = 0,
	DEBUG_ERROR,
	DEBUG_WARNING,
	DEBUG_SUCCESS,
};

enum E_DEBUG_MODE
{
	DEBUG_MODE_FILE = 0, //
	DEBUG_MODE_CONSOLE,	 //
	DEBUG_MODE_USB,
};

void DebugIncCount();

// base api
void setDebugMode(int eMode);

void DebugPrintf(char *fmt, ...);

void DebugMemory(void *data, int len);

int DebugCmd(char *szCmd);

int DebugFileSizeCheck();

int DebugRamInfo();

int DebugToScreen(char *fmt, ...);

unsigned long GetFreeRam();

#endif
