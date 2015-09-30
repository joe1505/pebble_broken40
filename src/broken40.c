// Joseph Rothweiler, Sensicomm LLC. 02Sep2015. Demo of character display problem.

#define BACK_COLOR GColorWhite
#define FRONT_COLOR GColorBlack

#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_seconds_layer;

static void handle_second_tick(struct tm* tick_time, TimeUnits units_changed) {
        // Needs to be static because it's used by the system later.
        static char s_seconds_text[3]; // "00" + null.

        // strftime(s_seconds_text, sizeof(s_seconds_text), "%S", tick_time);
        strcpy(s_seconds_text,"44");
        text_layer_set_text(s_seconds_layer, s_seconds_text);
}
//-----------------------------------------------------------------------
// Load and unload
static void main_window_load(Window *window) {

        // Find the whole-screen layer.
        Layer *window_layer = window_get_root_layer(window);
        GRect bounds = layer_get_frame(window_layer);

        // Seconds block (at bottom right).
        s_seconds_layer = text_layer_create(GRect(bounds.size.w-44, bounds.size.h-30, 44, 31));
        text_layer_set_text_color(s_seconds_layer, FRONT_COLOR);
        text_layer_set_background_color(s_seconds_layer, GColorClear);

        text_layer_set_font(s_seconds_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
        text_layer_set_text_alignment(s_seconds_layer, GTextAlignmentLeft);

        // Ensures time is displayed immediately (will break if NULL tick event accessed).
        // (This is why it's a good idea to have a separate routine to do the update itself.)
        time_t now = time(NULL);
        struct tm *current_time = localtime(&now);
        handle_second_tick(current_time, SECOND_UNIT);

        tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);

        layer_add_child(window_layer, text_layer_get_layer(s_seconds_layer));
}
static void main_window_unload(Window *window) {
        tick_timer_service_unsubscribe();
        text_layer_destroy(s_seconds_layer);
}

//-----------------------------------------------------------------------
// init and deinit, to be used by main.
// Defines the load and unload functions.
static void init() {
        s_main_window = window_create();
        window_set_background_color(s_main_window, BACK_COLOR);
        window_set_window_handlers(s_main_window, (WindowHandlers) {
          .load = main_window_load,
          .unload = main_window_unload,
        });
        window_stack_push(s_main_window, true);
}

static void deinit() {
        window_destroy(s_main_window);
}

//-----------------------------------------------------------------------
int main(void) {
        init();
        app_event_loop();
        deinit();
}