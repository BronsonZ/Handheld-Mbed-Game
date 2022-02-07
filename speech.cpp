#include "speech.h"

#include "globals.h"
#include "hardware.h"

/**
 * Draw the speech bubble background.
 */
//static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
//static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(char* line, int which);

/**
 * Delay until it is time to scroll.
 */
//static void speech_bubble_wait();

void draw_speech_bubble()
{
    uLCD.set_font(FONT_7X8);
    uLCD.color(BLACK);
    uLCD.textbackground_color(WHITE);
    uLCD.filled_rectangle(9, 75, 118, 115, WHITE);
    uLCD.rectangle(9, 75, 118, 115, RED);
    uLCD.rectangle(10, 76, 117, 114, RED);
    
}

void erase_speech_bubble()
{
    //uLCD.filled_rectangle(9, 75, 118, 115, OPAQUE);
    draw_game(true);
}

void draw_speech_line(char* line, int which)
{
    uLCD.set_font(FONT_7X8);
    if(which == TOP) uLCD.text_string(line, 3, 11, NULL , BLACK);
    if(which == BOTTOM) uLCD.text_string(line, 3, 12, NULL , BLACK);
}

void speech_bubble_wait()
{
    wait_ms(1500);
}

void speech(char* line1, char* line2)
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();
}

void end_speech(char* line1, char* line2)
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
}

void long_speech(const char* lines[], int n)
{
}