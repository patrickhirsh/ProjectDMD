////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#pragma once
#if VIRTUAL
#include <SFML/Graphics.hpp>
#endif
#include <vector>

/* RGB Matrix Library */
// https://github.com/hzeller/rpi-rgb-led-matrix
#include "../lib/rpi-rgb-led-matrix/include/led-matrix.h"
#include "../lib/rpi-rgb-led-matrix/include/threaded-canvas-manipulator.h"
#include "../lib/rpi-rgb-led-matrix/include/pixel-mapper.h"
#include "../lib/rpi-rgb-led-matrix/include/graphics.h"

// Local
#include "Error.h"

#if VIRTUAL
/* Generates, manages, and updates a virtual display using SFML,
designed to be used for remote development. This class is used when
VIRTUAL is enabled. */
class RenderVirtual
{
public:
    RenderVirtual                       (float scaleFactor);
    ~RenderVirtual                      ();

    void                                Poll();
    void                                Clear();
    void                                FinalizeFrame(rgb_matrix::Color* currentFrame[PANEL_WIDTH*PANEL_COUNT_X][PANEL_HEIGHT*PANEL_COUNT_Y]);

private:
    sf::RenderWindow*                   _window;
    sf::Image*                          _image;
    sf::Texture*                        _texture;
    sf::Sprite*                         _sprite;
    uint8_t                             _buffer[PANEL_WIDTH*PANEL_COUNT_X*VIRTUAL_SCALE_FACTOR*PANEL_HEIGHT*PANEL_COUNT_Y*VIRTUAL_SCALE_FACTOR*4];
    float                               _scaleFactor;       // factor by which we should scale the DMD display resolution
};
#endif // VIRTUAL