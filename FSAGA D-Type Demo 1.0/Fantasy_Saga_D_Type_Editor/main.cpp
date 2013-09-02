//*****************EDITOR MAIN*****************//

#define GAME_EDITOR_VERSION 1.0


#include <stdio.h>
#include <cmath>
#include "allegroincludes.h"
#include "share/fsagaconstants.h"
//#include "shared/fsagabitmaps.h"
#include "share/extern_bitmaps.h"			//allows for editor bitmaps access
#include "editor_prototypes.h"	  //file used to store prototypes for all functions in main
#include "share/mapfilestructures.h"
#include "share/crossexecutableprototypes.h" //file used for across-project functions
#include "editoruiconstants.h"


/************GAME WORLD DATA*********************/
//reference to the game sprite's array and the main character sprite variable
extern sprite mainchar;
extern sprite sprites[MAX_NUMBER_NPC_SPRITES];	//sprites that appear in game maps 
extern tag tags[MAX_NUMBER_TAG_TYPES]; //tag types


//extern int collision[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //collision table for scene

extern int backgroundbitmap[100];			//alpha blended bitmaps. Used for backgrouds (e.g., clouds behind a floating island, or trees or hills).
extern int bottombmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //bottom layer bitmap, always rendered under the player
extern int midbmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //mid layer bmp, squares that overlap the player's body (same level as the player)
extern int mid2bmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //another set of mid layer bmps, immediately rendered after the first. they
														//draw wall "patch" bitmaps, things that go over top of walls like pillars, torches, shields, or stairs..
extern int hibmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT];
extern int topbmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //top layer bmps, giant bitmap like clouds in lufia2

