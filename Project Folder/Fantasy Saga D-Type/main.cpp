//*********************GAME MAIN********************//




#include <stdio.h>
#include <iostream>
#include "allegroincludes.h"
#include "shared/fsagaconstants.h"
#include "main_prototypes.h"	  //file used to store prototypes for all functions in main
#include "shared/mapfilestructures.h"
#include "shared/crossexecutableprototypes.h" //file used for across-project functions
#include "shared/extern_bitmaps.h"
#include "TextWindow.h"

#define GAME_POSITION_ON 1      //turn on character coordinate display
#define GAME_SCROLL_COORDINATES_ON 1 //turn on scroll amount dsiplay

//background box for conversations interface
extern ALLEGRO_BITMAP *convo_box;

using namespace std;


/*********MAP DATA
 *
 *
 *
 *
 */

extern int backgroundbitmap[100];			//alpha blended bitmaps. Used for backgrouds (e.g., clouds behind a floating island, or trees or hills).
extern int bottombmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //bottom layer bitmap, always rendered under the player
extern int midbmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //mid layer bmp, squares that overlap the player's body (same level as the player)
extern int mid2bmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //another set of mid layer bmps, immediately rendered after the first. they
														//draw wall "patch" bitmaps, things that go over top of walls like pillars, torches, shields, or stairs..
