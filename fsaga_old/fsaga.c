#include <conio.h>
#include <stdio.h>
#include <allegro.h>

///these are sprite visible types or "actions"///

#define FRONT_STAND_1   0     //face=front,action=stand,frame=1
#define BACK_STAND_1    1
#define LEFT_STAND_1    2
#define RIGHT_STAND_1   3
#define BACK_MOVE_1     4
#define BACK_MOVE_2     5
#define BACK_MOVE_3     6
#define FRONT_MOVE_1    7
#define FRONT_MOVE_2    8
#define FRONT_MOVE_3    9
#define LEFT_MOVE_1    10
#define LEFT_MOVE_2    11
#define LEFT_MOVE_3    12
#define RIGHT_MOVE_1   13
#define RIGHT_MOVE_2   14
#define RIGHT_MOVE_3   15

//character actions in waiting

#define FIGHT 1
#define LIMIT 2


void init(void);
void maingame(void);
void renderer(void);
void input(void);
void renderbottom(void);
void rendermid(void);
void blitter(void);
void renderhi(void);
void rendersprite(void);
void actionhandler(void);
void tagcheck(void);
void load(void);
void tagchk();
void giveitem();
void txthandler();
void tagpatcher();
void scanner();
void checkcollide();
void spritechk(); //checks if sprite in front and does situation
void runaction(); //runs the actions
void mactionhandler(void);//main character action handler
void mrendersprite(void); //main character renderer
void bmpscanner(void);
void debugone();
void cleartxt();
void npc_ai();
void facer();
void scantags();
void center();
void battlechk();
void battleloop();
void renderbattle();
void inputbattle();
void load_batl();
void center_batl();
void sprite_batl();
void ct_handler();
void run_attack();
void atk_sub();
void exec_action();
void renderboxes();
void checkforzero();
void checkbatend();
void init_battle();
void won_battle();
void lost_battle();
void fadeout();
void enemy_ai();
void load_o();
void scantags_o();
void overworld();
void over_render();
void over_char();
void over_input();
void over_tagchk();
void over_action();
void over_check();
void enemy_showatk();


char demo[80];
int loopz;
int xc;
int yc;
int txpos;
int typos;
int tagjmpr;
int scrollx;
int scrolly;
int batsx; //battle scale x
int batsy; //battle scale y
int mnstercntr;
int data1;
int color;

BITMAP *floor1;
BITMAP *floor2;
BITMAP *buffer;
BITMAP *bufferx;
BITMAP *sprite1;
BITMAP *pill1;
BITMAP *pill2;
BITMAP *serf1;
BITMAP *serf2;
BITMAP *serf3;
BITMAP *serf4;
BITMAP *serb1;
BITMAP *serb2;
BITMAP *serb3;
BITMAP *serb4;
BITMAP *serr1;
BITMAP *serr2;
BITMAP *serr3;
BITMAP *serr4;
BITMAP *serl1;
BITMAP *serl2;
BITMAP *serl3;
BITMAP *serl4;
BITMAP *mech;
FILE *level;
PALETTE the_pallete;
COLOR_MAP trans_table;
BITMAP *wall1;
BITMAP *wall2;
BITMAP *wall3;
BITMAP *wall4;
BITMAP *chest;
BITMAP *chest2;
BITMAP *beda1;
BITMAP *door1;
BITMAP *arch1;
BITMAP *arch2;
BITMAP *textbox;
BITMAP *stairs1;
BITMAP *stairs2;
BITMAP *ninjaf1;
BITMAP *ninjaf2;
BITMAP *ninjaf3;
BITMAP *ninjaf4;
BITMAP *ninjab1;
BITMAP *ninjab2;                
BITMAP *ninjab3;
BITMAP *ninjab4;
BITMAP *ninjar1;
BITMAP *ninjar2;
BITMAP *ninjar3;
BITMAP *ninjar4;
BITMAP *ninjal1;
BITMAP *ninjal2;
BITMAP *ninjal3;
BITMAP *ninjal4;
BITMAP *serff1;
BITMAP *top1;
BITMAP *top2;
BITMAP *shield1;
BITMAP *torch1;
BITMAP *torch2;
BITMAP *pic1;
BITMAP *batlback;
BITMAP *cursor;
BITMAP *select1;
BITMAP *select2;
BITMAP *bitmap2;
BITMAP *map1;
BITMAP *colblock;
BITMAP *tagblock;
BITMAP *slash1;
BITMAP *flare1;
BITMAP *meter1;

char *argv[];
char filename[80];
char tmp[80];

int monnum;
int slow; //is slow mode on or not??
int actionchk; //is action check mode on??
int spritel;
int itemgive;
int yslot;
int xslot;
int looper;
int attacker;
int defender;
int slotter;
int scnenum; //current scnenum
int oldscne; //old scene number
char savefile[80];
char savefile_o[80];
char textstring[78];
int textchar;
char textstring2[78];
char textstring3[78];
char textstring4[78];
char textstring5[78];
char textstring6[78];
char textstring7[78];
char textstring8[78];
char textstring9[78];
char textstring10[78];

int bottombmp[100][100]; //bottom layer bitmap, under player always
int midbmp[100][100]; //mid layer bmp, squares that overlap the player's body
int mid2bmp[100][100];
int hibmp[100][100];
int topbmp[100][100]; //top layer bmp, giant bitmap like clouds in lufia2
int tagmap[100][100]; //locations of tags ---> their id's 
int collision[100][100]; //collision table for scene
int pages; //number of pages of text
int boxx;
int boxy;
int boxid;
int spritenum;
int mnster;    //monster types encountered
int looptt;

typedef struct tag{
int scnenum; //scene to jump to (#)
int pressa; //if you need to press a button to trigger it
int item; //item to get
int patch[3]; //bmp patch id, x, y, and layer number
int scne[2]; //scene jump coordinates
int overworld;
int actrig;
int tagjmp;
}tag;

tag tags[10];

//overworld
typedef struct tag_o{
int scnenum; //scene to jump to (#)
int pressa;
int scne[2]; //scne x,y start point after the jump
int overworld; //jump into overworld #
int actrig; //action/event trigger number
int tagjmp;
}tag_o;

tag_o tags_o[20];

int tagmap_o[100][100]; //locations of  ---> their id's
int collision_o[100][100]; //collision table for scene

int mapt_o;
int mapt;


int win;

typedef struct sprite{
int xpos;
int ypos;
int zpos;
int bmpid; //if bmpid = 1 then it is the main character if it is 0 it is inactive
int action;
int inventory[30]; //number of items in inventory
int gp; //number of gp that sprite carries
int actrig;
int immobile; //if this bit is set, the character will not move
}sprite;

sprite sprites[10];
sprite mainchar;
sprite mainchar_o;

///combat and character information variables///

typedef struct member{
int id; //ids for the member
int action; //current action frame to use
int inwait; //action in waiting
int intarget; //the target of the action in waiting
int xpos; //xposition
int ypos; //yposition
int special; //if he has special attacks
int magic; //if he has magic
int psychic; //if he has psychic powers
int will;     //if he has any wills yet
int partypos; //his position in the lineup for the party
int maxhp;     //his max hit points
int hp;        //Hit Points--used to measure strength left
int maxmp;     //his max magical points
int mp;        //Magic Points--used for Magical Attacks
int maxep;     //his max energy points
int ep;        //Energy Points--used for Psychic Attacks
int ct;        //his current time--how long till his turn
int level;     //the characters current level
int exp;       //amount of experience he has so far
int nextexp;  //amount of experience he needs for the next level
int agility;   //how far he can move in a turn
int attack;   //how much raw damage he can do
int defense;  //how much hits he can take unarmored (AP-(Defense+AD))
int speed;    //what his chance is of dodging the attack
int accuracy; //his chance of hitting the character and not missing or hit others
int wisdom;   //the strength of his spells
int lore;     //the defense of his spells
int psidefense; //the defense of his mind against psychic attacks
int luck;     //the chance of doing a critical hit
int skill;    //his chance of learning a new skill on next level
int body;     //his body armour
int head;     //his head armour (usually a helm)
int weapon;   //the weapon he is using
int ap;       //his armour power
int wp;       //his weapon power
int relics[5]; //the current relics equipped
int range; //current range
int limit; //current fill rate of his limit meter
}member;

member party[20];
member enemy[20];

int select; //used for menus


main()
{
 printf("|Fantasy Saga: Millenium|\n");
 printf("Loading...\n");
 init();
 printf("Load Ok!\n");
 printf("press any key...\n");
 readkey();
 set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
 clear(screen);
 clear(buffer);
 clear(bufferx);
 clear(bitmap2);
 clear(sprite1);
 maingame();
 destroy_bitmap(floor1);
 destroy_bitmap(buffer);
 destroy_bitmap(bufferx);
 destroy_bitmap(bitmap2);
 destroy_bitmap(sprite1);
 destroy_bitmap(serf1);
 destroy_bitmap(serff1);
 destroy_bitmap(serf2);
 destroy_bitmap(serf3);
 destroy_bitmap(serf4);
 destroy_bitmap(serb1);
 destroy_bitmap(serb2);
 destroy_bitmap(serb3);
 destroy_bitmap(serb4);
 destroy_bitmap(serr1);
 destroy_bitmap(serr2);
 destroy_bitmap(serr3);
 destroy_bitmap(serr4);
 destroy_bitmap(serl1);
 destroy_bitmap(serl2);
 destroy_bitmap(serl3);
 destroy_bitmap(serl4);
 destroy_bitmap(mech);
}

void init(void)
{
 mainchar.xpos = 13;
 mainchar.ypos = 19; 
 mainchar.action = 0;
  //
 allegro_init();
 install_keyboard();
 set_color_depth(16);
 set_color_conversion(COLORCONV_TOTAL);
 buffer = create_bitmap(1280, 960);  
 bufferx = create_bitmap(640, 480);
 bitmap2 = create_bitmap(640, 480);
 sprite1 = create_bitmap(60, 60);
 //first load data map//
 strcpy(savefile, "main1.tpm");
 load();
 create_trans_table(&trans_table, the_pallete, 128, 128, 128, NULL);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "floor1.bmp");
 floor1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "floor2.bmp");
 floor2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "pill1.bmp");
 pill1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "pill2.bmp");
 pill2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serf1.bmp");
 serf1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serf2.bmp");
 serf2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serf3.bmp");
 serf3 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serf4.bmp");
 serf4 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serb1.bmp");
 serb1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serb2.bmp");
 serb2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serb3.bmp");
 serb3 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serb4.bmp");
 serb4 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serr1.bmp");
 serr1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serr2.bmp");
 serr2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serr3.bmp");
 serr3 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serr4.bmp");
 serr4 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serl1.bmp");
 serl1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serl2.bmp");
 serl2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serl3.bmp");
 serl3 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serl4.bmp");
 serl4 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "wall1.bmp");
 wall1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "wall2.bmp");
 wall2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "wall3.bmp");
 wall3 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "wall4.bmp");
 wall4 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "chest.bmp");
 chest = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "chest2.bmp");
 chest2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "beda1.bmp");
 beda1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "door1.bmp");
 door1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "arch1.bmp");
 arch1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "arch2.bmp");
 arch2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "textbox.bmp");
 textbox = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "stairs1.bmp");
 stairs1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "stairs1.bmp");
 stairs2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjaf1.bmp");
 ninjaf1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjaf2.bmp");
 ninjaf2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjaf3.bmp");
 ninjaf3 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjaf4.bmp");
 ninjaf4 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjab1.bmp");
 ninjab1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjab2.bmp");
 ninjab2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjab3.bmp");
 ninjab3 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjab4.bmp");
 ninjab4 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjal1.bmp");
 ninjal1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjal2.bmp");
 ninjal2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjal3.bmp");
 ninjal3 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjal4.bmp");
 ninjal4 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjar1.bmp");
 ninjar1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjar2.bmp");
 ninjar2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjar3.bmp");
 ninjar3 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "ninjar4.bmp");
 ninjar4 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serf1.bmp");
 serff1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "top1.bmp");
 top1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "top2.bmp");
 top2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "shield1.bmp");
 shield1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "torch1.bmp");
 torch1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "slash.bmp");
 slash1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "flare1.bmp");
 flare1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "meter1.bmp");
 meter1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "torch2.bmp");
 torch2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "pic1.bmp");
 pic1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "cursor.bmp");
 cursor = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "select1.bmp");
 select1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "select2.bmp");
 select2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "colblock.bmp");
 colblock = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "tagblock.bmp");
 tagblock = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "mech.bmp");
 mech = load_bmp(filename, the_pallete);

}

