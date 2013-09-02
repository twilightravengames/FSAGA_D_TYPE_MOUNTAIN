/*
 * TextWindow.cpp
 *
 *  Created on: Mar 21, 2012
 *      Author: waltergress
 */
#include <string>
#include <iostream>
#include "allegroincludes.h"


#include "TextWindow.h"

#include "shared/fsagaconstants.h"
using namespace std;

extern ALLEGRO_FONT *arial_rounded_convo;
extern ALLEGRO_FONT *arial_rounded;

extern bool redraw;


///text strings buffer
char *textstring[ARRAY_LENGTH_NUM_LINES_SPOKEN];



bool textIsOnScreen = false;

int num_pages;

int current_page;

int num_lines;


//extern int scroll_x;
//extern int scroll_y;


void drawTextToBuffer(char *inputText[], int nlines, int npages)
{

	for (int lines = 0; lines < nlines; lines++ ) {
			textstring[lines] = inputText[lines];
	}
	num_lines = nlines;
	num_pages = npages;

}


void renderBufferedText() {
	//cerr << "Inside render buffered text" << endl;
	//al_draw_textf(arial_rounded, BLUE_COLOR, 250, 250, ALLEGRO_ALIGN_LEFT, "TESTING");
	//al_draw_textf(arial_rounded_convo, BLUE_COLOR, 20, 20, ALLEGRO_ALIGN_LEFT, "Allegro Based FSAGA-D Type Game Editor Alpha %d", 1.0);

	//al_draw_textf(arial_rounded_convo, BLUE_COLOR, 100, 100, ALLEGRO_ALIGN_LEFT, "SCROLL X VALUE %d", scroll_x);
	//al_draw_textf(arial_rounded_convo, BLUE_COLOR, 100, 140, ALLEGRO_ALIGN_LEFT, "SCROLL Y VALUE %d", scroll_y);

	for (int i = 0; i < num_lines; i++) {
		al_draw_textf(arial_rounded_convo, WHITE_COLOR, 45, 20*(i+1)+10, ALLEGRO_ALIGN_LEFT, textstring[i]);
	}

	redraw = true;
}

void textAdvance() {

	//user pressed enter
	//if multi-page convo, advance to next page
	//otherwise end convo

	//cerr << "Running text advance" << endl;

	if(textIsOnScreen == false)
	{
		cerr << "Toggling text advance" << endl;
		textIsOnScreen = true;
	}

}
