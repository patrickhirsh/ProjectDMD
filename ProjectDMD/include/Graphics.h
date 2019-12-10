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


/* General purpose graphics utility implementations */
namespace GraphicsUtility
{
    /* Ref: https://stackoverflow.com/a/6930407
    Converts a color in HSV color space to an RGB Color.
    h : hue in degrees (0 - 360)
    s : saturation (0.0 - 1.0) 
    v : value (0.0 - 1.0) */
    rgb_matrix::Color HSV2RGB(double h, double s = 1.0, double v = 1.0);
}