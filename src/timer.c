#include <pebble.h>
#include "timer.h"

static struct MeditationTimer myTimer;
static bool timer_is_running = false;

static void (*timer_change_callback)(void);
static void (*phase_change_callback)(void);

void initializeTimer(int warmup, int meditate, int cooldown) {
  resetTimer(warmup, meditate, cooldown);
}

void resetTimer(int warmup, int meditate, int cooldown) {
  myTimer.phase_lengths[PHASE_WARMUP] = warmup;
  myTimer.phase_lengths[PHASE_MEDITATE] = meditate;
  myTimer.phase_lengths[PHASE_COOLDOWN] = cooldown;
  myTimer.phase_lengths[PHASE_FINISHED] = 0;
  
  strcpy(myTimer.phase_names[PHASE_WARMUP], "Warmup\0");
  strcpy(myTimer.phase_names[PHASE_MEDITATE], "Meditate\0");
  strcpy(myTimer.phase_names[PHASE_COOLDOWN], "Cooldown\0");
  strcpy(myTimer.phase_names[PHASE_FINISHED], "Finished\0");
  
  myTimer.current_phase = PHASE_WARMUP;
  myTimer.current_countdown = getCurrentPhaseLength();
  
  updateTimerText();
  updatePhaseText();
}

int getCurrentPhaseLength() {
  return myTimer.phase_lengths[myTimer.current_phase];
}

char* getCurrentPhaseName() {
  return myTimer.phase_names[myTimer.current_phase];
}

int getCurrentPhaseRemainingTime() {
  return myTimer.current_countdown;
}

void advanceTimerPhase() {
  switch (myTimer.current_phase) {
    case PHASE_WARMUP:
      vibes_short_pulse();
      myTimer.current_phase = PHASE_MEDITATE;
      break;
    case PHASE_MEDITATE:
      vibes_long_pulse();
      myTimer.current_phase = PHASE_COOLDOWN;
      break;
    case PHASE_COOLDOWN:
      vibes_short_pulse();
      myTimer.current_phase = PHASE_FINISHED;
      break;
    case PHASE_FINISHED:
      break;
  }
  myTimer.current_countdown = getCurrentPhaseLength();
}

void onSecondTick(struct tm *tick_time, TimeUnits units_changed) {
  if (myTimer.current_phase == PHASE_FINISHED) {
    return;
  }
  myTimer.current_countdown -= 1;
  if (myTimer.current_countdown <= 0) {
    advanceTimerPhase();
    updatePhaseText();
  }
  updateTimerText();
}

bool timerIsRunning() {
  return timer_is_running;
}

void turnTimerOn() {
  tick_timer_service_subscribe(SECOND_UNIT, onSecondTick);
  timer_is_running = true;
}

void turnTimerOff() {
  tick_timer_service_unsubscribe();
  timer_is_running = false;
}

void updateTimerText() {
  if (timer_change_callback) {
    timer_change_callback();
  }
}

void updatePhaseText() {
  if (phase_change_callback) {
    phase_change_callback();
  }
}

void setTimerChangeCallback(void (*timerChangeCallback)(void)) {
  timer_change_callback = timerChangeCallback;
}

void setPhaseChangeCallback(void (*phaseChangeCallback)(void)) {
  phase_change_callback = phaseChangeCallback;
}