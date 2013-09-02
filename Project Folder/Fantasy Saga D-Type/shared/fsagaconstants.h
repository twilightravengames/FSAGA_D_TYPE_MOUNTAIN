#ifndef FSAGACONSTANTS_HEADER
#define FSAGACONSTANTS_HEADER


//language related constants

#define ARRAY_LENGTH_LINE_TEXT_SPOKEN 78
#define ARRAY_LENGTH_NUM_LINES_SPOKEN 10


//display related constants
#define CLEAR_COLOR al_map_rgb(1,1,1)
#define MAGIC_PINK_COLOR al_map_rgb(255,0,255)
#define BLUE_COLOR al_map_rgb(0,0,255)
#define WHITE_COLOR al_map_rgb(255,255,255)
#define GREY_COLOR al_map_rgb(125, 125, 128)
#define BLACK_COLOR al_map_rgb(0,0,0)
#define YELLOW_COLOR al_map_rgb(255, 255, 0)
#define RED_COLOR al_map_rgb(255, 0, 0)
#define PURPLE_COLOR al_map_rgb(255, 0, 255)
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_ORIGIN_X 0
#define SCREEN_ORIGIN_Y 0

#define SCROLL_SCALING_FACTOR_X 20
#define SCROLL_SCALING_FACTOR_Y 10

#define DEFAULT_SCROLL_X 0
#define DEFAULT_SCROLL_Y 0

#define SCROLL_CENTER_MAGIC_X 15
#define SCROLL_CENTER_MAGIC_Y 22

//new sprite sizes
//#define SPRITE1_WIDTH 100
//#define SPRITE1_HEIGHT 100

#define SPRITE1_WIDTH 60
#define SPRITE1_HEIGHT 60

//map related constants

#define TEMP_MAPX 13
#define TEMP_MAPY 19
#define TEMP_MAPACTION 0
#define TEMP_SCROLLX 0
#define TEMP_SCROLLY 0 

#define FIRSTMAP "levels/main2.tpm"
#define SECONDMAP "levels/main1.tpm"
#define NEW_FIRSTMAP  "levels/new_main1.tpm"
#define NEW_SECONDMAP "levels/new_main2.tpm"
#define MOUNTAINMAP    "levels/mountain3.tpm"
//#define DEFAULTMAP     "/levels/default.tpm"
#define DEFAULTMAP	"levels/forest.tpm"

#define MOUNTAINXMAP "levels/mountainx.tpm"
#define A1MAP "levels/a1.tpm"
#define A2MAP "levels/a2.tpm"
#define FMAP  "levels/FMAP.TPM"

//maximum size for the map in pixels
#define MAX_MAP_WIDTH 1280
#define MAX_MAP_HEIGHT 960

//maximum number of tag 'types' allowed per map
//comment: numbers in tag matrix are actually look-up values
//to these tag types
#define MAX_NUMBER_TAG_TYPES 20


//maximum number of npc sprites allowed per map
//comment: numbers in sprite array
#define MAX_NUMBER_NPC_SPRITES 10

//maxmium number of tiles allowed in a map
#define TILE_GRID_MAX_WIDTH 100
#define TILE_GRID_MAX_HEIGHT 100

//tile scale and translate factors (used for converting from map grid to screen coordinates

#define TILE_SCALE_FACTOR_X 20
#define TILE_SCALE_FACTOR_Y 10
#define TILE_TRANSLATE_FACTOR_X 1
#define TILE_TRANSLATE_FACTOR_Y 1

//player sprite scale factors (used for converting from xpos and ypos to pixel coordinates)

#define PLAYER_SPRITE_SCALE_FACTOR_X 20
#define PLAYER_SPRITE_SCALE_FACTOR_Y 10
//new player sprite widths and heights
//#define PLAYER_SPRITE_HEIGHT 60
//#define PLAYER_SPRITE_WIDTH 60
#define PLAYER_SPRITE_HEIGHT 100
#define PLAYER_SPRITE_WIDTH 100


//player sprite source sizes

#define PLAYER_SPRITE_SOURCE_WIDTH 80
#define PLAYER_SPRITE_SOURCE_HEIGHT 80


