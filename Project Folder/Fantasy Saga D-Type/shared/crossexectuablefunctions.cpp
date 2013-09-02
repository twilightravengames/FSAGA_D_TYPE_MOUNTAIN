

#include <stdio.h>
#include "../allegroincludes.h"
#include "fsagabitmaps.h"
#include "fsagaconstants.h"
#include "crossexecutableprototypes.h"
#include "mapfilestructures.h"


//*********************************************
//NOTE ON DESIGN:
//Global variables instantiated here should only be
//universal to the game engine and usable in both the 
//the game and the editors. They are accessed via extern keywords
//in the application using the engine. Application level variables
//are accessed here using the extern keyword in a similar fashion.


//NOTES
//see ex_blend for different blending modes with fonts



//for rendering
ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP *back_buffer;
ALLEGRO_BITMAP *map_buffer; //a bitmap representing the map


//cursor
ALLEGRO_MOUSE_CURSOR *cursor;

//for mouse
ALLEGRO_BITMAP *gauntlet_cursor; 

//for fonts
ALLEGRO_FONT *arial_rounded;
ALLEGRO_FONT *arial_rounded_convo;
ALLEGRO_FONT *arial_big;
ALLEGRO_FONT *arial_tiny;
ALLEGRO_FONT *arial_dialogue;

//for logging and level loading
FILE *level;
FILE *log;





//Game Map Information

///////////////////////////////////////////////////////////////////////////////////
//GAME MAP DATA AND DATA STRUCTURES INSTANCES 

int scnenum; //current scene number (map number)
int lastscnenum; //old scene number (last map number visited)
int boxx;
int boxy;
int boxid;
int pages; //number of pages of text
int mnster;    //monster types encountered
int looptt;

//tiles

int backgroundbitmap[100];			//alpha blended bitmaps. Used for backgrouds (e.g., clouds behind a floating island, or trees or hills). 
int bottombmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //bottom layer bitmap, always rendered under the player
int midbmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //mid layer bmp, squares that overlap the player's body (same level as the player)
int mid2bmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //another set of mid layer bmps, immediately rendered after the first. they 
														//draw wall "patch" bitmaps, things that go over top of walls like pillars, torches, shields, or stairs..
int hibmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT];
int topbmp[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //top layer bmp, giant bitmap like clouds in lufia2
int tagmap[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //locations of tags. contains ids that reference tag type data structure
int collision[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT]; //collision table for scene
int blendbmp[100][100]; //blend bmp table Note: Not implemeneted yet - smaller alpha blended bitmaps overtop the scene, will be used for special effects bitmaps 


//tags for local map
tag tags[MAX_NUMBER_TAG_TYPES]; //tag types

//tag for world map
tag_o tags_o[20]; //overworld tag types

//tag and colision grid for local map
int tagmap_o[100][100]; //locations of  ---> their id's
int collision_o[100][100]; //collision table for scene

//overworld map type Note: note sure how this is used
int mapt_o;

//local map type Note: note sure how this is used
int mapt;

//won battle? Note: note sure how this is used
int win;


//sprite instances
sprite sprites[MAX_NUMBER_NPC_SPRITES];	//sprites that appear in game maps 
sprite mainchar;		//main character directed through game maps
sprite mainchar_o;		//main character directed through world maps


///////////////////////////////////END GAME MAP DATA//////////////////////////////////////


//max of these values are their respective max map sizes
extern int top_anim_x_roller;
extern int top_anim_y_roller;


//for scrolling (external from main.cpp)
extern int scroll_x;
extern int scroll_y;

void get_display() //gets the display bitmaps
{
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	display = al_create_display(800, 600);
	if (!display)
	{
		exit(CREATE_DISPLAY_FAIL_ERROR);
	}
	back_buffer = al_get_backbuffer(display);	
}

void destroy_display()
{
	al_destroy_display(display);
}

//


//ALLEGRO_BITMAP *al_create_bitmap(int w, int h)
void init_bitmaps() //create and clear bitmaps manipulated in-game
{
 
 al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP); //this flag sets the bitmap creation to occur in system memory
												//necessary otherwise maingame loop is VERY slow

 //create the following in video RAM
 if((bitmap2 = al_create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT)) == NULL)
	exit(BITMAP_CREATE_FAIL_ERROR);
 if((sprite1 = al_create_bitmap(SPRITE1_WIDTH, SPRITE1_HEIGHT)) == NULL)
	 exit(BITMAP_CREATE_FAIL_ERROR);
 if((map_buffer = al_create_bitmap(MAX_MAP_WIDTH, MAX_MAP_HEIGHT)) == NULL)
	 exit(MAP_BUFFER_CREATE_FAIL_ERROR);

 clear(bitmap2);
 clear(sprite1);
 clear(map_buffer);
}


void destroy_bitmaps() //destroys the non-display bitmaps manipulated in-game
{
	al_destroy_bitmap(bitmap2);
	al_destroy_bitmap(sprite1);
	al_destroy_bitmap(map_buffer);
}



