#include "game_play.h"
#include "speaker.h"


const unsigned long time_played = 150UL * 1000UL;
unsigned long start_game_time=0;

// Implement setup_launcher()
void setup_game_play(){
    pinMode(play_game_pin, INPUT_PULLUP);
}

bool is_playing_game(bool prev_playing){
    int playing_btn = !digitalRead(play_game_pin);

    if (playing_btn){
        start_game_time = millis();
        play_now(1, 3);
        
        return true;
    }

    if (millis() > (start_game_time + time_played)){
        return false;
    }

    return prev_playing;
}