//tile bitmap source sizes

#define TILE_SOURCE_SIZE_WIDTH 60
#define TILE_SOURCE_SIZE_HEIGHT 60

//tile bitmap destination sizes

//temporarily changing tile size
//#define TILE_DEST_SIZE_WIDTH 40
//#define TILE_DEST_SIZE_HEIGHT 40
#define TILE_DEST_SIZE_WIDTH 80
#define TILE_DEST_SIZE_HEIGHT 80


//tile/texture layer types

#define TILE_LAYER_BOTTOM 0 
#define TILE_LAYER_MIDDLE1 1
#define TILE_LAYER_MIDDLE2 2
#define TILE_LAYER_SPRITE 3
#define TILE_LAYER_HI 4
#define TILE_LAYER_TOP 5
#define TILE_LAYER_BLEND 6
#define TILE_LAYER_COLLISION 7
#define TILE_LAYER_TAG 8





///these are sprite movement types or "actions"///

#define FRONT_STAND_1   0     //face=front,action=stand,frame=1
#define BACK_STAND_1    1
#define LEFT_STAND_1    2
#define RIGHT_STAND_1   3
#define BACK_MOVE_1     4
#define BACK_MOVE_2     5
#define BACK_MOVE_3     6
#define BACK_MOVE_4		7
#define BACK_MOVE_5		8
#define BACK_MOVE_6		9
#define BACK_MOVE_7		10
#define BACK_MOVE_8		11
#define FRONT_MOVE_1    12
#define FRONT_MOVE_2    13
#define FRONT_MOVE_3    14
#define FRONT_MOVE_4	15
#define FRONT_MOVE_5	16
#define FRONT_MOVE_6	17
#define FRONT_MOVE_7	18
#define FRONT_MOVE_8	19
#define LEFT_MOVE_1    20
#define LEFT_MOVE_2    21
#define LEFT_MOVE_3    22
#define LEFT_MOVE_4		23
#define LEFT_MOVE_5		24
#define LEFT_MOVE_6		25
#define LEFT_MOVE_7		26
#define LEFT_MOVE_8		27
#define RIGHT_MOVE_1   28
#define RIGHT_MOVE_2   29
#define RIGHT_MOVE_3   30
#define RIGHT_MOVE_4	31
#define RIGHT_MOVE_5	32
#define RIGHT_MOVE_6	33
#define RIGHT_MOVE_7	34
#define RIGHT_MOVE_8	35

//-diagonal movements
#define DOWN_LEFT_MOVE_1 36
#define DOWN_LEFT_MOVE_2 37
#define DOWN_LEFT_MOVE_3 38
#define DOWN_LEFT_MOVE_4 39
#define DOWN_LEFT_MOVE_5 40
#define DOWN_LEFT_MOVE_6 41
#define DOWN_LEFT_MOVE_7 42
#define DOWN_LEFT_MOVE_8 43

#define DOWN_RIGHT_MOVE_1 44
#define DOWN_RIGHT_MOVE_2 45
#define DOWN_RIGHT_MOVE_3 46
#define DOWN_RIGHT_MOVE_4 47
#define DOWN_RIGHT_MOVE_5 48
#define DOWN_RIGHT_MOVE_6 49
#define DOWN_RIGHT_MOVE_7 50
#define DOWN_RIGHT_MOVE_8 51

#define UP_LEFT_MOVE_1 52
#define UP_LEFT_MOVE_2 53
#define UP_LEFT_MOVE_3 54
#define UP_LEFT_MOVE_4 55
#define UP_LEFT_MOVE_5 56
#define UP_LEFT_MOVE_6 57
#define UP_LEFT_MOVE_7 58
#define UP_LEFT_MOVE_8 59

#define UP_RIGHT_MOVE_1 60
#define UP_RIGHT_MOVE_2 61
#define UP_RIGHT_MOVE_3 62
#define UP_RIGHT_MOVE_4 63
#define UP_RIGHT_MOVE_5 64
#define UP_RIGHT_MOVE_6 65
#define UP_RIGHT_MOVE_7 66
#define UP_RIGHT_MOVE_8 67