extern int tagmap[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //locations of tags. contains ids that reference tag type data structure
extern int collision[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //collision table for scene

extern int blendbmp[100][100]; //blend bmp table Note: Not implemeneted yet - smaller alpha blended bitmaps overtop the scene, will be used for special effects bitmaps





/**************************************************/

//reference the display

extern ALLEGRO_DISPLAY *display;

//reference the map buffer

extern ALLEGRO_BITMAP *map_buffer;

//reference the cursor
extern ALLEGRO_MOUSE_CURSOR *cursor;

extern ALLEGRO_BITMAP *colblock;
extern ALLEGRO_BITMAP *tagblock;


//gauntlet cursor

extern ALLEGRO_BITMAP *gauntlet_cursor;

extern ALLEGRO_BITMAP *gold_cursor; //'snap' cursor

//reference fonts

extern ALLEGRO_FONT *arial_rounded;
extern ALLEGRO_FONT *arial_big;
extern ALLEGRO_FONT *arial_tiny;
extern ALLEGRO_FONT *arial_rounded_convo;

//define the current level file structure


extern FILE *logm;






//note: i'm not sure if i still need this kbdstate 
//define an allegro keyboard state
ALLEGRO_KEYBOARD_STATE kbdstate;

//an array containing the keyboard's keys to track state
//true is pressed, false is not
bool pressed_keys[ALLEGRO_KEY_MAX];


//dialog keyboard stuff

char dialog_keyboard_buffer[30];
int dialog_keyboard_buffer_index = 0;
char filename[30];

//define an allegro timer
ALLEGRO_TIMER *timer;


//define an allegro event queue

ALLEGRO_EVENT_QUEUE *queue;


//Allegro mouse state

ALLEGRO_MOUSE_STATE msestate;

//declare a variable that represents if the player is in the game map loop or not
bool in_game_map_edit = true;



//General editor state variables

int cursor_x_pos = 0; //the cursor's xpos in grid coordinates
int cursor_y_pos = 0; //the cursor's ypos in grid coordinates
int scroll_x = 0;   //the scroll adjustment x variable
int scroll_y = 0;   //the scroll adjustment y variable
int current_layer = 0; //the current layer being used by the cursor. Used for
					   //correct rendering order
int counter_id = 0;    //a revolving counter of the id of the object to be placed. 0 = no object

//
int top_anim_x_roller;
int top_anim_y_roller;

//management

int current_tile_id = 0;

/////////
int current_tile_type = 0;
//

//
int current_tag_id = 0;

//

//sprite identifier for sprite type
int current_sprite_id = 0;

//index into array of sprites for scene
int current_sprite_index = 0;

int sprite_layer_on = 0;

//background bitmap layer info

int current_background_bitmap_id = 0;
int current_background_bitmap_index = 0;

//top layer animated bitmaps info

int current_top_bitmap_id = 0;


//grid info

int grid_width = DEFAULT_GRID_WIDTH;
int grid_height = DEFAULT_GRID_HEIGHT;


//UI state variables



int root_menu_open = ROOT_MENU_NONE;  //only one root menu may be open
int hover_item = NONE; //menu item hovering over




//gold cursor info

int gold_cursor_screen_x = 0;  //position in screen coordiantes
int gold_cursor_screen_y = 0; //position in screen coordiantes

int gold_cursor_map_x = 0; //position in game coordiantes
int gold_cursor_map_y = 0; //position in game coordiantes

//

bool redraw = true;             //redraw the scene when true



//tile management


//dialog switches

int save_dialog_on = 0;
int load_dialog_on = 0;




int main(int argc, char **argv) {


 //open the debugging log
 
	
 if (!al_init())
	 exit(ALLEGRO_INIT_FAIL_ERROR);
 
 if (!al_install_keyboard())
	 exit(INSTALL_KEYBOARD_ERROR);
 
 if (!al_install_mouse())
	 exit(INSTALL_MOUSE_ERROR);
 

 
 if(!al_init_image_addon()) //initializes the add on for loading and saving bitmap files
 	 exit(NO_IMAGE_ADDON_LOAD_FAIL_ERROR); //image add on failed to load
 
 al_init_font_addon(); //initializes the add on for loading and saving bitmap files
						//no return type
  
 al_init_ttf_addon();		//initializes the allegro ttf font addon
	
  

 get_display(); //gets the display bitmaps

 change_homepath_init(); //changes to the games home directory

 init_bitmaps(); //initializes the non-display bitmaps
 load_bitmap_files(); //loads the bitmap file resources
 load_fonts();
 

 load_map(DEFAULTMAP, 2); //loads a game map
 
 if ((timer = al_create_timer(TIMER_INTERVAL)) == NULL)			//create timer used to control frames/sec rendering
	 exit(ALLEGRO_TIMER_CREATE_FAIL_ERROR);


 //create mouse cursor

 if (!(cursor = al_create_mouse_cursor(gauntlet_cursor, 0, 0)))
	 exit(ALLEGRO_CREATE_MOUSE_CURSOR_ERROR);
 
 if (!al_set_mouse_cursor(display, cursor))
 	 exit(ALLEGRO_FAILED_SETTING_MOUSE_CURSOR_ERROR);


 //mouse state



 //create event sources
 if ((queue = al_create_event_queue()) == NULL)
	exit(EVENT_QUEUE_CREATE_FAIL_ERROR);

 al_register_event_source(queue, al_get_keyboard_event_source());
 al_register_event_source(queue, al_get_mouse_event_source());
 al_register_event_source(queue, al_get_timer_event_source(timer));
 al_register_event_source(queue, al_get_display_event_source(display));

 al_start_timer(timer); //start the frames/sec timer

 mainedit(); //main game 
 
 

 destroy_fonts();
 //destroy_bitmap_files(); //destroys the bitmap file resources
 destroy_bitmaps(); //destroys the non-display bitmaps
 destroy_display(); //destroys the game o/s display data structures
 
 al_destroy_event_queue(queue);

 al_shutdown_font_addon();
 al_shutdown_image_addon();
 
 al_uninstall_mouse();
 al_uninstall_keyboard();
 
 
}











//*******************MAIN GAME LOOP************************//

void mainedit()
{
	//NOTE: Slow down player input here, player moves too fast (maybe use separate timer?)
	//bool key_held_input = true;		//gather "held key" input - input where the duration of the key 
								    //press matters NOTE: implement this later to reduce polling
	//main game map loop
	while(in_game_map_edit == true)
	{
		ALLEGRO_EVENT an_event; //define an allegro event
		
		//handle events that occured

		if (al_is_event_queue_empty(queue)) //if the event queue is empty
		{
			if (redraw == true)
			{
				//render the scene
				customrenderer();
				redraw = false;
			}
		}

		//handle methods that occur every cycle,
		//even if event queue is empty

			//pause and wait for an event to occur
		al_wait_for_event(queue, &an_event);

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
			 break;
			case ALLEGRO_EVENT_KEY_CHAR:
				key_held_input(an_event.keyboard.keycode);

				if (save_dialog_on)
				{
					save_dialog_handle_key_input(an_event.keyboard);
				}

				if (load_dialog_on)
				{
					load_dialog_handle_key_input(an_event.keyboard);
				}

				break;


			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				if (an_event.mouse.button == 1) {
					al_get_mouse_state(&msestate);

					//move gold cursor
					int c_x, c_y;
					//al_get_mouse_cursor_position(&c_x, &c_y);



					c_x = al_get_mouse_state_axis(&msestate, 0);
					c_y = al_get_mouse_state_axis(&msestate, 1);

					printf("%d %d ", c_x, c_y);
					relocate_gold_cursor(c_x, c_y);
				}
				break;

			case ALLEGRO_EVENT_MOUSE_AXES:

				//if (an_event.mouse.button == 2) {
					adjust_scroll(an_event.mouse.x, an_event.mouse.y);
				//}
				break;

			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				in_game_map_edit = false;
				break;
			case ALLEGRO_EVENT_TIMER:
			 redraw = true;
             break;
       }
    }		
	logger("exiting main editor loop");
}


//******************Input routines**************************

//registers input once for every time key is pressed down and released

void key_down_up_input(int keycode)
{

	//emergency escape
	if (keycode == ALLEGRO_KEY_ESCAPE)
	{
		bool decision = areyousurebox();
		if (decision)
			in_game_map_edit = false;
	}


	//grid controls
	else if (keycode == ALLEGRO_KEY_EQUALS)
	{
		if (grid_width+1 <= SCREEN_WIDTH && grid_height+1 <= SCREEN_HEIGHT) {
			grid_width++;
			grid_height++;
		}

	}

	else if (keycode == ALLEGRO_KEY_MINUS)
	{
		if (grid_width -1 >= WIDTH_MIN && grid_height-1 >= HEIGHT_MIN)
		{
			grid_width--;
			grid_height--;
		}
	}

	else if (keycode == ALLEGRO_KEY_OPENBRACE)
	{
		if (sprite_layer_on)
		{
			cycle_sprite_ids(-1);
		}
		else if (current_layer == 0) {
			cycle_background_image(-1);
		}
		else if (current_layer == 6)
		{
			//tag layer
			cycle_tags(-1);
		}
		else if (current_layer == 5) //top bmp
		{
			cycle_top_bmps(-1);
		}
		else
		{
			cycle_tiles(-1);
		}
	}


	else if (keycode == ALLEGRO_KEY_CLOSEBRACE)
	{
		if (sprite_layer_on)
		{
			cycle_sprite_ids(1);
		}
		else if (current_layer == 0) {
			cycle_background_image(-1);
		}

		else if (current_layer == 6)
		{
			cycle_tags(1);
		}
		else if (current_layer == 5) //top bmp
		{
			cycle_top_bmps(1);
		}
		else
		{
			cycle_tiles(1);
		}

	}

	else if (keycode == ALLEGRO_KEY_Z)
	{
		if (current_layer == 0) {
			cycle_background_image_index(-1);
			return;
		}
		cycle_sprite_index(-1);

	}


	else if (keycode == ALLEGRO_KEY_X)
	{

		if (current_layer == 0) {
			cycle_background_image_index(1);
			return;
		}

		cycle_sprite_index(1);
	}

	else if (keycode == ALLEGRO_KEY_R)
	{
		al_get_keyboard_state(&kbdstate);
		if (al_key_down(&kbdstate, ALLEGRO_KEY_LSHIFT))
		{
			//reset grid width and height to default
			grid_width = DEFAULT_GRID_WIDTH;
			grid_height = DEFAULT_GRID_HEIGHT;
		}
	}

	else if (keycode == ALLEGRO_KEY_O)
	{
		al_get_keyboard_state(&kbdstate);
		if (al_key_down(&kbdstate, ALLEGRO_KEY_LSHIFT))
		{
			//reset scroll coordinates
			scroll_x = DEFAULT_SCROLL_X;
			scroll_y = DEFAULT_SCROLL_Y;
		}

	}

	else if (keycode == ALLEGRO_KEY_S)
	{
		al_get_keyboard_state(&kbdstate);
		if (al_key_down(&kbdstate, ALLEGRO_KEY_LSHIFT))
		{
			snap_gold_cursor();
		}
	}

	else if (keycode == ALLEGRO_KEY_ENTER)
	{
		//place currently visible tile at gold cursor location
		//places currently selected tile in map
		place_tiles();
	}
	else if (keycode == ALLEGRO_KEY_C)
	{
		al_get_keyboard_state(&kbdstate);
		if (al_key_down(&kbdstate, ALLEGRO_KEY_LSHIFT))
		{
			//place currently visible tile at gold cursor location
			//places currently selected tile in map
			clear_tiles();
		}
	}
	else if (keycode == ALLEGRO_KEY_L)
	{
		cycle_layers();
	}
	else if (keycode == ALLEGRO_KEY_P)
	{
		flip_spritelayer();
	}

	else if (keycode == ALLEGRO_KEY_K)
	{
		al_get_keyboard_state(&kbdstate);
		if (al_key_down(&kbdstate, ALLEGRO_KEY_LSHIFT))
		{
			//clear entire map
			if (areyousurebox())
			{
				printf("Clearing map");

				clearmap();
			}
		}
	}
	else if (keycode == ALLEGRO_KEY_V)
	{
		al_get_keyboard_state(&kbdstate);
		if (al_key_down(&kbdstate, ALLEGRO_KEY_LSHIFT))
		{
			save_dialog_on = 1;
			clear_bufffered_input_string();

		}
	}
	else if (keycode == ALLEGRO_KEY_D)
	{
		al_get_keyboard_state(&kbdstate);
		if (al_key_down(&kbdstate, ALLEGRO_KEY_LSHIFT))
		{
			load_dialog_on = 1;
			clear_bufffered_input_string();
		}
	}

	else if (keycode == ALLEGRO_KEY_1)
	{
		al_get_keyboard_state(&kbdstate);
		if (al_key_down(&kbdstate, ALLEGRO_KEY_LSHIFT))
		{
			if (areyousurebox())
			{
				if (!is_empty_string(filename, 30))
					save_map_v2(filename);
			}
		}
	}
}



bool is_empty_string(char *filename, int length)
{

	for (int i = 0; i < length; i++)
	{
		if (filename[i] != ' ')
		{
			return false;
		}
	}
	return true;
}


//registers input for as long as the key is held

void key_held_input(int keycode)
{
	logger("key_held_input(): checking input");
	logger("key code is:");
	logint(keycode);


	if (keycode == ALLEGRO_KEY_LEFT)
	{
		al_get_keyboard_state(&kbdstate);
		if (al_key_down(&kbdstate, ALLEGRO_KEY_LSHIFT))
		{
			move_gold_cursor(-1 * CURSOR_SPEED * 2, 0);

		}
		else {
			move_gold_cursor(-1 * CURSOR_SPEED * 2, 0);
			scroll_left(CURSOR_SPEED*2);
		}
	}


	else if (keycode == ALLEGRO_KEY_RIGHT)
	{
		al_get_keyboard_state(&kbdstate);
		if (al_key_down(&kbdstate, ALLEGRO_KEY_LSHIFT))
		{

			move_gold_cursor(CURSOR_SPEED * 2, 0);
		}

		else {
			move_gold_cursor(CURSOR_SPEED * 2, 0);

			scroll_right(CURSOR_SPEED*2);
		}

	}

	else if (keycode == ALLEGRO_KEY_UP)
	{
		al_get_keyboard_state(&kbdstate);
		if (al_key_down(&kbdstate, ALLEGRO_KEY_LSHIFT))
		{
			move_gold_cursor(0, -1 * CURSOR_SPEED * 2);
		}
		else {
			move_gold_cursor(0, -1 * CURSOR_SPEED * 2);

			scroll_up(CURSOR_SPEED*2);

		}

	}

	else if (keycode == ALLEGRO_KEY_DOWN)
	{
		al_get_keyboard_state(&kbdstate);
		if (al_key_down(&kbdstate, ALLEGRO_KEY_LSHIFT))
		{
			move_gold_cursor(0, CURSOR_SPEED * 2);
		}
		else
		{
			move_gold_cursor(0, CURSOR_SPEED * 2);
			scroll_down(CURSOR_SPEED*2);
		}
	}

// else if (keycode == ALLEGRO_KEY_A)
// {
//	 al_get_keyboard_state(&kbdstate);
//	 if (al_key_down(&kbdstate, ALLEGRO_KEY_SPACE))
//	 {
//
//		 move_gold_cursor(-1 * CURSOR_SPEED * 2, 0);
//	 }
// }
//
// else if (keycode == ALLEGRO_KEY_S)
// {
//	 al_get_keyboard_state(&kbdstate);
//	 if (al_key_down(&kbdstate, ALLEGRO_KEY_SPACE))
//	 {
//
//		 move_gold_cursor(1 * CURSOR_SPEED * 2, 0);
//	 }
// }
//
// else if (keycode == ALLEGRO_KEY_Z)
// {
//	 al_get_keyboard_state(&kbdstate);
//	 if (al_key_down(&kbdstate, ALLEGRO_KEY_SPACE))
//	 {
//		 move_gold_cursor(0, 1 * CURSOR_SPEED * 2);
//	 }
// }
//
// else if (keycode == ALLEGRO_KEY_W)
// {
//	 al_get_keyboard_state(&kbdstate);
//	 if (al_key_down(&kbdstate, ALLEGRO_KEY_SPACE))
//	 {
//		 move_gold_cursor(0, -1 * CURSOR_SPEED * 2);
//	 }
// }


/*
 if (key[KEY_INSERT])
 {
  tagchk();
  spritechk();
 }
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
 
 //if (al_key_down(&kbdstate, ALLEGRO_KEY_ESCAPE))
 //{
 // in_game_map_loop = false;
 //}
 */
}
//


void customrenderer()
{
	clear(map_buffer); //will set target to map buffer and clear it
	
	//render background bitmap

	render_back_bmps();


	//render game map and sprites
	renderbottom(); //render floor

	//if (current_layer == TILE_LAYER_BOTTOM)
	//	rendercursor();
	rendermid(); //render tiles below player (generally wall tiles)
	//if (current_layer == TILE_LAYER_MIDDLE1 || current_layer == TILE_LAYER_MIDDLE2)
	//	rendercursor();
	render_player_sprite(); //render player (was mrendersprite();)
	render_npc_sprite(); //renders the npc sprites (was rendersprite();)
	//if (current_layer == TILE_LAYER_SPRITE)
	//	rendercursor();
	renderhi(); //render tiles above player (generally ceiling tiles)
	//if (current_layer == TILE_LAYER_HI)
	//	rendercursor();
	rendercollision();
	rendertagmap();
	rendersprites();

	custom_rendertop(); //Note: render top layer, things such as clouds or fog -- new feature
	// */

	rendercursor();
	rendergrid();
	renderborder();
	//rendercursor
	rendercursor();

	blitter(); ////renders map buffer to back buffer and flips the back buffer with the screen
}

void rendercursor(void) //draws a simple editing cursor at cursor_xpos, cursor_ypos
{
	if (gold_cursor == NULL)
		exit(DRAW_GOLD_CUSOR_NULL_FAIL_ERROR);
	al_draw_bitmap(gold_cursor,
	    gold_cursor_map_x, gold_cursor_map_y, NULL);


}

void renderUI(void)
{
	//begin drawing text - change blender//
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
	//draw version info
	al_draw_textf(arial_rounded, WHITE_COLOR, 10, 0, ALLEGRO_ALIGN_LEFT, "Allegro Based FSAGA-D Type Game Editor Alpha %d", GAME_EDITOR_VERSION);
	
	// root menus in appropriate color

	ALLEGRO_COLOR menu_color = NO_FOCUS_COLOR;

	root_change_colors(ROOT_MENU_FILE);

	al_draw_textf(arial_rounded, menu_color, FILE_ORIGIN, ROOT_MENU_ORIGIN_Y, ALLEGRO_ALIGN_LEFT, FILE_TEXT);

	root_change_colors(ROOT_MENU_VIEW);

	al_draw_textf(arial_rounded, menu_color, VIEW_ORIGIN, ROOT_MENU_ORIGIN_Y, ALLEGRO_ALIGN_LEFT, VIEW_TEXT);

	root_change_colors(ROOT_MENU_TILES);

	al_draw_textf(arial_rounded, menu_color, TILES_ORIGIN, ROOT_MENU_ORIGIN_Y, ALLEGRO_ALIGN_LEFT, TILES_TEXT);

	root_change_colors(ROOT_MENU_LAYERS);

	al_draw_textf(arial_rounded, menu_color, LAYERS_ORIGIN, ROOT_MENU_ORIGIN_Y, ALLEGRO_ALIGN_LEFT, LAYERS_TEXT);

	root_change_colors(ROOT_MENU_COLLISION);

	al_draw_textf(arial_rounded, menu_color, COLLISION_ORIGIN, ROOT_MENU_ORIGIN_Y, ALLEGRO_ALIGN_LEFT, COLLISION_TEXT);

	root_change_colors(ROOT_MENU_TAGS);

	al_draw_textf(arial_rounded, menu_color, TAGS_ORIGIN, ROOT_MENU_ORIGIN_Y, ALLEGRO_ALIGN_LEFT, TAGS_TEXT);

	root_change_colors(ROOT_MENU_TILES);

	al_draw_textf(arial_rounded, menu_color, TILES_ORIGIN, ROOT_MENU_ORIGIN_Y, ALLEGRO_ALIGN_LEFT, TILES_TEXT);

	root_change_colors(ROOT_MENU_SPRITES);

	al_draw_textf(arial_rounded, menu_color, SPRITES_ORIGIN, ROOT_MENU_ORIGIN_Y, ALLEGRO_ALIGN_LEFT, SPRITES_TEXT);
	
	root_change_colors(ROOT_MENU_BATTLES);

	al_draw_textf(arial_rounded, menu_color, BATTLES_ORIGIN, ROOT_MENU_ORIGIN_Y, ALLEGRO_ALIGN_LEFT, BATTLES_TEXT);
		
	//end drawing text - restore blender//
	al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
}

void rendergrid(void)
{

	//adjusted window coordiantes for scrolling factors
	int width_min = scroll_x*SCROLL_SCALING_FACTOR_X;
	int height_min = scroll_y*SCROLL_SCALING_FACTOR_Y;
	int width_max = width_min + SCREEN_WIDTH;
	int height_max = width_max + SCREEN_HEIGHT;


	for (int x = width_min; x < width_max; x += grid_width)
	{
		for (int y = height_min; y < height_max; y+= grid_height)
		{
			//horizontal dashes
			for (int k = 0; k < grid_width; k++)
			{
				//horizontal dashes
				al_put_pixel(x+k, y,GREY_COLOR);



				//vetical dashes
				al_put_pixel(x, y+k, GREY_COLOR);
			}


		}
	}

}

void renderborder()
{
	//horizontal lines
	for (int x = WIDTH_MIN; x < MAX_MAP_WIDTH; x++)
	{
		al_put_pixel(x, HEIGHT_MIN, BLUE_COLOR);
		al_put_pixel(x, MAX_MAP_HEIGHT-2, BLUE_COLOR);
	}

	//vertical lines
	for (int y = HEIGHT_MIN; y < MAX_MAP_HEIGHT; y++)
	{
		al_put_pixel(WIDTH_MIN, y, BLUE_COLOR);
		al_put_pixel(MAX_MAP_WIDTH-2, y, BLUE_COLOR);
	}
}

ALLEGRO_COLOR root_change_colors(int menutype)
{
	ALLEGRO_COLOR menu_color;
    if (hover_item == menutype)
		menu_color = HOVER_COLOR;
	else if (root_menu_open == menutype)
		menu_color = HAS_FOCUS_COLOR;
	else
		menu_color = NO_FOCUS_COLOR;
	return menu_color;
}


void renderInterfaceLayer() { //render a static, interface layer over the moving screen

	//this function is for interfaces and images that do not change in value
	//as the screen is scrolled from left to right, etc.

	//render UI overtop of everything else
	renderUI();
	renderCurrentTileDisplay();
	renderTopLayerDisplay();
	renderCurrentSpriteDisplay();
	renderCurrentLayerName();

	//render dialogs if active
	if (save_dialog_on)
	{
		save_dialog_handle_rendering();
	}

	if (load_dialog_on)
	{
		load_dialog_handle_rendering();
	}

	//

	if (current_layer == 0)
	{
		renderBackgroundBitmapName();
	}



	renderCursorData(); //draws the cursor pos.
	//blit


	//rendercursor
	//rendercursor();

}

void renderCursorData()
{

	al_draw_textf(arial_rounded, WHITE_COLOR, 10, 420, ALLEGRO_ALIGN_LEFT, "Gold Cursor Map: ", NULL);
	al_draw_textf(arial_rounded, WHITE_COLOR, 200, 420, ALLEGRO_ALIGN_LEFT, "X: %d", gold_cursor_map_x);
	al_draw_textf(arial_rounded, WHITE_COLOR, 280, 420, ALLEGRO_ALIGN_LEFT, "Y: %d", gold_cursor_map_y);

	al_draw_textf(arial_rounded, WHITE_COLOR, 10, 450, ALLEGRO_ALIGN_LEFT, "Gold Cursor Screen: ", NULL);
	al_draw_textf(arial_rounded, WHITE_COLOR, 220, 450, ALLEGRO_ALIGN_LEFT, "X: %d", gold_cursor_screen_x);
	al_draw_textf(arial_rounded, WHITE_COLOR, 280, 450, ALLEGRO_ALIGN_LEFT, "Y: %d", gold_cursor_screen_y);


}

void renderCurrentLayerName()
{
	// layer 0 backgroundbitmap[] background bitmap Alpha Blended Background Bitmap - NOT IMPLEMENTED YET (Used for backgrounds like rolling hills,etc.)
	// layer 1 bottombmp[][] Floor Tile (Used to make floors, on a level below the player)
	// layer 2 midbmp[][] Wall Tile (Used to make walls, on the same level as the player)
	// layer 3 mid2bmp[][] Wall-Patch Tiles (Used to make things like pillars, or torches on walls)
	// layer 4 hibmp[][] Ceiling Tiles (Used to make ceiling borders for levels)
	// layer 5 topbmp[][] Top Tiles (Used for large effects like clouds, fog, etc..)
	// layer 6 tagmap[][] Tag Layer (Used to place tag blocks)
	// layer 7 collision[][] Collision Layer (Used to place collision blocks)
	// layer 8 blendbmp[][] Blend Bitmap (Used for special effects. Not implemented yet)


	char name[30];

	if (current_layer == 0)
	{
		strcpy(name, "Background Bmp Large Imgs");
	}
	if (current_layer == 1)
	{
		strcpy(name, "Bottom Floor Tiles");

	}
	if (current_layer == 2)
	{
		strcpy(name, "Mid Bmp Wall Tiles");

	}
	if (current_layer == 3)
	{
		strcpy(name, "Mid2 Bmp Wall Patch");

	}
	if (current_layer == 4)
	{
		strcpy(name, "Hi Bmp Ceiling Tiles");

	}
	if (current_layer == 5)
	{
		strcpy(name, "Top Bmp Large Imgs");

	}
	if (current_layer == 6)
	{
		strcpy(name, "Tag Blocks Layer");

	}
	if (current_layer == 7)
	{
		strcpy(name, "Collision Blocks Lyr");

	}
	if (current_layer == 8)
	{
		strcpy(name, "Special Effects Blend Lyr");

	}

	al_draw_textf(arial_rounded, YELLOW_COLOR, 350, 420, ALLEGRO_ALIGN_LEFT, name, NULL);


	//render sprite layer
	if (sprite_layer_on)
	{
		al_draw_textf(arial_rounded, YELLOW_COLOR, 350, 440, ALLEGRO_ALIGN_LEFT, "Sprite Layer Insert", NULL);
	}


}



void renderTopLayerDisplay()
{
	int value = current_top_bitmap_id;


	ALLEGRO_BITMAP *top_bitmap;

	switch (value) {
	case 1:
		top_bitmap = dmb_cloud;
		break;
	case 2:
		top_bitmap = fmb_cloud;
		break;
	case 3:
		top_bitmap = lcb_cloud;
		break;
	case 4:
		top_bitmap = smb_cloud;
		break;
	case 5:
		top_bitmap = smfb_cloud;
		break;
	default:
		break;
	}

	if (value > 0) {
		al_draw_scaled_bitmap(top_bitmap,
				0,0,al_get_bitmap_width(top_bitmap), al_get_bitmap_height(top_bitmap),
				546, 90,
				70, 70, NULL);
	}
}





//input screen coordinates c_x, c_y
int relocate_gold_cursor(int c_x, int c_y)
{


	int scn_x = screen_to_map(c_x, 0);
	int scn_y = screen_to_map(c_y, 1);


	//adjust c_x and c_y for scroll value

	//convert screen coordiantesto map

	//c_x += width_min;
	//c_y += height_min;


	//if gold cursor is below menu bar bottom boundary
	if (scn_y > ROOT_MENU_BOTTOM_BOUNDARY)
	{


		//if screen coordiantes are within screen bounds
		if (c_x > WIDTH_MIN && c_x < WIDTH_MAX && c_y > HEIGHT_MIN && c_y < HEIGHT_MAX)
		{

			gold_cursor_screen_x = c_x;
			gold_cursor_screen_y = c_y;
			gold_cursor_map_x = scn_x;
			gold_cursor_map_y = scn_y;

			//map coordinates
			//center_scrolling(scn_x, scn_y);
		}

		//gold_cursor_val_x = scroll_x*SCROLL_SCALING_FACTOR_X + c_x; //position in game coordiantes
		//gold_cursor_val_y = scroll_y*SCROLL_SCALING_FACTOR_Y + c_y; //position in game coordiantes

	}
	//outside of screen bounds

	return 0;

}



int move_gold_cursor(int dx, int dy)
{
	if (((gold_cursor_map_x + dx) > WIDTH_MIN && (gold_cursor_map_x + dx) < MAX_MAP_WIDTH) &&
			((gold_cursor_map_y + dy) > HEIGHT_MIN && (gold_cursor_map_y + dy) < MAX_MAP_HEIGHT))
	{

			printf("moving gold cursor");
			gold_cursor_map_x += dx;
			gold_cursor_map_y += dy;

			gold_cursor_screen_x = map_to_screen(gold_cursor_map_x, 0);
			gold_cursor_screen_y = map_to_screen(gold_cursor_map_y, 1);


	}


	return 0;
}


void snap_gold_cursor()
{
	//snap up and left
	int temp_x = gold_cursor_screen_x;
	int temp_y = gold_cursor_screen_y;

	while ((temp_x % grid_width != 0) && (temp_y % grid_height != 0)) {
		if (temp_x % grid_width != 0)
			{
				temp_x--;  ///decrease until is a multiple of the grid
			}
		if (temp_y % grid_height != 0) {
			temp_y--;  ///decrease until is a multiple of hte grid
		}

	}

	move_gold_cursor(temp_x, temp_y);


}



//given x and y coordaintes of the golden cursor to scroll to
//by using the keyboard
//x and y are map coordiantes of the golden cursor
void scroll_to(int x, int y)
{

	/*
	//does x or y take us past the border?


	int x_left_border = 0;//scroll_x*SCROLL_SCALING_FACTOR_X;
	int x_right_border = SCREEN_WIDTH;//x_left_border + SCREEN_WIDTH;

	int y_top_border = 0;//scroll_y*SCROLL_SCALING_FACTOR_Y;
	int y_bottom_border = SCREEN_HEIGHT;//y_top_border + SCREEN_HEIGHT;


	//borders with some buffered space

	int x_left_map_border = SCREEN_ORIGIN_X;
	int x_right_map_border = MAX_MAP_WIDTH;
	int y_top_map_border =   SCREEN_ORIGIN_Y;
	int y_bottom_map_border = MAX_MAP_HEIGHT;

	int x_left_screen_border = SCREEN_ORIGIN_X+BUFFERSPACE;
	int x_right_screen_border = SCREEN_WIDTH - BUFFERSPACE;
	int y_top_screen_border =   SCREEN_ORIGIN_Y + BUFFERSPACE;
	int y_bottom_screen_border = SCREEN_HEIGHT - BUFFERSPACE;

	scroll_x;
	scroll_y;



	if (scroll_x < x)
	{
		for (int x_s = 0; x_s < x; x_s++) {
			scroll_x++;
		}
	}
	if (scroll_x > x)
	{
		for (int x_s = x; x > 0; x_s--) {
			scroll_x--;
		}
	}


	if (scroll_y < y)
	{
		for (int y_s = 0; y_s < y; y_s++) {
			scroll_y++;
		}
	}

	if (scroll_y > y) {
		for (int y_s = y; y_s > 0; y_s--); {
			scroll_y--;
		}
	}

*/


}

//used by mouse to adjust scroll
//input is screen coordinates of mosue cursor
void adjust_scroll(int x, int y)
{

	int x_left_map_border = SCREEN_ORIGIN_X;
	int x_right_map_border = MAX_MAP_WIDTH;
	int y_top_map_border =   SCREEN_ORIGIN_Y;
	int y_bottom_map_border = MAX_MAP_HEIGHT;

	int x_left_screen_border = SCREEN_ORIGIN_X+BUFFERSPACE;
	int x_right_screen_border = SCREEN_WIDTH - BUFFERSPACE;
	int y_top_screen_border =   SCREEN_ORIGIN_Y + BUFFERSPACE;
	int y_bottom_screen_border = SCREEN_HEIGHT - BUFFERSPACE;


	if (x >= x_right_screen_border)
	{
		if (x < x_right_map_border)
		{
			scroll_right(1);
			return;
		}
	}

	if (x <= x_left_screen_border)
	{

		if (x > x_left_map_border)
		{
			scroll_left(1);
			return;
		}

	}

	if (y <= y_top_screen_border)
	{
		if (y > y_top_map_border)
		{
			scroll_up(1);
			return;
		}
	}

	if (y >= y_bottom_screen_border)
	{
		if (y < y_bottom_map_border)
		{
			scroll_down(1);
			return;
		}
	}

}


void scroll_right(int amt)
{
	int currentx = scroll_x*SCROLL_SCALING_FACTOR_X;

	currentx += amt;

	int maxx = MAX_MAP_WIDTH;

	if ((currentx+SCREEN_WIDTH) <= maxx)
	{
		printf("Maxx: %d", maxx);
		printf("CurrentX: %d", currentx);
		scroll_x+=amt;
	}
	else
	{
		printf("PAST");
		printf("Maxx: %d", maxx);
		printf("CurrentX: %d", currentx);

		//scroll_x = maxx-SCREEN_WIDTH - 2;
	}
	return;
}

void scroll_left(int amt)
{

	int currentx = scroll_x*SCROLL_SCALING_FACTOR_X;

	currentx -= amt;

	int minx = WIDTH_MIN;
	int maxx = MAX_MAP_WIDTH;


	if (currentx >= minx)
	{
		scroll_x-=amt;
	}
	else
	{
		scroll_x = minx+2;
	}
	return;


}

void scroll_up(int amt)
{
	int currenty = scroll_y*SCROLL_SCALING_FACTOR_Y;

	currenty -= amt;


	int miny = HEIGHT_MIN;
	int maxy = MAX_MAP_HEIGHT;

	if (currenty >= miny)
	{
		scroll_y-=amt;
	}
	else
	{
		scroll_y = miny+2;
	}
	return;

}

void scroll_down(int amt)
{
	int currenty = scroll_y*SCROLL_SCALING_FACTOR_Y;

	currenty += amt;

	int miny = HEIGHT_MIN;
	int maxy = MAX_MAP_HEIGHT;

	if ((currenty+SCREEN_HEIGHT) <= maxy)
	{
		scroll_y+=amt;
	}
	else
	{
		//scroll_y = maxy-2;
	}
	return;

}

//flag - 0 is x, 1 is y
int screen_to_map(int screen, int flag)
{
	int width_min = scroll_x*SCROLL_SCALING_FACTOR_X;
	int height_min = scroll_y*SCROLL_SCALING_FACTOR_Y;

	if (flag == 0) //x coordinate
	{
		screen = screen + width_min;
	}

	else if (flag == 1) {
		screen = screen + height_min;
	}
	return screen;
}

int map_to_screen(int map, int flag)
{
	int width_min = scroll_x*SCROLL_SCALING_FACTOR_X;
	int height_min = scroll_y*SCROLL_SCALING_FACTOR_Y;

	if (flag == 0)
	{
		map = map - width_min;
	}

	if (flag == 1)
	{
		map = map - height_min;
	}
	return map;

}

//
void center_scrolling(int x, int y) //centers screen around input (map) coordinates
{

/*
	printf("\n\n\nscrollx %d scrolly %d", scroll_x, scroll_y);

	//int sx = x - (int)(floor(SCREEN_WIDTH/2));
	///int sy = y - (int)(floor(SCREEN_HEIGHT/2));

	scroll_right(x/SCROLL_SCALING_FACTOR_X);

	printf("\n\n\nscrollx %d scrolly %d", scroll_x, scroll_y);
*/
/*
	int new_x = x;
	int new_y = y;

	int new_origin_x = new_x - (int)(floor(SCREEN_WIDTH/2));
	int new_origin_y = new_y - (int)(floor(SCREEN_HEIGHT/2));

	int dx = new_origin_x - scroll_x;
	int dy = new_origin_y = scroll_y;

	printf(" Scroll-down %d", dy);
	printf(" Scroll-right %d", dx);

	//scroll_down(dy);
	scroll_x = dx;
	//scroll_y = dy;
	//scroll_right(dx);

*/

	//centering "magic number" routine (centers around x or y0

	int scn_x = map_to_screen(x, 0);
	int scn_y = map_to_screen(y, 1);


	scroll_x = scn_x-SCROLL_CENTER_MAGIC_X;
	scroll_y = scn_y-SCROLL_CENTER_MAGIC_Y;


///	printf("x %d y %d", x, y);

	redraw = true;

	/*
	//map coordinates
	int oldx_mid = scroll_x + (int)floor(SCREEN_WIDTH/2);
	int oldy_mid = scroll_y + (int)floor(SCREEN_HEIGHT/2);

	int scroll_amt_x = x - oldx_mid;
	int scroll_amt_y = y - oldy_mid;

	//scroll_x = oldx_mid + scroll_amt_x;
	//scroll_y = oldy_mid + scroll_amt_y;

	if (scroll_amt_x > 0) {
		//scroll_right(scroll_amt_x);
		scroll_x += scroll_amt_x;
	}

	else if (scroll_amt_x < 0) {
		scroll_x += scroll_amt_x;
		//scroll_left(-1*scroll_amt_x);
	}
	if (scroll_amt_y > 0) {
		scroll_down(scroll_amt_y);
	}
	else if (scroll_amt_y < 0) {
		scroll_up(-1*scroll_amt_y);
	}
*/
	//printf("\nx %d y %d\n", x, y);
	//printf("\noldxmid %d oldymid %d\n", oldx_mid, oldy_mid);
	//printf("\nscrllamtx %d scrollamty %d", scroll_amt_x, scroll_amt_y);

}

void renderCurrentTileDisplay()
{

	ALLEGRO_BITMAP *current_tile;


	//create blank tile

	//background- create white screen to display tiles on
	ALLEGRO_BITMAP *blank_tile;
	blank_tile = al_create_bitmap(TILE_X, TILE_Y);

	al_set_target_bitmap(blank_tile);

	//clear tile bitmap
	al_clear_to_color(BLACK_COLOR);

	al_set_target_bitmap(al_get_backbuffer(display));


	switch (current_tile_id)
	{
		case 1:
			current_tile = floor1;
			break;
		case 2:
			current_tile = floor2;
			break;
		case 3:
			current_tile = wall1;
			break;
		case 4:
			current_tile = wall2;
			break;
		case 5:
			current_tile = wall3;
			break;
		case 6:
			current_tile = wall4;
			break;
		case 7:
			current_tile = chest;
			break;
		case 8:
			current_tile = chest2;
			break;
		case 9:
			current_tile = beda1;
			break;

		case 10:
			current_tile = door1;
			break;

		case 11:
			current_tile = arch1;
			break;

		case 12:
			current_tile = arch2;
			break;

		case 13:
			current_tile = stairs1;
			break;

		case 14:
			current_tile = stairs2;
			break;

		case 15:
			current_tile = top1;
			break;

		case 16:
			current_tile = top2;
			break;

		case 17:
			current_tile = shield1;
			break;

		case 18:
			current_tile = torch1;
			break;

		case 19:
			current_tile = torch2;
			break;

		case 20:
			current_tile = pill1;
			break;
		default:
			current_tile = blank_tile;
			break;

	}

	//if layer is collision block layer
	if (current_layer == 6) {
		current_tile = tagblock;
	}

	if (current_layer == 7) {
		current_tile = colblock;
	}

	if (current_tile == NULL)
		exit(LOAD_TILE_BITMAP_FAILURE);
	//draw current tile to upper rihgt corner


	//background- create white screen to display tiles on
	ALLEGRO_BITMAP *tile_back;
	tile_back = al_create_bitmap(TILE_X+5, TILE_Y+5);

	al_set_target_bitmap(tile_back);

	//clear tile bitmap
	al_clear_to_color(WHITE_COLOR);

	al_set_target_bitmap(al_get_backbuffer(display));

	//draw tile

	al_draw_scaled_bitmap(tile_back,
		    0,0,al_get_bitmap_width(tile_back), al_get_bitmap_height(tile_back),
			560, 47, TILE_DEST_SIZE_WIDTH+5, TILE_DEST_SIZE_HEIGHT+5, NULL);

	al_draw_scaled_bitmap(current_tile,
	    0,0,al_get_bitmap_width(current_tile), al_get_bitmap_height(current_tile),
		564, 49, TILE_DEST_SIZE_WIDTH, TILE_DEST_SIZE_HEIGHT, NULL);

	if (current_layer == 6)
	{
		al_draw_textf(arial_rounded, WHITE_COLOR, 564, 49, ALLEGRO_ALIGN_LEFT, "id#: %d", current_tag_id);
	}


}


void renderCurrentSpriteDisplay()
{
	//sprite layer is not on
	if (sprite_layer_on == false)
	{
		return;
	}

	ALLEGRO_BITMAP *current_sprite;


	//create blank tile

	//background- create white screen to display tiles on
	ALLEGRO_BITMAP *blank_tile;
	blank_tile = al_create_bitmap(TILE_X, TILE_Y);

	al_set_target_bitmap(blank_tile);

	//clear tile bitmap
	al_clear_to_color(BLACK_COLOR);

	al_set_target_bitmap(al_get_backbuffer(display));


	switch (current_sprite_id)  //bmpid, sprite id 'type'
	{
		case 1:
			//bmp1 = player
			current_sprite = serf1;
			break;
		case 2:
			//bmp2 = ninja sprite
			current_sprite = ninjaf1;
			break;
		case 3:
			//bmp2 = blue knight
			current_sprite = bluf1;
			break;
		default:
			current_sprite = blank_tile;
			break;

	}

		if (current_sprite == NULL)
		exit(LOAD_TILE_BITMAP_FAILURE);
	//draw current tile to upper rihgt corner


	//background- create white screen to display tiles on
	ALLEGRO_BITMAP *tile_back;
	tile_back = al_create_bitmap(TILE_X+5, TILE_Y+5);

	al_set_target_bitmap(tile_back);

	//clear tile bitmap
	al_clear_to_color(WHITE_COLOR);

	al_set_target_bitmap(al_get_backbuffer(display));

	//draw tile

	al_draw_scaled_bitmap(tile_back,
		    0,0,al_get_bitmap_width(tile_back), al_get_bitmap_height(tile_back),
			560, 100, TILE_DEST_SIZE_WIDTH+5, TILE_DEST_SIZE_HEIGHT+5, NULL);

	al_draw_scaled_bitmap(current_sprite,
	    0,0,al_get_bitmap_width(current_sprite), al_get_bitmap_height(current_sprite),
		564, 100, TILE_DEST_SIZE_WIDTH, TILE_DEST_SIZE_HEIGHT, NULL);


	//draw sprite info text for window
		al_draw_textf(arial_rounded, RED_COLOR, 534, 150, ALLEGRO_ALIGN_LEFT, "id#: %d", current_sprite_id);
		al_draw_textf(arial_rounded, RED_COLOR, 534, 180, ALLEGRO_ALIGN_LEFT, "inx#: %d", current_sprite_index);



}



void renderBackgroundBitmapName() {

	if (!sprite_layer_on)
		al_draw_textf(arial_rounded, WHITE_COLOR, 350, 450, ALLEGRO_ALIGN_LEFT, "B-ground BMP Select ID#: %d ", current_background_bitmap_id);

}



void cycle_tiles(int amt)
{
	current_tile_id = current_tile_id + amt;

	if (current_tile_id > TILEMAX) {
		current_tile_id = 0;
	} else if (current_tile_id < 0) {
		current_tile_id = TILEMAX;
	}

}

void cycle_top_bmps(int amt)
{
	current_top_bitmap_id += amt;

	if (current_top_bitmap_id > TOPBMPMAX)
	{
		current_top_bitmap_id = 0;
	}

	if (current_top_bitmap_id < 0) {
		current_top_bitmap_id = TOPBMPMAX;
	}
}

void cycle_tags(int amt)
{
	current_tag_id = current_tag_id + amt;

	if (current_tag_id < 0)
		current_tag_id = 0;

}


void cycle_sprite_ids(int amt)
{
	current_sprite_id = current_sprite_id + amt;
	if (current_sprite_id < 0)
		current_sprite_id = 0;
}

void cycle_sprite_index(int amt)
{
	current_sprite_index += amt;

	if (current_sprite_index > MAX_NUMBER_NPC_SPRITES) {
		current_sprite_index = 0;
	} else if (current_sprite_index < 0) {
		current_sprite_index = MAX_NUMBER_NPC_SPRITES;
	}

}


void place_tiles()
{
	int x = (int)floor(gold_cursor_map_x/TILE_SCALE_FACTOR_X - TILE_TRANSLATE_FACTOR_X);
	int y = (int)floor(gold_cursor_map_y/TILE_SCALE_FACTOR_Y - TILE_TRANSLATE_FACTOR_Y);



	if (sprite_layer_on) {
		sprites[current_sprite_index].xpos = x;
		sprites[current_sprite_index].ypos = y;
		sprites[current_sprite_index].bmpid = current_sprite_id;
	}

	if (current_layer == 0) {
		backgroundbitmap[current_background_bitmap_index] = current_background_bitmap_id;
	}

	switch(current_layer)
	{
		case 1:
			bottombmp[x][y] = current_tile_id;
			break;
		case 2:
			midbmp[x][y] = current_tile_id;
			break;
		case 3:
			mid2bmp[x][y] = current_tile_id;
			break;
		case 4:
			hibmp[x][y] = current_tile_id;
			break;
		case 5:
			topbmp[x][y] = current_top_bitmap_id;
			break;
		case 6:
			tagmap[x][y] = current_tag_id;
			break;
		case 7:
			if (collision[x][y] != 1)
				collision[x][y] = 1;
			else
				collision[x][y] = 0;
			break;
		default:
			break;
	}

}

void clear_tiles()
{

	int x = (int)floor(gold_cursor_map_x/TILE_SCALE_FACTOR_X - TILE_TRANSLATE_FACTOR_X);
	int y = (int)floor(gold_cursor_map_y/TILE_SCALE_FACTOR_Y - TILE_TRANSLATE_FACTOR_Y);


	if (sprite_layer_on) {
		for (int i=0;i<MAX_NUMBER_NPC_SPRITES;i++) {
			if (sprites[i].xpos == x && sprites[i].ypos == y) {
				sprites[i].bmpid = 0;
			}
		}
		return;
	}

	//check for type of layer
	switch(current_layer)
	{
		case 0:
		backgroundbitmap[y] = 0;
		break;
		case 1:
			bottombmp[x][y] = 0;
			break;
		case 2:
			midbmp[x][y] = 0;
			break;
		case 3:
			mid2bmp[x][y] = 0;
			break;
		case 4:
			hibmp[x][y] = 0;
			break;
		case 5:
			topbmp[x][y] = 0;
			 break;
		case 6:
			tagmap[x][y] = 0;
			break;
		case 7:
			collision[x][y] = 0;
			break;
		default:
			break;


	}



}

void cycle_layers()
{
	// layer 0 backgroundbitmap[] background bitmap Alpha Blended Background Bitmap - NOT IMPLEMENTED YET (Used for backgrounds like rolling hills,etc.)
	// layer 1 bottombmp[][] Floor Tile (Used to make floors, on a level below the player)
	// layer 2 midbmp[][] Wall Tile (Used to make walls, on the same level as the player)
	// layer 3 mid2bmp[][] Wall-Patch Tiles (Used to make things like pillars, or torches on walls)
	// layer 4 hibmp[][] Ceiling Tiles (Used to make ceiling borders for levels)
	// layer 5 topbmp[][] Top Tiles (Used for large effects like clouds, fog, etc..)
	// layer 6 tagmap[][] Tag Layer (Used to place tag blocks)
	// layer 7 collision[][] Collision Layer (Used to place collision blocks)
	// layer 8 blendbmp[][] Blend Bitmap (Used for special effects. Not implemented yet)

	current_layer += 1;

	if (current_layer > LAYERMAX) {
		current_layer = 0;
	} else if (current_layer < 0) {
		current_layer = LAYERMAX;
	}


}

void flip_spritelayer()
{
	if (sprite_layer_on) {
		sprite_layer_on = 0;
	} else {
		sprite_layer_on = 1;
	}
}

void rendercollision()
{
	for (int ycor=0;ycor<TILE_GRID_MAX_HEIGHT;ycor++)
	 {
	  for (int xcor=0;xcor<TILE_GRID_MAX_WIDTH;xcor++)
	  {
		  if (collision[xcor][ycor])
		  {
			  drawtile(colblock, xcor, ycor);
		  }

	   }
	 }
}

void rendertagmap()
{
	for (int ycor=0;ycor<TILE_GRID_MAX_HEIGHT;ycor++)
	 {
	  for (int xcor=0;xcor<TILE_GRID_MAX_WIDTH;xcor++)
	  {
		  if (tagmap[xcor][ycor])
		  {
			  drawtile(tagblock, xcor, ycor);
			  al_draw_textf(arial_tiny, WHITE_COLOR, xcor*TILE_SCALE_FACTOR_X, ycor*TILE_SCALE_FACTOR_Y, ALLEGRO_ALIGN_LEFT, "id#: %d", tagmap[xcor][ycor]);
		  }

	   }
	 }
}

void rendersprites()
{


	 //first thing to do is scan through all the sprites//
	 for (int spritenum=0;spritenum<MAX_NUMBER_NPC_SPRITES;spritenum++)
	 {


		 //switch over sprites type ids
		 switch(sprites[spritenum].bmpid)
		 {
		 	 case 1:
		 		 drawplayersprite_edit(serf1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		 		 drawspriteindexinfo(sprites[spritenum].bmpid, spritenum, sprites[spritenum].xpos, sprites[spritenum].ypos);
		 		 break;
		 	 case 2:
		 		drawplayersprite_edit(ninjaf1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		 		 drawspriteindexinfo(sprites[spritenum].bmpid, spritenum, sprites[spritenum].xpos, sprites[spritenum].ypos);
		 		break;
		 	 case 3:
		 		drawplayersprite_edit(bluf1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		 		 drawspriteindexinfo(sprites[spritenum].bmpid, spritenum, sprites[spritenum].xpos, sprites[spritenum].ypos);
		 		break;
		 	 default:
		 		 break;
		 }

	 }
}


void drawplayersprite_edit(ALLEGRO_BITMAP *bitmap, int xpos, int ypos)
{
	if (bitmap == NULL)
		exit(DRAW_PLAYER_SPRITE_BITMAP_NULL_ERROR);
	al_draw_scaled_bitmap(bitmap,
			0,0,al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap),
			xpos*PLAYER_SPRITE_SCALE_FACTOR_X, ypos*PLAYER_SPRITE_SCALE_FACTOR_Y,
			PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT, NULL);
}

void  drawspriteindexinfo(int id, int index, int x, int y) {

	  al_draw_textf(arial_tiny, WHITE_COLOR, x*PLAYER_SPRITE_SCALE_FACTOR_X, y*PLAYER_SPRITE_SCALE_FACTOR_Y, ALLEGRO_ALIGN_LEFT, "id#: %d", id);
	  al_draw_textf(arial_tiny, WHITE_COLOR, x*PLAYER_SPRITE_SCALE_FACTOR_X, y*PLAYER_SPRITE_SCALE_FACTOR_Y+8, ALLEGRO_ALIGN_LEFT, "indx#: %d", index);

}

void clearmap()
{
	//clear bottom bitmap layer
	clear_tile_grids();
	clear_collision_grids();
	clear_tagmap_grids();
	clear_sprite_types();
	clear_tagmap_types();

}

void save_map_v2(char savefile[])
{
	FILE *level;

	level = fopen(savefile, "w");


	// layer 0 backgroundbitmap[] background bitmap Alpha Blended Background Bitmap - NOT IMPLEMENTED YET (Used for backgrounds like rolling hills,etc.)
	// layer 1 bottombmp[][] Floor Tile (Used to make floors, on a level below the player)
	// layer 2 midbmp[][] Wall Tile (Used to make walls, on the same level as the player)
	// layer 3 mid2bmp[][] Wall-Patch Tiles (Used to make things like pillars, or torches on walls)
	// layer 4 hibmp[][] Ceiling Tiles (Used to make ceiling borders for levels)
	// layer 5 topbmp[][] Top Tiles (Used for large effects like clouds, fog, etc..)
	// layer 6 tagmap[][] Tag Layer (Used to place tag blocks)
	// layer 7 collision[][] Collision Layer (Used to place collision blocks)
	// layer 8 blendbmp[][] Blend Bitmap (Used for special effects. Not implemented yet)


	//save background bitmap layer
	for (int yslot=0;yslot<100; yslot++)
		fwrite(&backgroundbitmap[yslot], sizeof(int), 1, level);

	//save bottom bitmap layer
	for (int yslot=0;yslot<100;yslot++)
	  for (int xslot=0;xslot<100;xslot++)
	  fwrite(&bottombmp[xslot][yslot], sizeof(int), 1, level);


	 //save middle bitmap layer (wall layer)
	 for (int yslot=0;yslot<100;yslot++)
	  for (int xslot=0;xslot<100;xslot++)
	  fwrite(&midbmp[xslot][yslot], sizeof(int), 1, level);

	 //save middle 2 bitmap layer (wall patch layer)
	 for (int yslot=0;yslot<100;yslot++)
	  for (int xslot=0;xslot<100;xslot++)
	  fwrite(&mid2bmp[xslot][yslot], sizeof(int), 1, level);

	 //save hi bitmap layer (ceiling)
	 for (int yslot=0;yslot<100;yslot++)
	  for (int xslot=0;xslot<100;xslot++)
	  fwrite(&hibmp[xslot][yslot], sizeof(int), 1, level);

	 //save top bitmap layer (clouds, etc.)
	 for (int yslot=0;yslot<100;yslot++)
		 for (int xslot=0;xslot<100;xslot++)
			 fwrite(&topbmp[xslot][yslot], sizeof(int), 1, level);


	 //save collision layer
	 for (int yslot=0;yslot<100;yslot++)
	  for (int xslot=0;xslot<100;xslot++)
	  fwrite(&collision[xslot][yslot], sizeof(int), 1, level);

	 //save tag map data layer
	 for (int yslot=0;yslot<100;yslot++)
	  for (int xslot=0;xslot<100;xslot++)
	  fwrite(&tagmap[xslot][yslot], sizeof(int), 1, level);

	 //save tag data for tag maps
	 for (int yslot=0;yslot<20;yslot++)
	 {
	  fwrite(&tags[yslot].scnenum, sizeof(int), 1, level);
	  fwrite(&tags[yslot].pressa, sizeof(int), 1, level);
	  fwrite(&tags[yslot].item, sizeof(int), 1, level);
	  fwrite(&tags[yslot].patch[0], sizeof(int), 1, level);
	  fwrite(&tags[yslot].patch[1], sizeof(int), 1, level);
	  fwrite(&tags[yslot].patch[2], sizeof(int), 1, level);
	  fwrite(&tags[yslot].scne[0], sizeof(int), 1, level);
	  fwrite(&tags[yslot].scne[1], sizeof(int), 1, level);
	  fwrite(&tags[yslot].overworld, sizeof(int), 1, level);
	  fwrite(&tags[yslot].actrig, sizeof(int), 1, level);
	  fwrite(&tags[yslot].tagjmp, sizeof(int), 1, level);
	 }


	 //save blend bitmap layer (special effects)
	 for (int yslot=0;yslot<100;yslot++)
		 for (int xslot=0;xslot<100;xslot++)
			 fwrite(&blendbmp[xslot][yslot], sizeof(int), 1, level);

	 //save data for sprites types
	 for (int yslot=0;yslot<10;yslot++)
	 {
	  fwrite(&sprites[yslot].xpos, sizeof(int), 1, level);
	  fwrite(&sprites[yslot].ypos, sizeof(int), 1, level);
	  fwrite(&sprites[yslot].zpos, sizeof(int), 1, level);
	  fwrite(&sprites[yslot].bmpid, sizeof(int), 1, level);
	  fwrite(&sprites[yslot].action, sizeof(int), 1, level);
	  for (int i = 0; i<30; i++)
	  {
		  fwrite(&sprites[yslot].inventory[i], sizeof(int), 1, level);
	  }
	  fwrite(&sprites[yslot].gp, sizeof(int), 1, level);
	  fwrite(&sprites[yslot].actrig, sizeof(int), 1, level);
	  fwrite(&sprites[yslot].immobile, sizeof(int), 1, level);
	 }

	 fclose(level);
}

/*
 * THIS HAS BEEN MOVED TO THE CROSSEXECUTBLES FILE
 */
/*
void load_map_v2(char loadfile[])

{
	FILE *level;

	//first step is to open the file//
	level = fopen(loadfile, "r");


	//background bitmap layer

	//save background bitmap layer
	for (int yslot=0;yslot<100; yslot++)
		fread(&backgroundbitmap[yslot], sizeof(int), 1, level);

	//bottom tile bitmap layer

	for (int yslot=0;yslot<100;yslot++)
		for (int xslot=0;xslot<100;xslot++)
			fread(&bottombmp[xslot][yslot], sizeof(int), 1, level);


	//middle -wall layer bitmaps

	for (int yslot=0;yslot<100;yslot++)
		for (int xslot=0;xslot<100;xslot++)
			fread(&midbmp[xslot][yslot], sizeof(int), 1, level);

	//middie 2 wall patch layer bitmap

	for (int yslot=0;yslot<100;yslot++)
		for (int xslot=0;xslot<100;xslot++)
			fread(&mid2bmp[xslot][yslot], sizeof(int), 1, level);

	//hi - ceiling layer bitmaps

	for (int yslot=0;yslot<100;yslot++)
		for (int xslot=0;xslot<100;xslot++)
			fread(&hibmp[xslot][yslot], sizeof(int), 1, level);


	//save top bitmap layer (clouds, etc.)
	for (int yslot=0;yslot<100;yslot++)
		for (int xslot=0;xslot<100;xslot++)
			fread(&topbmp[xslot][yslot], sizeof(int), 1, level);


	//collision block layer

	for (int yslot=0;yslot<100;yslot++)
		for (int xslot=0;xslot<100;xslot++)
			fread(&collision[xslot][yslot], sizeof(int), 1, level);

	//tag map data (grid) layer

	for (int yslot=0;yslot<100;yslot++)
		for (int xslot=0;xslot<100;xslot++)
			fread(&tagmap[xslot][yslot], sizeof(int), 1, level);


	//Data for different types of tag maps
	for (int yslot=0;yslot<20;yslot++)
	{
		fread(&tags[yslot].scnenum, sizeof(int), 1, level);
		fread(&tags[yslot].pressa, sizeof(int), 1, level);
		fread(&tags[yslot].item, sizeof(int), 1, level);
		fread(&tags[yslot].patch[0], sizeof(int), 1, level);
		fread(&tags[yslot].patch[1], sizeof(int), 1, level);
		fread(&tags[yslot].patch[2], sizeof(int), 1, level);
		fread(&tags[yslot].scne[0], sizeof(int), 1, level);
		fread(&tags[yslot].scne[1], sizeof(int), 1, level);
		fread(&tags[yslot].overworld, sizeof(int), 1, level);
		fread(&tags[yslot].actrig, sizeof(int), 1, level);
		fread(&tags[yslot].tagjmp, sizeof(int), 1, level);
	}


	//load blend bitmap layer (special effects)
	for (int yslot=0;yslot<100;yslot++)
	{
		for (int xslot=0;xslot<100;xslot++)
		{
			fread(&blendbmp[xslot][yslot], sizeof(int), 1, level);
		}
	}

	//save data for sprites types
	for (int yslot=0;yslot<10;yslot++)
	{
		fread(&sprites[yslot].xpos, sizeof(int), 1, level);
		fread(&sprites[yslot].ypos, sizeof(int), 1, level);
		fread(&sprites[yslot].zpos, sizeof(int), 1, level);
		fread(&sprites[yslot].bmpid, sizeof(int), 1, level);
		fread(&sprites[yslot].action, sizeof(int), 1, level);
		for (int i = 0; i<30; i++)
		{
			fread(&sprites[yslot].inventory[i], sizeof(int), 1, level);
		}
		fread(&sprites[yslot].gp, sizeof(int), 1, level);
		fread(&sprites[yslot].actrig, sizeof(int), 1, level);
		fread(&sprites[yslot].immobile, sizeof(int), 1, level);
	}



	fclose(level);
}

*/

void clear_tile_grids() {
	 // background bitmap layer
	 for (int yslot=0;yslot<100; yslot++)
		 backgroundbitmap[yslot] = 0;


	 //bottom tile bitmap layer

	 for (int yslot=0;yslot<100;yslot++)
	  for (int xslot=0;xslot<100;xslot++)
		  bottombmp[xslot][yslot] = 0;

	 //middle -wall layer bitmaps

	 for (int yslot=0;yslot<100;yslot++)
	  for (int xslot=0;xslot<100;xslot++)
		  midbmp[xslot][yslot] = 0;

	 //middie 2 wall patch layer bitmap

	 for (int yslot=0;yslot<100;yslot++)
	  for (int xslot=0;xslot<100;xslot++)
		  mid2bmp[xslot][yslot] = 0;


	 //hi - ceiling layer bitmaps

	 for (int yslot=0;yslot<100;yslot++)
	  for (int xslot=0;xslot<100;xslot++)
		  hibmp[xslot][yslot] = 0;

	 // top bitmap layer (clouds, etc.)
	 for (int yslot=0;yslot<100;yslot++)
		 for (int xslot=0;xslot<100;xslot++)
			topbmp[xslot][yslot] = 0;


	 //clear blend bitmap layer (special effects)
	 for (int yslot=0;yslot<100;yslot++)
		 for (int xslot=0;xslot<100;xslot++)
			 blendbmp[xslot][yslot] = 0;
}

void clear_collision_grids()
{
	 for (int yslot=0;yslot<100;yslot++)
		  for (int xslot=0;xslot<100;xslot++)
			  collision[xslot][yslot] = 0;

}


void clear_tagmap_grids()
{

	 for (int yslot=0;yslot<100;yslot++)
	  for (int xslot=0;xslot<100;xslot++)
		  tagmap[xslot][yslot] = 0;
}


void clear_sprite_types()
{
	 //save data for sprites types
	 for (int yslot=0;yslot<10;yslot++)
	 {
		sprites[yslot].xpos = 0;
		sprites[yslot].ypos = 0;
		sprites[yslot].bmpid = 0;
		sprites[yslot].action = 0;
		 for (int i = 0; i<30; i++)
			 sprites[yslot].inventory[i] = 0;
		 sprites[yslot].gp = 0;
		 sprites[yslot].actrig = 0;
		 sprites[yslot].immobile = 0;
	 }

}

void clear_tagmap_types()
{
	//Data for different types of tag maps
	for (int yslot=0;yslot<20;yslot++)
		 {
			tags[yslot].scnenum = 0;
			tags[yslot].pressa = 0;
			tags[yslot].item = 0;
			tags[yslot].patch[0] = 0;
			tags[yslot].patch[1] = 0;
			tags[yslot].patch[2] = 0;
			tags[yslot].overworld = 0;
			tags[yslot].actrig = 0;
			tags[yslot].tagjmp = 0;
		 }

}


//***********************************//

//DIALOGS

///text strings buffer

bool areyousurebox()
{
	//show box
	char *dialogtext[MAX_DIALOG_LINES];

	//strcpy(dialogtext[0], "Are you sure?\n");
	//strcpy(dialogtext[1], "(Y/N)?\n");

	dialogtext[0] = "Are you sure?";
	dialogtext[1] = "(Y/N)?";

    //


	showdialogbackbox(100, 100, 250, 250, WHITE_COLOR);
	showdialogtext(dialogtext, 2, 100, 100);
	end_render();
	//

	char temp = getyesnodialoginput();

	if (temp == 'Y')
		return true;
	else if (temp == 'N')
		return false;


}



//data entry loop for y/n box
char getyesnodialoginput()
{
	char input;
	bool loopcond = true;

		while(1)
		{

			//exit(-1);

			al_get_keyboard_state(&kbdstate);

			if (al_key_down(&kbdstate, ALLEGRO_KEY_N))
			{
				input = 'N';
				return input;

			}
			else if (al_key_down(&kbdstate, ALLEGRO_KEY_Y))
			{
				input = 'Y';
				return input;
			}

		}
}

void showdialogtext(char *dialogtext[MAX_DIALOG_LINES], int nlines, int x, int y)
{
	for (int i= 0; i<nlines; i++)
	{
		al_draw_textf(arial_rounded_convo, BLACK_COLOR, x, y*(i+1), ALLEGRO_ALIGN_LEFT, dialogtext[i]);
	}
}

void showdialogbackbox(int x, int y, int width, int height, ALLEGRO_COLOR color)
{
	ALLEGRO_BITMAP *dialog_box_back;

	//background- create white screen to display tiles on

	dialog_box_back = al_create_bitmap(width, height);

	al_set_target_bitmap(dialog_box_back);

	//clear tile bitmap
	al_clear_to_color(color);

	al_set_target_bitmap(al_get_backbuffer(display));

	al_draw_bitmap_region(dialog_box_back, 0, 0, width, height, x, y, NULL);


}



void end_render()
{
	al_set_target_bitmap(al_get_backbuffer(display));

	al_flip_display();
}


void render_typewriter_text(int x, int y)
{
	al_draw_textf(arial_rounded_convo, BLUE_COLOR, x, y, ALLEGRO_ALIGN_LEFT, dialog_keyboard_buffer);
}

void render_typewriter_cursor(int x, int y)
{
	al_draw_textf(arial_rounded_convo, BLACK_COLOR, x+(dialog_keyboard_buffer_index*9), y, ALLEGRO_ALIGN_LEFT, "_");
}


/////save dialog

void save_dialog_handle_key_input(ALLEGRO_KEYBOARD_EVENT event)
{
	printf("%c", event.unichar);
	dialog_keyboard_buffer[dialog_keyboard_buffer_index] = event.unichar;

	if (dialog_keyboard_buffer_index > 30)
		dialog_keyboard_buffer_index = 0;
	else
		dialog_keyboard_buffer_index++;

	if (event.keycode == ALLEGRO_KEY_Q)
	{
		save_dialog_on = 0;
		return;
	}
	if (event.keycode == ALLEGRO_KEY_BACKSPACE)
	{
		dialog_keyboard_buffer_index -= 2;
		if (dialog_keyboard_buffer_index < 0)
			dialog_keyboard_buffer_index = 0;
	}
	if (event.keycode == ALLEGRO_KEY_ENTER)
	{
		bool decision = areyousurebox();
		if (decision) {
			save_map_v2(dialog_keyboard_buffer);
			strcpy(filename, dialog_keyboard_buffer);
		}
		save_dialog_on = 0;

		return;
	}
}

void save_dialog_handle_rendering()
{
	//render dialog box
	showdialogbackbox( 50,  50, 400, 200, WHITE_COLOR);

	//render dialog text

	char *dialogtext[MAX_DIALOG_LINES];

	dialogtext[0] = "Please enter a save file name";
	dialogtext[1] = "and press RETURN";

	showdialogtext(dialogtext, 2, 50, 50);

	//render typewriter text

	render_typewriter_text(50, 200);
	//render typewriter cursor
	render_typewriter_cursor(50, 200);

	end_render();
}


//*******************************END SAVE DIALOG

/////load dialog

void load_dialog_handle_key_input(ALLEGRO_KEYBOARD_EVENT event)
{
	printf("%c", event.unichar);
	dialog_keyboard_buffer[dialog_keyboard_buffer_index] = event.unichar;

	if (dialog_keyboard_buffer_index > 30)
		dialog_keyboard_buffer_index = 0;
	else
		dialog_keyboard_buffer_index++;

	if (event.keycode == ALLEGRO_KEY_Q)
	{
		load_dialog_on = 0;
		return;
	}
	if (event.keycode == ALLEGRO_KEY_BACKSPACE)
	{
		dialog_keyboard_buffer_index -= 2;
		if (dialog_keyboard_buffer_index < 0)
			dialog_keyboard_buffer_index = 0;
	}
	if (event.keycode == ALLEGRO_KEY_ENTER)
	{
		bool decision = areyousurebox();
		if (decision)
			load_map_v2(dialog_keyboard_buffer);

		load_dialog_on = 0;
		return;
	}
}

void load_dialog_handle_rendering()
{
	//render dialog box
	showdialogbackbox( 50,  50, 400, 200, WHITE_COLOR);

	//render dialog text

	char *dialogtext[MAX_DIALOG_LINES];

	dialogtext[0] = "Please enter a file name to load";
	dialogtext[1] = "and press RETURN";

	showdialogtext(dialogtext, 2, 50, 50);

	//render typewriter text

	render_typewriter_text(50, 200);
	//render typewriter cursor
	render_typewriter_cursor(50, 200);

	end_render();
}

void clear_bufffered_input_string()
{
	for (int i =0; i < 30; i++)
	{
		dialog_keyboard_buffer[i] = NULL;
	}
	dialog_keyboard_buffer_index = 0;
}

void cycle_background_image(int amt)
{
	current_background_bitmap_id += amt;

	if (current_background_bitmap_id > NUMBER_STOCK_BACKGROUND_IMAGES)
		current_background_bitmap_id = 0;
	else if (current_background_bitmap_id < 0)
		current_background_bitmap_id =  NUMBER_STOCK_BACKGROUND_IMAGES;

}



void cycle_background_image_index(int amt)
{
	current_background_bitmap_index += amt;

	if (current_background_bitmap_index > 100)
		current_background_bitmap_index = 0;
	else if (current_background_bitmap_index < 0)
		current_background_bitmap_index =  100;

}


void render_back_bmps()
{


	//draw stored bitmaps
	for (int i=0;i < 100; i++)
	{

		renderbackbmp(backgroundbitmap[i]);

	}

	//draw currently selected bitmap
	if (current_background_bitmap_id == 1) //cloud background
	{
		renderbackbmp(1);
	}

}

void renderbackbmp(int id)
{

	if (id == 1)
	{
		//"clouds" bitmap
		if (cloud1 == NULL)
					exit(DRAW_BACKGROUND_BITMAP_NULL_ERROR);

			al_draw_scaled_bitmap(cloud1,
					0,0,al_get_bitmap_width(cloud1), al_get_bitmap_height(cloud1),
					BACK_IMAGE_ORIGIN_X, BACK_IMAGE_ORIGIN_Y,
					BACK_IMAGE_WIDTH, BACK_IMAGE_HEIGHT, NULL);

	}

}


void custom_rendertop() //custom version of top renderer, without animation
{
	for (int x = 0; x< 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{
			int value = topbmp[x][y];

			ALLEGRO_BITMAP *top_bitmap;

			switch (value) {
				case 1:
					top_bitmap = dmb_cloud;
					break;
				case 2:
					top_bitmap = fmb_cloud;
					break;
				case 3:
					top_bitmap = lcb_cloud;
					break;
				case 4:
					top_bitmap = smb_cloud;
					break;
				case 5:
					top_bitmap = smfb_cloud;
					break;
				default:
					break;
			}

			if (value > 0) {

				//generate random number between 1 and 10
//				/* initialize random seed: */
//				  srand ( time(NULL) );
//
//				  /* generate secret number: */
//				  iSecret = rand() % 10 + 1;

				srand (time(NULL) ); //seed random generator

				//cloud tint with random tint between 0.50 and 0.75
				float rantint = (float)(rand() % 10 + 1);  //between 1 and 10
				rantint /= 25; //divide by 20, so between 0.1 and 0.25
				rantint += 0.5; //shift so between 0.6 and 0.85


				//printf("TINT: %f", rantint);

				/*al_draw_scaled_bitmap(top_bitmap,
				0,0,al_get_bitmap_width(top_bitmap), al_get_bitmap_height(top_bitmap),
				x*TOP_SCALE_FACTOR_X+TOP_TRANSLATE_FACTOR_X, y*TOP_SCALE_FACTOR_Y+TOP_TRANSLATE_FACTOR_Y,
				al_get_bitmap_width(top_bitmap)/2, al_get_bitmap_height(top_bitmap)/2, NULL);*/

				//draw cloud tinted !!with random tint
				//below draws iwth 50% tint
				//al_draw_tinted_bitmap(top_bitmap, al_map_rgba_f(1, 1, 1, 0.5), x, y, 0);			}

				//tinted by 50% and scaled
				al_draw_tinted_scaled_bitmap(top_bitmap,
						al_map_rgba_f(rantint, rantint, rantint, 0.05),
				    0, 0, al_get_bitmap_width(top_bitmap),al_get_bitmap_height(top_bitmap),
				    x*TOP_SCALE_FACTOR_X+TOP_TRANSLATE_FACTOR_X, y*TOP_SCALE_FACTOR_Y+TOP_TRANSLATE_FACTOR_Y,
				    al_get_bitmap_width(top_bitmap)/2, al_get_bitmap_height(top_bitmap)/2, NULL);
			}
		}
	}
}
