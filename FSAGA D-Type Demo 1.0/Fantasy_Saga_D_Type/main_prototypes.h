#ifndef MAINPROTOTYPES_HEADER
#define MAINPROTOTYPES_HEADER


#include "shared/mapfilestructures.h"

//
void setup();
void maingame();
void shutdown();


//input routines
void key_down_up_input(int keycode);
void key_held_input(int keycode);

//animation routines
void action_moveup(sprite *character);
void action_movedown(sprite *character);
void action_moveleft(sprite *character);
void action_moveright(sprite *character);

void action_reset_sprite(sprite *character);
void action_reset_player_sprite(void);			//wrapper to action_reset_sprite

void action_update_sprites(sprite *character);

void npc_ai_wander(); //causes npcs to wander around the screen

void action_reset_all_sprites();

void update_all_sprites();

void update_top_animations();

//collision detection
bool checkcollisiontrue(sprite *character);

int check_collision(int xpos, int ypos, int character_id);


//rule-based conversation - checks to see if facing sprite for convo first
//then calls runaction
void sprite_action_chk();



//rule-based conversation/action system output
void runaction(int i);


//

void gamerenderer();

//
//Draws the text boxes to the screen with text from the TextWindow structure

void draw_text_windows();

void draw_text_background_rectangle();

//

void render_back_bmp();

void scrolly(int amt);
void scrollx(int amt);

void cycle_top_bmps(int amt);


#endif
