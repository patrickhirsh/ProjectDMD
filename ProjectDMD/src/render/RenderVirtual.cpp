////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/RenderVirtual.h"

RenderVirtual::RenderVirtual(float scaleFactor, bool flipHorizontal, bool flipVertical)
{
    _virtualWidth = PANEL_WIDTH * PANEL_COUNT_X;
    _virtualHeight = PANEL_HEIGHT * PANEL_COUNT_Y;
    _scaleFactor = scaleFactor;
    _flipHorizontal = flipHorizontal;
    _flipVertical = flipVertical;
    _window = new sf::RenderWindow(sf::VideoMode(_virtualWidth * _scaleFactor, _virtualHeight * _scaleFactor), "ProjectDMD");
    _frame = new sf::Image();
    _texture = new sf::Texture();
    _sprite = new sf::Sprite();
}

void RenderVirtual::Clear()
{
    _window->clear();
}

void RenderVirtual::FinalizeFrame(rgb_matrix::Color* currentFrame[PANEL_WIDTH*PANEL_COUNT_X][PANEL_HEIGHT*PANEL_COUNT_Y])
{
    if (_window == NULL) { ErrorHandler::Log("RenderVirtual", "Window is null!", ErrorNum::FATAL_INVALID_VIRTUAL_WINDOW); }
    for (int y = 0; y < _virtualHeight; y++)
        for (int x = 0; x < _virtualWidth; x++)
        {
            _buffer[(y * _virtualWidth + x) * 4] = currentFrame[x][y]->r;
            _buffer[(y * _virtualWidth + x) * 4 + 1] = currentFrame[x][y]->g;
            _buffer[(y * _virtualWidth + x) * 4 + 2] = currentFrame[x][y]->b;
            _buffer[(y * _virtualWidth + x) * 4 + 3] = 255;
        }
    _frame->create(_virtualWidth, _virtualHeight, _buffer);
    _texture->loadFromImage(*_frame);
    _sprite->setTexture(*_texture, true);
    _sprite->setScale(_scaleFactor, _scaleFactor);
    _window->draw(*_sprite);
    _window->display();
}