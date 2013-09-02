/*
 * TextWindow.h
 *
 *  Created on: Mar 21, 2012
 *      Author: waltergress
 */

#ifndef TEXTWINDOW_H_
#define TEXTWINDOW_H_

#include <string>
#include "shared/fsagaconstants.h"





void renderBufferedText();
void textAdvance();

//void drawTextToBuffer(string *inputText, int nlines, int npages);

void drawTextToBuffer(char *inputText[], int nlines, int npages);

#endif /* TEXTWINDOW_H_ */
