#ifndef COMBATFILESTRUCTURES_HEADER
#define COMBATFILESTRUCTURES_HEADER


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

#endif