void maingame(void)
{
 color = 0;
 looper = 0;
 text_mode(-1);
 while(looper == 0)
 {
  center();
  debugone();
  input();
  renderer();
  npc_ai();
  actionhandler();
  mactionhandler();
  tagcheck();
  battlechk();
 }
}

void input(void)
{
 int xcor;
 int ycor;
 if (key[KEY_UP])
 {
  if (mainchar.action == BACK_MOVE_3)
  {
   mainchar.action = BACK_MOVE_1;
  }
  if (mainchar.action < 4)
  {
   mainchar.action = BACK_MOVE_1;
  }
 }
 if (key[KEY_DOWN])
 {
  if (mainchar.action == FRONT_MOVE_3)
  {
   mainchar.action = FRONT_MOVE_1;
  }
  if (mainchar.action < 4)
  {
   mainchar.action = FRONT_MOVE_1;
  }
 }
 if (key[KEY_LEFT])
 {
  if (mainchar.action == LEFT_MOVE_3)
  { mainchar.action = LEFT_MOVE_1; }
  if (mainchar.action < 4)
  {
     mainchar.action = LEFT_MOVE_1;
  }
 }
 if (key[KEY_RIGHT])
 {
  if (mainchar.action == RIGHT_MOVE_3)
  { mainchar.action = RIGHT_MOVE_1; }
  if (mainchar.action < 4)
  {
     mainchar.action = RIGHT_MOVE_1;
  }
 }
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
 if (key[KEY_ESC])
 {
  looper = 4;
 }
}

void renderer(void)
{
 renderbottom(); //render floor
 rendermid(); //render tiles below player
 for (yc=0;yc<100;yc++)
 {
  for (xc=0;xc<100;xc++)
  {
   mrendersprite(); //render player
   rendersprite();
  }
 }
 renderhi(); //render tiles above player
 input();
 blitter();
}

