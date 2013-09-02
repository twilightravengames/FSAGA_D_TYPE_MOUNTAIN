#include <conio.h>
#include <stdio.h>
#include <allegro.h>


BITMAP *map1;
BITMAP *buffer;
BITMAP *colblock;
BITMAP *tagblock;
FILE *level;

PALETTE the_pallete;

int i;

void init(void);
void editorloop(void);
void input(void);
void render(void);
void load(void);
void save(void);
void loopxs(void);
void renderblocks(void);

typedef struct tag{
int scnenum; //scene to jump to (#)
int pressa;
int scne[2]; //scne x,y start point after the jump
int overworld; //jump into overworld #
int actrig; //action/event trigger number
int tagjmp;
}tag;

tag tags[20];
int itmp;

char savefile[80]; //savefile name
char filename[80]; //filename
char tmp[80];
int tagmap[100][100]; //locations of  ---> their id's
int collision[100][100]; //collision table for scene
int mapt;

int loopvalue;
int looper;
int dummy;
char *argv[];

main()
{
 printf("Overworld Map Editor for FSAFA\n");
 printf("press any key...\n");
 getch();
 init();
 editorloop();
}

void init(void)
{
 allegro_init();
 install_keyboard();
 set_color_depth(16);
 set_color_conversion(COLORCONV_TOTAL);
 buffer = create_bitmap(640, 480);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "colblock.bmp");
 colblock = load_bmp(filename, the_pallete);
 strcpy(filename, argv[0]);
 strcpy(get_filename(filename), "tagblock.bmp");
 tagblock = load_bmp(filename, the_pallete);
 set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
 clear(screen);
}

void editorloop(void)
{
 while(looper == 0)
 {
  input();
  render();
 }
}

