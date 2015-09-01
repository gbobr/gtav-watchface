#pragma once
#include <pebble.h>
  
typedef struct {
  char *text;
  GFont* font;
  GRect draw_rect;
  int border_thickness;
} twb_data_t;

Layer* create_twb_layer(char* text, GFont* font, int border_thickness, GRect draw_rect);