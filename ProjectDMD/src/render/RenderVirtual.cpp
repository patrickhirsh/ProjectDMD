////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/RenderVirtual.h"

#if VIRTUAL
RenderVirtual::RenderVirtual(float scaleFactor)
{
    _scaleFactor = scaleFactor;
    _image = new sf::Image();
    _texture = new sf::Texture();
    _sprite = new sf::Sprite();
    _window = new sf::RenderWindow(sf::VideoMode(
        (PANEL_WIDTH*PANEL_COUNT_X) * _scaleFactor, 
        (PANEL_HEIGHT*PANEL_COUNT_Y) * _scaleFactor), 
        "ProjectDMD");
}

RenderVirtual::~RenderVirtual()
{
    delete _window;
    delete _image;
    delete _texture;
    delete _sprite;
}

void RenderVirtual::Poll()
{
    if (_window->isOpen())
    {
        sf::Event event;
        _window->pollEvent(event);
    }
}

void RenderVirtual::Clear()
{
    _window->clear();
}

void RenderVirtual::FinalizeFrame(rgb_matrix::Color* currentFrame[PANEL_WIDTH*PANEL_COUNT_X][PANEL_HEIGHT*PANEL_COUNT_Y])
{
    if (_window == NULL) { ErrorHandler::Log("RenderVirtual", "Window is null!", ErrorNum::FATAL_INVALID_VIRTUAL_WINDOW); }
    for (int y = 0; y < (PANEL_HEIGHT*PANEL_COUNT_Y); y++)
        for (int x = 0; x < (PANEL_WIDTH*PANEL_COUNT_X); x++)
        {
            _buffer[(y * (PANEL_WIDTH*PANEL_COUNT_X) + x) * 4] = currentFrame[x][y]->r;
            _buffer[(y * (PANEL_WIDTH*PANEL_COUNT_X) + x) * 4 + 1] = currentFrame[x][y]->g;
            _buffer[(y * (PANEL_WIDTH*PANEL_COUNT_X) + x) * 4 + 2] = currentFrame[x][y]->b;
            _buffer[(y * (PANEL_WIDTH*PANEL_COUNT_X) + x) * 4 + 3] = 255;
        }
    _image->create((PANEL_WIDTH*PANEL_COUNT_X), (PANEL_HEIGHT*PANEL_COUNT_Y), _buffer);
    _texture->loadFromImage(*_image);
    _sprite->setTexture(*_texture, true);
    _sprite->setScale(_scaleFactor, _scaleFactor);
    _window->draw(*_sprite);
    _window->display();
}
#endif // VIRTUAL