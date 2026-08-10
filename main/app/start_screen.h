#ifndef START_SCREEN_H
#define START_SCREEN_H

// ==================================================
// SHOW START SCREEN
// ==================================================
//
// Creates:
//
// - sleeping cat
// - animation timer
// - cat touch area
//
// State transitions:
//
// sleeping
//     ↓ tap
// stretching
//     ↓ automatically
// sitting
//     ↓ double tap
// sleeping
//
// START button appears while sitting.
//

void start_screen_show();

#endif