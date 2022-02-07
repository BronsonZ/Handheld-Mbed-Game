// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

#include <math.h>
#include<stdio.h>

// Helper function declarations
void playSound(char* wav);

// Top down camera view
struct {
    int x,y;    // Current locations
    int px, py;
    int ox, oy; // Previous locations
} Camera;

Character* movingPlayerData;
int numOfBad;
int numOfGood;
int modeNum;
int playerNum;

Character goodguy1 = {100, 60, 10, 1, 0, 0, 0};
Character goodguy2 = {100, 50 ,15, 1, 0, 0, 0};
Character goodguy3 = {100, 40 ,20, 1, 0, 0, 0};
Character badguy1 = {100, 60, 10, 2, 0, 0, 0};
Character badguy2 = {100, 50, 15, 2, 0, 0, 0};
Character badguy3 = {100, 40, 20, 2, 0, 0, 0};

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define BACK_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define PICKUP 7
#define MENU 8

int get_action(GameInputs inputs)
{
    if( !inputs.b1 ) return ACTION_BUTTON;
    if( !inputs.b2 ) return BACK_BUTTON;
    if( !inputs.b3 ) return PICKUP;
    if( !inputs.b4 ) return MENU;
    if( inputs.ax>=0.3 ) return GO_RIGHT;
    if( inputs.ax<=-0.3 ) return GO_LEFT;
    if( inputs.ay>=0.3 ) return GO_UP;
    if( inputs.ay<=-0.3 ) return GO_DOWN;
    return NO_ACTION;
}


/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2

