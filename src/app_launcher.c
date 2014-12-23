#include <pebble.h>
#include "main.h"

int main(void) {
  show_main();
  app_event_loop();
  hide_main();
}