//TODO: Make loading bitmap files more dynamic
void load_bitmap_files()
{
	//note: magic pinks bitmaps have to be converted using al_convert_mask_to_alpha(ALLEGRO_BITMAP *bitmap, ALLEGRO_COLOR mask_color)
	//note: may have to create bitmaps before loading images into them?

	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	//al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_NO_ALPHA);

	//floors
	load_magic_bitmap(&floor1, "bitmaps/floor1.bmp");

	load_magic_bitmap(&floor2, "bitmaps/floor2.bmp");
	
	//pillars	
	load_magic_bitmap(&pill1, "bitmaps/pill1.bmp");
	

			
	//seren images
	load_alpha_bitmap(&serf1, "bitmaps/character/serf1.png");
	load_alpha_bitmap(&serf2, "bitmaps/character/serf2.png");
	load_alpha_bitmap(&serf3, "bitmaps/character/serf3.png");
	load_alpha_bitmap(&serf4, "bitmaps/character/serf4.png");
	load_alpha_bitmap(&serf5, "bitmaps/character/serf5.png");
	load_alpha_bitmap(&serf6, "bitmaps/character/serf6.png");
	load_alpha_bitmap(&serf7, "bitmaps/character/serf7.png");
	load_alpha_bitmap(&serf8, "bitmaps/character/serf8.png");


	//seren

	//load_alpha_bitmap(&serf1, "bitmaps/character/zef1.png");

	//load_alpha_bitmap(&serf2, "bitmaps/character/zef2.png");

	//load_alpha_bitmap(&serf3, "bitmaps/character/zef3.png");

	//load_alpha_bitmap(&serf4, "bitmaps/character/zef4.png");

	load_alpha_bitmap(&serb1, "bitmaps/character/serb1.png");
	load_alpha_bitmap(&serb2, "bitmaps/character/serb2.png");
	load_alpha_bitmap(&serb3, "bitmaps/character/serb3.png");
	load_alpha_bitmap(&serb4, "bitmaps/character/serb4.png");
	load_alpha_bitmap(&serb5, "bitmaps/character/serb5.png");
	load_alpha_bitmap(&serb6, "bitmaps/character/serb6.png");
	load_alpha_bitmap(&serb7, "bitmaps/character/serb7.png");
	load_alpha_bitmap(&serb8, "bitmaps/character/serb8.png");


	load_alpha_bitmap(&serr1, "bitmaps/character/serr1.png");
	load_alpha_bitmap(&serr2, "bitmaps/character/serr2.png");
	load_alpha_bitmap(&serr3, "bitmaps/character/serr3.png");
	load_alpha_bitmap(&serr4, "bitmaps/character/serr4.png");
	load_alpha_bitmap(&serr5, "bitmaps/character/serr5.png");
	load_alpha_bitmap(&serr6, "bitmaps/character/serr6.png");
	load_alpha_bitmap(&serr7, "bitmaps/character/serr7.png");
	load_alpha_bitmap(&serr8, "bitmaps/character/serr8.png");



	load_alpha_bitmap(&serl1, "bitmaps/character/serl1.png");
	load_alpha_bitmap(&serl2, "bitmaps/character/serl2.png");
	load_alpha_bitmap(&serl3, "bitmaps/character/serl3.png");
	load_alpha_bitmap(&serl4, "bitmaps/character/serl4.png");
	load_alpha_bitmap(&serl5, "bitmaps/character/serl5.png");
	load_alpha_bitmap(&serl6, "bitmaps/character/serl6.png");
	load_alpha_bitmap(&serl7, "bitmaps/character/serl7.png");
	load_alpha_bitmap(&serl8, "bitmaps/character/serl8.png");

	//Seren's angled walking

	//down left
	load_alpha_bitmap(&serdl1, "bitmaps/character/serdl1.png");
	load_alpha_bitmap(&serdl2, "bitmaps/character/serdl2.png");
	load_alpha_bitmap(&serdl3, "bitmaps/character/serdl3.png");
	load_alpha_bitmap(&serdl4, "bitmaps/character/serdl4.png");
	load_alpha_bitmap(&serdl5, "bitmaps/character/serdl5.png");
	load_alpha_bitmap(&serdl6, "bitmaps/character/serdl6.png");
	load_alpha_bitmap(&serdl7, "bitmaps/character/serdl7.png");
	load_alpha_bitmap(&serdl8, "bitmaps/character/serdl8.png");

	//angled to the down right
	

	load_alpha_bitmap(&serdr1, "bitmaps/character/serdr1.png");
	load_alpha_bitmap(&serdr2, "bitmaps/character/serdr2.png");
	load_alpha_bitmap(&serdr3, "bitmaps/character/serdr3.png");
	load_alpha_bitmap(&serdr4, "bitmaps/character/serdr4.png");
	load_alpha_bitmap(&serdr5, "bitmaps/character/serdr5.png");
	load_alpha_bitmap(&serdr6, "bitmaps/character/serdr6.png");
	load_alpha_bitmap(&serdr7, "bitmaps/character/serdr7.png");
	load_alpha_bitmap(&serdr8, "bitmaps/character/serdr8.png");

	//angled to the up right


	load_alpha_bitmap(&serur1, "bitmaps/character/serur1.png");
	load_alpha_bitmap(&serur2, "bitmaps/character/serur2.png");
	load_alpha_bitmap(&serur3, "bitmaps/character/serur3.png");
	load_alpha_bitmap(&serur4, "bitmaps/character/serur4.png");
	load_alpha_bitmap(&serur5, "bitmaps/character/serur5.png");
	load_alpha_bitmap(&serur6, "bitmaps/character/serur6.png");
	load_alpha_bitmap(&serur7, "bitmaps/character/serur7.png");
	load_alpha_bitmap(&serur8, "bitmaps/character/serur8.png");

	//angled tothe up left

	load_alpha_bitmap(&serul1, "bitmaps/character/serul1.png");
	load_alpha_bitmap(&serul2, "bitmaps/character/serul2.png");
	load_alpha_bitmap(&serul3, "bitmaps/character/serul3.png");
	load_alpha_bitmap(&serul4, "bitmaps/character/serul4.png");
	load_alpha_bitmap(&serul5, "bitmaps/character/serul5.png");
	load_alpha_bitmap(&serul6, "bitmaps/character/serul6.png");
	load_alpha_bitmap(&serul7, "bitmaps/character/serul7.png");
	load_alpha_bitmap(&serul8, "bitmaps/character/serul8.png");


	load_alpha_bitmap(&seridle1, "bitmaps/character/seridle1.png");
	load_alpha_bitmap(&seridle2, "bitmaps/character/seridle2.png");
	load_alpha_bitmap(&seridle3, "bitmaps/character/seridle3.png");
	load_alpha_bitmap(&seridle4, "bitmaps/character/seridle4.png");

	//seren attacking
	/*
	load_magic_bitmap(&seral1, "bitmaps/character/seral1.bmp");
	load_magic_bitmap(&seral2, "bitmaps/character/seral2.bmp");
	load_magic_bitmap(&seral3, "bitmaps/character/seral3.bmp");
	load_magic_bitmap(&seral4, "bitmaps/character/seral4.bmp");

	load_magic_bitmap(&serar1, "bitmaps/character/serar1.bmp");
	load_magic_bitmap(&serar2, "bitmaps/character/serar2.bmp");
	load_magic_bitmap(&serar3, "bitmaps/character/serar3.bmp");
	load_magic_bitmap(&serar4, "bitmaps/character/serar4.bmp");

	load_magic_bitmap(&seraf1, "bitmaps/character/seraf1.bmp");
	load_magic_bitmap(&seraf2, "bitmaps/character/seraf2.bmp");
	load_magic_bitmap(&seraf3, "bitmaps/character/seraf3.bmp");
	load_magic_bitmap(&seraf4, "bitmaps/character/seraf4.bmp");

	load_magic_bitmap(&serab1, "bitmaps/character/serab1.bmp");
	load_magic_bitmap(&serab2, "bitmaps/character/serab2.bmp");
	load_magic_bitmap(&serab3, "bitmaps/character/serab3.bmp");
	load_magic_bitmap(&serab4, "bitmaps/character/serab4.bmp");
*/
	//blu enemy images
	//seren images

	load_magic_bitmap(&bluf_bmps_array[FRONT_STAND_1], "bitmaps/character/bluf1.bmp");
	load_magic_bitmap(&bluf_bmps_array[BACK_STAND_1], "bitmaps/character/blub1.bmp");
	load_magic_bitmap(&bluf_bmps_array[LEFT_STAND_1], "bitmaps/character/blul1.bmp");
	load_magic_bitmap(&bluf_bmps_array[RIGHT_STAND_1], "bitmaps/character/blur1.bmp");

	load_magic_bitmap(&bluf_bmps_array[BACK_MOVE_1], "bitmaps/character/blub1.bmp");
	load_magic_bitmap(&bluf_bmps_array[BACK_MOVE_2], "bitmaps/character/blub1.bmp");

	load_magic_bitmap(&bluf_bmps_array[BACK_MOVE_3], "bitmaps/character/blub2.bmp");
	load_magic_bitmap(&bluf_bmps_array[BACK_MOVE_4], "bitmaps/character/blub2.bmp");

	load_magic_bitmap(&bluf_bmps_array[BACK_MOVE_5], "bitmaps/character/blub3.bmp");
	load_magic_bitmap(&bluf_bmps_array[BACK_MOVE_6], "bitmaps/character/blub3.bmp");

	load_magic_bitmap(&bluf_bmps_array[BACK_MOVE_7], "bitmaps/character/blub4.bmp");
	load_magic_bitmap(&bluf_bmps_array[BACK_MOVE_8], "bitmaps/character/blub4.bmp");

	//

	load_magic_bitmap(&bluf_bmps_array[FRONT_MOVE_1], "bitmaps/character/bluf1.bmp");
	load_magic_bitmap(&bluf_bmps_array[FRONT_MOVE_2], "bitmaps/character/bluf1.bmp");

	load_magic_bitmap(&bluf_bmps_array[FRONT_MOVE_3], "bitmaps/character/bluf2.bmp");
	load_magic_bitmap(&bluf_bmps_array[FRONT_MOVE_4], "bitmaps/character/bluf2.bmp");

	load_magic_bitmap(&bluf_bmps_array[FRONT_MOVE_5], "bitmaps/character/bluf3.bmp");
	load_magic_bitmap(&bluf_bmps_array[FRONT_MOVE_6], "bitmaps/character/bluf3.bmp");

	load_magic_bitmap(&bluf_bmps_array[FRONT_MOVE_7], "bitmaps/character/bluf4.bmp");
	load_magic_bitmap(&bluf_bmps_array[FRONT_MOVE_8], "bitmaps/character/bluf4.bmp");

	//

	load_magic_bitmap(&bluf_bmps_array[LEFT_MOVE_1], "bitmaps/character/blul1.bmp");
	load_magic_bitmap(&bluf_bmps_array[LEFT_MOVE_2], "bitmaps/character/blul1.bmp");

	load_magic_bitmap(&bluf_bmps_array[LEFT_MOVE_3], "bitmaps/character/blul2.bmp");
	load_magic_bitmap(&bluf_bmps_array[LEFT_MOVE_4], "bitmaps/character/blul2.bmp");

	load_magic_bitmap(&bluf_bmps_array[LEFT_MOVE_5], "bitmaps/character/blul3.bmp");
	load_magic_bitmap(&bluf_bmps_array[LEFT_MOVE_6], "bitmaps/character/blul3.bmp");

	load_magic_bitmap(&bluf_bmps_array[LEFT_MOVE_7], "bitmaps/character/blul4.bmp");
	load_magic_bitmap(&bluf_bmps_array[LEFT_MOVE_8], "bitmaps/character/blul4.bmp");

	//

	load_magic_bitmap(&bluf_bmps_array[RIGHT_MOVE_1], "bitmaps/character/blur1.bmp");
	load_magic_bitmap(&bluf_bmps_array[RIGHT_MOVE_2], "bitmaps/character/blur1.bmp");


	load_magic_bitmap(&bluf_bmps_array[RIGHT_MOVE_3], "bitmaps/character/blur2.bmp");
	load_magic_bitmap(&bluf_bmps_array[RIGHT_MOVE_4], "bitmaps/character/blur2.bmp");

	load_magic_bitmap(&bluf_bmps_array[RIGHT_MOVE_5], "bitmaps/character/blur3.bmp");
	load_magic_bitmap(&bluf_bmps_array[RIGHT_MOVE_6], "bitmaps/character/blur3.bmp");


	load_magic_bitmap(&bluf_bmps_array[RIGHT_MOVE_7], "bitmaps/character/blur4.bmp");
	load_magic_bitmap(&bluf_bmps_array[RIGHT_MOVE_8], "bitmaps/character/blur4.bmp");



	//-----------------------------------load ninja


	load_magic_bitmap(&ninja_bmps_array[FRONT_STAND_1], "bitmaps/character/ninjaf1.bmp");
	load_magic_bitmap(&ninja_bmps_array[BACK_STAND_1], "bitmaps/character/ninjab1.bmp");
	load_magic_bitmap(&ninja_bmps_array[LEFT_STAND_1], "bitmaps/character/ninjal1.bmp");
	load_magic_bitmap(&ninja_bmps_array[RIGHT_STAND_1], "bitmaps/character/ninjar1.bmp");

	load_magic_bitmap(&ninja_bmps_array[BACK_MOVE_1], "bitmaps/character/ninjab1.bmp");
	load_magic_bitmap(&ninja_bmps_array[BACK_MOVE_2], "bitmaps/character/ninjab2.bmp");

	load_magic_bitmap(&ninja_bmps_array[BACK_MOVE_3], "bitmaps/character/ninjab2.bmp");
	load_magic_bitmap(&ninja_bmps_array[BACK_MOVE_4], "bitmaps/character/ninjab2.bmp");

	load_magic_bitmap(&ninja_bmps_array[BACK_MOVE_5], "bitmaps/character/ninjab3.bmp");
	load_magic_bitmap(&ninja_bmps_array[BACK_MOVE_6], "bitmaps/character/ninjab3.bmp");

	load_magic_bitmap(&ninja_bmps_array[BACK_MOVE_7], "bitmaps/character/ninjab4.bmp");
	load_magic_bitmap(&ninja_bmps_array[BACK_MOVE_8], "bitmaps/character/ninjab4.bmp");

	//

	load_magic_bitmap(&ninja_bmps_array[FRONT_MOVE_1], "bitmaps/character/ninjaf1.bmp");
	load_magic_bitmap(&ninja_bmps_array[FRONT_MOVE_2], "bitmaps/character/ninjaf1.bmp");

	load_magic_bitmap(&ninja_bmps_array[FRONT_MOVE_3], "bitmaps/character/ninjaf2.bmp");
	load_magic_bitmap(&ninja_bmps_array[FRONT_MOVE_4], "bitmaps/character/ninjaf2.bmp");

	load_magic_bitmap(&ninja_bmps_array[FRONT_MOVE_5], "bitmaps/character/ninjaf3.bmp");
	load_magic_bitmap(&ninja_bmps_array[FRONT_MOVE_6], "bitmaps/character/ninjaf3.bmp");

	load_magic_bitmap(&ninja_bmps_array[FRONT_MOVE_7], "bitmaps/character/ninjaf4.bmp");
	load_magic_bitmap(&ninja_bmps_array[FRONT_MOVE_8], "bitmaps/character/ninjaf4.bmp");

	//

	load_magic_bitmap(&ninja_bmps_array[LEFT_MOVE_1], "bitmaps/character/ninjal1.bmp");
	load_magic_bitmap(&ninja_bmps_array[LEFT_MOVE_2], "bitmaps/character/ninjal1.bmp");

	load_magic_bitmap(&ninja_bmps_array[LEFT_MOVE_3], "bitmaps/character/ninjal2.bmp");
	load_magic_bitmap(&ninja_bmps_array[LEFT_MOVE_4], "bitmaps/character/ninjal2.bmp");

	load_magic_bitmap(&ninja_bmps_array[LEFT_MOVE_5], "bitmaps/character/ninjal3.bmp");
	load_magic_bitmap(&ninja_bmps_array[LEFT_MOVE_6], "bitmaps/character/ninjal3.bmp");

	load_magic_bitmap(&ninja_bmps_array[LEFT_MOVE_7], "bitmaps/character/ninjal4.bmp");
	load_magic_bitmap(&ninja_bmps_array[LEFT_MOVE_8], "bitmaps/character/ninjal4.bmp");

	//

	load_magic_bitmap(&ninja_bmps_array[RIGHT_MOVE_1], "bitmaps/character/ninjar1.bmp");
	load_magic_bitmap(&ninja_bmps_array[RIGHT_MOVE_2], "bitmaps/character/ninjar1.bmp");


	load_magic_bitmap(&ninja_bmps_array[RIGHT_MOVE_3], "bitmaps/character/ninjar2.bmp");
	load_magic_bitmap(&ninja_bmps_array[RIGHT_MOVE_4], "bitmaps/character/ninjar2.bmp");

	load_magic_bitmap(&ninja_bmps_array[RIGHT_MOVE_5], "bitmaps/character/ninjar3.bmp");
	load_magic_bitmap(&ninja_bmps_array[RIGHT_MOVE_6], "bitmaps/character/ninjar3.bmp");


	load_magic_bitmap(&ninja_bmps_array[RIGHT_MOVE_7], "bitmaps/character/ninjar4.bmp");
	load_magic_bitmap(&ninja_bmps_array[RIGHT_MOVE_8], "bitmaps/character/ninjar4.bmp");


	//walls
	load_magic_bitmap(&wall1, "bitmaps/wall1.bmp");
	load_magic_bitmap(&wall2, "bitmaps/wall2.bmp");
	load_magic_bitmap(&wall3, "bitmaps/wall3.bmp");
	load_magic_bitmap(&wall4, "bitmaps/wall4.bmp");

	//chest
	load_magic_bitmap(&chest, "bitmaps/chest.bmp");
	load_magic_bitmap(&chest2, "bitmaps/chest2.bmp");

	//bed
	load_magic_bitmap(&beda1, "bitmaps/beda1.bmp");
		
	//door
	load_magic_bitmap(&door1, "bitmaps/door1.bmp");
		
	//arches
	load_magic_bitmap(&arch1, "bitmaps/arch1.bmp");
	load_magic_bitmap(&arch2, "bitmaps/arch2.bmp");

	//textbox
	load_magic_bitmap(&textbox, "bitmaps/textbox.bmp");

	//stairs
	load_magic_bitmap(&stairs1, "bitmaps/stairs1.bmp");
	load_magic_bitmap(&stairs2, "bitmaps/stairs1.bmp");

	//ninja soldier
/*
	load_magic_bitmap(&ninjaf1, "bitmaps/character/ninjaf1.bmp");
	load_magic_bitmap(&ninjaf2, "bitmaps/character/ninjaf2.bmp");
	load_magic_bitmap(&ninjaf3, "bitmaps/character/ninjaf3.bmp");
	load_magic_bitmap(&ninjaf4, "bitmaps/character/ninjaf4.bmp");
	load_magic_bitmap(&ninjab1, "bitmaps/character/ninjab1.bmp");
	load_magic_bitmap(&ninjab2, "bitmaps/character/ninjab2.bmp");
	load_magic_bitmap(&ninjab3, "bitmaps/character/ninjab3.bmp");
	load_magic_bitmap(&ninjab4, "bitmaps/character/ninjab4.bmp");
	load_magic_bitmap(&ninjar1, "bitmaps/character/ninjar1.bmp");
	load_magic_bitmap(&ninjar2, "bitmaps/character/ninjar2.bmp");
	load_magic_bitmap(&ninjar3, "bitmaps/character/ninjar3.bmp");
	load_magic_bitmap(&ninjar4, "bitmaps/character/ninjar4.bmp");
	load_magic_bitmap(&ninjal1, "bitmaps/character/ninjal1.bmp");
	load_magic_bitmap(&ninjal2, "bitmaps/character/ninjal2.bmp");
	load_magic_bitmap(&ninjal3, "bitmaps/character/ninjal3.bmp");
	load_magic_bitmap(&ninjal4, "bitmaps/character/ninjal4.bmp");
	*/
	//top of walls
	load_magic_bitmap(&top1, "bitmaps/top1.bmp");
	load_magic_bitmap(&top2, "bitmaps/top2.bmp");

	//decorative shield
	load_magic_bitmap(&shield1, "bitmaps/shield1.bmp");

	//wall torch
	load_magic_bitmap(&torch1, "bitmaps/torch1.bmp");
	load_magic_bitmap(&torch2, "bitmaps/torch2.bmp");

	//slash effect (battle)
	load_magic_bitmap(&slash1, "bitmaps/slash.bmp");

	//flare effect (battle)
	load_magic_bitmap(&flare1, "bitmaps/flare1.bmp");

	//battle meter (battle)
	load_magic_bitmap(&meter1, "bitmaps/meter1.bmp");

	//battle cursor (battle)
	load_magic_bitmap(&gold_cursor, "bitmaps/cursor.bmp");

	//character select box (battle)
	load_magic_bitmap(&select1, "bitmaps/select1.bmp");
	load_magic_bitmap(&select2, "bitmaps/select2.bmp");

	//mech - demo enemy
	load_magic_bitmap(&mech, "bitmaps/mech.bmp");

	//ninja soldier portrait
	load_magic_bitmap(&pic1, "bitmaps/pic1.bmp");

	//for debugging purposes
	//collision block marker
	load_magic_bitmap(&colblock, "bitmaps/colblock.bmp");
	//tag block marker
	load_magic_bitmap(&tagblock, "bitmaps/tagblock.bmp");

	//al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_ANY_WITH_ALPHA);



	load_magic_bitmap(&convo_box, "bitmaps/convobox.bmp");


	load_alpha_bitmap(&gauntlet_cursor, "bitmaps/cursor.png");
	

	load_alpha_bitmap(&t_grass, "bitmaps/t_grass.png");
	load_alpha_bitmap(&t_rock, "bitmaps/t_rock.png");
	load_alpha_bitmap(&t_tree, "bitmaps/t_tree.png");


	//

	load_alpha_bitmap(&cloud1, "bitmaps/background_bitmaps/cloud1.png");
	load_alpha_bitmap(&green1, "bitmaps/background_bitmaps/green_background.png");


	load_alpha_bitmap(&dmb_cloud, "bitmaps/clouds/dmb_cloud.png");
	load_alpha_bitmap(&fmb_cloud, "bitmaps/clouds/fmb_cloud.png");
	load_alpha_bitmap(&lcb_cloud, "bitmaps/clouds/lcb_cloud.png");
	load_alpha_bitmap(&smb_cloud, "bitmaps/clouds/smb_cloud.png");
	load_alpha_bitmap(&smfb_cloud, "bitmaps/clouds/smfb_cloud.png");

	//


	load_alpha_bitmap(&t_cloud, "bitmaps/clouds/t_cloud.png");


	//
}


