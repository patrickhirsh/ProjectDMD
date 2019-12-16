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

#include <algorithm>
#include <chrono>
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
#include "Graphics.h"

/* Modifiers provide means of generating values for various sources, colors, and
positions over time to create effects in Sources and Modes. These modifiers are 
blindly-implemented and designed to indirectly modify these Sources and Modes. */

////////////////////////////////////////////////////////////////////////////////
// HueShiftModifier

class HueShiftModifier
{
public:
    HueShiftModifier(
        double startHue,            // (0-360) hue start value
        double endHue,              // (0-360) hue end value
        double saturation = 1.0,    // (0.0-1.0) HSV saturation          
        double value = 1.0,         // (0.0-1.0) HSV value
        double shiftFactor = 10.0,  // shift speed (hue value shift per second)
        bool pingPong = false);     // shift back and forth, instead of looping

    /* Returns a color value based on the current state of the hue shift */
    rgb_matrix::Color GetColor      ();

private:
    double                          _startHue;
    double                          _endHue;
    double                          _saturation;
    double                          _value;
    double                          _shiftFactor;
    bool                            _pingPong;

    std::chrono::high_resolution_clock::time_point _startTime;
};

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// LinearInterpolationModifier

class LinearInterpolationModifier
{
public:
    LinearInterpolationModifier();
        
    /* Begin a linear interpolation session. GetPoint() will return interpolated points
    based on the current time. IsActive() will return false when duration has expired. */
    void Start(
        std::tuple<int, int> origin,            // linear interpolation start point
        std::tuple<int, int> destination,       // linear interpolation end point
        double duration);                       // duration (in seconds) to interpolate between points
    
    bool IsActive                   ();
    std::tuple<int, int> GetPoint   ();

    bool                            _isActive;
    std::tuple<int, int>            _origin;
    std::tuple<int, int>            _destination;
    double                          _duration;
    
    std::chrono::high_resolution_clock::time_point  _startTime;
};

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// CosineInterpolationModifier

class CosineInterpolationModifier
{
public:
    CosineInterpolationModifier();
        
    /* Begin a cosine interpolation session. GetPoint() will return interpolated points
    based on the current time. IsActive() will return false when duration has expired. */
    void Start(
        std::tuple<int, int> origin,            // cosine interpolation start point
        std::tuple<int, int> destination,       // cosine interpolation end point
        double duration);                       // duration (in seconds) to interpolate between points
    
    bool IsActive                   ();
    std::tuple<int, int> GetPoint   ();

    bool                            _isActive;
    std::tuple<int, int>            _origin;
    std::tuple<int, int>            _destination;
    double                          _duration;
    
    std::chrono::high_resolution_clock::time_point  _startTime;
};

////////////////////////////////////////////////////////////////////////////////