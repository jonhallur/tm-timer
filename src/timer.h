#pragma once
#include <pebble.h>

typedef enum {
  PHASE_WARMUP, 
  PHASE_MEDITATE, 
  PHASE_COOLDOWN, 
  PHASE_FINISHED
} MeditationPhase;

typedef struct MeditationTimer {
  MeditationPhase current_phase;
  int phase_lengths[4];
  char phase_names[4][10];
  int current_countdown;
} MeditationTimer;

void initializeTimer(int warmup_length, int meditate_length, int cooldown_length);
void resetTimer(int warmup_length, int meditate_length, int cooldown_length);
int getCurrentPhaseLength();
char* getCurrentPhaseName();
void advanceTimerPhase();
int getCurrentPhaseRemainingTime();
void onSecondTick(struct tm *tick_time, TimeUnits units_changed);
bool timerIsRunning();
void turnTimerOn();
void turnTimerOff();
void updateTimerText();
void updatePhaseText();
void updateStatusBar();
void setTimerChangeCallback(void(*timerTextCallback)(void));
void setPhaseChangeCallback(void(*phaseTextCallback)(void));