void input(void)
{
 int tagnum = 0;
 int wait = 0;
 int tilex = 0;
 int tiley = 0;
 int tileid = 0;
 int xcor = 0;
 int ycor = 0;
 int loopvar = 0;
 char reader;
 if (key[KEY_1])
 {
  clear(screen);
  clear(buffer);
  clear_keybuf();
  while(wait == 0)
  {
   blit(map1, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
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
    if (tilex == 1)
    {
     strcpy(savefile, argv[0]);
     strcpy(get_filename(savefile), "over1.bmp");
     map1 = load_bmp(savefile, the_pallete);
     mapt = tilex;
    }
    wait = 1;
   }
   textout(buffer, font, "Insert background Image #?", 5, 5, 1850);
   if (tilex == 0)
   { textout(buffer, font, "                    ", 5, 15, 1850); }
   if (tilex == 1)
   { textout(buffer, font, "Domon Island", 5, 15, 1850); }
   if (tilex > 1)
   { textout(buffer, font, "                   ", 5, 15, 1850); }
   blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   clear(buffer);
  }
 }
 if (key[KEY_2])
 {
  clear_keybuf();
  xcor = 1;
  ycor = 1;
  loopvar = 0;
  while(loopvar == 0)
  {
   blit(map1, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   if (key[KEY_LEFT])
   {
    tilex--;
   }
   if (key[KEY_RIGHT])
   {
    tilex++;
   }
   if (key[KEY_UP])
   {
    tiley--;
   }
   if (key[KEY_DOWN])
   {
    tiley++;
   }
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
   if (key[KEY_C])
   {
    tagmap[tilex][tiley] = 0;
   }
   if (key[KEY_ENTER])
   {
    tagmap[tilex][tiley] = tileid;
   }
   if (key[KEY_Q])
   {
    loopvar = 3;
   }
   stretch_sprite(buffer, tagblock, tilex*10+1, tiley*5+1, 30, 30);
   renderblocks();
   textout(buffer, font, "Tag Block Mode", 5, 5, 1850);
   textout(buffer, font, "Tag blocks are used to trigger events,etc.", 5, 13, 1850);
   textout(buffer, font, "(ARROWKEYS) Move block (C)lear Block (ENTER) Place Block", 5, 21, 1850);
   textout(buffer, font, "(Q)uit", 5, 29, 1850);
   sprintf(tmp, "Current Tag Id Num: %d", tileid);
   textout(buffer, font, tmp, 5, 470, 1850);
   blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   clear(buffer);
  }
 }  
 if (key[KEY_3])
 {
  clear_keybuf();
  clear(screen);
  clear(buffer);
  textout(buffer, font, "-Tag Edit Mode-                                     ", 5, 5, 1850);
  textout(buffer, font, "Please enter which tag # you would like to edit:    ", 5, 13, 1850);
  loopxs();
  tagnum = loopvalue;
  sprintf(tmp, "Editing tag number %d", tagnum);
  textout(screen, font, tmp, 5, 632, 1850);
  textout(screen, font, "What attribute would you like to edit?          ", 5, 5, 1850);
  textout(screen, font, "1) Scene Number to jump to.                     ", 5, 13, 1850);
  textout(screen, font, "2) Press enter to activate?                     ", 5, 21, 1850);
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
  if (itmp == 6)
  {
   tileid = 0;
   xcor = 1;
   ycor = 1;
   loopvar = 0;
   while(loopvar == 0)
   {
    blit(map1, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
    if (key[KEY_LEFT])
    {
      tilex--;
    }
    if (key[KEY_RIGHT])
    {
      tilex++;
    }
    if (key[KEY_UP])
    {
      tiley--;
    }  
    if (key[KEY_DOWN])
    {
      tiley++;
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
    renderblocks();
    stretch_sprite(buffer, colblock, tilex*20+1, tiley*10+1, 60, 60);
    //show current location of the start point//
    textout(buffer, font, "Use arrow key to select entrance position, enter to select", 5, 5, 1850);
    sprintf(tmp, "Xcor: %d", tilex);
    textout(buffer, font, tmp, 5, 200, 1850);
    sprintf(tmp, "Ycor: %d", tiley);
    textout(buffer, font, tmp, 5, 209, 1850);
    blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
    clear(buffer);
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
   clear_keybuf();
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
 if (key[KEY_4])
 {
  clear_keybuf();
  xcor = 1;
  ycor = 1;
  loopvar = 0;
  while(loopvar == 0)
  {
   blit(map1, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   if (key[KEY_LEFT])
   {
    tilex--;
   }
   if (key[KEY_RIGHT])
   {
    tilex++;
   }
   if (key[KEY_UP])
   {
    tiley--;
   }
   if (key[KEY_DOWN])
   {
    tiley++;
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
   stretch_sprite(buffer, colblock, tilex*10+1, tiley*5+1, 30, 30);
   renderblocks();
   textout(buffer, font, "Collision Block Mode", 5, 5, 1850);
   textout(buffer, font, "Collision blocks are used to trigger events,etc.", 5, 13, 1850);
   textout(buffer, font, "(ARROWKEYS) Move block (C)lear Block (ENTER) Place Block", 5, 21, 1850);
   textout(buffer, font, "(Q)uit", 5, 29, 1850);
   blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
   clear(buffer);
  }
 }  
 if (key[KEY_5])
 {
  clear(screen);
  clear_keybuf();
  textout(screen, font, "Enter tag number to view:", 5, 5, 1850);
  itmp = (readkey() >> 8) - 1;
  clear_keybuf();
  sprintf(tmp, "Tag number %d", itmp);
  textout(screen, font, tmp, 5, 29, 1850);
  sprintf(tmp, "On activate, jump to scene %d", tags[itmp].scnenum);
  textout(screen, font, tmp, 5, 37, 1850);
  sprintf(tmp, "Press a to activate- 0=false,1=true. value is %d", tags[itmp].pressa);
  textout(screen, font, tmp, 5, 45, 1850);
  sprintf(tmp, "Scene x and y jump to are %d,%d", tags[itmp].scne[0], tags[itmp].scne[1]);
  textout(screen, font, tmp, 5, 53, 1850);
  sprintf(tmp, "On activate, jump to overworld %d", tags[itmp].overworld);
  textout(screen, font, tmp, 5, 61, 1850);
  sprintf(tmp, "On activate, in next scene jump to tag # %d", tags[itmp].tagjmp);
  textout(screen, font, tmp, 5, 69, 1850);
  sprintf(tmp, "Map Background Type %d", mapt);
  textout(screen, font, tmp, 5, 78, 1850);
  readkey();
 }
 if (key[KEY_ESC])
 {
  looper = 3333;
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
  textout(screen, font, "The suggested extension for an overworld file is .ovw", 5, 13, 1850);
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
  }
 clear_keybuf();
 }
 if (key[KEY_S])
 {
  clear_keybuf();
  clear(screen);
  for (i=0;i<13;i++)
  {
   savefile[i] = 0;
  }
  textout(screen, font, "Save-Please enter a filename. Enter to finish.   ", 5, 5, 1850);
  textout(screen, font, "The suggested extension for an overworld file is .ovw", 5, 13, 1850);
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
   save();
  }
 clear_keybuf();
 }
}

void render(void)
{
 blit(map1, buffer, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 renderblocks();
 textout(buffer, font, "  Overworld Editor for Fantasy Saga M", 5, 5, 1850);
 textout(buffer, font, "----------------------------------------", 5, 13, 1850);
 textout(buffer, font, "1) Insert a backgrnd bitmap", 5, 23, 1850);
 textout(buffer, font, "2) Insert a tag block", 5, 33, 1850);
 textout(buffer, font, "3) Edit a tag block's attributes", 5, 43, 1850);
 textout(buffer, font, "4) Insert a collision block", 5, 53, 1850);
 textout(buffer, font, "5) View tag block attributes", 5, 63, 1850);
 textout(buffer, font, "S) Save an overworld map", 5, 73, 1850);
 textout(buffer, font, "L) Load an overworld map", 5, 83, 1850);
 sprintf(tmp, "Map type %d", mapt);
 textout(buffer, font, tmp, 5, 400, 1850);
 blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 clear(buffer);
}


void renderblocks(void)
{
 int xcor;
 int ycor;
 char txtmp[80];
 for (xcor=0;xcor<100;xcor++)
 {
  for (ycor=0;ycor<100;ycor++)
  {
   if (tagmap[xcor][ycor] != 0)
   {
    stretch_sprite(buffer, tagblock, xcor*10+1, ycor*5+1, 30, 30);
    sprintf(txtmp, "%d", tagmap[xcor][ycor]); 
    textout(buffer, font, txtmp, xcor*10+14, ycor*5+17, 1850);
   }
   if (collision[xcor][ycor] != 0)
   {
    stretch_sprite(buffer, colblock, xcor*10+1, ycor*5+1, 30, 30);
   }
  }
 }
}

void loopxs(void)
{
 int loops = 0;
 loopvalue = 0;
 clear_keybuf();
 while(loops == 0)
 { 
  textout(buffer, font, "                             ", 5, 312, 1850);   
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
  textout(buffer, font, tmp, 5, 312, 1850);   
  blit(buffer, screen, 0, 0, 0, 0, 640, 480);  // copy it to the screen
 }
}

void load(void)
{
 int yvalue;
 int yslot;
 int xslot;
 level = fopen(savefile, "r");
 for (xslot=0;xslot<100;xslot++)
 {
  for (yslot=0;yslot<100;yslot++)
  {
   fread(&collision[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (xslot=0;xslot<100;xslot++)
 {
  for (yslot=0;yslot<100;yslot++)
  {
   fread(&tagmap[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<100;yslot++)
 {
  fread(&tags[yslot].scnenum, sizeof(int), 1, level);
  fread(&tags[yslot].pressa, sizeof(int), 1, level);
  fread(&tags[yslot].scne[0], sizeof(int), 1, level);
  fread(&tags[yslot].scne[1], sizeof(int), 1, level);
  fread(&tags[yslot].overworld, sizeof(int), 1, level);
  fread(&tags[yslot].actrig, sizeof(int), 1, level);
  fread(&tags[yslot].tagjmp, sizeof(int), 1, level);
 }
 fread(&mapt, sizeof(int), 1, level);
 fclose(level);
 if (mapt == 1)
 {
  strcpy(savefile, argv[0]);
  strcpy(get_filename(savefile), "over1.bmp");
  map1 = load_bmp(savefile, the_pallete);
 }
}

void save(void)
{
 int yvalue;
 int yslot;
 int xslot;
 level = fopen(savefile, "w");
 for (xslot=0;xslot<100;xslot++)
 {
  for (yslot=0;yslot<100;yslot++)
  {
   fwrite(&collision[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (xslot=0;xslot<100;xslot++)
 {
  for (yslot=0;yslot<100;yslot++)
  {
   fwrite(&tagmap[xslot][yslot], sizeof(int), 1, level);
  }
 }
 for (yslot=0;yslot<100;yslot++)
 {
  fwrite(&tags[yslot].scnenum, sizeof(int), 1, level);
  fwrite(&tags[yslot].pressa, sizeof(int), 1, level);
  fwrite(&tags[yslot].scne[0], sizeof(int), 1, level);
  fwrite(&tags[yslot].scne[1], sizeof(int), 1, level);
  fwrite(&tags[yslot].overworld, sizeof(int), 1, level);
  fwrite(&tags[yslot].actrig, sizeof(int), 1, level);
  fwrite(&tags[yslot].tagjmp, sizeof(int), 1, level);
 }
 fwrite(&mapt, sizeof(int), 1, level);
 fclose(level);
}
