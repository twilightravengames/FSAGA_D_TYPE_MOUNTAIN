
//can use ex19.c for textout 

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
void save(void);
void load(void);
void patchbmp(void);
void rendersprite(void);
void loopxs(void);

BITMAP *floor1;
BITMAP *floor2;
BITMAP *buffer;
BITMAP *bufferx;
BITMAP *pill1;
BITMAP *pill2;
BITMAP *serf1;
BITMAP *wall1;
BITMAP *wall2;
BITMAP *wall3;
BITMAP *wall4;
BITMAP *chest;
BITMAP *chest2;
BITMAP *beda1;
BITMAP *colblock;
BITMAP *tagblock;
FILE *level;
PALETTE the_pallete;
BITMAP *arch1;
BITMAP *arch2;
BITMAP *door1;
BITMAP *stairs1;
BITMAP *stairs2;
BITMAP *ninjaf1;
BITMAP *scrn;
BITMAP *top1;
BITMAP *top2;
BITMAP *shield1;
BITMAP *torch1;
BITMAP *torch2;
BITMAP *grass1;
BITMAP *grass2;
BITMAP *grass3;
BITMAP *grass4;
BITMAP *stone1;
BITMAP *higras1;
BITMAP *watr1;
BITMAP *watr2;
BITMAP *mnts1;

char tmp[80];
char *argv[];
char filename[80];

char savefile[80]; //savefile name
int looper;
int scnenum; //current scnenum
int oldscne; //old scene number
int scrollx; //scrollx value
int scrolly; //scrolly value
int tilex; //tilex coordinate
int tiley; //tiley coordinate
int loopvalue; // for loopxs

//the following below are the main data tables for TP

int bottombmp[100][100]; //bottom layer bitmap, under player always
int midbmp[100][100]; //mid layer bmp, squares that overlap the player's body
int mid2bmp[100][100]; //second mid layer, below player but above mid
int hibmp[100][100];
int tagmap[100][100]; //locations of  ---> their id's 
int collision[100][100]; //collision table for scene
int blendbmp[100][100]; //blend bmp
int mnster; //monster class id to encounter here

int i;

typedef struct tag{
int scnenum; //scene to jump to (#)
int pressa;
int item;
int patch[3];
int scne[2]; //scne x,y start point after the jump
int overworld; //jump into overworld #
int actrig; //action/event trigger number
int tagjmp;
}tag;

tag tags[20];


int itmp;
typedef struct sprite{
int xpos;
int ypos;
int zpos;
int bmpid; //if bmpid = 1 then it is the main character if it is 0 it is inactive
int action;
int gp;
int actrig;
int immobile;
}sprite;

int loopvar = 0;
int tagnum;
int inpt;

sprite sprites[10];

int tagid;
int value;
int yslot;
int xslot;
int tagidp;
//

main()
{
 printf("Loading...\n");
 init();
 printf("Load Ok!\n");
 printf("press any key...\n");
 readkey();
 set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
 clear(screen);
 clear(buffer);
 clear(bufferx);
 maingame();
}

void init(void)
{
 tagid = 0;
 allegro_init();
 install_keyboard();
 set_color_depth(16);
 set_color_conversion(COLORCONV_TOTAL);
 buffer = create_bitmap(1280, 960);  
 bufferx = create_bitmap(640, 480);  
 scrn = create_bitmap(640, 480);  
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "floor1.bmp");
 floor1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "floor2.bmp");
 floor2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "pila1.bmp");
 pill1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "pila2.bmp");
 pill2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "serf1.bmp");
 serf1 = load_bmp(filename, the_pallete);
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
 strcpy(get_filename(filename), "beda1.bmp");
 beda1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "colblock.bmp");
 colblock = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "tagblock.bmp");
 tagblock = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "arch1.bmp");
 arch1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "arch2.bmp");
 arch2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "door1.bmp");
 door1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "chest2.bmp");
 chest2 = load_bmp(filename, the_pallete);
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
 strcpy(get_filename(filename), "top1.bmp");
 top1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "top2.bmp");
 top2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "torch1.bmp");
 torch1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "torch2.bmp");
 torch2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "shield1.bmp");
 shield1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "grass1.bmp");
 grass1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "grass2.bmp");
 grass2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "grass3.bmp");
 grass3 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "grass4.bmp");
 grass4 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "stone1.bmp");
 stone1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "higras.bmp");
 higras1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "watr1.bmp");
 watr1 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "watr2.bmp");
 watr2 = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "mnts1.bmp");
 mnts1 = load_bmp(filename, the_pallete);
}

void maingame(void)
{
 tilex = 1;
 tiley = 1;
 scrollx = 1;
 scrolly = 1;
 looper = 0;
 while(looper == 0)
 {
  clear_keybuf();
  renderer();
  input();
 }
}

