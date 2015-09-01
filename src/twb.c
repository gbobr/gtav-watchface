#include <pebble.h>
#include "twb.h"

void draw_with_border(char* text, GFont* font, int border_thickness, GRect drawRect, GContext *ctx);
void update_twb_layer(Layer *this_layer, GContext *ctx);
  
Layer* create_twb_layer(char* text, GFont* font, int border_thickness, GRect draw_rect){
  Layer* s_layer=layer_create_with_data(draw_rect, sizeof(twb_data_t));
  twb_data_t* data=(twb_data_t*)layer_get_data(s_layer);
  
  data->font=font;
  data->text=text;
  data->draw_rect=draw_rect;
  data->border_thickness=border_thickness;
  
  layer_set_update_proc(s_layer, update_twb_layer);
    
  return s_layer;
}
  
void draw_with_border(char* text, GFont* font, int border_thickness, GRect drawRect, GContext *ctx){  
  int i,j;
  GSize size=drawRect.size;
  graphics_context_set_text_color(ctx, GColorBlack);
  
  for(i=0;i<=border_thickness;i++)
    for(j=0;j<=border_thickness;j++)
      graphics_draw_text(ctx, text, font, GRect(i,j, size.w,size.h), GTextOverflowModeFill, GTextAlignmentLeft, NULL);        
  
  graphics_context_set_text_color(ctx, GColorWhite);  
  graphics_draw_text(ctx, text, font, GRect(border_thickness/2,border_thickness/2, size.w,size.h), GTextOverflowModeFill, GTextAlignmentLeft, NULL);
}

void update_twb_layer(Layer *this_layer, GContext *ctx){
  twb_data_t* data=(twb_data_t*)layer_get_data(this_layer);
  draw_with_border(data->text, data->font, data->border_thickness,data->draw_rect, ctx);
}