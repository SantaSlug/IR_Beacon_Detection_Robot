#include "speaker.h"
#include <DFRobotDFPlayerMini.h>

const int MAX_SIZE = 50;
int queue_folder[MAX_SIZE];
int queue_track[MAX_SIZE];
int currentSize = 0;

const int default_num = 9;
const int shoot_num = 4;
const int reload_num = 4;
const int reloaded_num = 3;
const int folder_count[] = {0, 3, default_num, shoot_num, reload_num, reloaded_num}; // Folder 0 is unused

#define FPSerial Serial3 // RX3=15, TX3=14
const int make_noise = true;
int volume = 23;

DFRobotDFPlayerMini myDFPlayer;

// State variables for managing playback
bool is_playing_queue = false;
bool waiting_for_delay = false;
unsigned long default_delay_start = 0;

bool add_to_queue(int folder, int track) {
    if (make_noise){
        if (currentSize >= MAX_SIZE) {
            return false;
        }
        queue_folder[currentSize] = folder;
        queue_track[currentSize] = track;
        currentSize++;
        return true;
    }
    return false;
}

bool play_next(int folder, int track, int position = 1) {
    if (make_noise){
        if (currentSize >= MAX_SIZE || position < 0 || position > currentSize) {
            return false;
        }
        for (int i = currentSize; i > position; i--) {
            queue_folder[i] = queue_folder[i - 1];
            queue_track[i] = queue_track[i - 1];
        }
        queue_folder[position] = folder;
        queue_track[position] = track;
        currentSize++;
        return true;
    }
    return false;
}

bool remove_elem(int position = 0) {
    if (make_noise){
    if (currentSize == 0 || position < 0 || position >= currentSize) {
        return false;
    }
    for (int i = position; i < currentSize - 1; i++) {
        queue_folder[i] = queue_folder[i + 1];
        queue_track[i] = queue_track[i + 1];
    }
    currentSize--;
    return true;
    }
    return false;
}

void clear() {
    if (make_noise){
    currentSize = 0;
    myDFPlayer.stop(); // Stop any ongoing playback immediately
    waiting_for_delay = false; // Cancel any pending delay
    is_playing_queue = false; // Reset state
    }
}

void play_now(int folder, int track) {
    if (make_noise){
        Serial.println("Test Sound");
        clear(); // Stop current track, clear queue, reset states
        add_to_queue(folder, track);
        // Immediately play the new track and mark it as part of the queue
        myDFPlayer.playFolder(folder, track);
        is_playing_queue = true;
    }
}

int get_rnd(int folder) {
  int track = random(1, folder_count[folder] + 1);
  return track;
}

void run_speaker() {
    if (make_noise){
        if (myDFPlayer.available()) {
            uint8_t type = myDFPlayer.readType();
            int value = myDFPlayer.read();

            if (type == DFPlayerPlayFinished) {
                if (is_playing_queue) {
                    remove_elem(0);
                    is_playing_queue = false;
                }

                if (currentSize > 0) {
                    myDFPlayer.playFolder(queue_folder[0], queue_track[0]);
                    is_playing_queue = true;
                } else {
                    waiting_for_delay = true;
                    
                    default_delay_start = millis();
                }
            }
        }

        if (waiting_for_delay && (millis() - default_delay_start >= 3000)) {
            if (currentSize == 0) {
                myDFPlayer.playFolder(2, get_rnd(2));
                // Default tracks are not part of the queue
                is_playing_queue = false;
                waiting_for_delay = false;
            }
        }
    }
}

void setup_speaker(){
    if (make_noise){
        randomSeed(analogRead(0)); // Initialize random seed for varied playback
        FPSerial.begin(9600);
        delay(100);
        if (!myDFPlayer.begin(FPSerial, true, true)) {  
        Serial.println("Speaker not Connected");
            while(true); // Halt if DFPlayer fails to initialize
        }
        Serial.println("Speaker Connected");
        myDFPlayer.volume(volume);
    }
}