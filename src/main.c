#include <pebble.h>
#include "main.h"
#include "timer.h"
#include "status_bar.h"

void updateTimerTextLayer(int current_countdown);
void updateStatusBarLayer(int current, int full);
static GBitmap *s_res_image_action_icon_pause;
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_action_icon_menu;
static GBitmap *s_res_image_action_icon_play;
static GBitmap *s_res_image_action_icon_delete;
static GFont s_res_roboto_condensed_21;
static GFont s_res_gothic_18;
static GFont s_res_bitham_42_medium_numbers;
static ActionBarLayer *s_actionbarlayer_1;
static TextLayer *s_header_text_layer;
static TextLayer *s_phase_text_layer;
static TextLayer *s_timer_text_layer;
static Layer *s_status_bar_layer;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, false);
  
  s_res_image_action_icon_menu = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_ICON_MENU);
  s_res_image_action_icon_play = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_ICON_PLAY);
  s_res_image_action_icon_delete = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_ICON_DELETE);
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  s_res_gothic_18 = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorBlack);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_image_action_icon_menu);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_image_action_icon_play);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_image_action_icon_delete);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
  
  // s_header_text_layer
  s_header_text_layer = text_layer_create(GRect(2, 2, 119, 27));
  text_layer_set_text(s_header_text_layer, "TM Timer");
  text_layer_set_text_alignment(s_header_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_header_text_layer, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_header_text_layer);
  
  // s_phase_text_layer
  s_phase_text_layer = text_layer_create(GRect(2, 29, 119, 23));
  text_layer_set_text(s_phase_text_layer, "warmup");
  text_layer_set_text_alignment(s_phase_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_phase_text_layer, s_res_gothic_18);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_phase_text_layer);
  
  // s_timer_text_layer
  s_timer_text_layer = text_layer_create(GRect(0, 52, 124, 48));
  text_layer_set_text(s_timer_text_layer, "00:00");
  text_layer_set_text_alignment(s_timer_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_timer_text_layer, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_timer_text_layer);
  
  // s_status_bar_layer
  s_status_bar_layer = layer_create(GRect(-1, 113, 124, 27));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_status_bar_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(s_actionbarlayer_1);
  text_layer_destroy(s_header_text_layer);
  text_layer_destroy(s_phase_text_layer);
  text_layer_destroy(s_timer_text_layer);
  layer_destroy(s_status_bar_layer);
  gbitmap_destroy(s_res_image_action_icon_menu);
  gbitmap_destroy(s_res_image_action_icon_play);
  gbitmap_destroy(s_res_image_action_icon_delete);
}
// END AUTO-GENERATED UI CODE

void timer_change_handler() {
  int current_countdown= getCurrentPhaseRemainingTime();
  int current_phase_length = getCurrentPhaseLength();
  updateTimerTextLayer(current_countdown);
  updateStatusBarLayer(current_countdown, current_phase_length);
}
void phase_change_handler() {
  static char phase_text[10];
  strcpy(phase_text, getCurrentPhaseName());
  text_layer_set_text(s_phase_text_layer, phase_text);
}

void updateTimerTextLayer(int current_countdown) {
  static char timer_text[10];
  int minutes = current_countdown / SECONDS_IN_MINUTE;
  int seconds = current_countdown % SECONDS_IN_MINUTE;
  snprintf(timer_text, sizeof(timer_text), "%.02d:%.02d", minutes, seconds);
  text_layer_set_text(s_timer_text_layer, timer_text);
}

void updateStatusBarLayer(int current, int full) {
  float percentage = (float)current / (float)full;
  set_status_bar_percentage(percentage);
  layer_mark_dirty(s_status_bar_layer);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(timerIsRunning()) {
    turnTimerOff();
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_image_action_icon_play);
  }
  else {
    turnTimerOn();
    action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_image_action_icon_pause);
  }
}

static void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  vibes_short_pulse();
  if(timerIsRunning()) {
    turnTimerOff();
  }
  int warmup = persist_exists(WARMUP_PKEY) ? persist_read_int(WARMUP_PKEY) : WARMUP_DEFAULT;
  int meditate = persist_exists(MEDITATE_PKEY) ? persist_read_int(MEDITATE_PKEY) : MEDITATE_DEFAULT;
  int cooldown = persist_exists(COOLDOWN_PKEY) ? persist_read_int(COOLDOWN_PKEY) : COOLDOWN_DEFAULT;
  
  resetTimer(warmup, meditate, cooldown);
}

static void down_long_release_handler(ClickRecognizerRef recognizer, void *context) {
  return;
}

static void click_config_provider(void *context) {
  const uint16_t long_press_interval_ms = 1000;
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)select_click_handler);
  window_long_click_subscribe(BUTTON_ID_DOWN, long_press_interval_ms, (ClickHandler)down_long_click_handler, (ClickHandler)down_long_release_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_main(void) {
  initialise_ui();
  s_res_image_action_icon_pause = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_ICON_PAUSE);
  action_bar_layer_set_click_config_provider(s_actionbarlayer_1, click_config_provider);
  
  int warmup = persist_exists(WARMUP_PKEY) ? persist_read_int(WARMUP_PKEY) : WARMUP_DEFAULT;
  int meditate = persist_exists(MEDITATE_PKEY) ? persist_read_int(MEDITATE_PKEY) : MEDITATE_DEFAULT;
  int cooldown = persist_exists(COOLDOWN_PKEY) ? persist_read_int(COOLDOWN_PKEY) : COOLDOWN_DEFAULT;
  
  setTimerChangeCallback(timer_change_handler);
  setPhaseChangeCallback(phase_change_handler);
  set_status_bar_margin(s_status_bar_layer, 3);
  layer_set_update_proc(s_status_bar_layer, status_bar_layer_update_callback);
  
  initializeTimer(warmup, meditate, cooldown);
  
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_main(void) {
  window_stack_remove(s_window, true);
}