#define IDLE_FRAME_1 68
#define IDLE_FRAME_2 69
#define IDLE_FRAME_3 70
#define IDLE_FRAME_4 71


#define MAX_SPRITE_BITMAPS 68  //number of sprite frames + 1


#define STANDING_CONSTANT 4 //all actions numbered 

// attack constants

#define RIGHT_ATTACK_1 72
#define RIGHT_ATTACK_2 73
#define RIGHT_ATTACK_3 74
#define RIGHT_ATTACK_4 75
#define LEFT_ATTACK_1  76
#define LEFT_ATTACK_2  77
#define LEFT_ATTACK_3  78
#define LEFT_ATTACK_4 79
#define FRONT_ATTACK_1 80
#define FRONT_ATTACK_2 81
#define FRONT_ATTACK_3 82
#define FRONT_ATTACK_4 83
#define BACK_ATTACK_1 84
#define BACK_ATTACK_2 85
#define BACK_ATTACK_3 86
#define BACK_ATTACK_4 87


//

#define MAX_NUM_ITEM_TYPES 50


#define IDLE_TIME_THRESHOLD  10 //represents the amount of time before displaying idle animation
//measured in seconds



//character battle actions in waiting

#define FIGHT 1
#define LIMIT 2

//timer interval

#define TIMER_INTERVAL (1.0 / 30.0)	//seconds / tick (1/30 fps)?



//background image constant

#define NUMBER_STOCK_BACKGROUND_IMAGES 2

#define BACK_IMAGE_ORIGIN_X 0
#define BACK_IMAGE_ORIGIN_Y 0
#define BACK_IMAGE_WIDTH 1280 //#define MAX_MAP_WIDTH 1280
#define BACK_IMAGE_HEIGHT 960 //#define MAX_MAP_HEIGHT 960

//text box magic constant
#define  TEXT_BOX_FLOAT_IN 40


//Top Layer Bitmap Magic Transformations


#define TOP_SCALE_FACTOR_X 20
#define TOP_SCALE_FACTOR_Y 10
#define TOP_TRANSLATE_FACTOR_X -5
#define TOP_TRANSLATE_FACTOR_Y -30


//exit error constants

#define ALLEGRO_INIT_FAIL_ERROR 1
#define ALLEGRO_TIMER_CREATE_FAIL_ERROR 2
#define INSTALL_KEYBOARD_ERROR 4
#define NO_IMAGE_ADDON_LOAD_FAIL_ERROR 5
#define LOAD_FAILED_ERROR 6
#define MAP_FILE_LOAD_ERROR 7
#define CREATE_DISPLAY_FAIL_ERROR 8
#define BITMAP_CREATE_FAIL_ERROR 9
#define MAP_BUFFER_CREATE_FAIL_ERROR 10
#define EVENT_QUEUE_CREATE_FAIL_ERROR 11
#define DRAW_TILE_BITMAP_NULL_FAIL_ERROR 12
#define LOAD_MAGIC_BITMAP_PATH_NULL_ERROR 13
#define LOAD_MAGIC_BITMAP_BITMAP_NULL_ERROR 14
#define DRAW_PLAYER_SPRITE_BITMAP_NULL_ERROR 15
#define ACTION_MOVE_PASSED_SPRITE_NULL_ERROR 16
#define FONT_LOAD_TTF_FAILED_ERROR 17
#define INSTALL_MOUSE_ERROR 18
#define ALLEGRO_CREATE_MOUSE_CURSOR_ERROR 19
#define ALLEGRO_FAILED_SETTING_MOUSE_CURSOR_ERROR 20
#define LOAD_BITMAP_PATH_NULL_ERROR 21
#define LOAD_BITMAP_BITMAP_NULL_ERROR 22
#define DRAW_GOLD_CUSOR_NULL_FAIL_ERROR 23
#define LOAD_TILE_BITMAP_FAILURE 24
#define DRAW_BACKGROUND_BITMAP_NULL_ERROR 25
#define NO_PRIMITIVES_ADDON_LOAD_FAIL_ERROR 26

#define BUFFERSPACE 10
#endif