extern int hibmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT];
extern int topbmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //top layer bmp, giant bitmap like clouds in lufia2
extern int tagmap[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //locations of tags. contains ids that reference tag type data structure
extern int collision[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //collision table for scene
extern int blendbmp[100][100]; //blend bmp table Note: Not implemeneted yet - smaller alpha blended bitmaps overtop the scene, will be used for special effects bitmaps


/*******************************************/

//GAMEPLAY DATA

//tiles


/*

//tag for world map
tag_o tags_o[20]; //overworld tag types

//tag and colision grid for local map
int collision_o[100][100]; //collision table for scene

//overworld map type Note: note sure how this is used
int mapt_o;

//local map type Note: note sure how this is used
int mapt;

//won battle? Note: note sure how this is used
int win;

*/



//reference to the game sprite's array and the main character sprite variable
extern sprite mainchar;
extern sprite sprites[MAX_NUMBER_NPC_SPRITES];	//sprites that appear in game maps 

extern tag tags[MAX_NUMBER_TAG_TYPES]; //tag types

extern int collision[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //collision table for scene

/****************************************/

//reference the display

extern ALLEGRO_DISPLAY *display;

//define the current level file structure


extern FILE *log;


//reference the fonts

//for fonts
extern ALLEGRO_FONT *arial_rounded;
extern ALLEGRO_FONT *arial_rounded_convo;

extern bool textIsOnScreen;

bool text_screen = false;


//map buffer extern
extern ALLEGRO_BITMAP *map_buffer;

//define the x,y coordinates of the origin in the map_buffer from the scrolling operation
int scroll_x;
int scroll_y;


//max of these values are their respective max map sizes
int top_anim_x_roller = 0;
int top_anim_y_roller = 0;



//a variable for text box animation
int text_box_curr_size = 0;

//note: i'm not sure if i still need this kbdstate 
//define an allegro keyboard state
ALLEGRO_KEYBOARD_STATE kbdstate;


bool left_key_on = false;
bool right_key_on = false;
bool up_key_on = false;
bool down_key_on = false;
//an array containing the keyboard's keys to track state
//true is pressed, false is not
bool pressed_keys[ALLEGRO_KEY_MAX];


//define an allegro timer
ALLEGRO_TIMER *timer;


//define an allegro event queue

ALLEGRO_EVENT_QUEUE *queue;

//declare a variable that represents if the player is in the game map loop or not
bool in_game_map_loop = true;

bool redraw = false;



//

#define EDITOR_MODE_OFF


int main(int argc, char **argv) {

 setup();    //sets up allegro library

 maingame(); //main game
 
 shutdown(); //closes allegro library
 
 return 0;
 
}


void setup()
{

	 //open the debugging log


	 if (!al_init())
		 exit(ALLEGRO_INIT_FAIL_ERROR);

	 if (!al_install_keyboard())
		 exit(INSTALL_KEYBOARD_ERROR);


	 if(!al_init_image_addon()) //initializes the add on for loading and saving bitmap files
	 	 exit(NO_IMAGE_ADDON_LOAD_FAIL_ERROR); //image add on failed to load


	 if(!al_init_primitives_addon())
		 exit(NO_PRIMITIVES_ADDON_LOAD_FAIL_ERROR);


	  al_init_font_addon(); //initializes the add on for loading and saving bitmap files
							//no return type
	  al_init_ttf_addon();		//initializes the allegro ttf font addon

	 get_display(); //gets the display bitmaps


	 change_dir_game_home(); //sets the current directory to the app home


	 init_bitmaps(); //initializes the non-display bitmaps
	 load_bitmap_files(); //loads the bitmap file resources
	 load_fonts();


	 //load_map(SECONDMAP); //loads a game map

	 load_map(DEFAULTMAP, 2);

	 if ((timer = al_create_timer(TIMER_INTERVAL)) == NULL)			//create timer used to control frames/sec rendering
		 exit(ALLEGRO_TIMER_CREATE_FAIL_ERROR);


	 //create event sources
	 if ((queue = al_create_event_queue()) == NULL)
		exit(EVENT_QUEUE_CREATE_FAIL_ERROR);

	 al_register_event_source(queue, al_get_keyboard_event_source());
	 al_register_event_source(queue, al_get_timer_event_source(timer));
	 al_register_event_source(queue, al_get_display_event_source(display));

	 al_start_timer(timer); //start the frames/sec timer




}

void shutdown()
{
	 destroy_fonts();
	 //destroy_bitmap_files(); //destroys the bitmap file resources
	 destroy_bitmaps(); //destroys the non-display bitmaps
	 destroy_display(); //destroys the game o/s display data structures

	 al_destroy_event_queue(queue);

	 al_shutdown_font_addon();

	 al_shutdown_primitives_addon();

	 al_shutdown_image_addon();

	 al_uninstall_keyboard();

}






//*******************MAIN GAME LOOP************************//

void maingame()
{
	//NOTE: Slow down player input here, player moves too fast (maybe use separate timer?)


	//bool key_held_input = true;		//gather "held key" input - input where the duration of the key 
								    //press matters NOTE: implement this later to reduce polling
	//main game map loop

	//decides whether scene should be redrawn
	//bool redraw = true;             //redraw the scene when true


	time_t start_time, end_time; //for timing idle animations

	time(&start_time);


	while(in_game_map_loop == true)
	{
		ALLEGRO_EVENT an_event; //define an allegro event
		



		//handle events that occured

		if (al_is_event_queue_empty(queue)) //if the event queue is empty
		{
			if (redraw == true)
			{
				//render the scene
				center();
				gamerenderer();

				redraw = false;
			}
		}

		//handle methods that occur every cycle,
		//even if event queue is empty

		npc_ai_wander(); //causes npcs to wander around the screen

		action_reset_all_sprites();

		update_all_sprites();

	    update_top_animations(); //updates top layer animations
		//pause and wait for an event to occur

	    //poll for tag events
	   // findanytag_in_tagmap();

	    tag_action_chk();


		al_wait_for_event(queue, &an_event);

		//update timer for idle frame check

		time(&end_time);

		if (difftime(end_time ,start_time) > IDLE_TIME_THRESHOLD)
		{
			///reset timer
			time(&start_time);

			//change seren's animation to idle

			action_change_player_idle();

		}


		switch(an_event.type) {
			case ALLEGRO_EVENT_KEY_DOWN:
			 logger("key pressed");
			 //pressed_keys[an_event.keyboard.keycode] = true; //key is being held
			 //Note: May no longer be needed
			 key_down_up_input(an_event.keyboard.keycode); //check for events based off of key


			 break;
			case ALLEGRO_EVENT_KEY_UP:
			 //key_down_up_input(an_event.keyboard.keycode); //check for events based off of key
				//down and then up input
			 logger("key released");
			 //pressed_keys[an_event.keyboard.keycode] = false; //key is no longer being held
			 //Note: May be no longer be needed

			 	 //check for left and right condiitions
			 if (an_event.keyboard.keycode == ALLEGRO_KEY_LEFT)
			 {
				 left_key_on = false;
			 }
			 if (an_event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
			 {
				 right_key_on = false;
			 }
			 if (an_event.keyboard.keycode == ALLEGRO_KEY_UP)
			 {
				 up_key_on = false;
			 }
			 if (an_event.keyboard.keycode == ALLEGRO_KEY_DOWN)
			 {
				 down_key_on = false;
			 }

			 break;
			case ALLEGRO_EVENT_KEY_CHAR:
				key_held_input(an_event.keyboard.keycode);
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
             in_game_map_loop = false;
             break;
			case ALLEGRO_EVENT_TIMER:
			 redraw = true;
             break;
       }
    }		
	logger("exiting maingame loop");
}


//******************Input routines**************************

//registers input once for every time key is pressed down and released

void key_down_up_input(int keycode)
{
	//emergency escape
 if (keycode == ALLEGRO_KEY_ESCAPE)
 {
	 in_game_map_loop = false;
 }

}


//registers input for as long as the key is held
void key_held_input(int keycode)
{
	int xcor = 0;
	int ycor = 0;

	logger("key_held_input(): checking input");
	logger("key code is:");
	logint(keycode);


	if (keycode == ALLEGRO_KEY_DOWN)
	{
		down_key_on = true;
		if (left_key_on) {
			//if (pressed_keys[ALLEGRO_KEY_LEFT]) {
			logger("Pressed keys for diagonal detected");
			action_movedown_left(&mainchar);

		}


		else if (right_key_on) {
			action_movedown_right(&mainchar);
			logger("Detected down right top key down");

		}
		else
		{
			logger("pressed_key: key down");
			action_movedown(&mainchar);
		}
	}


	else if (keycode == ALLEGRO_KEY_UP)
	{
		up_key_on = true;

		if (left_key_on) {
			action_moveup_left(&mainchar);
		}
		else if (right_key_on) {
			action_moveup_right(&mainchar);
		}
		else {
			logger("pressed_key: key up");
			action_moveup(&mainchar);
		}
	}

	else if (keycode == ALLEGRO_KEY_LEFT)
	{

		left_key_on = true;

		if (down_key_on) {
			action_movedown_left(&mainchar);
		} else if (up_key_on) {
			action_moveup_left(&mainchar);
		} else {
			logger("pressed_key: key left");
			action_moveleft(&mainchar);
		}
	}

	else if (keycode == ALLEGRO_KEY_RIGHT)
	{


		right_key_on = true;

		if (down_key_on) {
			action_movedown_right(&mainchar);
		} else if (up_key_on) {
			action_moveup_right(&mainchar);
		} else {
			logger("pressed_key: key right");
			action_moveright(&mainchar);
		}

	}


	if (keycode == ALLEGRO_KEY_ENTER)
	{

		if (mainchar_close_to_enemy())
		{
			//attack in enemy direction
			action_mainchar_attack(&mainchar);
		}

		if (textIsOnScreen == false)
		{
			sprite_action_chk();
		} else {
			textIsOnScreen = false;
			text_screen = false;
		}

		tag_action_activate_chk(); //poll tag event map (action key entered)

	}

	/*
 if (key[KEY_I])
 {
  clear_keybuf();
  bmpscanner();
 }
 if (key[KEY_A])
 {
  if (actionchk == 1)
  {
   actionchk = 0;
  }
  if (actionchk != 1)
  {
   actionchk = 1;
  }
 }
 if (key[KEY_P])
 {
   slow = 1;
 }
 if (key[KEY_O])
 {
  slow = 0;
 }
 if (key[KEY_1])
 {
  strcpy(filename, argv[0]);
  strcpy(get_filename(filename), "screenshot.bmp");
  save_bitmap(filename, buffer, the_pallete);
  textout(screen, font, "Took a screenshot", 7, 7, 1850);
 }
	 */
	//if (al_key_down(&kbdstate, ALLEGRO_KEY_ESCAPE))
	//{
	// in_game_map_loop = false;
	//}

}

/********************Combat Engine**********************/

bool mainchar_close_to_enemy()
{
	//check to see if close to enemy
	//if so, return true, else false
	return true;
}

//intiates attack animations
void action_mainchar_attack(sprite * character)
{


	//check direction character is facing

	//and change frame to attack

	if (character->action == RIGHT_STAND_1 ||
		character->action == RIGHT_ATTACK_1 ||
		character->action == RIGHT_ATTACK_2 ||
		character->action == RIGHT_ATTACK_3) {

		//character is facing right so switch to attack right

		character->action = RIGHT_ATTACK_1;

	}

	//check direction character is facing
	if (character->action == LEFT_STAND_1 ||
		character->action == LEFT_ATTACK_1 ||
		character->action == LEFT_ATTACK_2 ||
		character->action == LEFT_ATTACK_3) {

		//character is facing right so switch to attack right

		character->action = LEFT_ATTACK_1;

	}

	//check direction character is facing
	if (character->action == FRONT_STAND_1 ||
		character->action == FRONT_ATTACK_1 ||
		character->action == FRONT_ATTACK_2 ||
		character->action == FRONT_ATTACK_3) {

		//character is facing right so switch to attack right

		character->action = FRONT_ATTACK_1;

	}



	//check direction character is facing
	if (character->action == BACK_STAND_1 ||
		character->action == BACK_ATTACK_1 ||
		character->action == BACK_ATTACK_2 ||
		character->action == BACK_ATTACK_3) {

		//character is facing right so switch to attack right

		character->action = BACK_ATTACK_1;

	}

	//works like action_move, initiating animation

	//performs a check to see if attack collides with enemy
	//(like the collision check used for talking)


}

//similar to moveup, movedown
void action_attack_down(sprite *character) {

	 if (character == NULL)
			exit(ACTION_MOVE_PASSED_SPRITE_NULL_ERROR);

	   logger("attacking, action:");
		logint(character->action);


	   //if movement is standing or not already movment in this
		//direction
	   if ((character->action < FRONT_ATTACK_1) ||
		    character->action > FRONT_ATTACK_3)
	   {

		   character->action = FRONT_ATTACK_1;
	   }

	   else if (character->action == FRONT_ATTACK_3) //and the frame is the last frame, then
	   {
	    character->action = FRONT_ATTACK_1;
	   }
	   else if (character->action == FRONT_ATTACK_2)
	   {
	    character->action = FRONT_ATTACK_3;
	   }
	   else if (character->action == FRONT_ATTACK_1)
	   {
		character->action = FRONT_ATTACK_2;
	   }

	   /*
		character->ypos++;
		//scroll_y++;
		scrolly(1);

		if(checkcollisiontrue(character))
	    {
		  //scroll_y--;
		 scrolly(-1);
			character->ypos--;
	    }*/

}

void action_attack_up(sprite *character) {

	 if (character == NULL)
			exit(ACTION_MOVE_PASSED_SPRITE_NULL_ERROR);

	   logger("attacking, action:");
		logint(character->action);


	   //if movement is standing or not already movment in this
		//direction
	   if ((character->action < BACK_ATTACK_1) ||
		    character->action > BACK_ATTACK_3)
	   {

		   character->action = BACK_ATTACK_1;
	   }

	   else if (character->action == BACK_ATTACK_3) //and the frame is the last frame, then
	   {
	    character->action = BACK_ATTACK_1;
	   }
	   else if (character->action == BACK_ATTACK_2)
	   {
	    character->action = BACK_ATTACK_3;
	   }
	   else if (character->action == BACK_ATTACK_1)
	   {
		character->action = BACK_ATTACK_2;
	   }

	   /*
		character->ypos++;
		//scroll_y++;
		scrolly(1);

		if(checkcollisiontrue(character))
	    {
		  //scroll_y--;
		 scrolly(-1);
			character->ypos--;
	    }*/

}

void action_attack_left(sprite *character) {

	 if (character == NULL)
			exit(ACTION_MOVE_PASSED_SPRITE_NULL_ERROR);

	   logger("attacking, action:");
		logint(character->action);


	   //if movement is standing or not already movment in this
		//direction
	   if ((character->action < LEFT_ATTACK_1) ||
		    character->action > LEFT_ATTACK_3)
	   {

		   character->action = LEFT_ATTACK_1;
	   }

	   else if (character->action == LEFT_ATTACK_3) //and the frame is the last frame, then
	   {
	    character->action = LEFT_ATTACK_1;
	   }
	   else if (character->action == LEFT_ATTACK_2)
	   {
	    character->action = LEFT_ATTACK_3;
	   }
	   else if (character->action == LEFT_ATTACK_1)
	   {
		character->action = LEFT_ATTACK_2;
	   }

	   /*
		character->ypos++;
		//scroll_y++;
		scrolly(1);

		if(checkcollisiontrue(character))
	    {
		  //scroll_y--;
		 scrolly(-1);
			character->ypos--;
	    }*/

}

void action_attack_right(sprite *character) {

	 if (character == NULL)
			exit(ACTION_MOVE_PASSED_SPRITE_NULL_ERROR);

	   logger("attacking, action:");
		logint(character->action);


	   //if movement is standing or not already movment in this
		//direction
	   if ((character->action < RIGHT_ATTACK_1) ||
		    character->action > RIGHT_ATTACK_3)
	   {

		   character->action = RIGHT_ATTACK_1;
	   }

	   else if (character->action == RIGHT_ATTACK_3) //and the frame is the last frame, then
	   {
	    character->action = RIGHT_ATTACK_1;
	   }
	   else if (character->action == RIGHT_ATTACK_2)
	   {
	    character->action = RIGHT_ATTACK_3;
	   }
	   else if (character->action == RIGHT_ATTACK_1)
	   {
		character->action = RIGHT_ATTACK_2;
	   }

	   /*
		character->ypos++;
		//scroll_y++;
		scrolly(1);

		if(checkcollisiontrue(character))
	    {
		  //scroll_y--;
		 scrolly(-1);
			character->ypos--;
	    }*/


}


/*********************Conversation/Event Tag Routines************/

void sprite_action_chk(void)
{
	//1st job is to scan through sprites and see if position is equal//


	for (int i = 0 ; i <10; i++)
	{
		sprite npc = sprites[i];

		if (npc.xpos == mainchar.xpos-1 && npc.ypos == mainchar.ypos+1)
		{
			runaction(i);
		}
		if (npc.xpos == mainchar.xpos && npc.ypos == mainchar.ypos+1)
		{
			runaction(i);
		}
		if (npc.xpos == mainchar.xpos+1 && npc.ypos == mainchar.ypos+1)
		{
			runaction(i);
		}
		if (npc.xpos == mainchar.xpos+1 && npc.ypos == mainchar.ypos)
		{
			runaction(i);
		}
		if (npc.xpos == mainchar.xpos+1 && npc.ypos == mainchar.ypos-1)
		{
			runaction(i);
		}
		if (npc.xpos == mainchar.xpos && npc.ypos == mainchar.ypos-1)
		{
			runaction(i);
		}
		if (npc.xpos-1 == mainchar.xpos && npc.ypos == mainchar.ypos-1)
		{
			runaction(i);
		}
		if (npc.xpos-1 == mainchar.xpos && npc.ypos == mainchar.ypos)
		{
			runaction(i);
		}
	}
}


void tag_action_chk() //run every cycle
{

	//printf("MAINCHAR X AND Y %d %d", mainchar.xpos, mainchar.ypos);
	logger("tag action check:");

	int xpos = mainchar.xpos;//;/PLAYER_SPRITE_SCALE_FACTOR_X;
	int ypos = mainchar.ypos;///PLAYER_SPRITE_SCALE_FACTOR_Y;

	int center_tag_id = tagmap[xpos][ypos];
	int north_tag_id = tagmap[xpos][ypos-1];
	int south_tag_id = tagmap[xpos][ypos+1];
	int west_tag_id = tagmap[xpos-1][ypos];
	int east_tag_id  = tagmap[xpos+1][ypos];

	logger("mainchar.xpos");
	logint(mainchar.xpos);
	logger("mainchar.ypos");
	logint(mainchar.ypos);

	if (center_tag_id > 0 && tags[center_tag_id].pressa != 1)
	{
		run_tag_action(center_tag_id);
	}
	else if (north_tag_id > 0 && tags[center_tag_id].pressa != 1)
	{
		run_tag_action(north_tag_id && tags[center_tag_id].pressa != 1);
	}
	else if (south_tag_id > 0 && tags[center_tag_id].pressa != 1)
	{
		run_tag_action(south_tag_id && tags[center_tag_id].pressa != 1);
	}
	else if (west_tag_id > 0 && tags[center_tag_id].pressa != 1)
	{
		run_tag_action(west_tag_id && tags[center_tag_id].pressa != 1);
	}
	else if (east_tag_id > 0 && tags[center_tag_id].pressa != 1)
	{
		run_tag_action(east_tag_id);
	}

}

void tag_action_activate_chk() //run when player presses action key
{
	logger("tag activate action check");


	int xpos = mainchar.xpos;//PLAYER_SPRITE_SCALE_FACTOR_X;
	int ypos = mainchar.ypos;//PLAYER_SPRITE_SCALE_FACTOR_Y;

	logint(xpos);
	logint(ypos);

	logger("---------------------");


	int center_tag_id = tagmap[xpos][ypos];
	int north_tag_id = tagmap[xpos][ypos-1];
	int south_tag_id = tagmap[xpos][ypos+1];
	int west_tag_id = tagmap[xpos-1][ypos];
	int east_tag_id  = tagmap[xpos+1][ypos];



	if (center_tag_id > 0 && tags[center_tag_id].pressa == 1)
	{
		run_tag_action(center_tag_id);
	}
	else if (north_tag_id > 0 && tags[center_tag_id].pressa == 1)
	{
		run_tag_action(north_tag_id && tags[center_tag_id].pressa == 1);
	}
	else if (south_tag_id > 0 && tags[center_tag_id].pressa == 1)
	{
		run_tag_action(south_tag_id && tags[center_tag_id].pressa == 1);
	}
	else if (west_tag_id > 0 && tags[center_tag_id].pressa == 1)
	{
		run_tag_action(west_tag_id && tags[center_tag_id].pressa == 1);
	}
	else if (east_tag_id > 0 && tags[center_tag_id].pressa == 1)
	{
		run_tag_action(east_tag_id);
	}

}




void draw_text_windows() {
	//if (textIsOnScreen)
	if (text_screen && text_box_curr_size <= TEXT_BOX_FLOAT_IN)
	{
		cerr << "Toggle value is " << textIsOnScreen << endl;
		renderBufferedText();
	}
}

void draw_text_background_rectangle() {
	if (text_screen) {
		if (text_box_curr_size < TEXT_BOX_FLOAT_IN) {
			text_box_curr_size++;
		}
		int sw = 300;///(20 - text_box_curr_size + 1);
		int sh = 150;///(20 - text_box_curr_size + 1);

		int centerx = 300 / 2;
		int centery = 150 / 2;

		int sx = 0;
		int sy = 0;
		int dx = TEXT_BOX_FLOAT_IN-text_box_curr_size;
		int dy = TEXT_BOX_FLOAT_IN-text_box_curr_size;
		al_draw_tinted_bitmap_region(convo_box, al_map_rgba_f(1, 1, 1, 0.75),
				sx, sy, sw, sh, dx, dy, 0);
	}
	if (text_screen == false && text_box_curr_size != 0) {
			text_box_curr_size = 0;
	}

}


//temporary rule based system for conversation with everything hard-coded
void cleartxt() { }

void runaction(int i)
{
	cerr << "Running runaction" << endl;

	if (text_screen == false) {

		if (sprites[i].actrig == 1) {

		char *text_string[ARRAY_LENGTH_NUM_LINES_SPOKEN];
		//sprintf(text_string[0], "There is something evil");
		//sprintf(text_string[1], "in this castle, I can");
		//sprintf(text_string[2], "feel it!");
		text_string[0] = "CENTURION BLACK GUARD:";
		text_string[1] = "There is something evil";
		text_string[2] = "in this castle...I can feel";
		text_string[3] = "it in my bones";


		drawTextToBuffer(text_string, 4, 1);
		text_screen = true;
		textAdvance();

		}

		else {
		//if (sprites[i].actrig == 2) {

		char *text_string[ARRAY_LENGTH_NUM_LINES_SPOKEN];
		//sprintf(text_string[0], "There is something evil");
		//sprintf(text_string[1], "in this castle, I can");
		//sprintf(text_string[2], "feel it!");
		text_string[0] = "CENTURION BLACK GUARD:";
		text_string[1] = "Emperor Blade has been plotting ";
		text_string[2] = "against the northerly kingdom of";
		text_string[3] = "Damon. But I'm sure you know this.";


		drawTextToBuffer(text_string, 4, 1);
		text_screen = true;
		textAdvance();
		}
		//}


	}

}

/*
	cerr << "Running runaction" << endl;
	//if (sprites[i].actrig == 1)
	//{
		cleartxt();
		//boxx = sprites[i].xpos;
		//boxy = sprites[i].ypos;
		//textchar = sprites[i].bmpid;
		//facer();
		//sprintf(textstring[0], "There is something evil");
		//sprintf(textstring[1], "in this castle, I can");
		//sprintf(textstring[2], "feel it!");
		//txthandler(textstring, 3, 1);
	//}

	if (sprites[i].actrig == 2)
	{
		cleartxt();
		//textchar = sprites[i].bmpid;
		//boxx = sprites[i].xpos;
		//boxy = sprites[i].ypos;
		sprintf(textstring[0], "Emperor Blade seeks to");
		sprintf(textstring[1], "resurrect power to");
		sprintf(textstring[2], "destroy our sister");
		sprintf(textstring[3], "kingdom-Damon");
		//facer();
		txthandler(textstring, 4, 1);
	}

	if (sprites[i].actrig == 3)
	{
		cleartxt();
		//textchar = sprites[i].bmpid;
		//boxx = sprites[i].xpos;
		//boxy = sprites[i].ypos;
		sprintf(textstring[0], "The Emperor seeks your");
		sprintf(textstring[1], "presence immediately! Go");
		sprintf(textstring[2], "up the outside hall and");
		sprintf(textstring[3], "through the stairs until");
		sprintf(textstring[4],  "you reach the outside.");
		sprintf(textstring[5],  "Then continue up into his");
		sprintf(textstring[6], "throne room.");
		//facer();
		txthandler(textstring, 7, 2);
	}
	if (sprites[i].actrig == 4)
	{
		cleartxt();
		//textchar = sprites[i].bmpid;
		//boxx = sprites[i].xpos;
		//boxy = sprites[i].ypos;
		sprintf(textstring[0], "Hey, being succesor to the");
		sprintf(textstring[1], "last commander must be");
		sprintf(textstring[2], "exciting eh? I wish I");
		sprintf(textstring[3], "had your power!");
		//facer();
		txthandler(textstring, 4, 1);
	}
	if (sprites[i].actrig == 5)
	{
		cleartxt();
		//textchar = sprites[i].bmpid;
		//boxx = sprites[i].xpos;
		//boxy = sprites[i].ypos;
		sprintf(textstring[0], "Why is the domonian army");
		sprintf(textstring[1], "mobolizing? People say the");
		sprintf(textstring[2], "Emperor seeks the lost power");
		sprintf(textstring[3], " of the Komit once again!");
		sprintf(textstring[4], "Could this be true?");
		//facer();
		txthandler(textstring, 5, 1);
	}
	if (sprites[i].actrig == 6)
	{
		cleartxt();
		//textchar = sprites[i].bmpid;
		//boxx = sprites[i].xpos;
		//boxy = sprites[i].ypos;
		sprintf(textstring[0],  "1,000 years ago our");
		sprintf(textstring[1],  "army entered a war");
		sprintf(textstring[2],  "with Damonia. There");
		sprintf(textstring[3], "was a stalemate and");
		sprintf(textstring[4], "soon the people of");
		sprintf(textstring[5],  "the world became");
		sprintf(textstring[6],  "angry, so they prayed");
		sprintf(textstring[7],  "to the master for");
		sprintf(textstring[8],  "salvation. And he gave");
		sprintf(textstring[9], " them 'Techno'.");
		//facer();
		txthandler(textstring,10, 2);
	}
}*/





//********************Action/Animation Routines**************/
//called by both input and actionhandler (which updates the animation)
void action_moveup(sprite *character)
{
	if (character == NULL)
		exit(ACTION_MOVE_PASSED_SPRITE_NULL_ERROR);

	logger("moving, action:");
	logint(character->action);


	

	//if movement is standing or not already movment in this
	//direction
   if ((character->action < BACK_MOVE_1) ||
	    character->action > BACK_MOVE_3) 
   {
	   
	   character->action = BACK_MOVE_1;
   }

   else if (character->action == BACK_MOVE_3) //and the frame is the last frame, then 
   {
    character->action = BACK_MOVE_1;
   }
   else if (character->action == BACK_MOVE_2)
   {
    character->action = BACK_MOVE_3;
   }
   else if (character->action == BACK_MOVE_1)
   {
	character->action = BACK_MOVE_2;
   }

   //move character's position up
   
   character->ypos--;

   // scroll_y--;
   scrolly(-1);
   if(checkcollisiontrue(character))
   {
	  //scroll_y++;
	  scrolly(1);
	  character->ypos++;
   }

}

	


void action_movedown(sprite *character)
{
   if (character == NULL)
		exit(ACTION_MOVE_PASSED_SPRITE_NULL_ERROR);

   logger("moving, action:");
	logint(character->action);


   //if movement is standing or not already movment in this
	//direction
   if ((character->action < FRONT_MOVE_1) ||
	    character->action > FRONT_MOVE_8)
   {
	   
	   character->action = FRONT_MOVE_1;
   }

   else if (character->action == FRONT_MOVE_1)
   {
	character->action = FRONT_MOVE_2;
   }
   else if (character->action == FRONT_MOVE_2)
   {
    character->action = FRONT_MOVE_3;
   }
   else if (character->action == FRONT_MOVE_3)
   {
	character->action = FRONT_MOVE_4;
   }

   else if (character->action == FRONT_MOVE_4)
   {
	character->action = FRONT_MOVE_5;
   }

   else if (character->action == FRONT_MOVE_5)
   {
	character->action = FRONT_MOVE_6;
   }

   else if (character->action == FRONT_MOVE_6)
   {
	character->action = FRONT_MOVE_7;
   }
   else if (character->action == FRONT_MOVE_7)
   {
	character->action = FRONT_MOVE_8;
   }

   else if (character->action == FRONT_MOVE_8) //and the frame is the last frame, then
   {
    character->action = FRONT_MOVE_1;
   }


	character->ypos++;
	//scroll_y++;
	scrolly(1);

	if(checkcollisiontrue(character))
    {
	  //scroll_y--;
	 scrolly(-1);
		character->ypos--;
    }
}

void action_moveleft(sprite *character)
{

	if (character == NULL)
		exit(ACTION_MOVE_PASSED_SPRITE_NULL_ERROR);

	logger("moving, action:");
	logint(character->action);


	//if movement is standing or not already movment in this
	//direction
   if ((character->action < LEFT_MOVE_1) ||
	    character->action > LEFT_MOVE_8)
   {
	   
	   character->action = LEFT_MOVE_1;
   }

   else if (character->action == LEFT_MOVE_1)
   {
	   character->action = LEFT_MOVE_2;
   }
   else if (character->action == LEFT_MOVE_2)
   {
    character->action = LEFT_MOVE_3;
   }
   else if (character->action == LEFT_MOVE_3)
   {
	character->action = LEFT_MOVE_4;
   }
   else if (character->action == LEFT_MOVE_4)
   {
   	character->action = LEFT_MOVE_5;
      }
   else if (character->action == LEFT_MOVE_5)
      {
   	character->action = LEFT_MOVE_6;
      }
   else if (character->action == LEFT_MOVE_6)
      {
   	character->action = LEFT_MOVE_7;
      }
   else if (character->action == LEFT_MOVE_7)
      {
   	character->action = LEFT_MOVE_8;
      }
   else if (character->action == LEFT_MOVE_8)
      {
   	character->action = LEFT_MOVE_1;
      }

   //scroll_x--;
   scrollx(-1);
   character->xpos--;
   if(checkcollisiontrue(character))
   {
	  scrollx(1);
	  //scroll_x++;
	  character->xpos++;
   }
}

void action_moveright(sprite *character)
{

	if (character == NULL)
		exit(ACTION_MOVE_PASSED_SPRITE_NULL_ERROR);


	logger("moving, action:");
	logint(character->action);

   
	//if movement is standing or not already movment in this
	//direction
   if ((character->action < RIGHT_MOVE_1) ||
	    character->action > RIGHT_MOVE_8)
   {
	   
	   character->action = RIGHT_MOVE_1;
   }
   else if (character->action == RIGHT_MOVE_1)
   {
    character->action = RIGHT_MOVE_2;
   }
   else if (character->action == RIGHT_MOVE_2)
   {
	character->action = RIGHT_MOVE_3;
   }
   else if (character->action == RIGHT_MOVE_3)
      {
   	character->action = RIGHT_MOVE_4;
      }
   else if (character->action == RIGHT_MOVE_4)
      {
   	character->action = RIGHT_MOVE_5;
      }
   else if (character->action == RIGHT_MOVE_5)
      {
   	character->action = RIGHT_MOVE_6;
      }
   else if (character->action == RIGHT_MOVE_6)
      {
   	character->action = RIGHT_MOVE_7;
      }
   else if (character->action == RIGHT_MOVE_7)
      {
   	character->action = RIGHT_MOVE_8;
      }

   else if (character->action == RIGHT_MOVE_8)
      {
   	character->action = RIGHT_MOVE_1;
      }

   character->xpos++;
   //scroll_x++;
   scrollx(1);
   if(checkcollisiontrue(character))
   {
	  //scroll_x--;
	  scrollx(-1);
	   character->xpos--;
   }
}


///diagonals for movement (main character)

//additional movement directions
void action_movedown_left(sprite *character)
{
	if (character == NULL)
		exit(ACTION_MOVE_PASSED_SPRITE_NULL_ERROR);


	logger("moving, action:");
	logint(character->action);


	//if movement is standing or not already movment in this
	//direction
   if ((character->action < DOWN_LEFT_MOVE_1) ||
	    character->action > DOWN_LEFT_MOVE_8)
   {

	   character->action = DOWN_LEFT_MOVE_1;
   }
   else if (character->action == DOWN_LEFT_MOVE_1)
   {
    character->action = DOWN_LEFT_MOVE_2;
   }
   else if (character->action == DOWN_LEFT_MOVE_2)
   {
	character->action = DOWN_LEFT_MOVE_3;
   }
   else if (character->action == DOWN_LEFT_MOVE_3)
      {
   	character->action = DOWN_LEFT_MOVE_4;
      }
   else if (character->action == DOWN_LEFT_MOVE_4)
      {
   	character->action = DOWN_LEFT_MOVE_5;
      }
   else if (character->action == DOWN_LEFT_MOVE_5)
      {
   	character->action = DOWN_LEFT_MOVE_6;
      }
   else if (character->action == DOWN_LEFT_MOVE_6)
      {
   	character->action = DOWN_LEFT_MOVE_7;
      }
   else if (character->action == DOWN_LEFT_MOVE_7)
      {
   	character->action = DOWN_LEFT_MOVE_8;
      }

   else if (character->action == DOWN_LEFT_MOVE_8)
      {
   	character->action = DOWN_LEFT_MOVE_1;
      }

   //scroll_x--;
   scrollx(-1);
   //scroll_y++;
   	scrolly(1);

   character->xpos--;
   character->ypos++;
   if(checkcollisiontrue(character))
   {
	  scrollx(1);
	  //scroll_x++;
	  character->xpos++;
	  //scroll_y--;
	 scrolly(-1);
		character->ypos--;
   }
}



void action_movedown_right(sprite *character) {
	if (character == NULL)
		exit(ACTION_MOVE_PASSED_SPRITE_NULL_ERROR);


	logger("moving, action:");
	logint(character->action);


	//if movement is standing or not already movment in this
	//direction
   if ((character->action < DOWN_RIGHT_MOVE_1) ||
	    character->action > DOWN_RIGHT_MOVE_8)
   {

	   character->action = DOWN_RIGHT_MOVE_1;
   }
   else if (character->action == DOWN_RIGHT_MOVE_1)
   {
    character->action = DOWN_RIGHT_MOVE_2;
   }
   else if (character->action == DOWN_RIGHT_MOVE_2)
   {
	character->action = DOWN_RIGHT_MOVE_3;
   }
   else if (character->action == DOWN_RIGHT_MOVE_3)
      {
   	character->action = DOWN_RIGHT_MOVE_4;
      }
   else if (character->action == DOWN_RIGHT_MOVE_4)
      {
   	character->action = DOWN_RIGHT_MOVE_5;
      }
   else if (character->action == DOWN_RIGHT_MOVE_5)
      {
   	character->action = DOWN_RIGHT_MOVE_6;
      }
   else if (character->action == DOWN_RIGHT_MOVE_6)
      {
   	character->action = DOWN_RIGHT_MOVE_7;
      }
   else if (character->action == DOWN_RIGHT_MOVE_7)
      {
   	character->action = DOWN_RIGHT_MOVE_8;
      }

   else if (character->action == DOWN_RIGHT_MOVE_8)
      {
   	character->action = DOWN_RIGHT_MOVE_1;
      }

   //scroll_x--;
   scrollx(1);
   //scroll_y++;
   	scrolly(1);

   character->xpos++;
   character->ypos++;
   if(checkcollisiontrue(character))
   {
	  scrollx(-1);
	  //scroll_x++;
	  character->xpos--;
	  //scroll_y--;
	 scrolly(-1);
		character->ypos--;
   }
}

void action_moveup_left(sprite *character) {

	if (character == NULL)
		exit(ACTION_MOVE_PASSED_SPRITE_NULL_ERROR);


	logger("moving, action:");
	logint(character->action);


	//if movement is standing or not already movment in this
	//direction
   if ((character->action < UP_LEFT_MOVE_1) ||
	    character->action > UP_LEFT_MOVE_8)
   {

	   character->action = UP_LEFT_MOVE_1;
   }
   else if (character->action == UP_LEFT_MOVE_1)
   {
    character->action = UP_LEFT_MOVE_2;
   }
   else if (character->action == UP_LEFT_MOVE_2)
   {
	character->action = UP_LEFT_MOVE_3;
   }
   else if (character->action == UP_LEFT_MOVE_3)
      {
   	character->action = UP_LEFT_MOVE_4;
      }
   else if (character->action == UP_LEFT_MOVE_4)
      {
   	character->action = UP_LEFT_MOVE_5;
      }
   else if (character->action == UP_LEFT_MOVE_5)
      {
   	character->action = UP_LEFT_MOVE_6;
      }
   else if (character->action == UP_LEFT_MOVE_6)
      {
   	character->action = UP_LEFT_MOVE_7;
      }
   else if (character->action == UP_LEFT_MOVE_7)
      {
   	character->action = UP_LEFT_MOVE_8;
      }

   else if (character->action == UP_LEFT_MOVE_8)
      {
   	character->action = UP_LEFT_MOVE_1;
      }

   //scroll_x--;
   scrollx(-1);
   //scroll_y++;
   	scrolly(-1);

   character->xpos--;
   character->ypos--;
   if(checkcollisiontrue(character))
   {
	  scrollx(1);
	  //scroll_x++;
	  character->xpos++;
	  //scroll_y--;
	 scrolly(1);
		character->ypos++;
   }


}
void action_moveup_right(sprite *character)
{

	if (character == NULL)
		exit(ACTION_MOVE_PASSED_SPRITE_NULL_ERROR);


	logger("moving, action:");
	logint(character->action);


	//if movement is standing or not already movment in this
	//direction
   if ((character->action < UP_RIGHT_MOVE_1) ||
	    character->action > UP_RIGHT_MOVE_8)
   {

	   character->action = UP_RIGHT_MOVE_1;
   }
   else if (character->action == UP_RIGHT_MOVE_1)
   {
    character->action = UP_RIGHT_MOVE_2;
   }
   else if (character->action == UP_RIGHT_MOVE_2)
   {
	character->action = UP_RIGHT_MOVE_3;
   }
   else if (character->action == UP_RIGHT_MOVE_3)
      {
   	character->action = UP_RIGHT_MOVE_4;
      }
   else if (character->action == UP_RIGHT_MOVE_4)
      {
   	character->action = UP_RIGHT_MOVE_5;
      }
   else if (character->action == UP_RIGHT_MOVE_5)
      {
   	character->action = UP_RIGHT_MOVE_6;
      }
   else if (character->action == UP_RIGHT_MOVE_6)
      {
   	character->action = UP_RIGHT_MOVE_7;
      }
   else if (character->action == UP_RIGHT_MOVE_7)
      {
   	character->action = UP_RIGHT_MOVE_8;
      }

   else if (character->action == UP_RIGHT_MOVE_8)
      {
   	character->action = UP_RIGHT_MOVE_1;
      }

   //scroll_x--;
   scrollx(1);
   //scroll_y++;
   	scrolly(-1);

   character->xpos++;
   character->ypos--;
   if(checkcollisiontrue(character))
   {
	  scrollx(-1);
	  //scroll_x++;
	  character->xpos--;
	  //scroll_y--;
	 scrolly(1);
		character->ypos++;
   }


}


void action_change_player_idle() //changes players animation etc. to idle
{
	mainchar.action = IDLE_FRAME_1;
}



//reset all sprite's actions

void action_reset_all_sprites(void)
{
	action_reset_player_sprite();
	for (int i=0;i<MAX_NUMBER_NPC_SPRITES;i++)
	{
		action_reset_sprite(&sprites[i]);
	}
}


//
void update_all_sprites(void)
{
	for (int i=0;i<MAX_NUMBER_NPC_SPRITES;i++)
	{
		action_update_sprites(&sprites[i]);
	}
}

//

//causes npcs to wander around the screen
void npc_ai_wander(void)
{
	int spritenum;
	int randnum;

	//iterate through all visible sprites
	for (spritenum=0;spritenum<MAX_NUMBER_NPC_SPRITES;spritenum++)
	 {
	  if (sprites[spritenum].immobile == 0) //is not immobile?
	  {
	   randnum = ((random()/random())*5);
	   if (randnum > 20 && randnum < 30)
	   {
	    sprites[spritenum].action = LEFT_MOVE_1;
	   }
	   if (randnum > 30 && randnum < 40)
	   {
	    sprites[spritenum].action = RIGHT_MOVE_1;
	   }
	   if (randnum > 100 && randnum < 150)
	   {
	    sprites[spritenum].action = FRONT_MOVE_1;
	   }
	   if (randnum > 150 && randnum < 200)
	   {
	    sprites[spritenum].action = BACK_MOVE_1;
	   }
	  }
	 }

}

//*** Player sprite action updater : was mactionhandler

void action_reset_player_sprite(void)
{
	action_reset_sprite(&mainchar);
}




//UPDATE ANIMATIONS
//keeps the player moving until his action frame returns to standstill
void action_reset_sprite(sprite *character)
{


 //if in the final frame of moving with back facing return to standing position motionless
 
 if (character->action == BACK_MOVE_8)
 {
  character->action = BACK_STAND_1;
  //character->ypos--; //adjust position - come to a stand still
  scrolly(-1); //adjust scrolling
  if(checkcollisiontrue(character))
	  scrolly(1);
 }
 if (character->action == FRONT_MOVE_8)
 {
  character->action = FRONT_STAND_1;
  //character->ypos++; //adjust position - come to a stand still
  scrolly(1); //adjust scrolling
  if(checkcollisiontrue(character))
	  scrolly(-1);
 }
 if (character->action == RIGHT_MOVE_8)
 {
   character->action = RIGHT_STAND_1;
   scrollx(1); //adjust scrolling
   if(checkcollisiontrue(character))
	  scrollx(-1);
 }
 if (character->action == LEFT_MOVE_8)
 {
   character->action = LEFT_STAND_1;
   scrollx(1); //adjust scrolling
   if(checkcollisiontrue(character))
	  scrollx(-1);
 }

 if (character->action == FRONT_ATTACK_3) {
	 character->action = FRONT_STAND_1;
 }
 if (character->action == BACK_ATTACK_3) {
	 character->action = BACK_STAND_1;
 }
 if (character->action == LEFT_ATTACK_3) {
	 character->action = LEFT_STAND_1;
 }
 if (character->action == RIGHT_ATTACK_3) {
	 character->action = RIGHT_STAND_1;
 }

}


bool checkcollisiontrue(sprite *character)
{
	//check against collision blocks
	if (collision[character->xpos][character->ypos])
	{
		return true; //collision block collision
	}
	for (int i=0;i<MAX_NUMBER_NPC_SPRITES;i++)
	{
		if (character->xpos == sprites[i].xpos && character->ypos == sprites[i].ypos
			&& character != &sprites[i]) //are we checking the character against itself?
			return true;
	}
	return false;
}


//*************GAME RENDERER

void gamerenderer()
{
	 clear(map_buffer); //will set the map_buffer as target and clear it

	 render_back_bmp();
	 renderbottom(); //render floor
	 rendermid(); //render tiles below player (generally wall tiles)
	 render_player_sprite(); //render player (was mrendersprite();)
	 render_npc_sprite(); //renders the npc sprites (was rendersprite();)
	 renderhi(); //render tiles above player (generally ceiling tiles)
	 rendertop(); //Note: render top layer, things such as clouds or fog -- new feature

	 blitter(); ////renders map buffer to back buffer and flips the back buffer with the screen

}



void renderInterfaceLayer() { //render a static, interface layer over the moving screen
	draw_text_background_rectangle();
	draw_text_windows();


	if (GAME_POSITION_ON) //debug mode
	{
		al_draw_textf(arial_rounded, WHITE_COLOR, 0, 10, ALLEGRO_ALIGN_LEFT, "Player pos," );
		al_draw_textf(arial_rounded, WHITE_COLOR, 0, 40, ALLEGRO_ALIGN_LEFT, "X %d", mainchar.xpos);
		al_draw_textf(arial_rounded, WHITE_COLOR, 0, 70, ALLEGRO_ALIGN_LEFT, "Y %d", mainchar.ypos);

	}
	if (GAME_SCROLL_COORDINATES_ON)
	{
		al_draw_textf(arial_rounded, WHITE_COLOR, 200, 10, ALLEGRO_ALIGN_LEFT, "Game Scroll," );
		al_draw_textf(arial_rounded, WHITE_COLOR, 200, 40, ALLEGRO_ALIGN_LEFT, "X %d", scroll_x);
		al_draw_textf(arial_rounded, WHITE_COLOR, 200, 70, ALLEGRO_ALIGN_LEFT, "Y %d", scroll_y);

	}
}


void render_back_bmp()
{

	for (int i=0; i<100; i++)
	{
		if (backgroundbitmap[i] == 1)
		{
			if (cloud1 == NULL)
				exit(DRAW_BACKGROUND_BITMAP_NULL_ERROR);
			//printf("found cloud background");
			al_draw_scaled_bitmap(cloud1,
					0,0,al_get_bitmap_width(cloud1), al_get_bitmap_height(cloud1),
					BACK_IMAGE_ORIGIN_X, BACK_IMAGE_ORIGIN_Y,
					BACK_IMAGE_WIDTH, BACK_IMAGE_HEIGHT, NULL);

		}
		else if (backgroundbitmap[i] == 2)
		{
			if (green1 == NULL)
						exit(DRAW_BACKGROUND_BITMAP_NULL_ERROR);
			al_draw_scaled_bitmap(green1,
							0,0,al_get_bitmap_width(green1), al_get_bitmap_height(green1),
							BACK_IMAGE_ORIGIN_X, BACK_IMAGE_ORIGIN_Y,
							BACK_IMAGE_WIDTH, BACK_IMAGE_HEIGHT, NULL);


		}

	}

}

//****repairing


void action_update_sprites(sprite *character)
{
	int move;
	move = 0;

	if (character->action == BACK_MOVE_1 ||
			character->action == BACK_MOVE_2 ||
			character->action == BACK_MOVE_3 ||
			character->action == BACK_MOVE_3 ||
			character->action == BACK_MOVE_4||
			character->action == BACK_MOVE_5 ||
			character->action == BACK_MOVE_6 ||
			character->action == BACK_MOVE_7 ||
			character->action == BACK_MOVE_8)
	{
		action_moveup(character);
	}


	if (character->action == FRONT_MOVE_1 ||
			character->action == FRONT_MOVE_2 ||
			character->action == FRONT_MOVE_3 ||
			character->action == FRONT_MOVE_4 ||
			character->action == FRONT_MOVE_5 ||
			character->action == FRONT_MOVE_6 ||
			character->action == FRONT_MOVE_7 ||
			character->action == FRONT_MOVE_8)

	{
		action_movedown(character);
	}


	if (character->action == LEFT_MOVE_1 ||
			character->action == LEFT_MOVE_2 ||
			character->action == LEFT_MOVE_3 ||
			character->action == LEFT_MOVE_3 ||
			character->action == LEFT_MOVE_4 ||
			character->action == LEFT_MOVE_5 ||
			character->action == LEFT_MOVE_6 ||
			character->action == LEFT_MOVE_7 ||
			character->action == LEFT_MOVE_8)
	{
		action_moveleft(character);
	}


	if (character->action == RIGHT_MOVE_1 ||
			character->action == RIGHT_MOVE_2 ||
			character->action == RIGHT_MOVE_3 ||
			character->action == RIGHT_MOVE_4 ||
			character->action == RIGHT_MOVE_5 ||
			character->action == RIGHT_MOVE_6 ||
			character->action == RIGHT_MOVE_7 ||
			character->action == RIGHT_MOVE_8)

	{
		action_moveright(character);
	}


	if (character->action == IDLE_FRAME_1)
	{
		character->action = IDLE_FRAME_2;
	}
	else if (character->action == IDLE_FRAME_2)
	{
		character->action = IDLE_FRAME_3;
	}
	else if (character->action == IDLE_FRAME_3)
	{
		character->action = IDLE_FRAME_4;
	}
	else if (character->action == IDLE_FRAME_4)
	{
		character->action = IDLE_FRAME_1;
	}

	//update attack animations
	if (character->action == RIGHT_ATTACK_1 ||
			character->action == RIGHT_ATTACK_2 ||
			character->action == RIGHT_ATTACK_3)
	{
		action_attack_right(character);
	}


	if (character->action == FRONT_ATTACK_1 ||
			character->action == FRONT_ATTACK_2 ||
			character->action == FRONT_ATTACK_3)
	{
		action_attack_down(character);
	}

	if (character->action == LEFT_ATTACK_1 ||
			character->action == LEFT_ATTACK_2 ||
			character->action == LEFT_ATTACK_3)
	{
		action_attack_left(character);
	}

	if (character->action == BACK_ATTACK_1 ||
			character->action == BACK_ATTACK_2 ||
			character->action == BACK_ATTACK_3)
	{
		action_attack_up(character);
	}

}


//////////

void scrolly(int amt)
{
	scroll_y += amt;

	if ((scroll_y*SCROLL_SCALING_FACTOR_Y + SCREEN_HEIGHT) > MAX_MAP_HEIGHT)
	{
		//scroll_y = MAX_MAP_HEIGHT- SCREEN_HEIGHT;
		scroll_y = (int)((MAX_MAP_HEIGHT - SCREEN_HEIGHT) / SCROLL_SCALING_FACTOR_Y);
	}

	if ((scroll_y < SCREEN_ORIGIN_Y))
	{
		scroll_y = SCREEN_ORIGIN_Y/SCROLL_SCALING_FACTOR_Y;
	}

}

void scrollx(int amt)
{

	scroll_x += amt;

	if ((scroll_x*SCROLL_SCALING_FACTOR_X + SCREEN_WIDTH) >= MAX_MAP_WIDTH)
	{
	     //scroll_x = 0;
		//exit(1);
		scroll_x = (int)((MAX_MAP_WIDTH - SCREEN_WIDTH) / SCROLL_SCALING_FACTOR_X);
	}

	if (scroll_x*SCROLL_SCALING_FACTOR_X < SCREEN_ORIGIN_X)
	{
		//scroll_x -= amt;
		scroll_x = SCREEN_ORIGIN_X/SCROLL_SCALING_FACTOR_X;
	}

}

void update_top_animations()
{


	top_anim_x_roller+= 5;
	top_anim_y_roller+= 2;


	//max of these values are their respective max map sizes
		//int top_anim_x_roller = 0;
		//int top_anim_y_roller = 0;


/*

	if (top_anim_x_roller > MAX_MAP_WIDTH) {
		top_anim_x_roller = 0;
	}
	if (top_anim_x_roller < 0) {
		top_anim_x_roller = MAX_MAP_WIDTH;
	}

	if (top_anim_y_roller > MAX_MAP_HEIGHT) {
			top_anim_y_roller = 0;
	}

	if (top_anim_y_roller < 0) {
		top_anim_y_roller = MAX_MAP_HEIGHT;
	}
*/
}

//run found tag id

int run_tag_action(int tag_id)
{
	//printf("Run tag action running");
	logger("Run tag action running, tag_id:");
	logint(tag_id);

	logger("Patch 0");
	logint(tags[tag_id].patch[0]);
	logger("Patch 1:");
	logint(tags[tag_id].patch[1]);
	logger("Patch 2: ");
	logint(tags[tag_id].patch[2]);
	logger("Patch 3: ");
	logint(tags[tag_id].patch[3]);

	logger("Finished tag action run");
	if (tags[tag_id].scnenum > 0)
	{
		//jump to another scene
	}

	if (tags[tag_id].tagjmp > 0)
	{
		//if jumping to another scene, id of tag to jump to within scene
	}

	if (tags[tag_id].item > 0)
	{
		//gained an item, run function to handle this

	}

	if (tags[tag_id].patch[0] != 0)
	{
		logger("Running wall patch");
		//activated a patch bitmap
		//[0] = bitmap id
		//[1] = x coord
		//[2] = y_coord
		//[3] = layer
		int bmp_id = tags[tag_id].patch[0];
		int x_coord = tags[tag_id].patch[1];
		int y_coord = tags[tag_id].patch[2];
		int layer = tags[tag_id].patch[3];

		//background layer is 1
		//bottom layer
		//middle wall layer
		//middle wall patch layer
		//hi bitmap layer

		//bottom layer
		if (layer == 2) {
			bottombmp[x_coord][y_coord] = bmp_id;
		}
		if (layer == 3) {
			midbmp[x_coord][y_coord] = bmp_id;
		}
		if (layer == 4) {
			mid2bmp[x_coord][y_coord] = bmp_id;
		}
		if (layer == 5) {
			hibmp[x_coord][y_coord] = bmp_id;
		}
	}

	if (tags[tag_id].actrig > 0)
	{
		//trigger sripted event
	}

}


void findanytag_in_tagmap()
{
	for (int i = 0; i< 100; i++)
	{
		for (int j = 0; j<100; j++)
		{
			if (tagmap[i][j] != 0)
			{
				logger("Found a tag entry");
				logger("At x, ");
				logint(i);
				logger("At y: ");
				logint(j);
			}

		}
	}
}