void load_fonts()
{
	arial_rounded = al_load_ttf_font("fonts/ARLRDBD.TTF", 20, 0);
		
    if (!arial_rounded) 
	{
		logger("Failed to load Arial Font");
		exit(FONT_LOAD_TTF_FAILED_ERROR);
	}

    arial_rounded_convo = al_load_ttf_font("fonts/ARLRDBD.TTF", 15, 0);

    if (!arial_rounded_convo)
    {
    		logger("Failed to load Arial Font for Convo System");
    		exit(FONT_LOAD_TTF_FAILED_ERROR);
    	}

    arial_big = al_load_ttf_font("fonts/ARLRDBD.TTF", 25, 0);

    if (!arial_big)
    {
    	logger("Failed to load Arial Big Font");
    	exit(FONT_LOAD_TTF_FAILED_ERROR);
    }

    arial_tiny = al_load_ttf_font("fonts/ARLRDBD.TTF", 10, 0);

       if (!arial_tiny)
       {
       	logger("Failed to load Arial Big Font");
       	exit(FONT_LOAD_TTF_FAILED_ERROR);
       }


}

void destroy_fonts()
{
	al_destroy_font(arial_rounded);
}
void destroy_bitmap_files()
{
 al_destroy_bitmap( floor1);
al_destroy_bitmap( floor2);
al_destroy_bitmap(sprite1);
al_destroy_bitmap(pill1);
al_destroy_bitmap(pill2);
al_destroy_bitmap(serf1);
al_destroy_bitmap(serf2);
al_destroy_bitmap( serf3);
al_destroy_bitmap( serf4);
al_destroy_bitmap( serb1);
al_destroy_bitmap( serb2);
al_destroy_bitmap( serb3);
al_destroy_bitmap( serb4);
al_destroy_bitmap( serr1);
al_destroy_bitmap( serr2);
al_destroy_bitmap( serr3);
al_destroy_bitmap( serr4);
al_destroy_bitmap( serl1);
al_destroy_bitmap( serl2);
al_destroy_bitmap( serl3);
al_destroy_bitmap( serl4);
al_destroy_bitmap( mech);
al_destroy_bitmap( wall1);
al_destroy_bitmap( wall2);
al_destroy_bitmap( wall3);
al_destroy_bitmap( wall4);
al_destroy_bitmap( chest);
al_destroy_bitmap( chest2);
al_destroy_bitmap( beda1);
al_destroy_bitmap( door1);
al_destroy_bitmap( arch1);
al_destroy_bitmap( arch2);
al_destroy_bitmap( textbox);
al_destroy_bitmap( stairs1);
al_destroy_bitmap( stairs2);
al_destroy_bitmap( ninjaf1);
al_destroy_bitmap( ninjaf2);
al_destroy_bitmap( ninjaf3);
al_destroy_bitmap( ninjaf4);
al_destroy_bitmap( ninjab1);
al_destroy_bitmap( ninjab2);                
al_destroy_bitmap( ninjab3);
al_destroy_bitmap( ninjab4);
al_destroy_bitmap( ninjar1);
al_destroy_bitmap( ninjar2);
al_destroy_bitmap( ninjar3);
al_destroy_bitmap( ninjar4);
al_destroy_bitmap( ninjal1);
al_destroy_bitmap( ninjal2);
al_destroy_bitmap( ninjal3);
al_destroy_bitmap( ninjal4);

al_destroy_bitmap( top1);
al_destroy_bitmap( top2);
al_destroy_bitmap( shield1);
al_destroy_bitmap( torch1);
al_destroy_bitmap( torch2);
al_destroy_bitmap( pic1);


al_destroy_bitmap( batlback);
al_destroy_bitmap( gold_cursor);
al_destroy_bitmap( select1);
al_destroy_bitmap( select2);


al_destroy_bitmap( map1);

al_destroy_bitmap( colblock);
al_destroy_bitmap( tagblock);
al_destroy_bitmap( slash1);
al_destroy_bitmap( flare1);
al_destroy_bitmap( meter1);

al_destroy_bitmap( convo_box);
 
}