void input(void)
{
 char reader;
 int spritebox;
 int xcor;
 int ycor;
 int tileid;
 char text[80];
 if (key[KEY_1])
 {
  clear_keybuf();
  tileid = 1;
  loopvar = 0;
  while(loopvar == 0)
  {
   //print text//
   //get input//
   if (key[KEY_N])
   {
    tileid--;
   }
   if (key[KEY_M])
   {
    tileid++;
   }
   if (key[KEY_LEFT])
   {
    if (tilex != 0)
    {
     tilex--;
     if (tilex < 0+scrollx)
     {
      scrollx--;
     }
    }
   }
   if (key[KEY_RIGHT])
   {
    if (tilex != 62)
    {
     tilex++;
     if (tilex > 30+scrollx)
     {
      scrollx++;
     }
    }
   }
   if (key[KEY_UP])
   {
    if (tiley > 0)
    {
     tiley--;
     if (tiley < 0+scrolly)
     {
      scrolly--;
     }
    }
   }
   if (key[KEY_DOWN])
   {
    if (tiley != 92)
    {
     tiley++;
     if (tiley >= 44+scrolly)
     {
      scrolly++;
     }
    }
   }
   if (key[KEY_ENTER])
   {
    bottombmp[tilex][tiley] = tileid;
   }
   if (key[KEY_C])
   {
    bottombmp[tilex][tiley] = 0;
   }
   if (key[KEY_Q])
   {
    loopvar = -3344;
   }
 
   renderbottom();
   rendermid();
   renderhi();

   if (tileid == 1)
   {
    stretch_sprite(buffer, floor1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 2)
   {
    stretch_sprite(buffer, floor2, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 3)
   {
    stretch_sprite(buffer, grass1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 4)
   {
    stretch_sprite(buffer, grass2, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 5)
   {
    stretch_sprite(buffer, grass3, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 6)
   {
    stretch_sprite(buffer, grass4, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 7)
   {
    stretch_sprite(buffer, stone1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 8)
   {
    stretch_sprite(buffer, higras1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 9)
   {
    stretch_sprite(buffer, watr1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 10)
   {
    stretch_sprite(buffer, watr2, tilex*20+1, tiley*10+1, 40, 40);
   }
   blit(buffer, bufferx, 0+(scrollx*20), 0+(scrolly*10), 0, 0, 640, 480);  // copy it to the screen
   textout(bufferx, font, "-Bottom Tile Layer-", 5, 5, 1850);
   textout(bufferx, font, "M/N cycles through tiles, Q to quit", 5, 13, 1850);   
   textout(bufferx, font, "ENTER places tile in location, C clears the location", 5, 21, 1850);   
   sprintf(tmp, "Tile x coordinate is %d, Tile y is %d", tilex, tiley);
   textout(bufferx, font, tmp, 5, 470, 1850);
   sprintf(tmp, "Tile 1,1 value is bot%d, mid%d, mid2%d, hi%d", bottombmp[1][1], midbmp[1][1], mid2bmp[1][1], hibmp[1][1]);
   textout(bufferx, font, tmp, 5, 460, 1850);
   blit(bufferx, screen, 0, 0, 0, 0, 640, 480);
   clear(bufferx);
   clear(buffer);
  }
 }
 if (key[KEY_2])
 {
  clear_keybuf();
  tileid = 0;
  loopvar = 0;
  while(loopvar == 0)
  {
   //print text//
   //get input//
   if (key[KEY_N])
   {
    tileid--;
   }
   if (key[KEY_M])
   {
    tileid++;
    textout(bufferx, font, "Mid-Select tile type to use and local", 5, 10, 1850);
   }
   if (key[KEY_LEFT])
   {
    if (tilex != 0)
    {
     tilex--;
     if (tilex < 0+scrollx)
     {
      scrollx--;
     }
    }
   }
   if (key[KEY_RIGHT])
   {
    if (tilex != 62)
    {
     tilex++;
     if (tilex > 30+scrollx)
     {
      scrollx++;
     }
    }
   }
   if (key[KEY_UP])
   {
    if (tiley > 0)
    {
     tiley--;
     if (tiley < 0+scrolly)
     {
      scrolly--;
     }
    }
   }
   if (key[KEY_DOWN])
   {
    if (tiley != 92)
    {
     tiley++;
     if (tiley >= 44+scrolly)
     {
      scrolly++;
     }
    }
   }
   if (key[KEY_ENTER])
   {
    midbmp[tilex][tiley] = tileid;
   }
   if (key[KEY_C])
   {
    midbmp[tilex][tiley] = 0;
   }
   if (key[KEY_Q])
   {
    loopvar = -3344;
   }

   renderbottom();
   rendermid();
   if (tileid == 1)
   {
    stretch_sprite(buffer, wall1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 2)
   {
    stretch_sprite(buffer, wall2, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 3)
   {
    stretch_sprite(buffer, chest, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 4)
   {
    stretch_sprite(buffer, beda1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 5)
   {
    stretch_sprite(buffer, chest2, tilex*20+1, tiley*10+1, 40, 40);
   }   
   if (tileid == 6)
   {
    stretch_sprite(buffer, wall3, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 7)
   {
    stretch_sprite(buffer, wall4, tilex*20+1, tiley*10+1, 40, 40);
   }
   renderhi();
   blit(buffer, bufferx, 0+(scrollx*20), 0+(scrolly*10), 0, 0, 640, 480);  // copy it to the screen
   textout(bufferx, font, "-Middle Tile Layer-", 5, 5, 1850);
   textout(bufferx, font, "M/N cycles through tiles, Q to quit", 5, 13, 1850);   
   textout(bufferx, font, "ENTER places tile in location, C clears the location", 5, 21, 1850);   
   blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   clear(buffer);
   clear(bufferx);
  }
 }
 if (key[KEY_3])
 {
  clear_keybuf();
  tileid = 0;
  loopvar = 0;
  while(loopvar == 0)
  {
   if (key[KEY_N])
   {
    tileid--;
   }
   if (key[KEY_M])
   {
    tileid++;
   }
   if (key[KEY_LEFT])
   {
    if (tilex != 0)
    {
     tilex--;
     if (tilex < 0+scrollx)
     {
      scrollx--;
     }
    }
   }
   if (key[KEY_RIGHT])
   {
    if (tilex != 62)
    {
     tilex++;
     if (tilex > 30+scrollx)
     {
      scrollx++;
     }
    }
   }
   if (key[KEY_UP])
   {
    if (tiley > 0)
    {
     tiley--;
     if (tiley < 0+scrolly)
     {
      scrolly--;
     }
    }
   }
   if (key[KEY_DOWN])
   {
    if (tiley != 92)
    {
     tiley++;
     if (tiley >= 44+scrolly)
     {
      scrolly++;
     }
    }
   }

   if (key[KEY_ENTER])
   {
    hibmp[tilex][tiley] = tileid;
   }
   if (key[KEY_C])
   {
    hibmp[tilex][tiley] = 0;
   }
   if (key[KEY_Q])
   {
    loopvar = -3344;
   }

   renderbottom();
   rendermid();
   renderhi();
   if (tileid == 1)
   {
    stretch_sprite(buffer, top1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 2)
   {
    stretch_sprite(buffer, top2, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 3)
   {
    stretch_sprite(buffer, wall1, tilex*20+1, tiley*10+1, 40, 40);
   }
   blit(buffer, bufferx, 0+(scrollx*20), 0+(scrolly*10), 0, 0, 640, 480);  // copy it to the screen
   textout(bufferx, font, "-Ceiling Tile Layer-", 5, 5, 1850);
   textout(bufferx, font, "M/N cycles through tiles, Q to quit", 5, 13, 1850);   
   textout(bufferx, font, "ENTER places tile in location, C clears the location", 5, 21, 1850);   
   blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   clear(buffer);
   clear(bufferx);
  }
 }

 if (key[KEY_4])
 {
  clear_keybuf();
  tileid = 0;
  loopvar = 0;
  while(loopvar == 0)
  {
   if (key[KEY_LEFT])
   {
    if (tilex != 0)
    {
     tilex--;
     if (tilex < 0+scrollx)
     {
      scrollx--;
     }
    }
   }
   if (key[KEY_RIGHT])
   {
    if (tilex != 62)
    {
     tilex++;
     if (tilex > 30+scrollx)
     {
      scrollx++;
     }
    }
   }
   if (key[KEY_UP])
   {
    if (tiley > 0)
    {
     tiley--;
     if (tiley < 0+scrolly)
     {
      scrolly--;
     }
    }
   }
   if (key[KEY_DOWN])
   {
    if (tiley != 92)
    {
     tiley++;
     if (tiley >= 44+scrolly)
     {
      scrolly++;
     }
    }
   }


   if (key[KEY_ENTER])
   {
    tagmap[tilex][tiley] = tagid;
   }
   if (key[KEY_Q])
   {
    loopvar = -3344;
   }
   if (key[KEY_C])
   {
    tagmap[tilex][tiley] = 0;
   }
   if (key[KEY_M])
   {
    tagid++;
   }
   if (key[KEY_N])
   {
    tagid--;
   }
   renderbottom();
   rendermid();
   renderhi();
   stretch_sprite(buffer, tagblock, tilex*20+1, tiley*10+1, 60, 60);
   blit(buffer, bufferx, 0+(scrollx*20), 0+(scrolly*10), 0, 0, 640, 480);  // copy it to the screen
   textout(bufferx, font, "-Insert Tag Blocks-", 5, 5, 1850);
   textout(bufferx, font, "Tag blocks trigger events in the game when touched", 5, 13, 1850);   
   textout(bufferx, font, "(Arrow Keys) move block (M/N) Change ID of the current tagblock", 5, 21, 1850);   
   textout(bufferx, font, "(ENTER) place a block (C)lear a block (Q)uit", 5, 29, 1850);   
   sprintf(tmp, "Current Tag Id Num: %d", tagid);
   textout(bufferx, font, tmp, 5, 470, 1850);
   blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   clear(buffer);
   clear(bufferx);
  }
 }

 if (key[KEY_5])
 {
  clear_keybuf();
  tileid = 0;
  xcor = 1;
  ycor = 1;
  loopvar = 0;
  while(loopvar == 0)
  {
   if (key[KEY_LEFT])
   {
    if (tilex != 0)
    {
     tilex--;
     if (tilex < 0+scrollx)
     {
      scrollx--;
     }
    }
   }
   if (key[KEY_RIGHT])
   {
    if (tilex != 62)
    {
     tilex++;
     if (tilex > 30+scrollx)
     {
      scrollx++;
     }
    }
   }
   if (key[KEY_UP])
   {
    if (tiley > 0)
    {
     tiley--;
     if (tiley < 0+scrolly)
     {
      scrolly--;
     }
    }
   }
   if (key[KEY_DOWN])
   {
    if (tiley != 92)
    {
     tiley++;
     if (tiley >= 44+scrolly)
     {
      scrolly++;
     }
    }
   }
   if (key[KEY_C])
   {
    collision[tilex][tiley] = 0;
   }
   if (key[KEY_ENTER])
   {
    collision[tilex][tiley] = 1;
   }
   if (key[KEY_Q])
   {
    loopvar = 3;
   }
   renderbottom();
   rendermid();
   renderhi();
   stretch_sprite(buffer, colblock, tilex*20+1, tiley*10+1, 60, 60);
   blit(buffer, bufferx, 0+(scrollx*20), 0+(scrolly*10), 0, 0, 640, 480);  // copy it to the screen
   textout(bufferx, font, "Collision Detection Block Mode", 5, 5, 1850);
   textout(bufferx, font, "Collision blocks are invisibe barriers used for walls,etc.", 5, 13, 1850);
   textout(bufferx, font, "(ARROWKEYS) Move block (C)lear Block (ENTER) Place Block", 5, 21, 1850);
   textout(bufferx, font, "(Q)uit", 5, 29, 1850);
   blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   clear(buffer);
   clear(bufferx);
  }
 }
 if (key[KEY_C])
 {
  textout(bufferx, font, "Clearing Collision Blocks", 5, 5, 1850);
  for (xcor=0;xcor<100;xcor++)
  {
   for (ycor=0;ycor<100;ycor++)
   {
    collision[xcor][ycor] = 0;
   }
  }
 }
 if (key[KEY_6])
 {
  clear_keybuf();
  clear(screen);
  clear(bufferx);
  textout(bufferx, font, "-Tag Edit Mode-                                     ", 5, 5, 1850);
  textout(bufferx, font, "Please enter which tag # you would like to edit:    ", 5, 13, 1850);
  loopxs();
  tagnum = loopvalue;
  sprintf(tmp, "Editing tag number %d", tagnum);
  textout(screen, font, tmp, 5, 632, 1850);
  textout(screen, font, "What attribute would you like to edit?          ", 5, 5, 1850);
  textout(screen, font, "1) Scene Number to jump to.                     ", 5, 13, 1850);
  textout(screen, font, "2) Press enter to activate?                     ", 5, 21, 1850);
  textout(screen, font, "3) Enter an item type                           ", 5, 29, 1850);
  textout(screen, font, "4) Enter a patch bitmap for coordinates         ", 5, 37, 1850);
  textout(screen, font, "5) Clear tag from having item                   ", 5, 45, 1850);
  textout(screen, font, "6) Scene Jump new x y coordinates               ", 5, 53, 1850);
  textout(screen, font, "7) Overworld Number to jump to                  ", 5, 61, 1850);
  textout(screen, font, "8) In Next Scene tag to jump to                 ", 5, 69, 1850);
  clear_keybuf();
  itmp = (readkey() >> 8) - 1;
  sprintf(tmp, "Editing tag number %d attribute type num %d", tagnum, itmp);
  textout(screen, font, tmp, 5, 632, 1850);
  if (itmp == 1)
  {
   clear(screen);
   clear_keybuf();
   textout(screen, font, "Enter the scene number:                       ", 5, 5, 1850);
   itmp = (readkey() >> 8) - 1;
   sprintf(tmp, "Are you sure you want to use scene number %d?          ", itmp);
   textout(screen, font, tmp, 5, 13, 1850);
   reader = readkey() & 0xff;
   if (reader == 'y')
   {
    tags[tagnum].scnenum = itmp;
    if (itmp == 10)
    {
     itmp = 0;
    }
    itmp = 0;  
   }
  }
  if (itmp == 2)
  {
   clear(screen);
   textout(screen, font, "Enter value for slot.0=false 1=true           ", 5, 5, 1850);
   itmp = (readkey() >> 8) - 1;
   tags[tagnum].pressa = itmp;
   if (itmp != 1)
   {
    tags[tagnum].pressa = 0;
   }
  }
  if (itmp == 3)
  {
   clear(screen);
   textout(screen, font, "Enter item number:                 ", 5, 5, 1850);
   clear_keybuf();
   tileid = 1;
   loopvar = 0;
   while(loopvar == 0)
   {
    if (key[KEY_M])
    {
     tileid++;
    }
    if (key[KEY_N])
    {
     tileid--;
    }
    if (key[KEY_Q])
    {
     loopvar = 56;
    }
    if (tileid == 1)
    {
     textout(bufferx, font, "Place Item-Empty Box                  ", 5, 21, 1850);
     tags[tagnum].item = -1;
    }
    if (tileid == 2)
    {
     textout(bufferx, font, "Place Item- 100GP                     ", 5, 21, 1850);
     tags[tagnum].item = 1;
    }
    if (tileid == 3)
    {
     textout(bufferx, font, "Place Item-Small Potion +50 hp           ", 5, 21, 1850);
     tags[tagnum].item = 2;
    }
    if (tileid == 4)
    {
     textout(bufferx, font, "Place Item-Hot Dog +15hp                ", 5, 21, 1850);
     tags[tagnum].item = 3;
    }
    if (tileid == 5)
    {
     textout(bufferx, font, "Place Item-Food +45hp                   ", 5, 21, 1850);
     tags[tagnum].item = 4;
    }
    if (tileid == 6)
    {
     textout(bufferx, font, "Place Item-Skull Amulet +2AP             ", 5, 21, 1850);
     tags[tagnum].item = 5;
    }
    if (tileid == 7)
    {
     textout(bufferx, font, "Place Item-Scale Petal Temp.+1 Def.     ", 5, 21, 1850);
     tags[tagnum].item = 6;
    }
    if (tileid == 8)
    {
     textout(bufferx, font, "Place Item-Adamantiun Rod Temp.+100%AP  ", 5, 21, 1850);
     tags[tagnum].item = 7;
    }
    if (tileid == 9)
    {
     textout(bufferx, font, "Place Item-Antidote [Cure Poison]       ", 5, 21, 1850);
     tags[tagnum].item = 8;
    }
    if (tileid == 10)
    {
     textout(bufferx, font, "Place Item-Slug Oil [Cure Paralysis]    ", 5, 21, 1850);
     tags[tagnum].item = 9;
    }
    textout(bufferx, font, "Select an Item to put in:", 5, 5, 1850);
    blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
    clear(bufferx);
   }
  }
  if (itmp == 4)
  {
   tagidp = tagnum;
   patchbmp();
  }
  if (itmp == 5)
  {
   tags[tagnum].item = 0;
   textout(screen, font, "Item slot cleared                 ", 5, 5, 1850);
  }
  if (itmp == 6)
  {
   tileid = 0;
   xcor = 1;
   ycor = 1;
   loopvar = 0;
   while(loopvar == 0)
   {
    if (key[KEY_LEFT])
    {
     if (tilex != 0)
     {
      tilex--;
      if (tilex < 0+scrollx)
      {
       scrollx--;
      }
     }
    }
    if (key[KEY_RIGHT])
    {
     if (tilex != 62)
     {
      tilex++;
      if (tilex > 30+scrollx)
      {
       scrollx++;
      }
     }
    }
    if (key[KEY_UP])
    {
     if (tiley > 0)
     {
      tiley--;
      if (tiley < 0+scrolly)
       {
       scrolly--;
      }
     } 
    }  
    if (key[KEY_DOWN])
    {
     if (tiley != 92)
     {
      tiley++;
       if (tiley >= 44+scrolly)
      {
       scrolly++;
      }
     }
     }
    if (key[KEY_C])
    {
     tags[tagnum].scne[0] = 0;
     tags[tagnum].scne[1] = 0;
    }
    if (key[KEY_ENTER])
    {
     tags[tagnum].scne[0] = tilex;
     tags[tagnum].scne[1] = tiley;
    }
    if (key[KEY_Q])
    {
     loopvar = 3;
    }  
    renderbottom();
    rendermid();
    renderhi();
    stretch_sprite(buffer, colblock, tilex*20+1, tiley*10+1, 60, 60);
    blit(buffer, bufferx, 0+(scrollx*20), 0+(scrolly*10), 0, 0, 640, 480);  // copy it to the screen
    //show current location of the start point//
    textout(bufferx, font, "Use arrow key to select entrance position, enter to select", 5, 5, 1850);
    sprintf(tmp, "Xcor: %d", tilex);
    textout(bufferx, font, tmp, 5, 200, 1850);
    sprintf(tmp, "Ycor: %d", tiley);
    textout(bufferx, font, tmp, 5, 209, 1850);
    blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
    clear(buffer);
    clear(bufferx);
   }
  }
  if (itmp == 7)
  {
   clear(screen);
   clear_keybuf();
   textout(screen, font, "Enter the overworld number:                       ", 5, 5, 1850);
   itmp = (readkey() >> 8) - 1;
   sprintf(tmp, "Are you sure you want to use overworld number %d?          ", itmp);
   textout(screen, font, tmp, 5, 13, 1850);
   reader = readkey() & 0xff;
   if (reader == 'y')
   {
    tags[tagnum].overworld = itmp;
    if (itmp == 10)
    {
     itmp = 0;
    }
    itmp = 0;  
   }
  }
  if (itmp == 8)
  {
   clear(screen);
   clear_keybuf();
   textout(screen, font, "Enter the tag to jump to number:                  ", 5, 5, 1850);
   itmp = (readkey() >> 8) - 1;
   sprintf(tmp, "Are you sure you want to use tag number %d?                 ", itmp);
   textout(screen, font, tmp, 5, 13, 1850);
   reader = readkey() & 0xff;
   if (reader == 'y')
   {
    tags[tagnum].tagjmp = itmp;
    if (itmp == 10)
    {
      itmp = 0;
    }
    itmp = 0;  
   }  
  }   
 }
 if (key[KEY_7])
 {
  tileid = 0;
  spritebox = 0;
  xcor = 1;
  ycor = 1;
  loopvar = 0;
  while(loopvar == 0)
  {
   if (key[KEY_LEFT])
   {
    if (tilex != 0)
    {
     tilex--;
     if (tilex < 0+scrollx)
     {
      scrollx--;
     }
    }
   }
   if (key[KEY_RIGHT])
   {
    if (tilex != 62)
    {
     tilex++;
     if (tilex > 30+scrollx)
     {
      scrollx++;
     }
    }
   }
   if (key[KEY_UP])
   {
    if (tiley > 0)
    {
     tiley--;
     if (tiley < 0+scrolly)
     {
      scrolly--;
     }
    }
   }
   if (key[KEY_DOWN])
   {
    if (tiley != 92)
    {
     tiley++;
     if (tiley >= 44+scrolly)
     {
      scrolly++;
     }
    }
   }

   if (key[KEY_M])
   {
    tileid++;
   }
   if (key[KEY_N])
   {
    tileid--;
   }
   if (key[KEY_L])
   {
    spritebox--;
   }
   if (key[KEY_K])
   {
    spritebox++;
   }
   if (key[KEY_C])
   {
    sprites[spritebox].bmpid = 0;
    sprites[spritebox].xpos = 0;
    sprites[spritebox].ypos = 0;
   }
   if (key[KEY_ENTER])
   {
    sprites[spritebox].bmpid = tileid;
    sprites[spritebox].xpos = tilex;
    sprites[spritebox].ypos = tiley;
   }
   if (key[KEY_Q])
   {
    loopvar = 3;
   }
   renderbottom();
   rendermid();
   renderhi();
   if (tileid == 1)
   {
    stretch_sprite(buffer, serf1, tilex*20, tiley*10, 60, 60);
   }
   if (tileid == 2)
   {
    stretch_sprite(buffer, ninjaf1, tilex*20, tiley*10, 60, 60);
   }
   blit(buffer, bufferx, 0+(scrollx*20), 0+(scrolly*10), 0, 0, 640, 480);  // copy it to the screen
   //show current location of the start point//
   textout(bufferx, font, "Sprite Insertion Mode", 7, 7, 1850);
   textout(bufferx, font, "N/M change sprite type, ENTER to place, C-clear, K/L to change spritebox", 7, 15, 1850);
   sprintf(tmp, "Current id is %d, Current box num is %d", tileid, spritebox);
   textout(bufferx, font, tmp, 7, 472, 1850);
   blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   clear(buffer);
   clear(bufferx);
  }
 }
 if (key[KEY_8])
 {
  clear_keybuf();
  clear(screen);
  tileid = 0;
  loopvar = 0;
  clear_keybuf();
  textout(screen, font, "-Sprite Edit Mode-                                 ", 5, 5, 1850);
  textout(screen, font, "Please enter which sprite # you would like to edit:", 5, 13, 1850);
  tagnum = (readkey() >> 8) - 1;
  textout(screen, font, "What attribute would you like to edit?              ", 5, 5, 1850);
  textout(screen, font, "1) Action number to activate!                        ", 5, 13, 1850);
  textout(screen, font, "2) GP the character holds!                         ", 5, 21, 1850);
  itmp = (readkey() >> 8) - 1;
  if (itmp == 1)
  {
   clear_keybuf();
   renderbottom();
   rendermid();
   renderhi();
   blit(buffer, bufferx, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   textout(bufferx, font, "-Edit Action num mode-use m/n to cycle through action num, enter to select    ", 5, 5, 1850);
   while(loopvar == 0)
   {
    if (key[KEY_M])
    {
     tileid++;
     clear_keybuf();
    }
    if (key[KEY_N])
    {
     tileid--;
     clear_keybuf();
    }
    if (key[KEY_ENTER])
    {
     sprites[tagnum].actrig = tileid;
     sprintf(tmp, "Is a value of %d ok? press any key", tileid);
     textout(bufferx, font, tmp, 5, 33, 1850);
     loopvar = 12;
     clear_keybuf();
    }
    if (key[KEY_Q])
    {
     loopvar = -999;
     clear_keybuf();
    }
    if (tileid == 0)
    {
     textout(bufferx, font, "No action trig selected                   ", 5, 13, 1850);
     textout(bufferx, font, "                                          ", 5, 21, 1850);
    }
    if (tileid == 1)
    {
     textout(bufferx, font, "Text: There is something evil in this castle, I can feel it!", 5, 13, 1850);
     textout(bufferx, font, "                                          ", 5, 21, 1850);
    }
    if (tileid == 2)
    {
     textout(bufferx, font, "Text: Emperor Blade seeks to resurrect power to destroy our sister kingdom-Damon", 5, 13, 1850);
     textout(bufferx, font, "                                          ", 5, 21, 1850);
    }
    if (tileid == 3)
    {
     textout(bufferx, font, "Text: The Emperor seeks your presence immediately! Go up the outside hall and throught the stairs until you reach the outside. Then continue up into his throne room.", 5, 13, 1850);
     textout(bufferx, font, "                                          ", 5, 21, 1850);
    }
    if (tileid == 4)
    {
     textout(bufferx, font, "Text: Hey, being succesor to the last commander must be exciting eh? I wish I had your power!", 5, 13, 1850);
     textout(bufferx, font, "                                          ", 5, 21, 1850);
    }
    if (tileid == 5)
    {
     textout(bufferx, font,"Text: Why is the domonian army mobolizing? People say the Emperor seeks the power of the Komit once again! Is this true?", 5, 13, 1850);
     textout(bufferx, font, "                                          ", 5, 21, 1850);
    } 
    if (tileid == 6)
    {
     textout(bufferx, font,"Text: 1,000 years ago our army entered a war with Damonia. There was a stalemate, the people of the world under our rule became so angry, they prayed to the master for help, and he gave them the gift of Techno. They became techno knights, defeated our armies and sealed away our powers. In their final breath, they made their bodies dormant so if we would ever return to war, they could stop us again.", 5, 13, 1850);
     textout(bufferx, font, "                                          ", 5, 21, 1850);
    }
    sprintf(tmp, "Current action trig is %d, sprite number is %d", tileid, tagnum);
    textout(bufferx, font, tmp, 5, 470, 1850);
    blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
    clear(buffer);
    clear(bufferx);
   }
  }
 }
 if (key[KEY_9])
 {
  clear(screen);
  clear_keybuf();
  textout(screen, font, "Please enter which you would like to view:", 5, 5, 1850);
  textout(screen, font, "1) Tag Characteristics", 5, 13, 1850);
  textout(screen, font, "2) Sprite Characteristics", 5, 21, 1850);
  textout(screen, font, "Q) Quit", 5, 29, 1850);
  itmp = (readkey() >> 8) - 1;
  if (itmp == 1)
  {
   clear(screen);
   textout(screen, font, "Enter tag number to view:", 5, 5, 1850);
   itmp = (readkey() >> 8) - 1;
   sprintf(tmp, "Tag number %d", itmp);
   textout(screen, font, tmp, 5, 29, 1850);
   sprintf(tmp, "On activate, jump to scene %d", tags[itmp].scnenum);
   textout(screen, font, tmp, 5, 37, 1850);
   sprintf(tmp, "Press a to activate- 0=false,1=true. value is %d", tags[itmp].pressa);
   textout(screen, font, tmp, 5, 45, 1850);
   sprintf(tmp, "The tag contains item ID %d", tags[itmp].item);
   textout(screen, font, tmp, 5, 53, 1850);
   sprintf(tmp, "Patch bmp id is %d, it's x is %d, it's y is %d", tags[itmp].patch[0], tags[itmp].patch[1], tags[itmp].patch[2]);
   textout(screen, font, tmp, 5, 61, 1850);
   sprintf(tmp, "Scene x and y jump to are %d,%d", tags[itmp].scne[0], tags[itmp].scne[1]);
   textout(screen, font, tmp, 5, 69, 1850);
   sprintf(tmp, "On activate, jump to overworld %d", tags[itmp].overworld);
   textout(screen, font, tmp, 5, 69, 1850);
   sprintf(tmp, "On activate, in next scene jump to tag # %d", tags[itmp].tagjmp);
   textout(screen, font, tmp, 5, 78, 1850);
   readkey();
  }
  if (itmp == 2)
  {
   clear(screen);
   textout(screen, font, "Enter sprite number to view:", 5, 5, 1850);
   itmp = (readkey() >> 8) - 1;
   textout(screen, font, "Remember 0 is always false, 1 is true", 5, 29, 1870);
   sprintf(tmp, "Sprite's Number is %d", itmp);
   textout(screen, font, tmp, 5, 37, 1850);
   sprintf(tmp, "Sprite's position is x,y,z-- %d,%d,%d", sprites[itmp].xpos, sprites[itmp].ypos, sprites[itmp].zpos);
   textout(screen, font, tmp, 5, 45, 1850);
   sprintf(tmp, "Sprite's Bitmap Id is %d", sprites[itmp].bmpid);
   textout(screen, font, tmp, 5, 53, 1850);
   sprintf(tmp, "Sprite's action trigger is %d", sprites[itmp].actrig);
   textout(screen, font, tmp, 5, 61, 1850);
   sprintf(tmp, "Is sprite immobile?--%d", sprites[itmp].immobile);
   textout(screen, font, tmp, 5, 69, 1850);
   readkey();
  }
 }
 if (key[KEY_V])
 {
  clear_keybuf();
  tileid = 0;
  loopvar = 0;
  while(loopvar == 0)
  {
   //print text//
   //get input//
   if (key[KEY_N])
   {
    tileid--;
   }
   if (key[KEY_M])
   {
    tileid++;
   }
   if (key[KEY_LEFT])
   {
    if (tilex != 0)
    {
     tilex--;
     if (tilex < 0+scrollx)
     {
      scrollx--;
     }
    }
   }
   if (key[KEY_RIGHT])
   {
    if (tilex != 62)
    {
     tilex++;
     if (tilex > 30+scrollx)
     {
      scrollx++;
     }
    }
   }
   if (key[KEY_UP])
   {
    if (tiley > 0)
    {
     tiley--;
     if (tiley < 0+scrolly)
     {
      scrolly--;
     }
    }
   }
   if (key[KEY_DOWN])
   {
    if (tiley != 92)
    {
     tiley++;
     if (tiley >= 44+scrolly)
     {
      scrolly++;
     }
    }
   }
   if (key[KEY_ENTER])
   {
    mid2bmp[tilex][tiley] = tileid;
   }
   if (key[KEY_C])
   {
    mid2bmp[tilex][tiley] = 0;
   }
   if (key[KEY_Q])
   {
    loopvar = -3344;
   }

   renderbottom();
   rendermid();

   if (tileid == 1)
   {
    stretch_sprite(buffer, arch1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 2)
   {
    stretch_sprite(buffer, arch2, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 3)
   {
    stretch_sprite(buffer, door1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 4)
   {
    stretch_sprite(buffer, pill1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 5)
   {
    stretch_sprite(buffer, pill2, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 6)
   {
    stretch_sprite(buffer, shield1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 7)
   {
    stretch_sprite(buffer, torch1, tilex*20+1, tiley*10+1, 40, 40);
   }   
   if (tileid == 8)
   {
    stretch_sprite(buffer, torch2, tilex*20+1, tiley*10+1, 40, 40);
   }   
   if (tileid == 9)
   {
    stretch_sprite(buffer, stairs1, tilex*20+1, tiley*10+1, 40, 40);
   }   
   if (tileid == 10)
   {
    stretch_sprite(buffer, stairs2, tilex*20+1, tiley*10+1, 40, 40);
   }   
   renderhi();
   blit(buffer, bufferx, 0+(scrollx*20), 0+(scrolly*10), 0, 0, 640, 480);  // copy it to the screen
   textout(bufferx, font, "-Wall patch layer-", 5, 5, 1850);
   textout(bufferx, font, "M/N cycles through tiles, Q to quit", 5, 13, 1850);   
   textout(bufferx, font, "ENTER places tile in location, C clears the location", 5, 21, 1850);   
   sprintf(tmp, "Block 1-1 value is bot%d mid%d mid2%d hi%d");
   blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   clear(buffer);
   clear(bufferx);
  }
 }
 if (key[KEY_B])
 {
  clear_keybuf();
  tileid = 0;
  loopvar = 0;
  while(loopvar == 0)
  {
   //print text//
   //get input//
   if (key[KEY_N])
   {
    tileid--;
   }
   if (key[KEY_M])
   {
    tileid++;
   }
   if (key[KEY_LEFT])
   {
    if (tilex != 0)
    {
     tilex--;
     if (tilex < 0+scrollx)
     {
      scrollx--;
     }
    }
   }
   if (key[KEY_RIGHT])
   {
    if (tilex != 62)
    {
     tilex++;
     if (tilex > 30+scrollx)
     {
      scrollx++;
     }
    }
   }
   if (key[KEY_UP])
   {
    if (tiley > 0)
    {
     tiley--;
     if (tiley < 0+scrolly)
     {
      scrolly--;
     }
    }
   }
   if (key[KEY_DOWN])
   {
    if (tiley != 92)
    {
     tiley++;
     if (tiley >= 44+scrolly)
     {
      scrolly++;
     }
    }
   }
   if (key[KEY_ENTER])
   {
    blendbmp[tilex][tiley] = tileid;
   }
   if (key[KEY_C])
   {
    blendbmp[tilex][tiley] = 0;
   }
   if (key[KEY_Q])
   {
    loopvar = -3344;
   }
   renderbottom();
   rendermid();
   renderhi();

   blit(buffer, bufferx, 0+(scrollx*20), 0+(scrolly*10), 0, 0, 640, 480);  // copy it to the screen
   textout(bufferx, font, "-Blender Tile Insert-", 5, 5, 1850);
   textout(bufferx, font, "M/N cycles through tiles, Q to quit", 5, 13, 1850);   
   textout(bufferx, font, "ENTER places tile in location, C clears the location", 5, 21, 1850);   
   blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   clear(buffer);
   clear(bufferx);
  }
 } 
 if (key[KEY_E])
 {
  clear(screen);
  clear_keybuf();
  textout(screen, font, "Please enter which scene attribute you would like to edit:", 5, 5, 1850);
  textout(screen, font, "1) Monster Class Encountered", 5, 13, 1850);
  itmp = (readkey() >> 8) - 1;
  clear(screen);
  if (itmp == 1)
  {
   xcor = 0;
   clear_keybuf();
   while (xcor == 0)
   {
    if (key[KEY_UP])
    {
     tilex++;
     clear_keybuf();
    }
    if (key[KEY_DOWN])
    {
     tilex--;
     clear_keybuf();
    }
    if (key[KEY_ENTER])
    {
     mnster = tilex;
     xcor = 1;
    }
    if (key[KEY_C])
    { mnster = 0; }
    if (tilex == 0)
    {
     textout(screen, font, "No Monsters encountered in this area              ", 5, 20, 1850);
    }
    if (tilex == 1)
    {
     textout(screen, font, "Underwater--Blowfish,ShrimpCocktail,Eel,SnokkFish ", 5, 20, 1850);
    }
    if (tilex > 1)
    {
     textout(screen, font, "No Monsters encountered in this area              ", 5, 20, 1850);
    }
    textout(screen, font, "Press UP/DOWN to cycle through, C to clear, ENTER to select", 5, 5, 1850);
   }
  }
 } 
 if (key[KEY_K])
 {
  textout(bufferx, font, "Clearing TagMap Blocks", 5, 5, 1850);
  for (xcor=0;xcor<100;xcor++)
  {
   for (ycor=0;ycor<100;ycor++)
   {
    tagmap[xcor][ycor] = 0;
   }
  }
  for (ycor=0;ycor<20;ycor++)
  {
   tags[ycor].scnenum = 0;
   tags[ycor].pressa = 0;
   tags[ycor].item = 0;
   tags[ycor].scne[0] = 0;
   tags[ycor].scne[1] = 0;
   tags[ycor].patch[0] = 0;
   tags[ycor].patch[1] = 0;
   tags[ycor].patch[2] = 0;
   tags[ycor].scne[0] = 0;
   tags[ycor].scne[1] = 0;
   tags[ycor].overworld = 0;
   tags[ycor].actrig = 0; 
   tags[ycor].tagjmp = 0;
  }
 }
 if (key[KEY_S])
 {
  clear_keybuf();
  clear(screen);
  for (i=0;i<13;i++)
  {
   savefile[i] = 0;
  }
  textout(screen, font, "Save-Please enter a filename. Enter to finish.    ", 5, 5, 1850);
  textout(screen, font, "The suggested extension for a map file is .tpm", 5, 13, 1850);
  for (i=0;i<12;i++)
  {
   itmp = readkey();
   if ((itmp >> 8) == KEY_ENTER)
   {
    i = 13;
   }
   savefile[i] = itmp & 0xff;
   textout(screen, font, savefile, 5, 21, 1850);
  }
  savefile[13] = '\0'; //you need the end of string character
  textout(screen, font, savefile, 5, 21, 1850);
  textout(screen, font, "Are you sure?(y/n)", 5, 220, 1650);
  itmp = readkey();
  reader = itmp & 0xff;
  if (reader == 'y')
  {
   save();
   //textout(screen, font, "Saving...", 5, 220, 1650);
   //readkey();
  } 
 }
 if (key[KEY_L])
 {
  clear_keybuf();
  clear(screen);
  for (i=0;i<13;i++)
  {
   savefile[i] = 0;
  }
  textout(screen, font, "Load-Please enter a filename. Enter to finish.   ", 5, 5, 1850);
  textout(screen, font, "The suggested extension for a map file is .tpm", 5, 13, 1850);
  for (i=0;i<12;i++)
  {
   itmp = readkey();
   if ((itmp >> 8) == KEY_ENTER)
   {
    i = 13;
   }
   savefile[i] = itmp & 0xff;
   textout(screen, font, savefile, 5, 21, 1850);
  }
  savefile[13] = '\0'; //you need the end of string character
  textout(screen, font, savefile, 5, 21, 1850);
  textout(screen, font, "Are you sure?(y/n)", 300, 220, 1650);
  itmp = readkey();
  reader = itmp & 0xff;
  if (reader == 'y')
  {
   load();
   //textout(screen, font, "Loading...", 300, 220, 1650);
   //readkey();
  }
 }
 if (key[KEY_ESC])
 {
  looper = 3;
 }
}

void renderer(void)
{
 char text[80];
 renderbottom(); //render floor
 rendermid(); //render tiles below player
 renderhi(); //render tiles above player
 blitter();
 textout(bufferx, font, "S/L save/load, Escape to quit", 5, 5, 1850);
 textout(bufferx, font, "1. Insert a floor tile", 5, 13, 1850);
 textout(bufferx, font, "2. Insert a wall tile", 5, 21, 1850);
 textout(bufferx, font, "3. Insert a ceiling tile", 5, 29, 1850);
 textout(bufferx, font, "4. Insert a tag block", 5, 37, 1850);
 textout(bufferx, font, "5. Insert a collision detection block", 5, 46, 1850);
 textout(bufferx, font, "6. Edit Tag Attributes Mode", 5, 54, 1850);
 textout(bufferx, font, "7. Insert a sprite", 5, 62, 1850);
 textout(bufferx, font, "8. Edit Sprite Attributes Mode", 5, 70, 1850);
 textout(bufferx, font, "9. View tag/sprite Attributes", 5, 78, 1850);
 textout(bufferx, font, "C) Clear Collision Blocks", 5, 86, 1850);
 textout(bufferx, font, "K) Clear Tags-blocks and attributes", 5, 94, 1850);
 textout(bufferx, font, "V) Insert a wall add on (e.g., torch, door, etc..)", 5, 102, 1850);
 textout(bufferx, font, "B) Insert a blend map (e.g, Trees or hills)", 5, 110, 1850);
 textout(bufferx, font, "E) Edit Scene Attributes", 5, 110, 1850);
 sprintf(tmp, "Monster class encountered is %d", mnster);
 textout(bufferx, font, tmp, 5, 471, 1850);
 blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
}

void renderbottom(void)
{
 int xcor;
 int ycor;
 for (xcor=0;xcor<100;xcor++)
 {
  for (ycor=0;ycor<100;ycor++)
  {
   if (bottombmp[xcor][ycor] == 1)
   {
    stretch_sprite(buffer, floor1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (bottombmp[xcor][ycor] == 2)
   {
    stretch_sprite(buffer, floor2, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (bottombmp[xcor][ycor] == 3)
   {
    stretch_sprite(buffer, grass1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (bottombmp[xcor][ycor] == 4)
   {
    stretch_sprite(buffer, grass2, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (bottombmp[xcor][ycor] == 5)
   {
    stretch_sprite(buffer, grass3, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (bottombmp[xcor][ycor] == 6)
   {
    stretch_sprite(buffer, grass4, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (bottombmp[xcor][ycor] == 7)
   {
    stretch_sprite(buffer, stone1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (bottombmp[xcor][ycor] == 8)
   {
    stretch_sprite(buffer, higras1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (bottombmp[xcor][ycor] == 9)
   {
    stretch_sprite(buffer, watr1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (bottombmp[xcor][ycor] == 10)
   {
    stretch_sprite(buffer, watr2, xcor*20+1, ycor*10+1, 40, 40);
   }
  }
 }
}

void rendermid(void)
{
 int xcor;
 int ycor;
 for (xcor=0;xcor<100;xcor++)
 {
  for (ycor=0;ycor<100;ycor++)
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
  }
 }
 for (xcor=0;xcor<100;xcor++)
 {
  for (ycor=0;ycor<100;ycor++)
  {
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
 int ytile;
 char txtmp[3];
 for (xcor=0;xcor<100;xcor++)
 {
  for (ycor=0;ycor<100;ycor++)
  {
   if (hibmp[xcor][ycor] == 1)
   {
    stretch_sprite(buffer, top1, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (hibmp[xcor][ycor] == 2)
   {
    stretch_sprite(buffer, top2, xcor*20+1, ycor*10+1, 40, 40);
   }
   if (hibmp[xcor][ycor] == 3)
   {
    stretch_sprite(buffer, wall1, xcor*20+1, ycor*10+1, 40, 40);
   }
  }
 }
 for (xcor=0;xcor<100;xcor++)
 {
  for (ycor=0;ycor<100;ycor++)
  {
   if (collision[xcor][ycor] == 1)
   {
    stretch_sprite(buffer, colblock, xcor*20+1, ycor*10+1, 60, 60);
   }
  }
 }
 for (xcor=0;xcor<100;xcor++)
 {
  for (ycor=0;ycor<100;ycor++)
  {
   if (tagmap[xcor][ycor] != 0)
   {
    stretch_sprite(buffer, tagblock, xcor*20+1, ycor*10+1, 60, 60);
    sprintf(txtmp, "%d", tagmap[xcor][ycor]); 
    textout(buffer, font, txtmp, xcor*20+28, ycor*10+34, 1850);
   }
  }
 }
 rendersprite();
}

void blitter(void)
{
 //blit(buffer, bufferx, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 blit(buffer, bufferx, 0+(scrollx*20), 0+(scrolly*10), 0, 0, 640, 480);  // copy it to the screen
}

void load(void)
{
 //first step is to open the file//
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

void save(void)
{
 //first step is to open the file//
 level = fopen(savefile, "w");
 //next step is to scan in the info for the first map...//
 for (yslot=0;yslot<100;yslot++)
 {
  for (xslot=0;xslot<100;xslot++)
  {
  fwrite(&bottombmp[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<100;yslot++)
 {
  for (xslot=0;xslot<100;xslot++)
  {
  fwrite(&midbmp[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<100;yslot++)
 {
  for (xslot=0;xslot<100;xslot++)
  {
  fwrite(&mid2bmp[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<100;yslot++)
 {
  for (xslot=0;xslot<100;xslot++)
  {
  fwrite(&hibmp[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<100;yslot++)
 {
  for (xslot=0;xslot<100;xslot++)
  {
  fwrite(&collision[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<100;yslot++)
 {
  for (xslot=0;xslot<100;xslot++)
  {
  fwrite(&tagmap[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<20;yslot++)
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
 }
 for (yslot=0;yslot<10;yslot++)
 {
  fwrite(&sprites[yslot].xpos, sizeof(int), 1, level);
  fwrite(&sprites[yslot].ypos, sizeof(int), 1, level);
  fwrite(&sprites[yslot].zpos, sizeof(int), 1, level);
  fwrite(&sprites[yslot].bmpid, sizeof(int), 1, level);
  fwrite(&sprites[yslot].action, sizeof(int), 1, level);
  fwrite(&sprites[yslot].gp, sizeof(int), 1, level);
  fwrite(&sprites[yslot].actrig, sizeof(int), 1, level);
  fwrite(&sprites[yslot].immobile, sizeof(int), 1, level);
 }
 for (yslot=0;yslot<10;yslot++)
 {
  fwrite(&tags[yslot].tagjmp, sizeof(int), 1, level);
 }
 fwrite(&mnster, sizeof(int), 1, level);
 fclose(level);
}

void patchbmp(void)
{
  int tileid;
  int tilex;
  int tiley;
  clear_keybuf();
  tileid = 0;
  loopvar = 0;
  while(loopvar == 0)
  {
   if (key[KEY_N])
   {
    tileid--;
   }
   if (key[KEY_M])
   {
    tileid++;
   }
   if (key[KEY_LEFT])
   {
    if (tilex != 0)
    {
     tilex--;
     if (tilex < 0+scrollx)
     {
      scrollx--;
     }
    }
   }
   if (key[KEY_RIGHT])
   {
    if (tilex != 62)
    {
     tilex++;
     if (tilex > 30+scrollx)
     {
      scrollx++;
     }
    }
   }
   if (key[KEY_UP])
   {
    if (tiley > 0)
    {
     tiley--;
     if (tiley < 0+scrolly)
     {
      scrolly--;
     }
    }
   }
   if (key[KEY_DOWN])
   {
    if (tiley != 92)
    {
     tiley++;
     if (tiley >= 44+scrolly)
     {
      scrolly++;
     }
    }
   }
   if (key[KEY_ENTER])
   {
    tags[tagidp].patch[0] = tileid;
    tags[tagidp].patch[1] = tilex;
    tags[tagidp].patch[2] = tiley;
   }
   if (key[KEY_C])
   {
    tags[tagidp].patch[0] = 0;
    tags[tagidp].patch[1] = 0;
    tags[tagidp].patch[2] = 0;
   }
   if (key[KEY_Q])
   {
    loopvar = -3344;
   }
   renderbottom();
   rendermid();
   renderhi();

   if (tileid == 1)
   {
    stretch_sprite(buffer, chest2, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 2)
   {
    stretch_sprite(buffer, arch1, tilex*20+1, tiley*10+1, 40, 40);
   }
   if (tileid == 3)
   {
    stretch_sprite(buffer, door1, tilex*20+1, tiley*10+1, 40, 40);
   }
   blit(buffer, bufferx, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   textout(bufferx, font, "Patchbmp mode-when tag is trgged, this bmp is created   ", 5, 5, 1850);
   textout(bufferx, font, "m/n cycles through patches, enter to place, q to quit", 5, 13, 1850);   
   blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   clear(buffer);
   clear(bufferx);
  }
}


void rendersprite(void)
{
 int spritenum;
 char txtmp[2];
 //first thing to do is scan through all the sprites//
 for (spritenum=0;spritenum<10;spritenum++)
 {
  if (sprites[spritenum].bmpid == 1) //if it's the sprite # 1--> player
  {
   stretch_sprite(buffer, serf1, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   sprintf(txtmp, "%d", spritenum);
   textout(buffer, font, txtmp, sprites[spritenum].xpos*20+20, sprites[spritenum].ypos*10+20, 1850);
  }
  if (sprites[spritenum].bmpid == 2) //if it's the sprite # 2--> ninja guard
  {
   stretch_sprite(buffer, ninjaf1, sprites[spritenum].xpos*20, sprites[spritenum].ypos*10, 60, 60);
   sprintf(txtmp, "%d", spritenum);
   textout(buffer, font, txtmp, sprites[spritenum].xpos*20+20, sprites[spritenum].ypos*10+20, 1850);  }
 }
}

void loopxs(void)
{
 int loops = 0;
 loopvalue = 0;
 clear_keybuf();
 while(loops == 0)
 { 
  textout(bufferx, font, "                             ", 5, 312, 1850);   
  if (key[KEY_M])
  {
   loopvalue++;
   clear_keybuf();
  }
  if (key[KEY_N])
  {
   loopvalue--;
   clear_keybuf();
  }
  if (key[KEY_ENTER])
  {
   loops = 56;
  }
  sprintf(tmp, "Current number is %d", loopvalue);
  textout(bufferx, font, tmp, 5, 312, 1850);   
  blit(bufferx, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 }
}
