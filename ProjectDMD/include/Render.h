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
#include "Modifier.h"

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

    /* Render globals */
    static float																        GlobalBrightness;

    /* Initializes the panel canvas using application arguments. Render should be 
    initialized before any other systems. */
    static void																	        Initialize(int argc, char* argv[]);

    static void																         Clear();
    static int                          GetDisplayWidth();
    static int                          GetDisplayHeight();

    /* Renders text to the panel. */
    static void Text(
        const std::vector<Modifier*>&   activeModifiers,                            // modifiers to be applied to the render
        std::string													        text,                                       // the text to render
        const DMDF*																			  font,                                       // the font used to render the text
        std::tuple<int, int>				        origin,                                     // top left/right/center-most point at which to begin the render (depending on justification)
        const rgb_matrix::Color*							 color,                                      // color to render the text in
        TextJustification							        justification = TextJustification::Left,    // render text leftward/rightward/outward from the origin
        int																					        horizontalSpacing = 0);                     // number of pixels to skip between characters

    /* Renders a notification in a notification box. */
    static void Notification(
        const std::vector<Modifier*>&   activeModifiers,                            // modifiers to be applied to the render
        std::string                     text,                                       // the text to render (leading whitespace ignored)
        const DMDF*                     font,                                       // the font used to render the text
        std::tuple<int, int>            origin,                                     // top left/right/center-most point at which to begin the render (depending on justification)
        rgb_matrix::Color               color,                                      // color to render the text in
        TextJustification               justification = TextJustification::Left,    // render text leftward/rightward/outward from the origin
        int                             horizontalSpacing = 0);                     // number of pixels to skip between characters

    /* Applies the activeModifiers to the entire panel. NOTE: Modifier transforms 
    do not apply here. */
    static void ModifyPanel(
        const std::vector<Modifier*>&   activeModifiers);

private:
    static Canvas*														        _canvas;

    /* Store the state of the canvas per-frame so we can get current pixel data to 
    perform post-update full-panel modifiers. */
    static rgb_matrix::Color            _currentFrame[PANEL_WIDTH*PANEL_COUNT_X][PANEL_HEIGHT*PANEL_COUNT_Y];

    /* internal setPixel method, which automatically applies any active transitions 
    and gloabl modifiers as well as updates the _currentFrame. */
    static void setPixel(
        int                             xPos,
        int                             yPos,
        const rgb_matrix::Color*        color,
        const std::vector<Modifier*>&   activeModifiers);

    /* internal setPixel that doesn't require any activeModifiers be provided. */
    static void setPixel(
        int                             xPos,
        int                             yPos,
        const rgb_matrix::Color*        color);

    /* Apply modifiers to the pixel at <xPos,yPos> */
    static void modifyPixel(
        int                             xPos,
        int                             yPos,
        const std::vector<Modifier*>&   activeModifiers);

    /* Determines where to begin rendering based on selected Justification and text length */
    static std::tuple<int, int> getOriginAfterJustification(
        std::string													        text,
        const DMDF*																			  font,
        std::tuple<int, int>				        origin,
        int																					        horizontalSpacing,
        TextJustification							        justification);

    /* Get the total width of "text" rendered in "font" with "horizontalSpacing" */
    static int getTextWidth(
        std::string													        text,
        const DMDF*																			  font,
        int																					        horizontalSpacing);
};