//



//utility functions to interface with allegro

void clear(ALLEGRO_BITMAP *bitmap)
{
	//al_lock_bitmap(bitmap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);
    al_set_target_bitmap(bitmap);
    al_clear_to_color(CLEAR_COLOR);
    //al_unlock_bitmap(bitmap);
}

void load_magic_bitmap(ALLEGRO_BITMAP **bitmap, const char *path)
{
	if (path == NULL)
	{
		logger("Load Magic Bitmap Path Null Error:");
		logger(path);
		logger("\n");
		exit(LOAD_MAGIC_BITMAP_PATH_NULL_ERROR);
	}

	*bitmap = al_load_bitmap(path);

	if (*bitmap == NULL)
	{
		logger("Load Magic Bitmap, Bitmap Null Error:");
		logger(path);
		logger("\n");
		exit(LOAD_MAGIC_BITMAP_BITMAP_NULL_ERROR);
	}
	al_convert_mask_to_alpha(*bitmap, MAGIC_PINK_COLOR);
}


void load_alpha_bitmap(ALLEGRO_BITMAP **bitmap, const char *path)
{
	if (path == NULL)
	{
		logger("Load Bitmap Path Null Error:");
		logger(path);
		logger("\n");
		exit(LOAD_BITMAP_PATH_NULL_ERROR);
	}

	*bitmap = al_load_bitmap(path);

	if (*bitmap == NULL)
	{
		logger("Load Bitmap, Bitmap Error:");
		logger(path);
		logger("\n");
		exit(LOAD_BITMAP_BITMAP_NULL_ERROR);
	}
}

