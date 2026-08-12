#ifndef START_SCREEN_H
#define START_SCREEN_H

// ==================================================
// START SCREEN
// ==================================================

void start_screen_show();

// ==================================================
// CLEANUP
// ==================================================
//
// Stops the start-screen animation timer and
// releases its object references before the screen
// is replaced.
//

void start_screen_cleanup();

#endif