#define MODE_FREE_ROAM 1
#define MODE_SELECTED 2
int update_game(int action)
{
    // Save player previous location before updating
    Camera.px = Camera.x;
    Camera.py = Camera.y;
    // Do different things based on the action
    // You can define smaller functions that get called for each case
    //pc.printf("Running switch %d \n", action);
if(modeNum == MODE_FREE_ROAM){
    switch(action) {
        case ACTION_BUTTON:{
            MapItem* item = get_here(Camera.x, Camera.y);
            Character* data = (Character*)(item->data);
            //If over player on correct team select that player for movement
            
            if(item->type != GOOD_C && item->type != BAD_C) return NO_RESULT;
            
            if(playerNum == 1 && item->type == GOOD_C){
                    printf("Has_moved = %d\n", data->has_moved);
                    if(data->has_moved == 1){
                        speech("Can't move", "fighter again");
                        return NO_RESULT;
                        }
                    modeNum = MODE_SELECTED;
                    pc.printf("Mode set to %d\n", modeNum);
                    
                    //Character* data = (Character*)(item->data);
                    int health = data->health;
                    pc.printf("Health = %d\n", health);
                    
                    Camera.ox = Camera.x;
                    Camera.oy = Camera.y;
                    movingPlayerData = (Character*)(item->data);
                    map_erase(Camera.x, Camera.y);
                    wait_ms(200);
                    return FULL_DRAW;
                }
            if(playerNum == 2 && item->type == BAD_C){
                    if(data->has_moved == 1){
                        speech("Can't move", "fighter again");
                        return NO_RESULT;
                        }                    
                    modeNum = MODE_SELECTED;
                    pc.printf("Mode set to %d\n", modeNum);
                    
                    //Character* data = (Character*)(item->data);
                    int health = data->health;
                    pc.printf("Health = %d\n", health);
                    
                    Camera.ox = Camera.x;
                    Camera.oy = Camera.y;
                    movingPlayerData = (Character*)(item->data);
                    map_erase(Camera.x, Camera.y);
                    wait_ms(200);
                    return FULL_DRAW;
                }
                if(playerNum == 1 && item->type == BAD_C){
                    speech("Wrong", "Character");
                    return NO_RESULT;
                    }
                if(playerNum == 2 && item->type == BAD_C){
                    speech("Wrong", "Character");
                    return NO_RESULT;
                    }
                break;
            }
        case BACK_BUTTON:
            //End the turn and switch player numbers
            playerNum = (playerNum == 1) ? 2 : 1;
            goodguy1.has_moved = 0;
            goodguy2.has_moved = 0;
            goodguy3.has_moved = 0;
            badguy1.has_moved = 0;
            badguy2.has_moved = 0;
            badguy3.has_moved = 0;
            pc.printf("Player Num = %d\n", playerNum);
            switch(playerNum){
                case 1:
                    speech("Player 1's", "Turn");
                    break;
                case 2:
                    speech("Player 2's", "Turn");
                    break;
                }
            break;
        case GO_UP:
            //If next item is the perimeter then dont move
            if(Camera.y != 1) Camera.y = Camera.y-1;
            break;
        case GO_LEFT:
            if(Camera.x != 1) Camera.x = Camera.x-1;
            break;
        case GO_DOWN:
            if(Camera.y != map_height()-2) Camera.y = Camera.y+1;
            break;                                                
        case GO_RIGHT:
            if(Camera.x != map_width()-2) Camera.x = Camera.x+1;
            break;
        case MENU:{
            MapItem* item = get_here(Camera.x, Camera.y);
            Character* data = (Character*)(item->data);
            if(item->type == GOOD_C || item->type == BAD_C){
                int health = data->health;
                int sword = data->has_sword;
                int attack = data->attack;
                int defense = data->defense;
                int bow = data->has_bow;
            
                draw_speech_bubble();
            
                uLCD.locate(3, 10);
                uLCD.printf("Health= %d", health);
            
                uLCD.locate(3, 11);
                uLCD.printf("Defense= %d", defense);
            
                uLCD.locate(3, 12);
                if(sword == 1){
                    if(bow == 1) uLCD.printf("Sword and Bow");
                    else uLCD.printf("Sword");
                     
                }
                else if(bow == 1) uLCD.printf("Bow");
                else uLCD.printf("No weapons");
            
                uLCD.locate(3, 13);
                uLCD.printf("Damage= %d", attack);
            
                speech_bubble_wait();
                wait_ms(500);
            
                erase_speech_bubble();  
            }
            else {
                draw_speech_bubble();
                uLCD.locate(2, 10);
                uLCD.printf("Player %ds turn", playerNum);
                uLCD.locate(2, 12);
                uLCD.printf("P1 fighters= %d", numOfGood);
                uLCD.locate(2, 13);
                uLCD.printf("P2 fighters= %d", numOfBad);
                speech_bubble_wait();
                wait_ms(1000);
                erase_speech_bubble();
            }  
        }
            break;
        default:
            break;        
    }
}
if(modeNum == MODE_SELECTED){
    //float square;
    int distance;
    int using_bow = 0;
    MapItem* badguy;
    Character* bData;
    Character* gData;
    MapItem* goodguy;
    switch(action){
        case ACTION_BUTTON:{
            //If already controlling player then drop that player at that location
            //If dropping player within 1 block of enemy player then attack
            
            if(get_here(Camera.x, Camera.y) != NULL) return NO_RESULT;
            
            if(playerNum == 1){
                add_good_character(Camera.x, Camera.y, movingPlayerData);
                Character* gData = movingPlayerData;
                gData->has_moved = 1;
                
                if(get_north(Camera.x, Camera.y)->type == BAD_C || (get_north(Camera.x, Camera.y-1)->type == BAD_C && gData->has_bow == 1 && get_north(Camera.x, Camera.y)==NULL)){
                    
                    if(get_north(Camera.x, Camera.y)->type == BAD_C){
                        
                    badguy = get_north(Camera.x, Camera.y);
                    bData = (Character*)badguy->data;
                    } 
                    else{
                        badguy = get_north(Camera.x, Camera.y-1);
                        bData = (Character*)badguy->data;
                        using_bow = 1;
                    }
                    int prevNumOfGood = numOfGood;
                    int prevNumOfBad = numOfBad;
                    int result = attack_player(gData, bData, using_bow);
                    if(result == GAME_OVER) return result;
                    if(numOfGood < prevNumOfGood){
                        map_erase(Camera.x, Camera.y);
                        movingPlayerData = NULL;
                        modeNum = MODE_FREE_ROAM;
                        pc.printf("Mode set to %d\n", modeNum);
                        return result;
                        }
                    if(numOfBad < prevNumOfBad){
                        map_erase(Camera.x, Camera.y-1);
                    }
                }
                if(get_south(Camera.x, Camera.y)->type == BAD_C || (get_south(Camera.x, Camera.y+1)->type == BAD_C && gData->has_bow == 1 && get_south(Camera.x, Camera.y)==NULL)){
                    
                    if(get_south(Camera.x, Camera.y)->type == BAD_C){
                        
                        badguy = get_south(Camera.x, Camera.y);
                        bData = (Character*)badguy->data;
                    } 
                    else{
                        
                        badguy = get_south(Camera.x, Camera.y+1);
                        bData = (Character*)badguy->data;
                        using_bow = 1;
                    }
                    
                    int prevNumOfGood = numOfGood;
                    int prevNumOfBad = numOfBad;
                    int result = attack_player(gData, bData, using_bow);
                    if(result == GAME_OVER) return result;
                    if(numOfGood < prevNumOfGood){
                        map_erase(Camera.x, Camera.y);
                        movingPlayerData = NULL;
                        modeNum = MODE_FREE_ROAM;
                        pc.printf("Mode set to %d\n", modeNum);
                        return result;
                    }
                    if(numOfBad < prevNumOfBad){
                        map_erase(Camera.x, Camera.y+1);
                    }
                }
                if(get_east(Camera.x, Camera.y)->type == BAD_C || (get_east(Camera.x+1, Camera.y)->type == BAD_C && gData->has_bow == 1 && get_east(Camera.x, Camera.y)==NULL)){
                    if(get_east(Camera.x, Camera.y)->type == BAD_C){
                        
                        badguy = get_east(Camera.x, Camera.y);
                        bData = (Character*)badguy->data;
                    } 
                    else{
                        
                        badguy = get_east(Camera.x+1, Camera.y);
                        bData = (Character*)badguy->data;
                        using_bow = 1;
                    }
                    int prevNumOfGood = numOfGood;
                    int prevNumOfBad = numOfBad;
                    int result = attack_player(gData, bData, using_bow);
                    if(result == GAME_OVER) return result;
                    if(numOfGood < prevNumOfGood){
                        map_erase(Camera.x, Camera.y);
                        movingPlayerData = NULL;
                        modeNum = MODE_FREE_ROAM;
                        pc.printf("Mode set to %d\n", modeNum);
                        return result;
                    }
                    if(numOfBad < prevNumOfBad){
                        map_erase(Camera.x+1, Camera.y);
                    }
                }
                if(get_west(Camera.x, Camera.y)->type == BAD_C || (get_west(Camera.x-1, Camera.y)->type == BAD_C && gData->has_bow == 1 && get_west(Camera.x, Camera.y)==NULL)){
                    if(get_west(Camera.x, Camera.y)->type == BAD_C){
                        badguy = get_west(Camera.x, Camera.y);
                        bData = (Character*)badguy->data;
                    } 
                    else{
                        badguy = get_west(Camera.x-1, Camera.y);
                        bData = (Character*)badguy->data;
                        using_bow = 1;
                    }
                    int prevNumOfGood = numOfGood;
                    int prevNumOfBad = numOfBad;
                    int result = attack_player(gData, bData, using_bow);
                    if(result == GAME_OVER) return result;
                    if(numOfGood < prevNumOfGood){
                        map_erase(Camera.x, Camera.y);
                        movingPlayerData = NULL;
                        modeNum = MODE_FREE_ROAM;
                        pc.printf("Mode set to %d\n", modeNum);
                        return result;
                    }
                    if(numOfBad < prevNumOfBad){
                        map_erase(Camera.x-1, Camera.y);
                    }
                }
                movingPlayerData = NULL;
                modeNum = MODE_FREE_ROAM;
                pc.printf("Mode set to %d\n", modeNum);
                return FULL_DRAW;
            }
            if(playerNum == 2){
                add_bad_character(Camera.x, Camera.y, movingPlayerData);
                bData = movingPlayerData;
                bData->has_moved = 1;
                if(get_north(Camera.x, Camera.y)->type == GOOD_C || (get_north(Camera.x, Camera.y-1)->type == GOOD_C && bData->has_bow == 1 && get_north(Camera.x, Camera.y)==NULL)){
                    
                    if(get_north(Camera.x, Camera.y)->type == GOOD_C){
                        goodguy = get_north(Camera.x, Camera.y);
                        gData = (Character*)goodguy->data;
                    } 
                    else{
                        goodguy = get_north(Camera.x, Camera.y-1);
                        gData = (Character*)goodguy->data;
                        using_bow = 1;
                    }
                    int prevNumOfGood = numOfGood;
                    int prevNumOfBad = numOfBad;
                    int result = attack_player(gData, bData, using_bow);
                    if(result == GAME_OVER) return result;
                    if(numOfBad < prevNumOfBad){
                        map_erase(Camera.x, Camera.y);
                        movingPlayerData = NULL;
                        modeNum = MODE_FREE_ROAM;
                        pc.printf("Mode set to %d\n", modeNum);
                        return result;
                    }
                    if(numOfGood < prevNumOfGood){
                        map_erase(Camera.x, Camera.y-1);
                    }
                }
                if(get_south(Camera.x, Camera.y)->type == GOOD_C || (get_south(Camera.x, Camera.y+1)->type == GOOD_C && bData->has_bow == 1 && get_south(Camera.x, Camera.y)==NULL)){
                    
                    if(get_south(Camera.x, Camera.y)->type == GOOD_C){
                        goodguy = get_south(Camera.x, Camera.y);
                        gData = (Character*)goodguy->data;
                    } 
                    else{
                        goodguy = get_south(Camera.x, Camera.y+1);
                        gData = (Character*)goodguy->data;
                        using_bow = 1;
                    }
                    int prevNumOfGood = numOfGood;
                    int prevNumOfBad = numOfBad;
                    int result = attack_player(gData, bData, using_bow);
                    if(result == GAME_OVER) return result;
                    if(numOfBad < prevNumOfBad){
                        map_erase(Camera.x, Camera.y);
                        movingPlayerData = NULL;
                        modeNum = MODE_FREE_ROAM;
                        pc.printf("Mode set to %d\n", modeNum);
                        return result;
                    }
                    if(numOfGood < prevNumOfGood){
                        map_erase(Camera.x, Camera.y+1);
                    }
                }
                if(get_east(Camera.x, Camera.y)->type == GOOD_C || (get_east(Camera.x+1, Camera.y)->type == GOOD_C && bData->has_bow == 1 && get_east(Camera.x, Camera.y)==NULL)){
                    if(get_east(Camera.x, Camera.y)->type == GOOD_C){
                        goodguy = get_east(Camera.x, Camera.y);
                        gData = (Character*)goodguy->data;
                    } 
                    else{
                        goodguy = get_east(Camera.x+1, Camera.y);
                        gData = (Character*)goodguy->data;
                        using_bow = 1;
                    }
                    int prevNumOfGood = numOfGood;
                    int prevNumOfBad = numOfBad;
                    int result = attack_player(gData, bData, using_bow);
                    if(result == GAME_OVER) return result;
                    if(numOfBad < prevNumOfBad){
                        map_erase(Camera.x, Camera.y);
                        movingPlayerData = NULL;
                        modeNum = MODE_FREE_ROAM;
                        pc.printf("Mode set to %d\n", modeNum);
                        return result;
                    }
                    if(numOfGood < prevNumOfGood){
                        map_erase(Camera.x+1, Camera.y);
                    }    
                }
                if(get_west(Camera.x, Camera.y)->type == GOOD_C || (get_west(Camera.x-1, Camera.y)->type == GOOD_C && bData->has_bow == 1 && get_west(Camera.x, Camera.y)==NULL)){
                    if(get_west(Camera.x, Camera.y)->type == GOOD_C){
                        goodguy = get_west(Camera.x, Camera.y);
                        gData = (Character*)goodguy->data;
                    } 
                    else{
                        goodguy = get_west(Camera.x-1, Camera.y);
                        gData = (Character*)goodguy->data;
                        using_bow = 1;
                    }
                    int prevNumOfGood = numOfGood;
                    int prevNumOfBad = numOfBad;
                    int result = attack_player(gData, bData, using_bow);
                    if(result == GAME_OVER) return result;
                    if(numOfBad < prevNumOfBad){
                        map_erase(Camera.x, Camera.y);
                        movingPlayerData = NULL;
                        modeNum = MODE_FREE_ROAM;
                        pc.printf("Mode set to %d\n", modeNum);
                        return result;
                    }
                    if(numOfGood < prevNumOfGood){
                        map_erase(Camera.x-1, Camera.y);
                    }
                }
            movingPlayerData = NULL;
            modeNum = MODE_FREE_ROAM;
            pc.printf("Mode set to %d\n", modeNum);
            return FULL_DRAW;
            }
        }
        case BACK_BUTTON:{
            //If not controlling player then end turn
            //If controlling player then deselect and put player in its preivous position
            
            modeNum = MODE_FREE_ROAM;
            pc.printf("Mode set to %d\n", modeNum);
            pc.printf("Player Num = %d\n", playerNum);
            
            if(movingPlayerData->team == 1){
                 add_good_character(Camera.ox, Camera.oy, movingPlayerData);
                 movingPlayerData = NULL;
                 return FULL_DRAW;
                 }
            if(movingPlayerData->team == 2) {
                add_bad_character(Camera.ox, Camera.oy, movingPlayerData);
                movingPlayerData = NULL;
                return FULL_DRAW;
                }
            break;
        }
        case GO_UP:{
            //If controlling character then move both the camera and the character
            //If next item is an unwalkable object dont move
            //square = (((Camera.ox)-(Camera.x)))*((Camera.ox)-(Camera.x))+(((Camera.oy)-(Camera.y-1))*((Camera.oy)-(Camera.y-1)));
            //distance = sqrt(square);
            
            distance = abs(Camera.x - Camera.ox)+abs((Camera.y-1) - Camera.oy);
            if(distance > 10) break; 
            
            if(get_north(Camera.x, Camera.y) == NULL){
                Camera.y = Camera.y-1;
                    break;
                }
            if(get_north(Camera.x, Camera.y)->walkable == true) Camera.y = Camera.y-1;
            break;
        }
        case GO_LEFT:{
            //square = (((Camera.ox)-(Camera.x-1)))*((Camera.ox)-(Camera.x-1))+(((Camera.oy)-(Camera.y))*((Camera.oy)-(Camera.y)));
            //distance = sqrt(square);
            
            distance = abs((Camera.x-1) - Camera.ox)+abs((Camera.y) - Camera.oy);
            if(distance > 10) break;
            
            if(get_west(Camera.x, Camera.y) == NULL){
                Camera.x = Camera.x-1;
                break;
                }
            if(get_west(Camera.x, Camera.y)->walkable == true) Camera.x = Camera.x-1;
            break;
        }
        case GO_DOWN:{
            //square = (((Camera.ox)-(Camera.x)))*((Camera.ox)-(Camera.x))+(((Camera.oy)-(Camera.y+1))*((Camera.oy)-(Camera.y+1)));
            //distance = sqrt(square);
            
            distance = abs(Camera.x - Camera.ox)+abs((Camera.y+1) - Camera.oy);
            if(distance > 10) break;
            
            if(get_south(Camera.x, Camera.y) == NULL){
                Camera.y = Camera.y+1;
                break;
                }
            if(get_south(Camera.x, Camera.y)->walkable == true) Camera.y = Camera.y+1;
            break;
        }                                              
        case GO_RIGHT:
            //square = (((Camera.ox)-(Camera.x+1)))*((Camera.ox)-(Camera.x+1))+(((Camera.oy)-(Camera.y))*((Camera.oy)-(Camera.y)));
            //distance = sqrt(square);
            
            distance = abs((Camera.x+1) - Camera.ox)+abs((Camera.y) - Camera.oy);
            if(distance > 10) break;
            
            if(get_east(Camera.x, Camera.y) == NULL){
                Camera.x = Camera.x+1;
                break;
                }
            if(get_east(Camera.x, Camera.y)->walkable == true) Camera.x = Camera.x+1;
            break;
        case PICKUP:{
            MapItem* item = get_here(Camera.x, Camera.y);
            if(item->type == APPLE){
                if(movingPlayerData->health <= 85){
                movingPlayerData->health = movingPlayerData->health + 15;
                speech("Gained 15 HP", "From Apple");
                map_erase(Camera.x, Camera.y);
                }else{
                    speech("Health not", "low enough");
                    }
                break;
            }
            if(item->type == SWORD ){
                if( movingPlayerData->has_sword == 1){
                    speech("Already have", "a sword.");
                    return NO_RESULT;
                    }
                movingPlayerData->attack = movingPlayerData->attack + 10;
                movingPlayerData->has_sword = 1;
                speech("Got Sword!", "+10 damage");
                map_erase(Camera.x, Camera.y);
                }
            if(item->type == BOW ){
                if( movingPlayerData->has_bow == 1){
                    speech("Already have", "a bow.");
                    return NO_RESULT;
                    }
                movingPlayerData->has_bow = 1;
                speech("Got Bow!", "+1 range");
                map_erase(Camera.x, Camera.y);
                }
            break;
        }
        case MENU:{
            int health = movingPlayerData->health;
            int sword = movingPlayerData->has_sword;
            int attack = movingPlayerData->attack;
            int defense = movingPlayerData->defense;
            int bow = movingPlayerData->has_bow;
            
            draw_speech_bubble();
            
            uLCD.locate(3, 10);
            uLCD.printf("Health= %d", health);
            
            uLCD.locate(3, 11);
            uLCD.printf("Defense= %d", defense);
            
            uLCD.locate(3, 12);
            if(sword == 1){
                if(bow == 0) uLCD.printf("Sword");
                else uLCD.printf("Sword and Bow");
                }
            else if(bow == 1) uLCD.printf("Bow");
            else uLCD.printf("No weapons");
            
            uLCD.locate(3, 13);
            uLCD.printf("Damage= %d", attack);
            
            speech_bubble_wait();
            wait_ms(500);
            
            erase_speech_bubble();  
        }
        default:
            break;        
    }
}

    return NO_RESULT;
}