//map file structure utility functions

int getstartx()
{
	return TEMP_MAPX;
}

int getstarty()
{
	return TEMP_MAPY;
}

int getstartmapscrollx()
{
	return TEMP_SCROLLX;
}

int getstartmapscrolly()
{
	return TEMP_SCROLLY;
}

int getstartmapaction()
{
	return TEMP_MAPACTION; //used to render the direction the player is facing at level init
}

//*******Gameplay related functions***************//


void load_map(char mapname[], int maptype)
{

	//load map file
	if (maptype == 2)
	{
		load_map_v2(mapname);
	}
	if (maptype == 1) //original fasaga map format
	{
		load_map_file(mapname);
	}
	

	//****initialize character data from loaded map

	//initialize main character's position

	mainchar.xpos = getstartx();
	mainchar.ypos = getstarty(); 

	//initialize main character's action
	mainchar.action = getstartmapaction();

	//initialize scrolling window onto map 
	scroll_x = getstartmapscrollx();
	scroll_y = getstartmapscrolly();
}

void load_map_file(char savefile[])
{
 al_set_standard_file_interface();

 //note: need to output an error if this file is not found
 level = fopen(savefile, "r");

 
 if (level)
	 printf("level file was opened \n");
 else 
 {
	 printf("level file was not opened\n");
	 exit(MAP_FILE_LOAD_ERROR);
 }
 

 //next step is to scan in the info for the first map...//
 for (int yslot=0;yslot<TILE_GRID_MAX_HEIGHT;yslot++)
  for (int xslot=0;xslot<TILE_GRID_MAX_WIDTH;xslot++)
	fread(&bottombmp[xslot][yslot], sizeof(int), 1, level);
 
 for (int yslot=0;yslot<TILE_GRID_MAX_HEIGHT;yslot++)
  for (int xslot=0;xslot<TILE_GRID_MAX_WIDTH;xslot++)
	fread(&midbmp[xslot][yslot], sizeof(int), 1, level);  

 for (int yslot=0;yslot<TILE_GRID_MAX_HEIGHT;yslot++)
  for (int xslot=0;xslot<TILE_GRID_MAX_WIDTH;xslot++)
	fread(&mid2bmp[xslot][yslot], sizeof(int), 1, level);  

 for (int yslot=0;yslot<TILE_GRID_MAX_HEIGHT;yslot++)
  for (int xslot=0;xslot<TILE_GRID_MAX_WIDTH;xslot++)
	fread(&hibmp[xslot][yslot], sizeof(int), 1, level);  

 for (int yslot=0;yslot<TILE_GRID_MAX_HEIGHT;yslot++)
  for (int xslot=0;xslot<TILE_GRID_MAX_WIDTH;xslot++)
	fread(&collision[xslot][yslot], sizeof(int), 1, level);  

 for (int yslot=0;yslot<TILE_GRID_MAX_HEIGHT;yslot++)
  for (int xslot=0;xslot<TILE_GRID_MAX_WIDTH;xslot++)
	fread(&tagmap[xslot][yslot], sizeof(int), 1, level);  
 
 //read in tag type structures
 for (int slot=0;slot<MAX_NUMBER_TAG_TYPES;slot++)
 {
  fread(&tags[slot].scnenum, sizeof(int), 1, level);
  fread(&tags[slot].pressa, sizeof(int), 1, level);
  fread(&tags[slot].item, sizeof(int), 1, level);
  fread(&tags[slot].patch[0], sizeof(int), 1, level);
  fread(&tags[slot].patch[1], sizeof(int), 1, level);
  fread(&tags[slot].patch[2], sizeof(int), 1, level);
  fread(&tags[slot].scne[0], sizeof(int), 1, level);
  fread(&tags[slot].scne[1], sizeof(int), 1, level);
  fread(&tags[slot].overworld, sizeof(int), 1, level);
 }

 //read in sprite types structure
 for (int slot=0;slot<MAX_NUMBER_NPC_SPRITES;slot++)
 { 
  fread(&sprites[slot].xpos, sizeof(int), 1, level);
  fread(&sprites[slot].ypos, sizeof(int), 1, level);
  fread(&sprites[slot].zpos, sizeof(int), 1, level);
  fread(&sprites[slot].bmpid, sizeof(int), 1, level);
  fread(&sprites[slot].action, sizeof(int), 1, level);
  fread(&sprites[slot].gp, sizeof(int), 1, level);
  fread(&sprites[slot].actrig, sizeof(int), 1, level);
  fread(&sprites[slot].immobile, sizeof(int), 1, level);
 }

 //note: newly added tag feature, level's have to be
 //      resaved to be able to move this to above with
 //      the rest of the tag info
 for (int slot=0;slot<10;slot++)
  fread(&tags[slot].tagjmp, sizeof(int), 1, level);
 
 //note: temporary.indicates the type of monster encountered
 //      in this level. should be adapted to use a more dynamic
 //      format. (structure indicating types and frequencies of monsters)
 fread(&mnster, sizeof(int), 1, level);
 
 fclose(level);

} 


//utility functions for whole game engine

void logger(const char * text)
{
	log = fopen("fsaga.log", "a");
	fputs("\n",log);
	fputs(text, log);
	fputs("\n",log);
	fclose(log);
}

void logint(int errcode)
{
	log = fopen("fsaga.log", "a");
	fputs("\n",log);
	fprintf(log, "%d", errcode);
	fputs("\n",log);
	fclose(log);
}


//******************Rendering routines********************

void center(void)
{

	//centering "magic number" routine
 scroll_x = mainchar.xpos-SCROLL_CENTER_MAGIC_X;
 scroll_y = mainchar.ypos-SCROLL_CENTER_MAGIC_Y;

 if (scroll_x < 0) {
	 scroll_x = 0;
 }
 if (scroll_y < 0) {
	 scroll_y = 0;
 }


 if ((scroll_x*SCROLL_SCALING_FACTOR_X + SCREEN_WIDTH) > MAX_MAP_WIDTH)
 {
	 scroll_x = (int)((MAX_MAP_WIDTH - SCREEN_WIDTH)/SCROLL_SCALING_FACTOR_X);
	// scroll_x = 0;
 }

 if ((scroll_y*SCROLL_SCALING_FACTOR_Y + SCREEN_HEIGHT) >= MAX_MAP_HEIGHT)
 {
	 scroll_y = (int)((MAX_MAP_HEIGHT - SCREEN_HEIGHT)/SCROLL_SCALING_FACTOR_Y);
	// scroll_y = 0;

 }

 logger("SCroll amount x ");
 logint(scroll_x);
 logger("Scroll amoutn y");
 logint(scroll_y);

}

void set_render_to_map_buffer(void)
{
	al_set_target_bitmap(map_buffer);
}

void renderer(void)
{
 clear(map_buffer); //will set the map_buffer as target and clear it
 renderbottom(); //render floor
 rendermid(); //render tiles below player (generally wall tiles)
 render_player_sprite(); //render player (was mrendersprite();)
 render_npc_sprite(); //renders the npc sprites (was rendersprite();)
 renderhi(); //render tiles above player (generally ceiling tiles)
 rendertop(); //Note: render top layer, things such as clouds or fog -- new feature

 blitter(); ////renders map buffer to back buffer and flips the back buffer with the screen
}


//renders map buffer to back buffer and flips the back buffer with the screen
void blitter(void)
{
 al_set_target_bitmap(al_get_backbuffer(display));
 al_draw_bitmap_region(map_buffer, scroll_x*SCROLL_SCALING_FACTOR_X, scroll_y*SCROLL_SCALING_FACTOR_Y, SCREEN_WIDTH,SCREEN_HEIGHT, SCREEN_ORIGIN_X, SCREEN_ORIGIN_Y, NULL);

 renderInterfaceLayer();

 al_flip_display();
}
 

//********DRAW THE MAP


void drawtile(ALLEGRO_BITMAP *bitmap, int xcor, int ycor)
{
	if (bitmap == NULL)
		exit(DRAW_TILE_BITMAP_NULL_FAIL_ERROR);
	/*al_draw_scaled_bitmap(bitmap,
	    0,0,al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap),	
		xcor*TILE_SCALE_FACTOR_X+TILE_TRANSLATE_FACTOR_X, ycor*TILE_SCALE_FACTOR_Y+TILE_TRANSLATE_FACTOR_Y, TILE_DEST_SIZE_WIDTH, TILE_DEST_SIZE_HEIGHT, NULL);
		*/

	al_draw_bitmap(bitmap, xcor*TILE_SCALE_FACTOR_X+TILE_TRANSLATE_FACTOR_X,
									ycor*TILE_SCALE_FACTOR_Y+TILE_TRANSLATE_FACTOR_Y, NULL);
		

}

