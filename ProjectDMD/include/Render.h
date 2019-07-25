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
#include "Transition.h"

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

    static float																        GlobalBrightness;
    static void																	        Initialize(int argc, char* argv[]);
    static bool																	        IsCanvasValid() { return _canvas ? true : false; }
    static void																	        Clear() { _canvas->Clear(); }

    static void Text(
        const std::vector<Transition*>  activeTransitions,
        std::string													        text,
        const DMDF*																			  font,
        std::tuple<int, int>				        origin,
        const rgb_matrix::Color*							 color,
        TextJustification							        justification = TextJustification::Left,
        int																					        horizontalSpacing = 0);

    static void Notification(
        const std::vector<Transition*>  activeTransitions,
        std::string                     text,
        const DMDF*                     font,
        std::tuple<int, int>            origin,
        rgb_matrix::Color               color,
        TextJustification               justification = TextJustification::Left,
        int                             horizontalSpacing = 0);

private:
    static Canvas*														        _canvas;

    static void setPixel(
        rgb_matrix::Canvas*             canvas,
        int                             xPos,
        int                             yPos,
        const rgb_matrix::Color*        color,
        const std::vector<Transition*>  activeTransitions);

    static std::tuple<int, int> getOriginAfterJustification(
        std::string													        text,
        const DMDF*																			  font,
        std::tuple<int, int>				        origin,
        int																					        horizontalSpacing,
        TextJustification							        justification);

    static int getTextWidth(
        std::string													        text,
        const DMDF*																			  font,
        int																					        horizontalSpacing);
};