int attack_player(Character* gData, Character* bData, int using_bow){
                    
                    int gHealth = gData->health;
                    int gAttack = gData->attack;
                    int gDefense = gData->defense;
                    
                    int bHealth = bData->health;
                    int bAttack = bData->attack;
                    int bDefense = bData->defense;
                   
                   
                if(playerNum == 1){ 
                    if(using_bow) speech("Player 1", "Bow Attack!");
                    else speech("Player 1", "Attacks!");
                    int damageDealt = ((gAttack - bDefense) > 0)? gAttack - bDefense : 1;
                    bData->health = bHealth - damageDealt;
                    if(bData->health <= 0){
                        speech("Player 2", "Defeated!");
                        numOfBad = numOfBad - 1;
                        if(numOfBad == 0) return GAME_OVER;
                        return FULL_DRAW;
                    }
                    if(using_bow && bData->has_bow == 0) return NO_RESULT;
                    speech("Player 2", "Attacks Back!");
                    damageDealt = ((bAttack - gDefense) > 0)? bAttack - gDefense : 1;
                    gData->health = gHealth - damageDealt;
                    if(gData->health <= 0){
                        speech("Player 1", "Defeated!");
                        numOfGood = numOfGood - 1;
                        if(numOfGood == 0) return GAME_OVER;
                        return FULL_DRAW;
                    }
                }
                if(playerNum == 2){
                    if(using_bow) speech("Player 2", "Bow Attack!");
                    else speech("Player 2", "Attacks!");
                    int damageDealt = ((bAttack - gDefense) > 0)? bAttack - gDefense : 1;
                    gData->health = gHealth - damageDealt;
                    if(gData->health <= 0){
                        speech("Player 1", "Defeated!");
                        numOfGood = numOfGood - 1;
                        if(numOfGood == 0) return GAME_OVER;
                        return FULL_DRAW;
                    }
                    if(using_bow && gData->has_bow == 0) return NO_RESULT;
                    speech("Player 1", "Attacks Back!");
                    damageDealt = ((gAttack - bDefense) > 0)? gAttack - bDefense : 1;
                    bData->health = bHealth - damageDealt;
                    if(bData->health <= 0){
                        speech("Player 2", "Defeated!");
                        numOfBad = numOfBad - 1;
                        if(numOfBad == 0) return GAME_OVER;
                        return FULL_DRAW;
                    }
                }
                    return NO_RESULT;;            
}




