#ifndef _vKEYBOARD_H_
#define _vKEYBOARD_H_

#define KEY_ENT 20

void KeyBoardDraw(void);
int KeyBoardInput(char *pInputBuffer, const int numInputBufferSize, int *pInputLen);
void KeyBoardInputFeedback(int numCurrentKeyIndex);
int KeyBoardGetCode(char *pInputBuffer, const int numInputBufferSize, int *pInputLen);

#endif
