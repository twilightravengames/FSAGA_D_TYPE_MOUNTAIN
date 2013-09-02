/*
 * action_interface_exposed.cpp
 *
 *  Created on: Jul 9, 2012
 *      Author: waltergress
 *
 * This API acts as an interface between the main game application
 * and the parseable, scripting engine.
 *
 * For a refresher, script actions are each hard-coded tied to an id
 * i.e., "Intro board room script" might be tied to script number 3.
 *
 * When an action trigger with id 3 is triggered, the game automatically
 * runs the script it is tied to (in this case, "Intro board room script".
 *
 * "Running" the script means parsing the commands of the script and
 * calling the appropriate matching, functions in this, the interface layer,
 * with the parameters specified in the sccript.
 *
 */


//script command parsing


//exposed interface functions

//***************MISCELLANEOUS***************//


/* System_Delay_Pause(TimeOut) **/

//Causes the system to pause for a given amount of time


/* Call_Script(ScriptName) **/

//Call_Script passses control from this script on to another script
//parameter: ScriptName - filename for called script

//*****************MOVEMENT*******************//


/* MainChar_Move(DestX, DestY) **/

//MainCHar_Move causes the main character to move from his current location
//to DestX,DestY waypoint

/* NPC_Move(DestX, Dest Y) **/

//NPC_Move causes an npc (good or evil) to move from his current location
//to DestX, Dest Y waypoint


/* MainChar_Teleport(DestX, DestY) **/

//MainCHar_Teleport causes the main character to be instantly transported
//from his current location to DestX, DestY, without intermediate walking
//animations.

/* NPC_Teleport(DestX, DestY) **/

//MainChar_Teleport causes any npc to be instantly transported
//from his current location to DestX, DestYm without intermediate walking
//animations


//*********************ATTACK***************//

/* MainChar_Attack(TargetNPCId) **/

//MainChar_Attack causes the main character to deliver a normal
//attack at the TargetNPC(Id), moving towards the target npc by running
//if necessary

/* MainChar_Attack_Walk(TargetNPCId) **/

//MainChar_Attack_Walk performs the same action as MainCHar_Attack,
//but if necessary to move towards target, will walk casually instead
//of runnning

/* MainChar_Attack_Location(DestX, DestY) **/

//MainChar_Attack_Location will move to the location DestX DestY
//and deliver an attack at those coordinates.

/* NPC_Attack(SrcNPCId, TargetNPCId) **/

//NPC_Attack causes the character identified by SrcNPCId to travel to
//DestX, DestY and deliver an attack.

/* NPC_Attack_Walk(SrcNPCId, TargetNPCId) **/

//NPC_Attack_Walk performs the same action as NPC_Attack but if
//necessary to move towards opponent, will walk casually instead of run.

/* NPC_Attack_Location(SrcNPCId, DestX, DestY) **/

//NPC_Attack_Location will cause the npc with id SrcNPCId to move to
//location DestX, DestY and attack the spot noted by those coordinates.


//**************SPELLS (MP)*************//

/* MainChar_Spell(SpellNum, DestX, DestY) **/

// The maincharacter will cast the spell (spell num) at
// the DestX, DestY

/* NPCCHar_Spell(SrcNPCId, SpellNum, DestX, Desty) **/

//Causes the chosen NPC to cast spell SpellNum at DestX, DestY

//**************ABILITIES *(AP)*************//

/* MainChar_Ability(AbilityNum, TargetNPCId) **/

// Similar to spell casting, except the maincharacter
// will approach the enemy by running if not in range

/* MainChar_Ability_Walk(AbilityNum, TargetNPCId) **/

// LIke MainCharAbility except will walk to instead of
// running at enemy

/* MainChar_Ability_Target(AbilityNum, DestX, DestY) **/

// Main Character will attack at DestX, DestY using
// an ability. If out of range will run into range for attack.

/* MainChar_Ability_Walk_Target(AbilityNum, DestX, DestY) **/

// Like MainChar_Ability_Target except will casually approach
// target.

/* NPC_Ability(SrcNPCId, AbilityNum, TargetNPCId) **/

// Similar to spell casting, except the NPC
// will approach the enemy by running if not in range

/*  NPC_Ability_Walk(SrcNPCId, AbilityNum, TargetNPCId) **/

// LIke NPCAbility except will walk to instead of
// running at enemy

/* NPC_Ability_Target(SrcNPCId, AbilityNum, DestX, DestY) **/

// NPC will attack at DestX, DestY using
// an ability. If out of range will run into range for attack.

/* NPC_Ability_Walk_Target(SrcNPCId, AbilityNum, DestX, DestY) **/

// Like NPC_Ability_target except will casually approach
// target.



//******************ACTIONS**********************//

/* MainChar_PerformEnterAction(ActionID) **/

// Causes the main character to perform an 'enter' action
// these include, opening chests, opening doors, talking to people, etc.
// Anything that can be done with the enter key

/* NPC_PerformEnterAction(SrcNPCId, ActionID) **/

// Same function as MainChar_PerformEnterAction, except performed
// by an NPC of SrcNPCId


//**********************SPEECH*******************//

/* MainChar_Speak(Text[lines][pages]) **/

//Causes main character to say something

/* NPCCHar_Speak(SrcNPCId, Text[lines][pages] **/

//Causes a character with SrcNPCId to say something