void drawplayersprite(ALLEGRO_BITMAP *bitmap, int xpos, int ypos)
{
	if (bitmap == NULL)
		exit(DRAW_PLAYER_SPRITE_BITMAP_NULL_ERROR);
	al_draw_scaled_bitmap(bitmap,
		0,0,al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap),
		xpos*PLAYER_SPRITE_SCALE_FACTOR_X, ypos*PLAYER_SPRITE_SCALE_FACTOR_Y,
		PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT, NULL);
}

//note: for now just calls drawplayersprite
void drawnpcsprite(ALLEGRO_BITMAP *bitmap, int xpos, int ypos)
{
	drawplayersprite(bitmap, xpos, ypos);
}




void rendertiles(int a[TILE_GRID_MAX_WIDTH][TILE_GRID_MAX_HEIGHT], int x, int y)
{
	//if (a[1][1] == 5)
		//printf("Success!!");
	switch (a[x][y])
	{
	case 1:
		drawtile(floor1, x, y);
		break;
	case 2:
		drawtile(floor2, x, y);
		break;
	case 3:
		drawtile(wall1, x, y);
		break;
	case 4:
		drawtile(wall2, x, y);
		break;
	case 5:
		printf("Found tile id 5");
		drawtile(wall3, x, y);
		break;
	case 6:
		drawtile(wall4, x, y);
		break;
	case 7:
		drawtile(chest, x, y);
		break;
	case 8:
		drawtile(chest2, x, y);
		break;
	case 9:
		drawtile(beda1, x, y);
		break;
	case 10:
		drawtile(door1, x, y);
		break;
	case 11:
		drawtile(arch1, x, y);
		break;
	case 12:
		drawtile(arch2, x, y);
		break;
	case 13:
		drawtile(stairs1, x, y);
		break;
	case 14:
		drawtile(stairs2, x, y);
		break;
	case 15:
		drawtile(top1, x, y);
		break;
	case 16:
		drawtile(top2, x, y);
		break;
	case 17:
		drawtile(shield1, x, y);
		break;
	case 18:
		drawtile(torch1, x, y);
		break;
	case 19:
		drawtile(torch2, x, y);
		break;
	case 20:
		drawtile(pill1, x, y);
		break;
	case 21:
		drawtile(t_grass, x, y);
		break;
	case 22:
		drawtile(t_rock, x, y);
		break;
	case 23:
		drawtile(t_tree, x, y);
		break;
	default:
		break;
	}
}




void renderbottom(void)
{
 

 for (int ycor=0;ycor<TILE_GRID_MAX_HEIGHT;ycor++)
 {
  for (int xcor=0;xcor<TILE_GRID_MAX_WIDTH;xcor++)
  {
	  //printf("Rendering bottom bmp tileset");
	  rendertiles(bottombmp, xcor, ycor);
	  /*
	  switch (bottombmp[xcor][ycor])
	  {


	  case 1:
	    drawtile(floor1, xcor, ycor);
		break;
	  case 2:
		drawtile(floor2, xcor, ycor);
		break;
	  default:
		  break;

	  }*/
   }
  }
}


void rendermid(void)
{

 

 for (int ycor=0;ycor<TILE_GRID_MAX_HEIGHT;ycor++)
 {
  for (int xcor=0;xcor<TILE_GRID_MAX_WIDTH;xcor++)
  {
	  rendertiles(midbmp, xcor, ycor);
	  rendertiles(mid2bmp, xcor, ycor);

	  //draw walls
	  /*
	  switch (midbmp[xcor][ycor])
	  {
   
	  case 1:
		  drawtile(wall1, xcor, ycor);
		  break;
	  case 2:
		  drawtile(wall2, xcor, ycor);
		  break;
	  case 3:
		  drawtile(chest, xcor, ycor);
		  break;
	  case 4:
		  drawtile(beda1, xcor, ycor);
		  break;
	  case 5:
		  drawtile(chest2, xcor, ycor);
		  break;
	  case 6:
		  drawtile(wall3, xcor, ycor);
		  break;
	  case 7:
		  drawtile(wall4, xcor, ycor);
		  break;
	  }

	  //draw wall "patch" bitmaps, things that go over top of
	  //walls like pillars, torches, shields, or stairs..
	  switch (mid2bmp[xcor][ycor])
	  {
	  case 1:
		  drawtile(arch1, xcor, ycor);
		  break;
	  case 2:
		  drawtile(arch2, xcor, ycor);
		  break;
	  case 3:
		  drawtile(door1, xcor, ycor);
		  break;
	  case 4:
		  drawtile(pill1, xcor, ycor);
		  break;
	  case 5:
		  drawtile(pill2, xcor, ycor);
		  break;
	  case 6:
		  drawtile(shield1, xcor, ycor);
		  break;
	  case 7:
		  drawtile(torch1, xcor, ycor);
		  break;
	  case 8:
		  drawtile(torch2, xcor, ycor);
		  break;
	  case 9:
		  drawtile(stairs1, xcor, ycor);
		  break;
	  case 10:
	      drawtile(stairs2, xcor, ycor);
		  break;
	  }*/
	}
  }
}

void renderhi(void)
{
 
 for (int ycor=0;ycor<100;ycor++)
 {
  for (int xcor=0;xcor<100;xcor++)
  {
	  /*
	  switch(hibmp[xcor][ycor])
	  {
	  case 1:
		  drawtile(top1, xcor, ycor);
		  break;
	  case 2:
		  drawtile(top2, xcor, ycor);
		  break;
	  }*/
	  rendertiles(hibmp, xcor, ycor);

  }
 }
}


//version of custom rendertop from editor, but with unidirectional translation
//animation for bitmaps
void rendertop(void)
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
			case 6:
				top_bitmap = t_cloud;
				break;
			default:
				break;
			}



			if (value > 0) {

				srand (time(NULL) ); //seed random generator

				//cloud tint with random tint between 0.50 and 0.75
				float rantint = (float)(rand() % 10 + 1);  //between 1 and 10
				rantint /= 25; //divide by 20, so between 0.1 and 0.25
				rantint += 0.5; //shift so between 0.6 and 0.85


				//draw cloud tinted !!with random tint
				//below draws iwth 50% tint
				//al_draw_tinted_bitmap(top_bitmap, al_map_rgba_f(1, 1, 1, 0.5), x, y, 0);			}


				int x_dest_value = x*TOP_SCALE_FACTOR_X+TOP_TRANSLATE_FACTOR_X + top_anim_x_roller;

				int y_dest_value = y*TOP_SCALE_FACTOR_Y+TOP_TRANSLATE_FACTOR_Y  + top_anim_y_roller;


				//
				x_dest_value = x_dest_value % MAX_MAP_WIDTH;
				y_dest_value = y_dest_value % MAX_MAP_HEIGHT;

				//tinted by 50% and scaled
				al_draw_tinted_scaled_bitmap(top_bitmap,
						al_map_rgba_f(rantint, rantint, rantint, 0.05),
						0, 0, al_get_bitmap_width(top_bitmap),al_get_bitmap_height(top_bitmap),
						x_dest_value, y_dest_value,
						al_get_bitmap_width(top_bitmap)/2, al_get_bitmap_height(top_bitmap)/2, NULL);
			}
		}
	}
}




//*****DRAW THE PLAYER

