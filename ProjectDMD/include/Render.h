////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <vector>

/* RGB Matrix Library */
// https://github.com/hzeller/rpi-rgb-led-matrix
#include "../lib/rpi-rgb-led-matrix/include/led-matrix.h"
#include "../lib/rpi-rgb-led-matrix/include/threaded-canvas-manipulator.h"
#include "../lib/rpi-rgb-led-matrix/include/pixel-mapper.h"
#include "../lib/rpi-rgb-led-matrix/include/graphics.h"

// Local
#include "../SYSTEMGLOBALS.h"
#include "Error.h"
#include "ResourceManager.h"

using namespace rgb_matrix;

/* Resource forward declaration */
class DMDF;
class DMDFC;

class Render
{
public:
    enum TextJustification
    {
        Left,
        Center,
        Right
    };

    /* initializes the panel canvas using application arguments. Render should be 
    initialized before any other systems. */
    static void																	        Initialize(int argc, char* argv[]);

    /* renders the current frame to the panel */
    static void                         FinalizeFrame();

    /* scales the brightness of the entire panel (between 0.0f and 1.0f). */
    static float                        GetGlobalBrightness();
    static void                         SetGlobalBrightness(float brightness);

    static int                          GetDisplayWidth();
    static int                          GetDisplayHeight();
    static void																         Clear();

    /* renders text to the panel. */
    static void Text(
        std::string													        text,                                       // the text to render
        const DMDF*																			  font,                                       // the font used to render the text
        std::tuple<int, int>				        origin,                                     // top left/right/center-most point at which to begin the render (depending on justification)
        const rgb_matrix::Color*							 color,                                      // color to render the text in
        float                           opactity,                                   // opacity to render this text at
        TextJustification							        justification = TextJustification::Left,    // render text leftward/rightward/outward from the origin
        int																					        horizontalSpacing = 0);                     // number of pixels to skip between characters

    /* renders a notification in a notification box. */
    static void Notification(
        std::string                     text,                                       // the text to render (leading whitespace ignored)
        const DMDF*                     font,                                       // the font used to render the text
        std::tuple<int, int>            origin,                                     // top left/right/center-most point at which to begin the render (depending on justification)
        rgb_matrix::Color               color,                                      // color to render the text in
        float                           opacity,                                    // opacity to render this text at
        TextJustification               justification = TextJustification::Left,    // render text leftward/rightward/outward from the origin
        int                             horizontalSpacing = 0,                      // number of pixels to skip between characters
        int                             outlineSpacing = 1);                        // buffer space between text and notif box

private:
    static Canvas*														        _canvas;
    static float                        _globalBrightness;

    /* all render calls per-frame mutate this canvas prior to drawing to the panel */
    static rgb_matrix::Color*           _currentFrame[PANEL_WIDTH*PANEL_COUNT_X][PANEL_HEIGHT*PANEL_COUNT_Y];

    /* sets the pixel at <xPos,yPos> in _currentFrame to "color" */
    static void setPixel(
        int                             xPos,
        int                             yPos,
        const rgb_matrix::Color*        color);

    /* sets the pixel at <xPos,yPos> in _currentFrame to "color" with "opacity".
    Opacity should be a value between 0.0f and 1.0f. */
    static void setPixel(
        int                             xPos,
        int                             yPos,
        const rgb_matrix::Color*        color,
        float                           opacity);

    /* determines where to begin rendering based on selected Justification and text length */
    static std::tuple<int, int> getOriginAfterJustification(
        std::string													        text,
        const DMDF*																			  font,
        std::tuple<int, int>				        origin,
        int																					        horizontalSpacing,
        TextJustification							        justification);

    /* get the total width of "text" rendered in "font" with "horizontalSpacing" */
    static int getTextWidth(
        std::string													        text,
        const DMDF*																			  font,
        int																					        horizontalSpacing);
};