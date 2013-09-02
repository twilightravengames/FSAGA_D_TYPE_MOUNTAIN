#ifndef EDITORPROTOTYPES_HEADER
#define EDITORPROTOTYPES_HEADER

#include "editoruiconstants.h"

void mainedit(void);




//input routines
void key_down_up_input(int keycode);
void key_held_input(int keycode);


void customrenderer(void); //a custom renderer built just for the map editor

void rendercursor(void); //draws a simple cursor at cursor_xpos, cursor_ypos

void renderUI(void); //renders the map editor's UI

ALLEGRO_COLOR root_change_colors(int menutype); //update the root menu's colors dependent upon 
												//menu flags

int relocate_gold_cursor(int c_x, int c_y);

int move_gold_cursor(int dx, int dy);

void snap_gold_cursor();

void renderCursorData();

void renderCurrentLayerName();

void rendercollision();

void rendertagmap();


void rendergrid(void);

void renderborder();

void adjust_scroll(int x, int y);

void center_scrolling(int x, int y); //centers screen around input coordinates


void scroll_to(int x, int y);

int map_to_screen(int map, int flag);
int screen_to_map(int screen, int flag);

void adjust_keyboard_scroll(int dx, int dy);

void scroll_up(int amt);
void scroll_down(int amt);
void scroll_left(int amt);
void scroll_right(int amt);



void renderCurrentTileDisplay();
void renderTopLayerDisplay();

void renderCurrentSpriteDisplay();

void cycle_tiles(int amt);

void cycle_tags(int amt);

void place_tiles();

void clear_tiles();

void cycle_layers();

void cycle_sprite_ids(int amt);

void cycle_sprite_index(int amt);

void flip_spritelayer();

void rendersprites();

void drawplayersprite_edit(ALLEGRO_BITMAP *bitmap, int xpos, int ypos);

void drawspriteindexinfo(int id, int index, int x, int y);

//
void save_map_v2(char savefile[]);
void load_map_v2(char loadfile[]);


//clear bottom bitmap layer
void clear_tile_grids();
void clear_collision_grids();
void clear_tagmap_grids();
void clear_sprite_types();
void clear_tagmap_types();
void clearmap();


//dialog stuff
bool areyousurebox();

char getyesnodialoginput();
//void showdialogtext(char dialogtext[MAX_DIALOG_TEXT][MAX_DIALOG_LINES], int nlines, int x, int y);

void showdialogtext(char *dialogtext[MAX_DIALOG_LINES], int nlines, int x, int y);


void showdialogbackbox(int x, int y, int width, int height, ALLEGRO_COLOR color);
void end_render();




//save/load dialog stuff

void save_dialog_handle_key_input(ALLEGRO_KEYBOARD_EVENT event);
void save_dialog_handle_rendering();

void load_dialog_handle_key_input(ALLEGRO_KEYBOARD_EVENT event);
void load_dialog_handle_rendering();

void render_typewriter_text(int x, int y);

void render_typewriter_cursor(int x, int y);
void clear_bufffered_input_string();


//



bool is_empty_string(char *filename, int length);


void renderBackgroundBitmapName();

void cycle_background_image(int amt);

void render_back_bmps();
void renderbackbmp(int id);

void cycle_background_image_index(int amt);


void cycle_top_bmps(int amt);


void custom_rendertop(); //nonanimated version of rendertop

#endif



