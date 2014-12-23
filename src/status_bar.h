#pragma once
void set_status_bar_percentage(float percentage);
void status_bar_layer_update_callback(struct Layer *layer, GContext *ctx);
void set_status_bar_margin(struct Layer *layer, int pixels);