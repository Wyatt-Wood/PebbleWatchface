#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_time_layer2;

static GFont s_time_font;
static GFont s_time_font2;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  static char s_buffer2[16];
  strftime(s_buffer, sizeof(s_buffer), "%H%n%M", tick_time);
  strftime(s_buffer2, sizeof(s_buffer2), "%B %d/%Y", tick_time);
  
  text_layer_set_text(s_time_layer, s_buffer);
  text_layer_set_text(s_time_layer2, s_buffer2);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(GRect(0, 50, 200, 200));
  s_time_layer2 = text_layer_create(GRect(30, 30, 200, 15));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
    // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer2, GColorWhite);
  text_layer_set_text_color(s_time_layer2, GColorBlack);
  text_layer_set_text(s_time_layer2, "00:00");
  text_layer_set_text_alignment(s_time_layer2, GTextAlignmentLeft);

  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_48));
    s_time_font2 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_12));

  // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
    text_layer_set_font(s_time_layer2, s_time_font2);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
    layer_add_child(window_layer, text_layer_get_layer(s_time_layer2));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
    text_layer_destroy(s_time_layer2);

  // Unload GFont
  fonts_unload_custom_font(s_time_font);
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

  // Make sure the time is displayed from the start
  update_time();

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