void render_player_sprite()
{
  //Note: This if clause was in the original but I'm not sure that
  //	  it is necessary or what it was used for
  //if (mainchar.xpos == tile_coord_x && mainchar.ypos == tile_coord_y)
  //{
	logger("Player action:");
	logint(mainchar.action);

	switch(mainchar.action)
	{
	case FRONT_STAND_1:
		logger("draw player stand front");
		drawplayersprite(serf1, mainchar.xpos, mainchar.ypos);
		break;
	case FRONT_MOVE_1:
		logger("draw player move 1 front");
		drawplayersprite(serf1, mainchar.xpos, mainchar.ypos);
		break;
	case FRONT_MOVE_2:
		drawplayersprite(serf2, mainchar.xpos, mainchar.ypos);
		break;
	case FRONT_MOVE_3:
		drawplayersprite(serf3, mainchar.xpos, mainchar.ypos);
		break;
	case FRONT_MOVE_4:
		drawplayersprite(serf4, mainchar.xpos, mainchar.ypos);
		break;
	case FRONT_MOVE_5:
		drawplayersprite(serf5, mainchar.xpos, mainchar.ypos);
		break;
	case FRONT_MOVE_6:
		drawplayersprite(serf6, mainchar.xpos, mainchar.ypos);
		break;
	case FRONT_MOVE_7:
		drawplayersprite(serf7, mainchar.xpos, mainchar.ypos);
		break;
	case FRONT_MOVE_8:
		drawplayersprite(serf8, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_STAND_1:
		logger("draw player stand back");
		drawplayersprite(serb1, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_MOVE_1:
		logger("draw player move 1 back");
		drawplayersprite(serb1, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_MOVE_2:
		drawplayersprite(serb2, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_MOVE_3:
		drawplayersprite(serb3, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_MOVE_4:
		drawplayersprite(serb4, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_MOVE_5:
		drawplayersprite(serb5, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_MOVE_6:
		drawplayersprite(serb6, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_MOVE_7:
		drawplayersprite(serb7, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_MOVE_8:
		drawplayersprite(serb8, mainchar.xpos, mainchar.ypos);
		break;

	case LEFT_STAND_1:
		drawplayersprite(serl1, mainchar.xpos, mainchar.ypos);
		break;
	case LEFT_MOVE_1:
		drawplayersprite(serl1, mainchar.xpos, mainchar.ypos);
		break;
	case LEFT_MOVE_2:
		drawplayersprite(serl2, mainchar.xpos, mainchar.ypos);
		break;
	case LEFT_MOVE_3:
		drawplayersprite(serl3, mainchar.xpos, mainchar.ypos);
		break;
	case LEFT_MOVE_4:
		drawplayersprite(serl4, mainchar.xpos, mainchar.ypos);
		break;
	case LEFT_MOVE_5:
		drawplayersprite(serl5, mainchar.xpos, mainchar.ypos);
		break;
	case LEFT_MOVE_6:
		drawplayersprite(serl6, mainchar.xpos, mainchar.ypos);
		break;
	case LEFT_MOVE_7:
		drawplayersprite(serl7, mainchar.xpos, mainchar.ypos);
		break;
	case LEFT_MOVE_8:
		drawplayersprite(serl8, mainchar.xpos, mainchar.ypos);
		break;

	case RIGHT_STAND_1:
		drawplayersprite(serr1, mainchar.xpos, mainchar.ypos);
		break;
	case RIGHT_MOVE_1:
		drawplayersprite(serr1, mainchar.xpos, mainchar.ypos);
		break;
	case RIGHT_MOVE_2:
		drawplayersprite(serr2, mainchar.xpos, mainchar.ypos);
		break;
	case RIGHT_MOVE_3:
		drawplayersprite(serr3, mainchar.xpos, mainchar.ypos);
		break;
	case RIGHT_MOVE_4:
		drawplayersprite(serr4, mainchar.xpos, mainchar.ypos);
		break;
	case RIGHT_MOVE_5:
		drawplayersprite(serr5, mainchar.xpos, mainchar.ypos);
		break;
	case RIGHT_MOVE_6:
		drawplayersprite(serr6, mainchar.xpos, mainchar.ypos);
		break;
	case RIGHT_MOVE_7:
		drawplayersprite(serr7, mainchar.xpos, mainchar.ypos);
		break;
	case RIGHT_MOVE_8:
		drawplayersprite(serr8, mainchar.xpos, mainchar.ypos);
		break;

		//diagonal movements
	case DOWN_LEFT_MOVE_1:
		drawplayersprite(serdl1, mainchar.xpos, mainchar.ypos);
		break;
	case DOWN_LEFT_MOVE_2:
		drawplayersprite(serdl2, mainchar.xpos, mainchar.ypos);
		break;
	case DOWN_LEFT_MOVE_3:
		drawplayersprite(serdl3, mainchar.xpos, mainchar.ypos);
		break;
	case DOWN_LEFT_MOVE_4:
		drawplayersprite(serdl4, mainchar.xpos, mainchar.ypos);
		break;
	case DOWN_LEFT_MOVE_5:
		drawplayersprite(serdl5, mainchar.xpos, mainchar.ypos);
		break;
	case DOWN_LEFT_MOVE_6:
		drawplayersprite(serdl6, mainchar.xpos, mainchar.ypos);
		break;
	case DOWN_LEFT_MOVE_7:
		drawplayersprite(serdl7, mainchar.xpos, mainchar.ypos);
		break;
	case DOWN_LEFT_MOVE_8:
		drawplayersprite(serdl8, mainchar.xpos, mainchar.ypos);
		break;





	case DOWN_RIGHT_MOVE_1:
		drawplayersprite(serdr1, mainchar.xpos, mainchar.ypos);
		break;
	case DOWN_RIGHT_MOVE_2:
			drawplayersprite(serdr2, mainchar.xpos, mainchar.ypos);
			break;
	case DOWN_RIGHT_MOVE_3:
			drawplayersprite(serdr3, mainchar.xpos, mainchar.ypos);
			break;
	case DOWN_RIGHT_MOVE_4:
			drawplayersprite(serdr4, mainchar.xpos, mainchar.ypos);
			break;
	case DOWN_RIGHT_MOVE_5:
			drawplayersprite(serdr5, mainchar.xpos, mainchar.ypos);
			break;
	case DOWN_RIGHT_MOVE_6:
			drawplayersprite(serdr6, mainchar.xpos, mainchar.ypos);
			break;
	case DOWN_RIGHT_MOVE_7:
			drawplayersprite(serdr7, mainchar.xpos, mainchar.ypos);
			break;
	case DOWN_RIGHT_MOVE_8:
			drawplayersprite(serdr8, mainchar.xpos, mainchar.ypos);
			break;



	case UP_LEFT_MOVE_1:
		drawplayersprite(serul1, mainchar.xpos, mainchar.ypos);
		break;


	case UP_LEFT_MOVE_2:
		drawplayersprite(serul2, mainchar.xpos, mainchar.ypos);
		break;


	case UP_LEFT_MOVE_3:
		drawplayersprite(serul3, mainchar.xpos, mainchar.ypos);
		break;


	case UP_LEFT_MOVE_4:
		drawplayersprite(serul4, mainchar.xpos, mainchar.ypos);
		break;


	case UP_LEFT_MOVE_5:
		drawplayersprite(serul5, mainchar.xpos, mainchar.ypos);
		break;


	case UP_LEFT_MOVE_6:
		drawplayersprite(serul6, mainchar.xpos, mainchar.ypos);
		break;


	case UP_LEFT_MOVE_7:
		drawplayersprite(serul7, mainchar.xpos, mainchar.ypos);
		break;


	case UP_LEFT_MOVE_8:
		drawplayersprite(serul8, mainchar.xpos, mainchar.ypos);
		break;





	case UP_RIGHT_MOVE_1:
		drawplayersprite(serur1, mainchar.xpos, mainchar.ypos);
		break;
	case UP_RIGHT_MOVE_2:
		drawplayersprite(serur2, mainchar.xpos, mainchar.ypos);
		break;
	case UP_RIGHT_MOVE_3:
		drawplayersprite(serur3, mainchar.xpos, mainchar.ypos);
		break;
	case UP_RIGHT_MOVE_4:
		drawplayersprite(serur4, mainchar.xpos, mainchar.ypos);
		break;
	case UP_RIGHT_MOVE_5:
		drawplayersprite(serur5, mainchar.xpos, mainchar.ypos);
		break;
	case UP_RIGHT_MOVE_6:
		drawplayersprite(serur6, mainchar.xpos, mainchar.ypos);
		break;
	case UP_RIGHT_MOVE_7:
		drawplayersprite(serur7, mainchar.xpos, mainchar.ypos);
		break;
	case UP_RIGHT_MOVE_8:
		drawplayersprite(serur8, mainchar.xpos, mainchar.ypos);
		break;



	case FRONT_ATTACK_1:
		drawplayersprite(seraf1, mainchar.xpos, mainchar.ypos);
		break;
	case FRONT_ATTACK_2:
		drawplayersprite(seraf2, mainchar.xpos, mainchar.ypos);
		break;
	case FRONT_ATTACK_3:
		drawplayersprite(seraf3, mainchar.xpos, mainchar.ypos);
		break;
	case FRONT_ATTACK_4:
		drawplayersprite(seraf4, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_ATTACK_1:
		drawplayersprite(serab1, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_ATTACK_2:
		drawplayersprite(serab2, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_ATTACK_3:
		drawplayersprite(serab3, mainchar.xpos, mainchar.ypos);
		break;
	case BACK_ATTACK_4:
		drawplayersprite(serab4, mainchar.xpos, mainchar.ypos);
		break;
	case LEFT_ATTACK_1:
		drawplayersprite(seral1, mainchar.xpos, mainchar.ypos);
		break;
	case LEFT_ATTACK_2:
		drawplayersprite(seral2, mainchar.xpos, mainchar.ypos);
		break;
	case LEFT_ATTACK_3:
		drawplayersprite(seral3, mainchar.xpos, mainchar.ypos);
		break;
	case LEFT_ATTACK_4:
		drawplayersprite(seral4, mainchar.xpos, mainchar.ypos);
		break;
	case RIGHT_ATTACK_1:
		drawplayersprite(serar1, mainchar.xpos, mainchar.ypos);
		break;
	case RIGHT_ATTACK_2:
		drawplayersprite(serar2, mainchar.xpos, mainchar.ypos);
		break;
	case RIGHT_ATTACK_3:
		drawplayersprite(serar3, mainchar.xpos, mainchar.ypos);
		break;
	case RIGHT_ATTACK_4:
		drawplayersprite(serar4, mainchar.xpos, mainchar.ypos);
		break;


	case IDLE_FRAME_1:
		drawplayersprite(seridle1, mainchar.xpos, mainchar.ypos);
		break;
	case IDLE_FRAME_2:
		drawplayersprite(seridle2, mainchar.xpos, mainchar.ypos);
		break;

	case IDLE_FRAME_3:
		drawplayersprite(seridle3, mainchar.xpos, mainchar.ypos);
		break;

	case IDLE_FRAME_4:
		drawplayersprite(seridle4, mainchar.xpos, mainchar.ypos);
		break;
	}
}

void render_sprite_movement(int spritenum)
{
	//blue sprite
	/*
	char bluf[]= { "bluf1", "bluf1", "bluf2", "bluf2", "bluf3", "bluf3",
				   "bluf4", "bluf4", "blub1", "blub1", "blub2", "blub2",
				   "blub3", "blub3", "blub4", "blub4", "blul1", "blul1",
				   "blul2", "blul2", "blul3", "blul3", "blur1", "blur1",
				   "blur2", "blur2", "blur3", "blur3", "blur4", "blur4"
				  };
	*/

	  //which sprite type? in this case the blue guy
	  if (sprites[spritenum].bmpid == 1) {
		   //which animation frame?
		  //draw animation name from char bluf[] (since bmpid = 1)
		  drawplayersprite(bluf_bmps_array[sprites[spritenum].action], sprites[spritenum].xpos, sprites[spritenum].ypos);

	  }
	  else if (sprites[spritenum].bmpid == 2) {
		  drawplayersprite(ninja_bmps_array[sprites[spritenum].action], sprites[spritenum].xpos, sprites[spritenum].ypos);
	  }


}





void render_npc_sprite(void)
{

 //first thing to do is scan through all the sprites//
 for (int spritenum=0;spritenum<MAX_NUMBER_NPC_SPRITES;spritenum++)
 {
	 //Once again, not sure why this clause is here. I've commented it out
	 //for now.
 	 //if (sprites[spritenum].xpos == xc && sprites[spritenum].ypos == yc)
	//{

	 render_sprite_movement(spritenum);

 }
}

/*
	 if (sprites[spritenum].bmpid == 1) //if it's the sprite # 1--> player bitmap
   {								  //virtually identical to render_player_sprite
	switch(sprites[spritenum].action)
	{
	case FRONT_STAND_1:
		drawplayersprite(bluf1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case FRONT_MOVE_1:
		drawplayersprite(bluf1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case FRONT_MOVE_2:
		drawplayersprite(bluf1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case FRONT_MOVE_3:
		drawplayersprite(bluf2, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case FRONT_MOVE_4:
		drawplayersprite(bluf2, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case FRONT_MOVE_5:
		drawplayersprite(bluf3, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case FRONT_MOVE_6:
		drawplayersprite(bluf3, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case FRONT_MOVE_7:
		drawplayersprite(bluf4, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case FRONT_MOVE_8:
		drawplayersprite(bluf4, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;

	case BACK_STAND_1:
		drawplayersprite(blub1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case BACK_MOVE_1:
		drawplayersprite(blub1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case BACK_MOVE_2:
		drawplayersprite(blub1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case BACK_MOVE_3:
		drawplayersprite(blub2, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case BACK_MOVE_4:
		drawplayersprite(blub2, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case BACK_MOVE_5:
		drawplayersprite(blub3, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case BACK_MOVE_6:
		drawplayersprite(blub3, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case BACK_MOVE_7:
		drawplayersprite(blub4, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case BACK_MOVE_8:
		drawplayersprite(blub4, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;



	case LEFT_STAND_1:
		drawplayersprite(blul1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case LEFT_MOVE_1:
		drawplayersprite(blul1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case LEFT_MOVE_2:
		drawplayersprite(blul1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case LEFT_MOVE_3:
		drawplayersprite(blul2, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case LEFT_MOVE_4:
		drawplayersprite(blul2, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case LEFT_MOVE_5:
		drawplayersprite(blul3, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case LEFT_MOVE_6:
		drawplayersprite(blul3, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case LEFT_MOVE_7:
		drawplayersprite(blul4, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case LEFT_MOVE_8:
		drawplayersprite(blul4, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;


	case RIGHT_STAND_1:
		drawplayersprite(blur1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case RIGHT_MOVE_1:
		drawplayersprite(blur1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case RIGHT_MOVE_2:
		drawplayersprite(blur1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case RIGHT_MOVE_3:
		drawplayersprite(blur2, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case RIGHT_MOVE_4:
		drawplayersprite(blur2, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case RIGHT_MOVE_5:
		drawplayersprite(blur3, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case RIGHT_MOVE_6:
		drawplayersprite(blur3, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case RIGHT_MOVE_7:
		drawplayersprite(blur4, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case RIGHT_MOVE_8:
		drawplayersprite(blur4, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;


	} //end switch
   } //end bmp == 1
*/
 /*  if (sprites[spritenum].bmpid == 2) //if it's the sprite # 1--> ninja bitmap
   {								  //virtually identical to render_player_sprite
	switch(sprites[spritenum].action)
	{
	case FRONT_STAND_1:
		drawnpcsprite(ninjaf1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case FRONT_MOVE_1:
		drawnpcsprite(ninjaf2, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case FRONT_MOVE_2:
		drawnpcsprite(ninjaf3, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case FRONT_MOVE_3:
		drawnpcsprite(ninjaf4, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case BACK_STAND_1:
		drawnpcsprite(ninjab1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case BACK_MOVE_1:
		drawnpcsprite(ninjab2, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case BACK_MOVE_2:
		drawnpcsprite(ninjab3, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case BACK_MOVE_3:
		drawnpcsprite(ninjab4, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case LEFT_STAND_1:
		drawnpcsprite(ninjal1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case LEFT_MOVE_1:
		drawnpcsprite(ninjal2, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case LEFT_MOVE_2:
		drawnpcsprite(ninjal3, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case LEFT_MOVE_3:
		drawnpcsprite(ninjal4, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case RIGHT_STAND_1:
		drawnpcsprite(ninjar1, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case RIGHT_MOVE_1:
		drawnpcsprite(ninjar2, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case RIGHT_MOVE_2:
		drawnpcsprite(ninjar3, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	case RIGHT_MOVE_3:
		drawnpcsprite(ninjar4, sprites[spritenum].xpos, sprites[spritenum].ypos);
		break;
	} //end switch
   } //end bmp == 2

*/

 //}//end for
//}


void load_map_v2(char loadfile[])
{
	FILE *level;

	//al_set_standard_file_interface();



	printf("Home directory is: %s\n", al_get_current_directory());


	//

	//first step is to open the file//
	level = fopen(loadfile, "r");


	if (level == NULL)
	{
		printf("Load File Failed");
		printf("Filename: %s", loadfile);
		perror(NULL);
		exit(-1);
	}

	//exit(1);


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
		fread(&tags[yslot].patch[3], sizeof(int), 1, level);
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


void change_dir_game_home()
{
	ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	al_append_path_component(path, "../");
	al_change_directory(al_path_cstr(path, '/'));  // change the working directory
	al_destroy_path(path);
}

