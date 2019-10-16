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

/* number of horizontal pixels to skip (leave "blank") when rendering 
text and an unsupported character is parsed. */
#define MISSING_CHAR_SPACING 5

/* enforces a sleep period after each content update in panel manager. 
This cuts back on unecessary memory churning. We only need to change
the contents of the canvas at each fixed frame. The panel scans at a
much higher frequency than this, so decreasing the delay can result in
too much load put on the Pi (and subsiquently, flickering/inconsistant 
scanning).*/
#define REFRESH_DELAY_MS 42000