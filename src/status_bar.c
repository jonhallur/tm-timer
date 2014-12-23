#include <pebble.h>
#include "status_bar.h"

GRect get_status_border(struct Layer *layer);
GRect get_status_bar(struct Layer *layer, float percentage);

static float current_percentage = 1.0;
static int current_margin = 1;
  
void set_status_bar_percentage(float percentage) {
  current_percentage = percentage;
}

void status_bar_layer_update_callback(struct Layer *layer, GContext *ctx) {
  (void)layer;
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_draw_rect(ctx, get_status_border(layer));
  graphics_fill_rect(ctx, get_status_bar(layer, current_percentage), 0, GCornerNone);
}

GRect get_status_border(struct Layer *layer) {
  const int16_t x = current_margin + 1;
  const int16_t y = x;
  const int16_t width = layer_get_frame(layer).size.w - 2*current_margin;
  const int16_t height = layer_get_frame(layer).size.h - 2*current_margin;
  return GRect(x, y, width, height);
}

GRect get_status_bar(struct Layer *layer, float percentage) {
  const int16_t x = current_margin + 3;
  const int16_t y = x;
  const int16_t width = layer_get_frame(layer).size.w - 2*current_margin - 4;
  const int16_t height = layer_get_frame(layer).size.h - 2*current_margin - 4;
  return GRect(x, y, (int)(width*percentage), height);
}

void set_status_bar_margin(struct Layer *layer, int pixels) {
  const int16_t width = layer_get_frame(layer).size.w - current_margin;
  const int16_t height = layer_get_frame(layer).size.h - current_margin;
  if ((width - (2*pixels) < 4) || height - (2*pixels) < 4) {
    current_margin = 1;
  }
  else {
    current_margin = pixels;
  }
  
}