void renderbottom(void)
{
 int xcor;
 int ycor;
 for (ycor=0;ycor<100;ycor++)
 {
  for (xcor=0;xcor<100;xcor++)
  {  
   if (bottombmp[xcor][ycor] == 1)
   {
    stretch_sprite(buffer, floor1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (bottombmp[xcor][ycor] == 2)
   {
    stretch_sprite(buffer, floor2, xcor*20+1, ycor*10+1, 40, 40);
   }
  }
 }
}

void rendermid(void)
{
 int xcor;
 int ycor;
 for (ycor=0;ycor<100;ycor++)
 {
  for (xcor=0;xcor<100;xcor++)
  {
   if (midbmp[xcor][ycor] == 1)
   {
    stretch_sprite(buffer, wall1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (midbmp[xcor][ycor] == 2)
   {
     stretch_sprite(buffer, wall2, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (midbmp[xcor][ycor] == 3)
   {
    stretch_sprite(buffer, chest, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (midbmp[xcor][ycor] == 4)
   {
     stretch_sprite(buffer, beda1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (midbmp[xcor][ycor] == 5)
   {
    stretch_sprite(buffer, chest2, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (midbmp[xcor][ycor] == 6)
   {
    stretch_sprite(buffer, wall3, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (midbmp[xcor][ycor] == 7)
   {
    stretch_sprite(buffer, wall4, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (mid2bmp[xcor][ycor] == 1)
   {
    stretch_sprite(buffer, arch1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (mid2bmp[xcor][ycor] == 2)
   {
    stretch_sprite(buffer, arch2, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (mid2bmp[xcor][ycor] == 3)
   {
    stretch_sprite(buffer, door1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (mid2bmp[xcor][ycor] == 4)
   {
    stretch_sprite(buffer, pill1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (mid2bmp[xcor][ycor] == 5)
   {
    stretch_sprite(buffer, pill2, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (mid2bmp[xcor][ycor] == 6)
   {
    stretch_sprite(buffer, shield1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (mid2bmp[xcor][ycor] == 7)
   {
    stretch_sprite(buffer, torch1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (mid2bmp[xcor][ycor] == 8)
   {
    stretch_sprite(buffer, torch2, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (mid2bmp[xcor][ycor] == 9)
   {
    stretch_sprite(buffer, stairs1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (mid2bmp[xcor][ycor] == 10)
   {
    stretch_sprite(buffer, stairs2, xcor*20+1, ycor*10+1, 40, 40);
   }
  }
 }
}

void renderhi(void)
{
 int xcor;
 int ycor;
 for (ycor=0;ycor<100;ycor++)
 {
  for (xcor=0;xcor<100;xcor++)
  {
   if (hibmp[xcor][ycor] == 1)
   {
    stretch_sprite(buffer, top1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (hibmp[xcor][ycor] == 2)
   {
    stretch_sprite(buffer, top2, xcor*20+1, ycor*10+1, 40, 40);
   }
  }
 }
}

void rendersprite(void)
{
 int spritenum;

 //first thing to do is scan through all the sprites//
 for (spritenum=0;spritenum<10;spritenum++)
 {
 if (sprites[spritenum].xpos == xc && sprites[spritenum].ypos == yc)
 {
   if (sprites[spritenum].bmpid == 1) //if it's the sprite # 1--> player
   {
   if (sprites[spritenum].action == FRONT_STAND_1)
   {
    stretch_sprite(buffer, serff1, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == FRONT_MOVE_1)
   {
    stretch_sprite(buffer, serf2, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == FRONT_MOVE_2)
   {
    stretch_sprite(buffer, serf3, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == FRONT_MOVE_3)
   {
    stretch_sprite(buffer, serf4, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == BACK_STAND_1)
   {
    stretch_sprite(buffer, serb1, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == BACK_MOVE_1)
   {
    stretch_sprite(buffer, serb2, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == BACK_MOVE_2)
   {
    stretch_sprite(buffer, serb3, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == BACK_MOVE_3)
   {
    stretch_sprite(buffer, serb4, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == LEFT_STAND_1)
   {
    stretch_sprite(buffer, serl1, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == LEFT_MOVE_1)
   {
    stretch_sprite(buffer, serl2, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == LEFT_MOVE_2)
   {
    stretch_sprite(buffer, serl3, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == LEFT_MOVE_3)
   {
    stretch_sprite(buffer, serl4, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == RIGHT_STAND_1)
   {
    stretch_sprite(buffer, serr1, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == RIGHT_MOVE_1)
   {
    stretch_sprite(buffer, serr2, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == RIGHT_MOVE_2)
   {
    stretch_sprite(buffer, serr3, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == RIGHT_MOVE_3)
   {
    stretch_sprite(buffer, serr4, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
  }
  if (sprites[spritenum].bmpid == 2) //if it's the sprite # 2--> ninja guard
  {
   if (sprites[spritenum].action == FRONT_STAND_1)
   {
    stretch_sprite(buffer, ninjaf1, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == FRONT_MOVE_1)
   {
    stretch_sprite(buffer, ninjaf2, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == FRONT_MOVE_2)
   {
    stretch_sprite(buffer, ninjaf3, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == FRONT_MOVE_3)
   {
    stretch_sprite(buffer, ninjaf4, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == BACK_STAND_1)
   {
    stretch_sprite(buffer, ninjab1, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == BACK_MOVE_1)
   {
   stretch_sprite(buffer, ninjab2, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == BACK_MOVE_2)
   {
    stretch_sprite(buffer, ninjab3, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == BACK_MOVE_3)
   {
    stretch_sprite(buffer, ninjab4, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == LEFT_STAND_1)
   {
    stretch_sprite(buffer, ninjal1, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == LEFT_MOVE_1)
   {
    stretch_sprite(buffer, ninjal2, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == LEFT_MOVE_2)
   {
    stretch_sprite(buffer, ninjal3, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == LEFT_MOVE_3)
   {
    stretch_sprite(buffer, ninjal4, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == RIGHT_STAND_1)
   {
    stretch_sprite(buffer, ninjar1, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == RIGHT_MOVE_1)
   {
    stretch_sprite(buffer, ninjar2, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == RIGHT_MOVE_2)
   {
    stretch_sprite(buffer, ninjar3, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
   if (sprites[spritenum].action == RIGHT_MOVE_3)
   {
    stretch_sprite(buffer, ninjar4, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   }
  }
  }
 }
}



void actionhandler(void)
{
 int ycor;
 int move;
 move = 0;
 for (spritel=0;spritel<10;spritel++)
 {
  if (sprites[spritel].action == BACK_MOVE_3)
  {
   sprites[spritel].action = BACK_STAND_1;
  }
  if (sprites[spritel].action == BACK_MOVE_2)
  {
   sprites[spritel].action = BACK_MOVE_3;
  }
  if (sprites[spritel].action == BACK_MOVE_1)
  {
   sprites[spritel].action = BACK_MOVE_2;
   sprites[spritel].ypos--;
   for (ycor=0;ycor<10;ycor++)
   {
    if (sprites[ycor].xpos == sprites[spritel].xpos && ycor != spritel)
    {
     if (sprites[ycor].ypos == sprites[spritel].ypos && ycor != spritel)
     {
      sprites[spritel].ypos++;
     }
    }
   }
   if (collision[sprites[spritel].xpos][sprites[spritel].ypos] == 1)
   {
    sprites[spritel].ypos++;
   }
  }
  if (sprites[spritel].action == FRONT_MOVE_3)
  {
   sprites[spritel].action = FRONT_STAND_1;
  }
  if (sprites[spritel].action == FRONT_MOVE_2)
  {
   sprites[spritel].action = FRONT_MOVE_3;
  }
  if (sprites[spritel].action == FRONT_MOVE_1)
  {
   sprites[spritel].action = FRONT_MOVE_2;
   sprites[spritel].ypos++;
   for (ycor=0;ycor<10;ycor++)
   {
    if (sprites[ycor].xpos == sprites[spritel].xpos && ycor != spritel)
    {
     if (sprites[ycor].ypos == sprites[spritel].ypos && ycor != spritel)
     {
      sprites[spritel].ypos--;
     }
    }
   }
   if (collision[sprites[spritel].xpos][sprites[spritel].ypos] == 1)
   {
    sprites[spritel].ypos--;
   }
  }
  if (sprites[spritel].action == LEFT_MOVE_3)
  {
   sprites[spritel].action = LEFT_STAND_1;
  }
  if (sprites[spritel].action == LEFT_MOVE_2)
  {
   sprites[spritel].action = LEFT_MOVE_3;
  }
  if (sprites[spritel].action == LEFT_MOVE_1)
  {
   sprites[spritel].action = LEFT_MOVE_2;
   sprites[spritel].xpos--;
   for (ycor=0;ycor<10;ycor++)
   {
    if (sprites[ycor].xpos == sprites[spritel].xpos && ycor != spritel)
    {
     if (sprites[ycor].ypos == sprites[spritel].ypos && ycor != spritel)
     {
      sprites[spritel].xpos++;
     }
    }
   }
   if (collision[sprites[spritel].xpos][sprites[spritel].ypos] == 1)
   {
    sprites[spritel].xpos++;
   }
  }
  if (sprites[spritel].action == RIGHT_MOVE_3)
  {
   sprites[spritel].action = RIGHT_STAND_1;
  }
  if (sprites[spritel].action == RIGHT_MOVE_2)
  {
   sprites[spritel].action = RIGHT_MOVE_3;
  }
  if (sprites[spritel].action == RIGHT_MOVE_1)
  {
   sprites[spritel].action = RIGHT_MOVE_2;
   sprites[spritel].xpos++;
   for (ycor=0;ycor<10;ycor++)
   {
    if (sprites[ycor].xpos == sprites[spritel].xpos && ycor != spritel)
    {
     if (sprites[ycor].ypos == sprites[spritel].ypos && ycor != spritel)
     {
      sprites[spritel].xpos--;      
     }
    }
   }
   if (collision[sprites[spritel].xpos][sprites[spritel].ypos] == 1)
   {
    sprites[spritel].xpos--;
   }
  }
 }
}


void blitter(void)
{
 blit(buffer, screen, scrollx*20, scrolly*10, 0, 0, 640, 480);  // copy it to the screen
 clear(buffer);
}

void tagcheck(void)
{
 if (tagmap[mainchar.xpos][mainchar.ypos] != 0 && tags[tagmap[mainchar.xpos][mainchar.ypos]].pressa == 0)
 {
  tagchk();
 }
}

void load(void)
{
 level = fopen(savefile, "r");
 //next step is to scan in the info for the first map...//
 for (yslot=0;yslot<100;yslot++)
 {
  for (xslot=0;xslot<100;xslot++)
  {
  fread(&bottombmp[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<100;yslot++)
 {
  for (xslot=0;xslot<100;xslot++)
  {
  fread(&midbmp[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<100;yslot++)
 {
  for (xslot=0;xslot<100;xslot++)
  {
  fread(&mid2bmp[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<100;yslot++)
 {
  for (xslot=0;xslot<100;xslot++)
  {
  fread(&hibmp[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<100;yslot++)
 {
  for (xslot=0;xslot<100;xslot++)
  {
  fread(&collision[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<100;yslot++)
 {
  for (xslot=0;xslot<100;xslot++)
  {
  fread(&tagmap[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<20;yslot++)
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
 }
 for (yslot=0;yslot<10;yslot++)
 {
  fread(&sprites[yslot].xpos, sizeof(int), 1, level);
  fread(&sprites[yslot].ypos, sizeof(int), 1, level);
  fread(&sprites[yslot].zpos, sizeof(int), 1, level);
  fread(&sprites[yslot].bmpid, sizeof(int), 1, level);
  fread(&sprites[yslot].action, sizeof(int), 1, level);
  fread(&sprites[yslot].gp, sizeof(int), 1, level);
  fread(&sprites[yslot].actrig, sizeof(int), 1, level);
  fread(&sprites[yslot].immobile, sizeof(int), 1, level);
 }
 for (yslot=0;yslot<10;yslot++)
 {
  fread(&tags[yslot].tagjmp, sizeof(int), 1, level);
 }
 fread(&mnster, sizeof(int), 1, level);
 fclose(level);
} 

void tagchk(void)
{
  int loopx;
  int tagnum;
  int looptag;
  int tagjmp;
  tagnum = tagmap[mainchar.xpos][mainchar.ypos];
  if (tags[tagnum].item != 0)
  {
   clear_keybuf();
   tagpatcher();
   cleartxt();
   itemgive = tags[tagnum].item;
   giveitem();
   itemgive = 0;
   tags[tagmap[mainchar.xpos][mainchar.ypos]].item = 0;
  }
  if (tags[tagnum].patch[0] != 0)
  {   
   tagpatcher();
   renderer();
  }
  if (tags[tagnum].overworld == 1)
  {
   tagpatcher();
   mainchar_o.xpos = tags[tagnum].scne[0];
   mainchar_o.ypos = tags[tagnum].scne[1];
   strcpy(savefile_o, "over1.ovw");
   tagjmpr = tags[tagnum].tagjmp;
   load_o();
   scantags_o();
   tagnum = 0;
   overworld();
  }
  if (tags[tagnum].scnenum == 1)
  {
   tagpatcher();
   mainchar.xpos = tags[tagnum].scne[0];
   mainchar.ypos = tags[tagnum].scne[1];
   strcpy(savefile, "main1.tpm");
   tagjmpr = tags[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
  }
  if (tags[tagnum].scnenum == 2)
  {
   tagpatcher();
   mainchar.xpos = tags[tagnum].scne[0];
   mainchar.ypos = tags[tagnum].scne[1];
   strcpy(savefile, "main2.tpm");
   load();
   tagnum = 0;
  }
  if (tags[tagnum].scnenum == 3)
  {
   tagpatcher();
   mainchar.xpos = tags[tagnum].scne[0];
   mainchar.ypos = tags[tagnum].scne[1];
   strcpy(savefile, "main3.tpm");
   tagjmpr = tags[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
  }
  if (tags[tagnum].scnenum == 4)
  {
   tagpatcher();
   mainchar.xpos = tags[tagnum].scne[0];
   mainchar.ypos = tags[tagnum].scne[1];
   strcpy(savefile, "main4.tpm");
   tagjmpr = tags[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
  }
  if (tags[tagnum].scnenum == 5)
  {
   tagpatcher();
   mainchar.xpos = tags[tagnum].scne[0];
   mainchar.ypos = tags[tagnum].scne[1];
   strcpy(savefile, "main5.tpm");
   tagjmpr = tags[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
  }
  if (tags[tagnum].scnenum == 6)
  {
   tagpatcher();
   mainchar.xpos = tags[tagnum].scne[0];
   mainchar.ypos = tags[tagnum].scne[1];
   strcpy(savefile, "main6.tpm");
   tagjmpr = tags[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
  }
  if (tags[tagnum].scnenum == 7)
  {
   tagpatcher();
   mainchar.xpos = tags[tagnum].scne[0];
   mainchar.ypos = tags[tagnum].scne[1];
   strcpy(savefile, "main7.tpm");
   tagjmpr = tags[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
  }
}

void giveitem(void)
{
 int ycor;
 int exit;
 for (ycor=0;ycor<30;ycor++)
 {
  if (mainchar.inventory[ycor] == 0)
  {
   if (itemgive == -1)
   {
    cleartxt();
    sprintf(textstring, "The box is empty!");
    textchar = 0;
    boxx = mainchar.xpos;
    boxy = mainchar.ypos;
    txthandler();
    mainchar.inventory[ycor] = 0;
    ycor = 31;
    clear_keybuf();
    exit = 1;
   }
   if (itemgive == 1)
   {
    cleartxt();
    sprintf(textstring, "Found 100 Gp!");
    textchar = 0;
    boxx = mainchar.xpos;
    boxy = mainchar.ypos;
    txthandler();
    //sprites[1].gp += 100;
    ycor = 31;
    clear_keybuf();
    exit = 1;
   }
   if (itemgive == 2)
   {
    cleartxt();
    sprintf(textstring, "Found a small potion");
    textchar = 0;
    boxx = mainchar.xpos;
    boxy = mainchar.ypos;
    txthandler();
    mainchar.inventory[ycor] = 1;
    ycor = 31;
    clear_keybuf();
    exit = 1;
   }
  }
  if (mainchar.inventory[30] != 0)
  {
   cleartxt();
   sprintf(textstring, "Inventory is full!!! 30 has %d", mainchar.inventory[30]);
   textchar = 0;
   boxx = mainchar.xpos;
   boxy = mainchar.ypos;
   txthandler();
   readkey();
  }
  if (exit == 1)
  {
   ycor = 31;
  }
 }
}

void tagpatcher(void)
{
 int tagnum;
 tagnum = tagmap[mainchar.xpos][mainchar.ypos];
 readkey();
 if (tags[tagnum].patch[0] == 1)
 {
  //will patch on image of opened treasure chest//
  midbmp[tags[tagnum].patch[1]][tags[tagnum].patch[2]] = 10; 
 }
 if (tags[tagnum].patch[0] == 2)
 {
  midbmp[tags[tagnum].patch[1]][tags[tagnum].patch[2]] = 8; 
 }
 if (tags[tagnum].patch[0] == 3)
 {
  midbmp[tags[tagnum].patch[1]][tags[tagnum].patch[2]] = 9; 
 }

}
void txthandler(void)
{
 int ytest;
 //do transition in//
 boxx = boxx*20;
 boxy = (boxy*10)-160;
 //if (boxx < 0)
 //{ boxx = 0; }
 //if (boxx > 320)
 //{ boxx = 320; }
 //if (boxy < 0)
 //{ boxy = 0; }
 //if (boxy > 320)
 //{ boxy = 320; }
 for (ytest=5;ytest>0;ytest--)
 {
  renderbottom(); //render floor
  rendermid(); //render tiles below player
  for (yc=0;yc<100;yc++)
  {
   for (xc=0;xc<100;xc++)
   {
    mrendersprite(); //render player
    rendersprite();
   }
  }
  renderhi(); //render tiles above player
  stretch_sprite(buffer, textbox, boxx + (32*ytest), boxy + (16*ytest), 320-64*ytest, 160-32*ytest);
  blitter();
 }
 //render text, etc..//
 renderbottom(); //render floor
 rendermid(); //render tiles below player
 for (yc=0;yc<100;yc++)
 {
  for (xc=0;xc<100;xc++)
  {
   mrendersprite(); //render player
   rendersprite();
  }
 }
 renderhi(); //render tiles above player
 stretch_sprite(buffer, textbox, boxx, boxy, 320, 160);
 textout(buffer, font, textstring, boxx+12, boxy+36, 1850);
 textout(buffer, font, textstring2, boxx+12, boxy+46, 1850);
 textout(buffer, font, textstring3, boxx+12, boxy+56, 1850);
 textout(buffer, font, textstring4, boxx+12, boxy+66, 1850);
 textout(buffer, font, textstring5, boxx+12, boxy+76, 1850);
 textout(buffer, font, textstring6, boxx+12, boxy+86, 1850);
 textout(buffer, font, textstring7, boxx+12, boxy+96, 1850);
 textout(buffer, font, textstring8, boxx+12, boxy+106, 1850);
 textout(buffer, font, textstring9, boxx+12, boxy+116, 1850);
 if (textchar == 0)
 {
  textout(buffer, font, "Seren", boxx+12, boxy+10, 1400);
  stretch_sprite(buffer, pic1, boxx+220, boxy+30, 70, 100);
 }
 if (textchar == 1)
 {
  textout(buffer, font, "Seren", boxx+12, boxy+10, 1400);
  stretch_sprite(buffer, pic1, boxx+220, boxy+30, 70, 100);
 } 
 if (textchar == 2)
 {
  textout(buffer, font, "Castle Guard", boxx+12, boxy+10, 1400);
  stretch_sprite(buffer, pic1, boxx+220, boxy+30, 70, 100);
 } 
 blitter();
 clear_keybuf();
 readkey();

 //render second page if it exists//

 if (pages > 1)
 {
  renderbottom(); //render floor
  rendermid(); //render tiles below player
  for (yc=0;yc<100;yc++)
  {
   for (xc=0;xc<100;xc++)
   {
    mrendersprite(); //render player
    rendersprite();
   }
  }
  renderhi(); //render tiles above player
  stretch_sprite(buffer, textbox, boxx, boxy, 320, 160);
  textout(buffer, font, textstring6, boxx+12, boxy+36, 1850);
  textout(buffer, font, textstring7, boxx+12, boxy+46, 1850);
  textout(buffer, font, textstring8, boxx+12, boxy+56, 1850);
  textout(buffer, font, textstring9, boxx+12, boxy+66, 1850);
  textout(buffer, font, textstring10, boxx+12, boxy+76, 1850);
  if (textchar == 0)
  {
   textout(buffer, font, "Seren", boxx+12, boxy+10, 1400);
   stretch_sprite(buffer, pic1, boxx, boxy, 70, 100);
  }
  if (textchar == 1)
  {
   textout(buffer, font, "Seren", boxx+12, boxy+10, 1400);
   stretch_sprite(buffer, pic1, boxy, boxx, 70, 100);
  } 
  if (textchar == 2)
  {
   textout(buffer, font, "Castle Guard", boxx+12, boxy+10, 1400);
   stretch_sprite(buffer, pic1, boxy, boxx, 70, 100);
  } 
  blitter();
  clear_keybuf();
  readkey();
 }

 //render transition out//

 for (ytest=0;ytest<5;ytest++)
 {
  renderbottom(); //render floor
  rendermid(); //render tiles below player
  for (yc=0;yc<100;yc++)
  {
   for (xc=0;xc<100;xc++)
   {
    mrendersprite(); //render player
    rendersprite();
   }
  }
  renderhi(); //render tiles above player
  stretch_sprite(buffer, textbox, boxx+32*ytest, boxy+16*ytest, 320-64*ytest, 160-32*ytest);
  blitter();
 }
 textchar = 0;
 pages = 0;
 clear_keybuf();
}


void scanner(void)
{
 int xcor;
 int ycor;
 int value;
 for (xcor=0;xcor<100;xcor++)
 {
  for (ycor=0;ycor<100;ycor++)
  {
   value = tagmap[mainchar.xpos][mainchar.ypos];
   if (value != 0)
   {
    sprintf(tmp, "Existing tag nums are %d", value);
    textout(buffer, font, tmp, 7, 7, 1850);
   }
  }
 }
}

void spritechk(void)
{
 //1st job is to scan through sprites and see if position is equal//
 int tagnum;
 loopz = 0;
 for (loopz=0;loopz<10;loopz++)
 {
  if (sprites[loopz].xpos == mainchar.xpos-1 && sprites[loopz].ypos == mainchar.ypos+1)
  {
   runaction();
  }
  if (sprites[loopz].xpos == mainchar.xpos && sprites[loopz].ypos == mainchar.ypos+1)
  {
   runaction();
  }
  if (sprites[loopz].xpos == mainchar.xpos+1 && sprites[loopz].ypos == mainchar.ypos+1)
  {
   runaction();
  }
  if (sprites[loopz].xpos == mainchar.xpos+1 && sprites[loopz].ypos == mainchar.ypos)
  {
   runaction();
  }
  if (sprites[loopz].xpos == mainchar.xpos+1 && sprites[loopz].ypos == mainchar.ypos-1)
  {
   runaction();
  }
  if (sprites[loopz].xpos == mainchar.xpos && sprites[loopz].ypos == mainchar.ypos-1)
  {
   runaction();
  }
  if (sprites[loopz].xpos-1 == mainchar.xpos && sprites[loopz].ypos == mainchar.ypos-1)
  {
   runaction();
  }
  if (sprites[loopz].xpos-1 == mainchar.xpos && sprites[loopz].ypos == mainchar.ypos)
  {
   runaction();
  }
 }
}

void runaction(void)
{
 if (sprites[loopz].actrig == 1)
 {
  cleartxt();
  boxx = sprites[loopz].xpos;
  boxy = sprites[loopz].ypos;
  textchar = sprites[loopz].bmpid;
  facer();
  sprintf(textstring, "There is something evil");
  sprintf(textstring2, "in this castle, I can"); 
  sprintf(textstring3, "feel it!");
  txthandler();
 }
 if (sprites[loopz].actrig == 2)
 {
  cleartxt();
  textchar = sprites[loopz].bmpid;
  boxx = sprites[loopz].xpos;
  boxy = sprites[loopz].ypos;
  sprintf(textstring, "Emperor Blade seeks to");
  sprintf(textstring2, "resurrect power to");
  sprintf(textstring3, "destroy our sister");
  sprintf(textstring4, "kingdom-Damon");
  facer();
  txthandler();
 } 
 if (sprites[loopz].actrig == 3)
 {
  cleartxt();
  textchar = sprites[loopz].bmpid;
  boxx = sprites[loopz].xpos;
  boxy = sprites[loopz].ypos;
  sprintf(textstring, "The Emperor seeks your");
  sprintf(textstring2, "presence immediately! Go");
  sprintf(textstring3, "up the outside hall and");
  sprintf(textstring4, "through the stairs until");
  sprintf(textstring5,  "you reach the outside.");
  sprintf(textstring6,  "Then continue up into his");
  sprintf(textstring7, "throne room.");
  pages = 2;
  facer();
  txthandler();
 } 
 if (sprites[loopz].actrig == 4)
 {
  cleartxt();
  textchar = sprites[loopz].bmpid;
  boxx = sprites[loopz].xpos;
  boxy = sprites[loopz].ypos;
  sprintf(textstring, "Hey, being succesor to the");
  sprintf(textstring2, "last commander must be");
  sprintf(textstring3, "exciting eh? I wish I");
  sprintf(textstring4, "had your power!");
  facer();
  txthandler();
 } 
 if (sprites[loopz].actrig == 5)
 {
  cleartxt();
  textchar = sprites[loopz].bmpid;
  boxx = sprites[loopz].xpos;
  boxy = sprites[loopz].ypos;
  sprintf(textstring, "Why is the domonian army");
  sprintf(textstring2, "mobolizing? People say the");
  sprintf(textstring3, "Emperor seeks the lost power");
  sprintf(textstring4, " of the Komit once again!");
  sprintf(textstring5, "Could this be true?");
  facer();
  txthandler();
 } 
 if (sprites[loopz].actrig == 6)
 {
  cleartxt();
  textchar = sprites[loopz].bmpid;
  boxx = sprites[loopz].xpos;
  boxy = sprites[loopz].ypos;
  sprintf(textstring,  "1,000 years ago our");
  sprintf(textstring2,  "army entered a war");
  sprintf(textstring3,  "with Damonia. There");
  sprintf(textstring4, "was a stalemate and");
  sprintf(textstring5, "soon the people of");
  sprintf(textstring6,  "the world became");
  sprintf(textstring7,  "angry, so they prayed");
  sprintf(textstring8,  "to the master for");
  sprintf(textstring9,  "salvation. And he gave");
  sprintf(textstring10, " them 'Techno'.");
  pages = 2;
  facer();
  txthandler();
 } 
}


void mrendersprite(void)
{
  if (mainchar.xpos == xc && mainchar.ypos == yc)
  {
   if (mainchar.action == FRONT_STAND_1)
   {
    stretch_sprite(buffer, serff1, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == FRONT_MOVE_1)
   {
    stretch_sprite(buffer, serf2, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == FRONT_MOVE_2)
   {
    stretch_sprite(buffer, serf3, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == FRONT_MOVE_3)
   {
    stretch_sprite(buffer, serf4, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == BACK_STAND_1)
   {
    stretch_sprite(buffer, serb1, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == BACK_MOVE_1)
   {
    stretch_sprite(buffer, serb2, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == BACK_MOVE_2)
   {
    stretch_sprite(buffer, serb3, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == BACK_MOVE_3)
   {
    stretch_sprite(buffer, serb4, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == LEFT_STAND_1)
   {
    stretch_sprite(buffer, serl1, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == LEFT_MOVE_1)
   {
    stretch_sprite(buffer, serl2, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == LEFT_MOVE_2)
   {
    stretch_sprite(buffer, serl3, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == LEFT_MOVE_3)
   {
    stretch_sprite(buffer, serl4, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == RIGHT_STAND_1)
   {
    stretch_sprite(buffer, serr1, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == RIGHT_MOVE_1)
   {
    stretch_sprite(buffer, serr2, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == RIGHT_MOVE_2)
   {
    stretch_sprite(buffer, serr3, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   }
   if (mainchar.action == RIGHT_MOVE_3)
   {
    stretch_sprite(buffer, serr4, mainchar.xpos*20, mainchar.ypos*10, 60, 60);
   } 
  }
}


void mactionhandler(void)
{
 int ycor;
 int move;
 move = 0;
 if (mainchar.action == BACK_MOVE_3)
 {
  mainchar.action = BACK_STAND_1;
  mainchar.ypos--;
  scrolly--;
  for (ycor=0;ycor<10;ycor++)
  {
   if (sprites[ycor].xpos == mainchar.xpos && ycor != spritel)
   {
    if (sprites[ycor].ypos == mainchar.ypos && ycor != spritel)
    {
     mainchar.ypos++;
     scrolly++;
    }
   }
  }
  if (collision[mainchar.xpos][mainchar.ypos] == 1)
  {
   mainchar.ypos++;
   scrolly++;
  }
 }
 if (mainchar.action == BACK_MOVE_2)
 {
  mainchar.action = BACK_MOVE_3;
  mainchar.ypos--;
  scrolly--;
  for (ycor=0;ycor<10;ycor++)
  {
   if (sprites[ycor].xpos == mainchar.xpos && ycor != spritel)
   {
    if (sprites[ycor].ypos == mainchar.ypos && ycor != spritel)
    {
     mainchar.ypos++;
     scrolly++;
    }
   }
  }
  if (collision[mainchar.xpos][mainchar.ypos] == 1)
  {
   mainchar.ypos++;
   scrolly++;
  }

 }
 if (mainchar.action == BACK_MOVE_1)
 {
  mainchar.action = BACK_MOVE_2;
  mainchar.ypos--;
  scrolly--;
  for (ycor=0;ycor<10;ycor++)
  {
   if (sprites[ycor].xpos == mainchar.xpos && ycor != spritel)
   {
    if (sprites[ycor].ypos == mainchar.ypos && ycor != spritel)
    {
     mainchar.ypos++;
     scrolly++;
    }
   }
  }
  if (collision[mainchar.xpos][mainchar.ypos] == 1)
  {
   mainchar.ypos++;
   scrolly++;
  }
 }
 if (mainchar.action == FRONT_MOVE_3)
 {
  mainchar.action = FRONT_STAND_1;
  mainchar.ypos++;
  scrolly++;
  for (ycor=0;ycor<10;ycor++)
  {
   if (sprites[ycor].xpos == mainchar.xpos && ycor != spritel)
   {
    if (sprites[ycor].ypos == mainchar.ypos && ycor != spritel)
    {
     mainchar.ypos--;
     scrolly--;
    }
   }
  }
  if (collision[mainchar.xpos][mainchar.ypos] == 1)
  {
   mainchar.ypos--;
   scrolly--;
  }
 }
 if (mainchar.action == FRONT_MOVE_2)
 {
  mainchar.action = FRONT_MOVE_3;
  mainchar.ypos++;
  scrolly++;
  for (ycor=0;ycor<10;ycor++)
  {
   if (sprites[ycor].xpos == mainchar.xpos && ycor != spritel)
   {
    if (sprites[ycor].ypos == mainchar.ypos && ycor != spritel)
    {
     mainchar.ypos--;
     scrolly--;
    }
   }
  }
  if (collision[mainchar.xpos][mainchar.ypos] == 1)
  {
   mainchar.ypos--;
   scrolly--;
  }

 }
 if (mainchar.action == FRONT_MOVE_1)
 {
  mainchar.action = FRONT_MOVE_2;
  mainchar.ypos++;
  scrolly++;
  for (ycor=0;ycor<10;ycor++)
  {
   if (sprites[ycor].xpos == mainchar.xpos && ycor != spritel)
   {
    if (sprites[ycor].ypos == mainchar.ypos && ycor != spritel)
    {
     mainchar.ypos--;
     scrolly--;
    }
   }
  }
  if (collision[mainchar.xpos][mainchar.ypos] == 1)
  {
   mainchar.ypos--;
   scrolly--;
  }
 }
 if (mainchar.action == LEFT_MOVE_3)
 {
  mainchar.action = LEFT_STAND_1;
  mainchar.xpos--;
  scrollx--;
  for (ycor=0;ycor<10;ycor++)
  {
   if (sprites[ycor].xpos == mainchar.xpos && ycor != spritel)
   {
    if (sprites[ycor].ypos == mainchar.ypos && ycor != spritel)
    {
    mainchar.xpos++;
     scrollx++;
    }
   }
  }
  if (collision[mainchar.xpos][mainchar.ypos] == 1)
  {
   mainchar.xpos++;
   scrollx++;
  }
 }
 if (mainchar.action == LEFT_MOVE_2)
 {
  mainchar.action = LEFT_MOVE_3;
  mainchar.xpos--;
  scrollx--;
  for (ycor=0;ycor<10;ycor++)
  {
   if (sprites[ycor].xpos == mainchar.xpos && ycor != spritel)
   {
    if (sprites[ycor].ypos == mainchar.ypos && ycor != spritel)
    {
    mainchar.xpos++;
     scrollx++;
    }
   }
  }
  if (collision[mainchar.xpos][mainchar.ypos] == 1)
  {
   mainchar.xpos++;
   scrollx++;
  }

 }
 if (mainchar.action == LEFT_MOVE_1)
 {
  mainchar.action = LEFT_MOVE_2;
  mainchar.xpos--;
  scrollx--;
  for (ycor=0;ycor<10;ycor++)
  {
   if (sprites[ycor].xpos == mainchar.xpos && ycor != spritel)
   {
    if (sprites[ycor].ypos == mainchar.ypos && ycor != spritel)
    {
     mainchar.xpos++;
     scrollx++;
    }
   }
  }
  if (collision[mainchar.xpos][mainchar.ypos] == 1)
  {
   mainchar.xpos++;
   scrollx++;
  }
 }
 if (mainchar.action == RIGHT_MOVE_3)
 {
  mainchar.action = RIGHT_STAND_1;
  mainchar.xpos++;
  scrollx++;
  for (ycor=0;ycor<10;ycor++)
  {
   if (sprites[ycor].xpos == mainchar.xpos && ycor != spritel)
   {
    if (sprites[ycor].ypos == mainchar.ypos && ycor != spritel)
    {
     mainchar.xpos--;      
     scrollx--;
    }
   }
  }
  if (collision[mainchar.xpos][mainchar.ypos] == 1)
  {
   mainchar.xpos--;
   scrollx--;
  }
 }
 if (mainchar.action == RIGHT_MOVE_2)
 {
  mainchar.action = RIGHT_MOVE_3;
  mainchar.xpos++;
  scrollx++;
  for (ycor=0;ycor<10;ycor++)
  {
   if (sprites[ycor].xpos == mainchar.xpos && ycor != spritel)
   {
    if (sprites[ycor].ypos == mainchar.ypos && ycor != spritel)
    {
     mainchar.xpos--;      
     scrollx--;
    }
   }
  }
  if (collision[mainchar.xpos][mainchar.ypos] == 1)
  {
   mainchar.xpos--;
   scrollx--;
  }

 }
 if (mainchar.action == RIGHT_MOVE_1)
 {
  mainchar.action = RIGHT_MOVE_2;
  mainchar.xpos++;
  scrollx++;
  for (ycor=0;ycor<10;ycor++)
  {
   if (sprites[ycor].xpos == mainchar.xpos && ycor != spritel)
   {
    if (sprites[ycor].ypos == mainchar.ypos && ycor != spritel)
    {
     mainchar.xpos--;      
     scrollx--;
    }
   }
  }
  if (collision[mainchar.xpos][mainchar.ypos] == 1)
  {
   mainchar.xpos--;
   scrollx--;
  }
 }
}

void bmpscanner(void)
{
 int ytest;
 for (ytest=0;ytest<10;ytest++)
 {
  sprintf(tmp, "Sprite BMP Id number %d is %d", ytest, sprites[ytest].bmpid);
  textout(screen, font, tmp, 7, 7, 1850);
  sprintf(tmp, "Sprite xpos is %d and ypos is %d", sprites[ytest].xpos, sprites[ytest].ypos);
  textout(screen, font, tmp, 7, 15, 1850);
  readkey();
  clear_keybuf();
 }
}

void debugone(void)
{
 if (actionchk == 1)
 {
  if (mainchar.action == 0)
  { textout(screen, font, "Action is standing from front", 7, 472, 1850); }
  if (mainchar.action == 1)
  { textout(screen, font, "Action is standing from back", 7, 472, 1850); }
  if (mainchar.action == 2)
  { textout(screen, font, "Action is standing from left", 7, 472, 1850); }
  if (mainchar.action == 3)
  { textout(screen, font, "Action is standing from right", 7, 472, 1850); }
  if (mainchar.action == 4)
  { textout(screen, font, "Action is moving frame 1 from back", 7, 472, 1850); }
  if (mainchar.action == 5)
  { textout(screen, font, "Action is moving frame 2 from back", 7, 472, 1850); }
  if (mainchar.action == 6)
  { textout(screen, font, "Action is moving frame 3 from back", 7, 472, 1850); }
  if (mainchar.action == 7)
  { textout(screen, font, "Action is moving frame 1 from front", 7, 472, 1850); }
  if (mainchar.action == 8)
  { textout(screen, font, "Action is moving frame 2 from front", 7, 472, 1850); }
  if (mainchar.action == 9)
  { textout(screen, font, "Action is moving frame 3 from front", 7, 472, 1850); }
  if (mainchar.action == 10)
  { textout(screen, font, "Action is moving frame 1 from left", 7, 472, 1850); }
  if (mainchar.action == 11)
  { textout(screen, font, "Action is moving frame 2 from left", 7, 472, 1850); }
  if (mainchar.action == 12)
  { textout(screen, font, "Action is moving frame 3 from left", 7, 472, 1850); }
  if (mainchar.action == 13)
  { textout(screen, font, "Action is moving frame 1 from right", 7, 472, 1850); }
  if (mainchar.action == 14)
  { textout(screen, font, "Action is moving frame 2 from right", 7, 472, 1850); }
  if (mainchar.action == 15)
  { textout(screen, font, "Action is moving frame 3 from right", 7, 472, 1850); }
 }
 if (slow == 1)
 {
  readkey();
 }
}

void cleartxt(void)
{
 int ycor;
 sprintf(textstring, "                                                                  ");
 sprintf(textstring2, "                                                                  ");
 sprintf(textstring3, "                                                                  ");
 sprintf(textstring4, "                                                                  ");
 sprintf(textstring5, "                                                                  ");
 sprintf(textstring6, "                                                                  ");
 sprintf(textstring7, "                                                                  ");
}

void npc_ai(void)
{
 int spritenum;
 int randnum;
 for (spritenum=0;spritenum<10;spritenum++)
 {
  if (sprites[spritenum].immobile == 0)
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

void facer(void)
{
 int xcod;
 if (sprites[loopz].xpos < mainchar.xpos)
 {
  sprites[loopz].action = RIGHT_STAND_1;
  mainchar.action = LEFT_STAND_1;
 }
 if (sprites[loopz].xpos > mainchar.xpos)
 {
  sprites[loopz].action = LEFT_STAND_1;
  mainchar.action = RIGHT_STAND_1;
 }
 if (sprites[loopz].ypos > mainchar.ypos)
 {
  sprites[loopz].action = BACK_STAND_1;
  mainchar.action = FRONT_STAND_1;
 }
 if (sprites[loopz].ypos < mainchar.ypos)
 {
  sprites[loopz].action = FRONT_STAND_1;
  mainchar.action = BACK_STAND_1;
 }
}
  
void scantags(void)
{
 int xcor;
 int ycor;
 for (xcor=0;xcor<100;xcor++)
 {
  for (ycor=0;ycor<100;ycor++)
  {
   if (tagmap[xcor][ycor] == tagjmpr)
   {
    mainchar.xpos = xcor;
    mainchar.ypos = ycor;
   }
  }
 }
}

void center(void)
{
 scrollx = mainchar.xpos-15;
 scrolly = mainchar.ypos-22;
}

void battlechk(void)
{
 int randnum;
 mnstercntr += mainchar.xpos / 10;
 if (mnstercntr > 100)
 {
  cleartxt();
  textchar = 0;
  boxx = mainchar.xpos;
  boxy = mainchar.ypos;
  sprintf(textstring,  "Encountered an Enemy!!");
  //txthandler();
  mnstercntr = 0;
  fadeout();
  battleloop();
 }
}

void battleloop(void)
{
 load_batl();
 data1 = 0;
 init_battle();
 while(data1 == 0)
 {
 blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 renderbattle();
 exec_action();
 checkforzero();
 checkbatend();
 inputbattle();
 }
 if (win == 0)
 {
  lost_battle();
 }
 if (win == 1)
 {
  won_battle();
 }
 sprintf(tmp, "%d", color);
 textout_centre(screen, font, tmp, 0, 0, color);
 clear(screen);
}

void inputbattle(void)
{
 int xcor;
 int ycor;
 if (key[KEY_DOWN])
 {
  party[1].ypos++;
  batsy++;
  batsx++;
 }
 if (key[KEY_UP])
 {
  party[1].ypos--;
  batsy--;
  batsx--;
 }
 if (key[KEY_LEFT])
 {
  party[1].xpos--;
 }
 if (key[KEY_RIGHT])
 {
  party[1].xpos++;
 }
 if (key[KEY_1])
 {
  strcpy(filename, argv[0]);
  strcpy(get_filename(filename), "battle.bmp");
  save_bitmap(filename, buffer, the_pallete);
  textout(screen, font, "Took a screenshot", 7, 7, 1850);
 }
 if (key[KEY_ESC])
 {
  data1 = 4;
 }
}

void renderbattle(void)
{
 sprite_batl();
 renderboxes();
 ct_handler();
 blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 clear(buffer);
}

void load_batl(void)
{
 if (mnster == 1)
 {
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "cstl1bck.bmp");
 batlback = load_bmp(filename, the_pallete);
 party[1].id = 1; //1 is seren//
 party[1].xpos = 25;
 party[1].ypos = 16;
 party[1].hp = 99999;
 party[1].maxhp = 99999;
 party[1].mp = 99999;
 party[1].maxmp = 99999;
 party[1].ep = 5401;
 party[1].maxep = 5401;
 party[1].level = 20;
 party[1].speed = 10;
 enemy[1].xpos = 5;
 enemy[1].ypos = 10;
 enemy[1].id = 1; //lancer
 enemy[1].action = RIGHT_STAND_1;
 party[1].action = LEFT_STAND_1;
 enemy[1].maxhp = 999;
 enemy[1].hp = 999;
 enemy[1].speed = 12;
 enemy[1].ct = 10;
 //grid about 30x43
 }
}

void center_batl(void)
{
}

void sprite_batl(void)
{
 int xcor;
 int ycor;
 for (ycor=0;ycor<20;ycor++)
 {
  if (party[ycor].id == 1) //you/seren
  {
   if (party[ycor].action == FRONT_STAND_1)
   { stretch_sprite(buffer, serff1, party[ycor].xpos*20, party[ycor].ypos*10, 60, 60); }
   if (party[ycor].action == BACK_STAND_1)
   { stretch_sprite(buffer, serb1, party[ycor].xpos*20, party[ycor].ypos*10, 60, 60); }
   if (party[ycor].action == LEFT_STAND_1)
   { stretch_sprite(buffer, serl1, party[ycor].xpos*20, party[ycor].ypos*10, 60, 60); }
   if (party[ycor].action == RIGHT_STAND_1)
   { stretch_sprite(buffer, serr1, party[ycor].xpos*20, party[ycor].ypos*10, 60, 60); }
  }
  if (party[ycor].id == 2) //ninja guard
  {
   if (party[ycor].action == FRONT_STAND_1)
   { stretch_sprite(buffer, ninjaf1, party[ycor].xpos*20, party[ycor].ypos*10, 60, 60); }
   if (party[ycor].action == BACK_STAND_1)
   { stretch_sprite(buffer, ninjab1, party[ycor].xpos*20, party[ycor].ypos*10, 60, 60); }
   if (party[ycor].action == LEFT_STAND_1)
   { stretch_sprite(buffer, ninjal1, party[ycor].xpos*20, party[ycor].ypos*10, 60, 60); }
   if (party[ycor].action == RIGHT_STAND_1)
   { stretch_sprite(buffer, ninjar1, party[ycor].xpos*20, party[ycor].ypos*10, 60, 60); }
  }
  if (enemy[ycor].id == 1) //you/seren
  {
   if (enemy[ycor].action == FRONT_STAND_1)
   { stretch_sprite(buffer, mech, enemy[ycor].xpos*20, enemy[ycor].ypos*10, 120, 120); }
   if (enemy[ycor].action == BACK_STAND_1)
   { stretch_sprite(buffer, mech, enemy[ycor].xpos*20, enemy[ycor].ypos*10, 120, 120); }
   if (enemy[ycor].action == LEFT_STAND_1)
   { stretch_sprite(buffer, mech, enemy[ycor].xpos*20, enemy[ycor].ypos*10, 120, 120); }
   if (enemy[ycor].action == RIGHT_STAND_1)
   { stretch_sprite(buffer, mech, enemy[ycor].xpos*20, enemy[ycor].ypos*10, 120, 120); }
  }
  if (enemy[ycor].id == 2) //ninja guard
  {
   if (enemy[ycor].action == FRONT_STAND_1)
   { stretch_sprite(buffer, ninjaf1, enemy[ycor].xpos*20, enemy[ycor].ypos*10, 60, 60); }
   if (enemy[ycor].action == BACK_STAND_1)
   { stretch_sprite(buffer, ninjab1, enemy[ycor].xpos*20, enemy[ycor].ypos*10, 60, 60); }
   if (enemy[ycor].action == LEFT_STAND_1)
   { stretch_sprite(buffer, ninjal1, enemy[ycor].xpos*20, enemy[ycor].ypos*10, 60, 60); }
   if (enemy[ycor].action == RIGHT_STAND_1)
   { stretch_sprite(buffer, ninjar1, enemy[ycor].xpos*20, enemy[ycor].ypos*10, 60, 60); }
  }
 }
}

void ct_handler(void)
{
 int xcor;
 int ycor;
 int time;
 for (loopz=0;loopz<20;loopz++)
 {
  if (party[loopz].id != 0)
  {
   if (party[loopz].ct < 114)
   {
    if (party[loopz].speed > 1)
    { party[loopz].ct += party[loopz].speed/7; }
    if (party[loopz].speed < 2)
    {
     party[loopz].ct++;
    }
    set_trans_blender(0, 0, 0, 200);
    drawing_mode(DRAW_MODE_TRANS, NULL, NULL, NULL);
    rectfill(buffer, 463, 401, 463+party[loopz].ct, 404, 1900);
    rectfill(buffer, 463, 408, 463+party[loopz].limit, 411, 320);
   }
   if (party[loopz].ct > 113)
   {
    textout(screen, font, "Run attack", 7, 7, 1850);
    run_attack();
   }
   if (party[loopz].limit > 112)
   {
    run_attack();
    party[loopz].limit = 0;
   }
  }
  if (enemy[loopz].id != 0)
  {
  if (enemy[loopz].ct < 114)
  {
   if (enemy[loopz].speed > 1)
   { enemy[loopz].ct += enemy[loopz].speed/7; }
   if (enemy[loopz].speed < 2)
   {
    enemy[loopz].ct++;
   }
  }
  if (enemy[loopz].ct > 113)
  {
   enemy_ai();
  }
 }
 }
}



void run_attack(void)
{
 int xcor;
 int ycor;
 int wait;
 int cycle;
 cycle = 1;
 ycor = 1;
 wait = 0;
 clear_keybuf();
 while(wait == 0)
 {
  if (cycle = 2)
  {
   cycle--;
  }
  else
  {
   cycle++;
  }
  if (key[KEY_UP])
  {
   if (ycor > 1 && xcor == 0)
   {
    ycor--;
    xcor = 1;
   }
   if (ycor == 1 && xcor == 0)
   {
    ycor = 6;
    xcor = 1;
   }
   clear_keybuf();
   xcor = 0;
   if (ycor == 5 && party[loopz].will == 0)
   { ycor = 4; }
   if (ycor == 4 && party[loopz].psychic == 0)
   { ycor = 3; }
   if (ycor == 3 && party[loopz].magic == 0)
   { ycor = 2; }
   if (ycor == 2 && party[loopz].special == 0)
   { ycor = 1; }
  }
  if (key[KEY_DOWN])
  {
   if (ycor == 6 && xcor == 0)
   {
    ycor = 1;
    xcor = 1;
   }
   if (ycor < 6 && xcor == 0)
   {
    ycor++;
    xcor = 1;
   }
   clear_keybuf();
   xcor = 0;
   if (ycor == 2 && party[loopz].special == 0)
   { ycor = 3; }
   if (ycor == 3 && party[loopz].magic == 0)
   { ycor = 4; }
   if (ycor == 4 && party[loopz].psychic == 0)
   { ycor = 5; }
   if (ycor == 5 && party[loopz].will == 0)
   { ycor = 6; }
  }
  if (key[KEY_ENTER])
  {
   wait = 10434;
   select = ycor;
  }
  blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  sprite_batl();
  renderboxes();
  stretch_sprite(buffer, select2, party[loopz].xpos*20, party[loopz].ypos*10, 60, 60);
  stretch_sprite(buffer, textbox, 200, 378, 100, 102);
  if (party[loopz].limit < 113)
  {
   textout(buffer, font, "Fight", 214, 378+16, 1850);
  }
  if (party[loopz].limit > 112)
  {
   textout(buffer, font, "Limit", 214, 378+16, 1850);
  }
  if (party[loopz].special != 0)
  { textout(buffer, font, "Special", 214, 378+28, 1850); }
  if (party[loopz].magic != 0)
  { textout(buffer, font, "Magic", 214, 378+40, 1850); }
  if (party[loopz].psychic != 0)
  { textout(buffer, font, "Psychic", 214, 378+52, 1850); }
  if (party[loopz].will != 0)
  { textout(buffer, font, "Will", 214, 378+64, 1850); }
  textout(buffer, font, "Item", 214, 378+76, 1850);
  stretch_sprite(buffer, cursor, 190, 378+((ycor*12)-2), 20, 20);
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  party[loopz].ct = 0;
 }
 atk_sub();
 wait = 133;
}

void atk_sub(void)
{
 int wait;
 int ytest;
 //for (ytest=5;ytest>0;ytest--)
 //{
 // blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 // sprite_batl();
 // stretch_sprite(buffer, textbox, 160+(32*ytest), 0+(5*ytest), 320-(64*ytest), 50-(10*ytest));
 // blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 //}
 clear_keybuf();
 monnum = 1;
 while(wait == 0)
 {
  blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  sprite_batl();
  renderboxes();
  if (select == 1)
  {
   if (key[KEY_UP])
   {
    if (monnum < 9)
    { monnum++; }
    else
    { monnum = 1; }
    if (monnum == 1 && enemy[monnum].id == 0)
    { monnum = 2; }
    if (monnum == 2 && enemy[monnum].id == 0)
    { monnum = 3; }
    if (monnum == 3 && enemy[monnum].id == 0)
    { monnum = 4; }
    if (monnum == 4 && enemy[monnum].id == 0)
    { monnum = 5; }
    if (monnum == 5 && enemy[monnum].id == 0)
    { monnum = 6; }
    if (monnum == 6 && enemy[monnum].id == 0)
    { monnum = 7; }
    if (monnum == 7 && enemy[monnum].id == 0)
    { monnum = 8; }
    if (monnum == 8 && enemy[monnum].id == 0)
    { monnum = 9; }
    if (monnum == 9 && enemy[monnum].id == 0)
    { monnum = 1; }
    clear_keybuf();
   }
   if (key[KEY_LEFT])
   {
    if (monnum < 9)
    { monnum++; }
    else
    { monnum = 1; }
    if (monnum == 1 && enemy[monnum].id == 0)
    { monnum = 2; }
    if (monnum == 2 && enemy[monnum].id == 0)
    { monnum = 3; }
    if (monnum == 3 && enemy[monnum].id == 0)
    { monnum = 4; }
    if (monnum == 4 && enemy[monnum].id == 0)
    { monnum = 5; }
    if (monnum == 5 && enemy[monnum].id == 0)
    { monnum = 6; }
    if (monnum == 6 && enemy[monnum].id == 0)
    { monnum = 7; }
    if (monnum == 7 && enemy[monnum].id == 0)
    { monnum = 8; }
    if (monnum == 8 && enemy[monnum].id == 0)
    { monnum = 9; }
    if (monnum == 9 && enemy[monnum].id == 0)
    { monnum = 1; }
    clear_keybuf();
   }
   if (key[KEY_DOWN])
   {
    if (monnum > 1)
    { monnum--; }
    else
    { monnum = 9; }
    if (enemy[monnum].id == 0)
    { monnum -= 1; }
    if (monnum == 9 && enemy[monnum].id == 0)
    { monnum = 8; }
    if (monnum == 8 && enemy[monnum].id == 0)
    { monnum = 7; }
    if (monnum == 7 && enemy[monnum].id == 0)
    { monnum = 6; }
    if (monnum == 6 && enemy[monnum].id == 0)
    { monnum = 5; }
    if (monnum == 5 && enemy[monnum].id == 0)
    { monnum = 4; }
    if (monnum == 4 && enemy[monnum].id == 0)
    { monnum = 3; }
    if (monnum == 3 && enemy[monnum].id == 0)
    { monnum = 2; }
    if (monnum == 2 && enemy[monnum].id == 0)
    { monnum = 1; }
    if (monnum == 1 && enemy[monnum].id == 0)
    { monnum = 9; }
    clear_keybuf();
   }
   if (key[KEY_RIGHT])
   {
    if (monnum > 1)
    { monnum--; }
    else
    { monnum = 9; }
    if (enemy[monnum].id == 0)
    { monnum -= 1; }
    if (monnum == 9 && enemy[monnum].id == 0)
    { monnum = 8; }
    if (monnum == 8 && enemy[monnum].id == 0)
    { monnum = 7; }
    if (monnum == 7 && enemy[monnum].id == 0)
    { monnum = 6; }
    if (monnum == 6 && enemy[monnum].id == 0)
    { monnum = 5; }
    if (monnum == 5 && enemy[monnum].id == 0)
    { monnum = 4; }
    if (monnum == 4 && enemy[monnum].id == 0)
    { monnum = 3; }
    if (monnum == 3 && enemy[monnum].id == 0)
    { monnum = 2; }
    if (monnum == 2 && enemy[monnum].id == 0)
    { monnum = 1; }
    if (monnum == 1 && enemy[monnum].id == 0)
    { monnum = 9; }
    clear_keybuf();
   }
   if (key[KEY_ENTER])
   {
    party[loopz].intarget = monnum;
    clear_keybuf();
    wait = 133;
    party[loopz].ct = 0;
    party[loopz].inwait = FIGHT;
    if (party[loopz].limit > 112)
    {
     party[loopz].inwait = LIMIT;
    }
   }
   stretch_sprite(buffer, cursor, enemy[monnum].xpos*20, enemy[monnum].ypos*10, 20, 20);
  }
  stretch_sprite(buffer, select2, party[loopz].xpos*20, party[loopz].ypos*10, 60, 60);
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 }
}


void exec_action(void)
{
 int wait;
 int xcor;
 int ycor;
 int basedmg;
 int basedef;
 int lucka;
 int luckb;
 int damage;
 int target;
 for (ycor=0;ycor<20;ycor++)
 {
  if (party[ycor].inwait == FIGHT)
  {
   target = party[ycor].intarget;
   basedmg = party[ycor].wp + party[ycor].attack;
   basedef = enemy[target].ap + enemy[target].defense;
   lucka = (party[ycor].luck*6)/2;
   luckb = (enemy[target].luck*6)/2;
   basedmg = basedmg * lucka / 3;
   basedef = basedef * luckb / 3;
   damage = ((basedmg - basedef)*3)/2;
   damage = 50;
   enemy[target].hp -= damage;
   sprintf(tmp, "%d", damage);
   attacker = ycor;
   defender = target;
   slotter = 0;
   enemy_showatk();
   for (xcor=0;xcor<5;xcor++)
   {
    blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
    sprite_batl();
    renderboxes();
    textout_centre(buffer, font, tmp, (enemy[target].xpos*20)+35, (enemy[target].ypos*10)+xcor, 63488);
    blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
    clear(buffer);
   }
   party[ycor].inwait = 0;
   basedmg = 0;
   basedef = 0;
   lucka = 0;
   luckb = 0;
   damage = 0;
  }
  if (party[ycor].inwait == LIMIT)
  {
   target = party[ycor].intarget;
   basedmg = party[ycor].wp + party[ycor].attack;
   basedef = enemy[target].ap + enemy[target].defense;
   lucka = (party[ycor].luck*6)/2;
   luckb = (enemy[target].luck*6)/2;
   basedmg = basedmg * lucka / 3;
   basedef = basedef * luckb / 3;
   damage = ((basedmg - basedef)*3)/2;
   damage = 50;
   enemy[target].hp -= damage;
   sprintf(tmp, "%d", damage);
   attacker = ycor;
   defender = target;
   slotter = 0;
   enemy_showatk();
   for (xcor=0;xcor<5;xcor++)
   {
    blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
    sprite_batl();
    renderboxes();
    textout_centre(buffer, font, tmp, (enemy[target].xpos*20)+35, (enemy[target].ypos*10)+xcor, 63488);
    blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
    clear(buffer);
   }
   party[ycor].inwait = 0;
   basedmg = 0;
   basedef = 0;
   lucka = 0;
   luckb = 0;
   damage = 0;
  }
  if (enemy[ycor].inwait == FIGHT)
  {
   target = enemy[ycor].intarget;
   target = 1;
   basedmg = enemy[ycor].wp + enemy[ycor].attack;
   basedef = party[target].ap + party[target].defense;
   lucka = (enemy[ycor].luck*6)/2;
   luckb = (party[target].luck*6)/2;
   basedmg = basedmg * lucka / 3;
   basedef = basedef * luckb / 3;
   damage = ((basedmg - basedef)*3)/2;
   damage = 100;
   party[target].hp -= damage;
   sprintf(tmp, "%d", damage);
   attacker = ycor;
   defender = target;
   slotter = 1;
   enemy_showatk();
   for (xcor=0;xcor<5;xcor++)
   {
    blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
    sprite_batl();
    renderboxes();
    textout_centre(buffer, font, tmp, (party[target].xpos*21)+5, (party[target].ypos*11)+xcor, 63488);
    blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
    clear(buffer);
   }
   party[target].limit += 20;
   if (party[target].limit > 113)
   {
    party[target].limit = 113;
   }
   enemy[ycor].inwait = 0;
   enemy[ycor].ct = 0;
   basedmg = 0;
   basedef = 0;
   lucka = 0;
   luckb = 0;
   damage = 0;
   for (ycor=0;ycor<12500000;ycor++)
   { }
  }
 }
}


void renderboxes(void)
{
 int hpperc;
 int mpperc;
 int xcor;
 char tpp[80];
 char txtmp[80];
 stretch_sprite(buffer, textbox, 0, 380, 240, 100);
 stretch_sprite(buffer, textbox, 240, 380, 418, 100);
 textout(buffer, font, "HP", 309, 387, 1850);
 textout(buffer, font, "MP", 359, 387, 1850);
 textout(buffer, font, "EP", 407, 387, 1850);
 textout(buffer, font, "METER", 497, 387, 1850);
 //stretch_sprite(buffer, textbox, 460, 397, 120, 15);
 stretch_sprite(buffer, meter1, 460, 398, 120, 15);
 for (xcor=0;xcor<20;xcor++)
 {
  if (party[xcor].id == 1)
  {
   textout(buffer, font, "SEREN", 248, 402, 1850);
   sprintf(txtmp, "%d", party[xcor].hp);
   if (party[xcor].hp > 9999)
   {
    sprintf(txtmp, "????");
   }
   textout(buffer, font, txtmp, 300, 402, 1850);
   sprintf(txtmp, "%d", party[xcor].mp);
   if (party[xcor].mp > 9999)
   {
    sprintf(txtmp, "????");
   }
   textout(buffer, font, txtmp, 350, 402, 1850);
   sprintf(txtmp, "%d", party[xcor].ep);
   if (party[xcor].ep > 9999)
   {
    sprintf(txtmp, "????");
   }
   textout(buffer, font, txtmp, 400, 402, 1850);
  }
 }
 textout(buffer, font, "HP/MP", 127, 387, 1850);
 for (xcor=0;xcor<20;xcor++)
 {
  if (enemy[xcor].id == 1)
  {
   mpperc = 0;
   hpperc = 0;
   textout(buffer, font, "NovaMech", 8, 402, 1850);
   stretch_sprite(buffer, meter1, 86, 398, 120, 15);
   set_trans_blender(0, 0, 0, 200);
   drawing_mode(DRAW_MODE_TRANS, NULL, NULL, NULL);
   if (enemy[xcor].hp != 0 && enemy[xcor].maxhp != 0)
   { hpperc = enemy[xcor].hp / enemy[xcor].maxhp * 100; }
   if (enemy[xcor].mp != 0 && enemy[xcor].maxmp != 0)
   { mpperc = enemy[xcor].mp / enemy[xcor].maxmp * 100; }
   sprintf(tpp, "HPPERC is %d", enemy[xcor].hp / enemy[xcor].maxhp);
   textout(buffer, font, tpp, 0, 0, 1850);
   rectfill(buffer, 90, 401, 89+hpperc+(hpperc/10), 404, 1908);
   rectfill(buffer, 90, 408, 89+mpperc+(mpperc/10), 411, 318);  
  }
 }
}
void checkforzero(void)
{
 int ycor;
 int killed;
 int xcor;
 int none;
 for (ycor=0;ycor<20;ycor++)
 {
  if (enemy[ycor].hp < 1)
  {
   killed = ycor;
   enemy[ycor].id = 0;
   for (xcor=0;xcor<10;xcor++)
   {
    //fade out picture//
   }
   monnum = 0;
  }
 }
}

void checkbatend(void)
{
 int ycor;
 int noner = 0; //no enemies left
 int nonen = 0; //no allies left
 for (ycor=0;ycor<20;ycor++)
 {
  if (enemy[ycor].id != 0)
  {
   noner++;
  }
  if (party[ycor].id != 0)
  {
   nonen++;
  }
 }
 if (noner == 0)
 {
  data1 = 5555;
  win = 1;
 }
 if (nonen == 0)
 {
  data1 = 5555;
  win = 0;
 }
}

void init_battle(void)
{
 int ycor;
 int xcor;
 for (ycor=20;ycor>0;ycor--)
 {
  blit(batlback, buffer, 0, ycor*12, 0, 0, 640, 480-(ycor*24));  // copy it to the screen
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  clear(buffer);
 }
 ///finished that routine
 //and of course the pause
 for (ycor=5;ycor>0;ycor--)
 {
  blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 //stretch_sprite(buffer, textbox, 0, 380, 240, 100);
 //stretch_sprite(buffer, textbox, 240, 380, 418, 100);
  stretch_sprite(buffer, textbox, (24*ycor), 380 + (10*ycor), 240-48*ycor, 100-20*ycor);
  stretch_sprite(buffer, textbox, 240+(64*ycor), 380 + (8*ycor), 418-83*ycor, 100-20*ycor);
  //stretch_sprite(buffer, textbox, (64*ycor), 400 + (8*ycor), 640-128*ycor, 80-16*ycor);
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  clear(buffer);
 }
 blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 stretch_sprite(buffer, textbox, 0, 380, 240, 100);
 stretch_sprite(buffer, textbox, 240, 380, 418, 100);
 blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 clear(buffer);
 //finished that routine

 for (ycor=0;ycor<10000000;ycor++)
 { }
 //gotta pause to catch our breath!!//

 blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 stretch_sprite(buffer, textbox, 0, 380, 240, 100);
 stretch_sprite(buffer, textbox, 240, 380, 418, 100);
 stretch_sprite(buffer, textbox, 240, 0, 160, 50);
 textout_centre(buffer, font, "Front Attack", 320, 21, 1850);
 blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 clear(buffer);

 //finished attack type flasher//
 for (ycor=0;ycor<10000000;ycor++)
 { }
 //gotta pause to catch our breath!!//
 for (ycor=0;ycor<15;ycor++)
 {
  blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  stretch_sprite(buffer, textbox, 0, 380, 240, 100);
  stretch_sprite(buffer, textbox, 240, 380, 418, 100);
  for (xcor=0;xcor<20;xcor++)
  {
   if (party[xcor].id == 1 && party[xcor].action == FRONT_STAND_1)
   {
    set_trans_blender(0, 0, 0, ycor*17);
    stretch_blit(serff1, sprite1, 0, 0, 80, 80, 0, 0, 60, 60);
    draw_trans_sprite(buffer, sprite1, party[xcor].xpos*20, party[xcor].ypos*10); 
   }
   if (enemy[xcor].id == 1 && enemy[xcor].action == BACK_STAND_1)
   {
    set_trans_blender(0, 0, 0, ycor*17);
    stretch_blit(serb1, sprite1, 0, 0, 80, 80, 0, 0, 60, 60);
    draw_trans_sprite(buffer, sprite1, enemy[xcor].xpos*20, enemy[xcor].ypos*10); 
   }
   if (enemy[xcor].id == 2 && enemy[xcor].action == BACK_STAND_1)
   {
    set_trans_blender(0, 0, 0, ycor*17);
    stretch_blit(ninjab1, sprite1, 0, 0, 60, 60, 0, 0, 60, 60);
    draw_trans_sprite(buffer, sprite1, enemy[xcor].xpos*20, enemy[xcor].ypos*10); 
   }
  }
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  clear(buffer);
 }
 //finished doin the transparency fade in//
 for (ycor=0;ycor<10000000;ycor++)
 { }
 blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 sprite_batl();
 renderboxes();
 blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 clear(buffer); 
 for (ycor=0;ycor<10000000;ycor++)
 { }
}

void won_battle(void)
{
 int xcor;
 int ycor;
 //first step is to run shrinker for data box//
 blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 sprite_batl();
 for (ycor=0;ycor<5;ycor++)
 {
  blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  stretch_sprite(buffer, textbox, (64*ycor), 400 + (8*ycor), 640-128*ycor, 80-16*ycor);

  blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  clear(buffer);
 }
 ///guys cheer, etc. etc.//

 blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 stretch_sprite(buffer, textbox, 240, 0, 160, 50);
 textout_centre(buffer, font, "Won the Battle!", 320, 21, 1850);
 blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 clear(buffer);
 for (ycor=0;ycor<100000000;ycor++)
 { }
 //ok, now that we shrank the box, we need to scroll off the battle field//
 for (ycor=0;ycor<20;ycor++)
 {
  blit(batlback, buffer, 0, ycor*12, 0, 0, 640, 480-(ycor*24));  // copy it to the screen
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  clear(buffer);
 }
 //
 //after this we show what the characters found/received//
 stretch_sprite(buffer, textbox, 0, 0, 640, 100);
 stretch_sprite(buffer, textbox, 0, 100, 640, 380);
 blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 clear_keybuf();
 readkey();
}

void lost_battle(void)
{
}

void fadeout(void)
{
 int alpha;
 int ycor;
 for (alpha=2;alpha>0;alpha--)
 {
  set_trans_blender(0, 0, 0, alpha*25);
  renderbottom(); //render floor
  rendermid(); //render tiles below player
  for (yc=0;yc<100;yc++)
  { 
   for (xc=0;xc<100;xc++)
   {
    mrendersprite(); //render player
    rendersprite();
   }
  }
  renderhi(); //render tiles above player
  blit(buffer, bufferx, scrollx*20, scrolly*10, 0, 0, 640, 480);  // copy it to the screen
  draw_trans_sprite(bitmap2, bufferx, 0, 0);
  blit(bitmap2, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  clear(buffer);
  clear(bufferx);
  clear(bitmap2);
 }
 for (ycor=0;ycor<12500000;ycor++)
 { }
 stretch_sprite(screen, textbox, 240, 0, 160, 50);
 textout_centre(screen, font, "Enemy Attack!", 320, 21, 1850);
 for (ycor=0;ycor<12500000;ycor++)
 { }
 for (alpha=1;alpha>0;alpha--)
 {
 set_trans_blender(0, 0, 0, alpha*25);
 renderbottom(); //render floor
 rendermid(); //render tiles below player
 for (yc=0;yc<100;yc++)
 { 
  for (xc=0;xc<100;xc++)
  {
   mrendersprite(); //render player
   rendersprite();
  }
 }
 renderhi(); //render tiles above player
 blit(buffer, bufferx, scrollx*20, scrolly*10, 0, 0, 640, 480);  // copy it to the screen
 draw_trans_sprite(bitmap2, bufferx, 0, 0);
 blit(bitmap2, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 clear(buffer);
 clear(bufferx);
 clear(bitmap2);
 }
}

void enemy_ai(void)
{
 //spritenum is loopz
 int randnum;
 int value1;
 int value2;
 if (enemy[loopz].id != 0 && party[loopz].id != 0)
 {
  value1 = (enemy[loopz].xpos * enemy[loopz].ypos);
  value2 = (party[loopz].ypos * party[loopz].xpos);
  if (value1 != 0 && value2 != 0)
  {
   randnum = value1 / value2;
  }
  //sprintf(tmp, "Enemy randnum is %d", randnum);
  //textout(screen, font, tmp, 0, 0, 1850);
  if (randnum < 10)
  {
   enemy[loopz].inwait = FIGHT;
  }
  //clear_keybuf();
  //readkey();
 }
}

void load_o(void)
{
 int yvalue;
 int yslot;
 int xslot;
 level = fopen("over1.ovw", "r+");
 for (xslot=0;xslot<100;xslot++)
 {
  for (yslot=0;yslot<100;yslot++)
  {
   fread(&collision_o[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (xslot=0;xslot<100;xslot++)
 {
  for (yslot=0;yslot<100;yslot++)
  {
   fread(&tagmap_o[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<20;yslot++)
 {
 fread(&tags_o[yslot].scnenum, sizeof(int), 1, level);
 fread(&tags_o[yslot].pressa, sizeof(int), 1, level);
 fread(&tags_o[yslot].scne[0], sizeof(int), 1, level);
 fread(&tags_o[yslot].scne[1], sizeof(int), 1, level);
 fread(&tags_o[yslot].overworld, sizeof(int), 1, level);
 fread(&tags_o[yslot].actrig, sizeof(int), 1, level);
 fread(&tags_o[yslot].tagjmp, sizeof(int), 1, level);
 }
 fread(&mapt, sizeof(int), 1, level);
 fclose(level);
}

void scantags_o(void)
{
 int xcor;
 int ycor;
 for (xcor=0;xcor<100;xcor++)
 {
  for (ycor=0;ycor<100;ycor++)
  {
   if (tagmap_o[xcor][ycor] == tagjmpr)
   {
    mainchar_o.xpos = xcor;
    mainchar_o.ypos = ycor;
   }
  }
 }
}

void overworld(void)
{
 looptt = 0;
 if (mapt == 1)
 {
  strcpy(savefile, argv[0]);
  strcpy(get_filename(savefile), "over1.bmp");
  map1 = load_bmp(savefile, the_pallete);
 }
 while(looptt == 0)
 {
  over_render();
  over_char();
  over_action();
  over_check();
  textout(buffer, font, tmp, 7, 7, 1850);
  over_input();
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  clear(buffer);
 }
}

void over_render(void)
{
 blit(map1, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
}

void over_char(void)
{
  if (mainchar_o.action == FRONT_STAND_1)
  {
   stretch_sprite(buffer, serff1, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == FRONT_MOVE_1)
  {
   stretch_sprite(buffer, serf2, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == FRONT_MOVE_2)
  {
   stretch_sprite(buffer, serf3, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == FRONT_MOVE_3)
  {
   stretch_sprite(buffer, serf4, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == BACK_STAND_1)
  {
   stretch_sprite(buffer, serb1, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == BACK_MOVE_1)
  {
   stretch_sprite(buffer, serb2, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == BACK_MOVE_2)
  {
   stretch_sprite(buffer, serb3, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == BACK_MOVE_3)
  {
   stretch_sprite(buffer, serb4, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == LEFT_STAND_1)
  {
   stretch_sprite(buffer, serl1, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == LEFT_MOVE_1)
  {
   stretch_sprite(buffer, serl2, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == LEFT_MOVE_2)
  {
   stretch_sprite(buffer, serl3, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == LEFT_MOVE_3)
  {
   stretch_sprite(buffer, serl4, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == RIGHT_STAND_1)
  {
   stretch_sprite(buffer, serr1, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == RIGHT_MOVE_1)
  {
   stretch_sprite(buffer, serr2, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == RIGHT_MOVE_2)
  {
   stretch_sprite(buffer, serr3, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  }
  if (mainchar_o.action == RIGHT_MOVE_3)
  {
   stretch_sprite(buffer, serr4, mainchar_o.xpos*10, mainchar_o.ypos*5, 30, 30);
  } 
}

void over_input(void)
{
 int xcor;
 int ycor;
 if (key[KEY_UP])
 {
  if (mainchar_o.action == BACK_MOVE_3)
  {
   mainchar_o.action = BACK_MOVE_1;
  }
  if (mainchar_o.action < 4)
  {
   mainchar_o.action = BACK_MOVE_1;
  }
 }
 if (key[KEY_DOWN])
 {
  if (mainchar_o.action == FRONT_MOVE_3)
  {
   mainchar_o.action = FRONT_MOVE_1;
  }
  if (mainchar_o.action < 4)
  {
   mainchar_o.action = FRONT_MOVE_1;
  }
 }
 if (key[KEY_LEFT])
 {
  if (mainchar_o.action == LEFT_MOVE_3)
  { mainchar_o.action = LEFT_MOVE_1; }
  if (mainchar_o.action < 4)
  {
     mainchar_o.action = LEFT_MOVE_1;
  }
 }
 if (key[KEY_RIGHT])
 {
  if (mainchar_o.action == RIGHT_MOVE_3)
  { mainchar_o.action = RIGHT_MOVE_1; }
  if (mainchar_o.action < 4)
  {
     mainchar_o.action = RIGHT_MOVE_1;
  }
 }
 if (key[KEY_INSERT])
 {
  over_tagchk();
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
  strcpy(get_filename(filename), "overworld.bmp");
  save_bitmap(filename, buffer, the_pallete);
  textout(screen, font, "Took a screenshot", 7, 7, 1850);
 }
 if (key[KEY_ESC])
 {
  looper = 4;
 }
}

void over_tagchk(void)
{
  int loopx;
  int tagnum;
  int looptag;
  int tagjmp;
  tagnum = tagmap_o[mainchar_o.xpos][mainchar_o.ypos];
  if (tags_o[tagnum].overworld == 1 && tags_o[tagnum].pressa == 0)
  {
   mainchar_o.xpos = tags_o[tagnum].scne[0];
   mainchar_o.ypos = tags_o[tagnum].scne[1];
   strcpy(savefile_o, "over1.ovw");
   tagjmpr = tags_o[tagnum].tagjmp;
   load_o();
   scantags_o();
   tagnum = 0;
  }
  if (tags[tagnum].scnenum == 1)
  {
   mainchar.xpos = tags_o[tagnum].scne[0];
   mainchar.ypos = tags_o[tagnum].scne[1];
   strcpy(savefile, "main1.tpm");
   tagjmpr = tags_o[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
   looptt = 1;
  }
  if (tags[tagnum].scnenum == 2)
  {
   mainchar.xpos = tags_o[tagnum].scne[0];
   mainchar.ypos = tags_o[tagnum].scne[1];
   strcpy(savefile, "main2.tpm");
   tagjmpr = tags_o[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
   looptt = 1;
  }
  if (tags[tagnum].scnenum == 3)
  {
   mainchar.xpos = tags_o[tagnum].scne[0];
   mainchar.ypos = tags_o[tagnum].scne[1];
   strcpy(savefile, "main3.tpm");
   tagjmpr = tags_o[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
   looptt = 1;
  }
  if (tags[tagnum].scnenum == 4)
  {
   mainchar.xpos = tags_o[tagnum].scne[0];
   mainchar.ypos = tags_o[tagnum].scne[1];
   strcpy(savefile, "main4.tpm");
   tagjmpr = tags_o[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
   looptt = 1;
  }
  if (tags[tagnum].scnenum == 5)
  {
   mainchar.xpos = tags_o[tagnum].scne[0];
   mainchar.ypos = tags_o[tagnum].scne[1];
   strcpy(savefile, "main5.tpm");
   tagjmpr = tags_o[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
   looptt = 1;
  }
  if (tags[tagnum].scnenum == 6)
  {
   mainchar.xpos = tags_o[tagnum].scne[0];
   mainchar.ypos = tags_o[tagnum].scne[1];
   strcpy(savefile, "main6.tpm");
   tagjmpr = tags_o[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
   looptt = 1;
  }
  if (tags[tagnum].scnenum == 7)
  {
   mainchar.xpos = tags_o[tagnum].scne[0];
   mainchar.ypos = tags_o[tagnum].scne[1];
   strcpy(savefile, "main7.tpm");
   tagjmpr = tags_o[tagnum].tagjmp;
   load();
   scantags();
   tagnum = 0;
   looptt = 1;
  }
}

void over_action(void)
{
 int ycor;
 int move;
 move = 0;
 if (mainchar_o.action == BACK_MOVE_3)
 {
  mainchar_o.action = BACK_STAND_1;
  if (collision_o[mainchar_o.xpos][mainchar_o.ypos-1] == 0)
  {
   mainchar_o.ypos--;
  }
 }
 if (mainchar_o.action == BACK_MOVE_2)
 {
  mainchar_o.action = BACK_MOVE_3;
  if (collision_o[mainchar_o.xpos][mainchar_o.ypos-1] == 0)
  {
   mainchar_o.ypos--;
  }  
 }
 if (mainchar_o.action == BACK_MOVE_1)
 {
  mainchar_o.action = BACK_MOVE_2;
  if (collision_o[mainchar_o.xpos][mainchar_o.ypos-1] == 0)
  {
   mainchar_o.ypos--;
  }
 }
 if (mainchar_o.action == FRONT_MOVE_3)
 {
  mainchar_o.action = FRONT_STAND_1;
  if (collision_o[mainchar_o.xpos][mainchar_o.ypos+1] == 0)
  {
   mainchar_o.ypos++;
  }
 }
 if (mainchar_o.action == FRONT_MOVE_2)
 {
  mainchar_o.action = FRONT_MOVE_3;
  if (collision_o[mainchar_o.xpos][mainchar_o.ypos+1] == 0)
  {
   mainchar_o.ypos++;
  }
 }
 if (mainchar_o.action == FRONT_MOVE_1)
 {
  mainchar_o.action = FRONT_MOVE_2;
  if (collision_o[mainchar_o.xpos][mainchar_o.ypos+1] == 0)
  {
   mainchar_o.ypos++;
  }
 }
 if (mainchar_o.action == LEFT_MOVE_3)
 {
  mainchar_o.action = LEFT_STAND_1;
  if (collision_o[mainchar_o.xpos-1][mainchar_o.ypos] == 0)
  {
   mainchar_o.xpos--;
  }
 }
 if (mainchar_o.action == LEFT_MOVE_2)
 {
  mainchar_o.action = LEFT_MOVE_3;
  if (collision_o[mainchar_o.xpos-1][mainchar_o.ypos] == 0)
  {
   mainchar_o.xpos--;
  }
 }
 if (mainchar_o.action == LEFT_MOVE_1)
 {
  mainchar_o.action = LEFT_MOVE_2;
  if (collision_o[mainchar_o.xpos-1][mainchar_o.ypos] == 0)
  {
   mainchar_o.xpos--;
  }
 }
 if (mainchar_o.action == RIGHT_MOVE_3)
 {
  mainchar_o.action = RIGHT_STAND_1;
  if (collision_o[mainchar_o.xpos+1][mainchar_o.ypos] == 0)
  {
   mainchar_o.xpos++;
  }
 }

 if (mainchar_o.action == RIGHT_MOVE_2)
 {
  mainchar_o.action = RIGHT_MOVE_3;
  if (collision_o[mainchar_o.xpos+1][mainchar_o.ypos] == 0)
  {
   mainchar_o.xpos++;
  }
 }
 if (mainchar_o.action == RIGHT_MOVE_1)
 {
  mainchar_o.action = RIGHT_MOVE_2;
  if (collision_o[mainchar_o.xpos+1][mainchar_o.ypos] == 0)
  {
   mainchar_o.xpos++;
  }
 }
} 

void over_check(void)
{
 int tagnum;
 tagnum = tagmap_o[mainchar_o.xpos][mainchar_o.ypos];
 if (tags_o[tagnum].scnenum == 1)
 {
  textout_centre(buffer, font, "Domon Castle", 320, 7, 1850);
 }
 if (tags_o[tagnum].scnenum == 2)
 {
  textout_centre(buffer, font, "Garden Mountain", 320, 7, 1850);
 }
 if (tags_o[tagnum].scnenum == 3)
 {
  textout_centre(buffer, font, "Forested Plateau", 320, 7, 1850);
 }
 if (tags_o[tagnum].scnenum == 4)
 {
  textout_centre(buffer, font, "Cave of the Tek", 320, 7, 1850);
 }
 if (tags_o[tagnum].scnenum == 5)
 {
  textout_centre(buffer, font, "South Beach", 320, 7, 1850);
 }
 if (tags_o[tagnum].scnenum == 6)
 {
  textout_centre(buffer, font, "Village of Zig", 320, 7, 1850);
 }
}

void enemy_showatk(void)
{
 int ycor;
 //show the attack type//
 blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 sprite_batl();
 renderboxes();
 if (slotter == 0)
 {
  if (party[attacker].id == 1)
  {
   if (party[attacker].inwait == LIMIT)
   {
    stretch_sprite(buffer, textbox, 240, 0, 160, 50);
    textout_centre(buffer, font, "Charge Breaker", 320, 21, 1850);
   }
  }
 }
 if (slotter == 1)
 {
  if (enemy[attacker].id == 1)
  {
   stretch_sprite(buffer, textbox, 240, 0, 160, 50);
   textout_centre(buffer, font, "Flare", 320, 21, 1850);
  }
 }
 blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 clear(buffer);
 for (ycor=0;ycor<10000000;ycor++)
 { }
 //show the effect//
 for (ycor=0;ycor<1200;ycor++);
 {
  blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  sprite_batl();
  renderboxes();
  if (slotter == 0) //if you are doing the first check in the battle handler
  {
   if (party[attacker].id == 1) //if the attacker id is 1 or seren
   {
    stretch_sprite(buffer, slash1, enemy[defender].xpos*20, enemy[defender].ypos*10, ycor/20, 60);
    set_trans_blender(0, 0, 0, 125);
    stretch_blit(slash1, sprite1, 0, 0, 83, 68, 0, 0, ycor/20, 60);
    draw_trans_sprite(buffer, sprite1, enemy[defender].xpos*20, enemy[defender].ypos*10); 
   }
  }
  if (slotter == 1) //if you are doing the second check in the battle handler
  {
   if (enemy[attacker].id == 1) //if the attacker id is 1 or mech
   {
    stretch_sprite(buffer, flare1, party[defender].xpos*20, party[defender].ypos*10, 60, 60);
   }
  }
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
  clear(buffer);
 }
 blit(batlback, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 sprite_batl();
 renderboxes();
 blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 clear(buffer);
 for (ycor=0;ycor<10000000;ycor++)
 { }
}

