#include <pebble.h>
#include "twb.h"
  
static Window *s_main_window;
static GFont s_time_font;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
Layer *time_layer;
Layer *date_layer;
static char time_buffer[] = "00:00";
static char date_buffer[] = "sab, 01/01";
int image=0;

static void main_window_load(Window *window) {
  // Create GBitmap, then set to created BitmapLayer
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FRANKLIN_IDENTIFIER);
  s_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  time_layer=create_twb_layer(time_buffer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PRICEDOWN_44)), 6, GRect(20,90,144,60));
  date_layer=create_twb_layer(date_buffer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PRICEDOWN_22)), 4, GRect(15,135,144,60));

  layer_add_child(window_get_root_layer(window), time_layer);
  layer_add_child(window_get_root_layer(window), date_layer);
}

static void main_window_unload(Window *window) {
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  fonts_unload_custom_font(s_time_font);
  layer_destroy(time_layer);
  layer_destroy(date_layer);
}

static void change_background(GBitmap *bitmap){
  
  gbitmap_destroy(s_background_bitmap);
  s_background_bitmap = bitmap;
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  
}

static void refresh_image(int hour){ 
  
  if(hour>=22 && image != 0){
    image=0;
    change_background(gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FRANKLIN_IDENTIFIER));
  } else if(hour>=14 && image != 1) {
    image=1;
    change_background(gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MICHAEL_IDENTIFIER));
  } else if(image != 2) {
    image=2;
    change_background(gbitmap_create_with_resource(RESOURCE_ID_IMAGE_TREVOR_IDENTIFIER));
  }
  
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  if(clock_is_24h_style() == true) {
    strftime(time_buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(time_buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  strftime(date_buffer, sizeof("sab, 01/01"), "%a, %e/%m", tick_time);
  
  refresh_image(tick_time->tm_hour);
  
  layer_mark_dirty(time_layer);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}



static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

