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
void action_moveup(sprite *character);          //Updates movement animation
void action_movedown(sprite *character);
void action_moveleft(sprite *character);
void action_moveright(sprite *character);

//additional movement directions
void action_movedown_left(sprite* character);
void action_movedown_right(sprite* character);

void action_moveup_left(sprite* character);
void action_moveup_right(sprite* character);


void action_change_player_idle(); //changes players animation etc. to idle

void action_reset_sprite(sprite *character);     //if in final frame, ceases animation
void action_reset_player_sprite(void);			//wrapper to action_reset_sprite

void action_update_sprites(sprite *character);

void npc_ai_wander(); //causes npcs to wander around the screen

void action_reset_all_sprites();

void update_all_sprites();

void update_top_animations();


//combat engine

bool mainchar_close_to_enemy(); 				//checks to see if main character is close to enemy

void action_mainchar_attack(sprite *character); //cause character to attack in facing direction

void action_attack_right(sprite *character); //update attack animation

void action_attack_left(sprite *character); //update attack animatino

void action_attack_down(sprite *character);  //update attack animation

void action_attack_up(sprite *character); //update attack animatino

//NOTE: action_reset_sprite will ALSO RESET ATTACK ANIMATIONS (FROM LAST FRAME TO STILL)

///


//collision detection
bool checkcollisiontrue(sprite *character);

int check_collision(int xpos, int ypos, int character_id);


//rule-based conversation - checks to see if facing sprite for convo first
//then calls runaction
void sprite_action_chk();



//rule-based conversation/action system output
void runaction(int i);

//rule-based tag scripting

void tag_action_chk(void);
void tag_action_activate_chk(void);
int run_tag_action(int tag_id);




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


//test functions

void findanytag_in_tagmap();



#endif
