////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#pragma once

/* Startup */
#define SKIP_SPLASH_SCREEN 0

/* Panel Properties*/
#define PANEL_WIDTH 64
#define PANEL_HEIGHT 32
#define PANEL_COUNT_X 4
#define PANEL_COUNT_Y 2
#define FLIP_VERTICAL 1
#define FLIP_HORIZONTAL 1
#define HARDWARE_MAPPING "regular"
#define GPIO_SLOWDOWN 3

/* Virtual Properties */
#define VIRTUAL_SCALE_FACTOR 3

/* number of horizontal pixels to skip (leave "blank") when rendering 
text and an unsupported character is parsed. */
#define MISSING_CHAR_SPACING 5