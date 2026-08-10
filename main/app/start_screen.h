#ifndef START_SCREEN_H
#define START_SCREEN_H

// ==================================================
// SHOW START SCREEN
// ==================================================

void start_screen_show();

// ==================================================
// CLEANUP START SCREEN
// ==================================================
//
// Called before moving to another screen.
//
// Stops the animation timer and removes all
// start-screen objects.
//

void start_screen_cleanup();

// ==================================================
// START BUTTON CALLBACK
// ==================================================

typedef void (*StartButtonCallback)();

void start_screen_set_start_callback(
    StartButtonCallback callback
);

#endif