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

enum TextJustification 
{
				Left,
				Center,
				Right
};

class Render
{
public:
				static float																GlobalBrightness;
				static void																	Initialize(int argc, char* argv[]);
				static bool																	IsCanvasValid() { return _canvas ? true : false; }
				static void																	Clear() { _canvas->Clear(); }

				static void Text(
								std::string													text,
								DMDF*																			font,
								std::tuple<int, int>				origin,
								rgb_matrix::Color							color,
								TextJustification							justification = TextJustification::Left,
								int																					horizontalSpacing = 0);

private:
				static Canvas*														_canvas;

				static std::tuple<int, int> getOriginAfterJustification(
								std::string													text, 
								DMDF*																			font, 
								std::tuple<int, int>				origin, 
								int																					horizontalSpacing, 
								TextJustification							justification);
};