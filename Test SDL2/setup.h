#pragma once
// setup.h

#define PROGRAM_NAME "NN Flappy Birds"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

// #define FPS_LIMIT 0      // No limits
// #define FPS_LIMIT 8    // Test
#define FPS_LIMIT 16     // floor(1000 / 60) == 16 -> 60 frames in 1000ms -> 60 FPS
// #define FPS_LIMIT 33     // floor(1000 / 30) == 33 -> 30 frames in 1000ms -> 30 FPS
// #define FPS_LIMIT 41     // 24 FPS
// #define FPS_LIMIT 100    // 10 FPS
