////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

/* RGB Matrix Library */
// https://github.com/hzeller/rpi-rgb-led-matrix
#include "../lib/rpi-rgb-led-matrix/include/led-matrix.h"
#include "../lib/rpi-rgb-led-matrix/include/threaded-canvas-manipulator.h"
#include "../lib/rpi-rgb-led-matrix/include/pixel-mapper.h"
#include "../lib/rpi-rgb-led-matrix/include/graphics.h"

// Local
#include "Error.h"

class RenderVirtual
{
public:
    RenderVirtual(
        float                           scaleFactor, 
        bool                            flipHorizontal, 
        bool                            flipVertical);

    void                                Clear();
    void                                FinalizeFrame(rgb_matrix::Color* currentFrame[PANEL_WIDTH*PANEL_COUNT_X][PANEL_HEIGHT*PANEL_COUNT_Y]);

private:
    sf::RenderWindow*                   _window;
    sf::Image*                          _frame;
    sf::Texture*                        _texture;
    sf::Sprite*                         _sprite;
    uint8_t                             _buffer[PANEL_WIDTH*PANEL_COUNT_X*VIRTUAL_SCALE_FACTOR*PANEL_HEIGHT*PANEL_COUNT_Y*VIRTUAL_SCALE_FACTOR*4];
    int                                 _virtualWidth;
    int                                 _virtualHeight; 
    float                               _scaleFactor;
    bool                                _flipHorizontal;      
    bool                                _flipVertical;
};