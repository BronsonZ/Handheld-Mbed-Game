#ifndef SPEECH_H
#define SPEECH_H

/**
 * Display a speech bubble.
 */
void speech(char* line1, char* line2);
void end_speech(char* line1, char* line2);

void draw_speech_bubble();

void erase_speech_bubble();

void speech_bubble_wait();

/**
 * Display a long speech bubble (more than 2 lines).
 * 
 * @param lines The actual lines of text to display
 * @param n The number of lines to display.
 */
void long_speech(const char* lines[], int n);

#endif // SPEECH_H