/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
 
#define NO_DRAW 0
#define DRAW 1
#define SAME_DRAW 2
#define DRAW_NOTHING 3
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();
    //if(modeNum == 2) init = true;

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Camera.x;
            int y = j + Camera.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Camera.px;
            int py = j + Camera.py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            

            // Figure out what to draw
            DrawFunc draw = NULL;
            int draw_line = NO_DRAW;
            int add_right = 0;
            int add_left = 0;
            int add_top = 0;
            int add_bottom = 0;
            
            if (i == 0 && j == 0) {
                // Decide what to draw at the 0 position
                if(modeNum == 1) draw = draw_selection;
                if(modeNum == 2){
                    switch(playerNum){
                        case 1:
                            draw = draw_player1sprite;
                            break;
                        case 2:
                            draw = draw_player2sprite;
                            break;
                        default:
                            break;
                    }
                //int distance = abs((x+1) - Camera.ox)+abs((y) - Camera.oy);
                //if(distance == 16) draw_right = draw_right_line;
                }
            } 
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map
                set_active_map(0);
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) { // Only draw if they're different
                    if (curr_item) { // There's something here! Draw it
                        draw = curr_item->draw;
                    } else { // There used to be something, but now there isn't
                        draw = draw_nothing;
                    } 
                }
                set_active_map(1);
                BarrierItem* bcurr_item = get_barrier_here(x, y);
                BarrierItem* bprev_item = get_barrier_here(px, py);
                
                if(modeNum == 2){
                    int distance = abs((x+1) - Camera.ox)+abs((y) - Camera.oy);
                    if(distance == 11 && Camera.ox <= x) add_right = 1;
                
                    distance = abs((x-1) - Camera.ox)+abs((y) - Camera.oy);
                    if(distance == 11 && Camera.ox >= x) add_left = 1;;
                
                    distance = abs((x) - Camera.ox)+abs((y+1) - Camera.oy);
                    if(distance == 11 && Camera.oy <= y) add_bottom = 1;
                
                    distance = abs((x) - Camera.ox)+abs((y-1) - Camera.oy);
                    if(distance == 11 && Camera.oy >= y) add_top = 1;
                
                    if(add_top==1 || add_bottom==1 || add_right==1 || add_left==1){
                        add_barrier(x,y);
                        bcurr_item = get_barrier_here(x, y);
                        bcurr_item->add_right = add_right;
                        bcurr_item->add_left = add_left;
                        bcurr_item->add_top = add_top;
                        bcurr_item->add_bottom = add_bottom;
                    }
                    else{
                        map_erase(x, y);
                        bcurr_item = NULL;
                    }
                } 
                else if(bcurr_item){
                     map_erase(x, y);
                     bcurr_item = NULL;
                     }
                
                if (bcurr_item != bprev_item) { // Only draw if they're different
                    if (bcurr_item) {
                        draw_line = DRAW;
                    } else {
                        if( curr_item ) draw_line= SAME_DRAW;
                        else draw_line = DRAW_NOTHING;
                    } 
                }
            } else if (init) { // If doing a full draw, but we're out of bounds, draw the walls.
                draw = draw_wall;
            }
            // Actually draw the tile
            if (draw) draw(u, v);
            //if (draw_line) 
            if (draw_line == DRAW && modeNum == 2){
                BarrierItem* bcurr_item = get_barrier_here(x, y);
                if(bcurr_item->add_right)draw_right_line(u, v);
                if(bcurr_item->add_left)draw_left_line(u, v);
                if(bcurr_item->add_top)draw_top_line(u, v);
                if(bcurr_item->add_bottom)draw_bottom_line(u, v);
                }
            else if(draw_line == SAME_DRAW) draw(u, v);
            else if(draw_line == DRAW_NOTHING) draw_nothing(u,v);
    }

    
            }
    // Draw status bars
    set_active_map(0);
    draw_upper_status();
    draw_lower_status();
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 39) {
        add_plant(i % map_width(), i / map_width());
    }
    pc.printf("plants done \r\n");
    
    int j = 0;
    for(int i = map_width(); i < map_area(); i += 45) {
         if (j){ 
                add_apple(i % map_width(), (i+10) / map_width());
                j = 0;
                }
        else j = 1;
    }
    //for(int i = map_width() + 3; i < map_area(); i += 233) {
    //     add_apple(i % map_width(), i / map_width());
    //}
    
    
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    add_wall(map_width()/2, map_height()/2, VERTICAL, map_height()/2);
    add_wall(50, 50, VERTICAL, map_height()/2);
    add_wall(50,              10,              VERTICAL, 10);
    add_wall(63, 25, HORIZONTAL, 75);
    pc.printf("Walls done!\r\n");
    
    
    add_bow(50, 21);
    add_sword(50, 23);
    
    add_sword(47, 30);
    add_sword(53, 30);
    
    add_bow(45, 25);
    add_bow(55, 25);
    
    
    add_good_character(39, 30, &goodguy1);
    add_good_character(40, 29, &goodguy2);
    add_good_character(41, 31, &goodguy3);
    add_bad_character(61, 30, &badguy1);
    add_bad_character(62, 29, &badguy2);
    add_bad_character(63, 31, &badguy3);
    
    
    print_map();
}


