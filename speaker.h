#ifndef SPEAKER_H
#define SPEAKER_H

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "PinConfig.h"

// Declare functions
bool add_to_queue(int folder, int track);
bool play_next(int folder, int track, int position = 1);
bool remove_elem(int position = 0);
void clear();
void play_now(int folder, int track);
int get_rnd(int folder);
void run_speaker();
void setup_speaker();
#endif