/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    

    pc.printf("Program Starting\n");
    
    playerNum = 1;
    modeNum = 1;
    pc.printf("Player Num = %d\n", playerNum);
    
    // Initialize the maps
    maps_init();
    init_main_map();
    
    
    Camera.x = Camera.px = 41;
    Camera.y = Camera.px = 27;
    

    // Initialize Characters
    
    numOfGood = 3;
    numOfBad = 3;
    
    // Initialize game state
    set_active_map(0);

    // Initial drawing
    draw_game(true);
    
    speech("Welcome To", "Block Arena!");
    speech("Player 1's", "Turn");
    
    // Main game loop
    while(1) {
        // Timer to measure game update speed
        Timer t;
        t.start();
        
        // 1. Read inputs
        GameInputs inputs = read_inputs();
        // 2. Determine action (move, act, menu, etc.)
        int action = get_action(inputs);
        // 3. Update game
        int update = update_game(action);
        // 3b. Check for game over
        if(update == GAME_OVER){
            draw_game(true);
            if(numOfGood == 0){
                end_speech("Player 2", "Wins!");
            }
            if(numOfBad == 0){
                end_speech("Player 1", "Wins!");
            }
            break;
        }
        
        // 4. Draw screen
        draw_game(update);
         //pc.printf("Get Here = %d\n", get_here(Camera.x, Camera.y));\
         
         
         
         
         //wait_ms(100);

        // Compute update time
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
}

