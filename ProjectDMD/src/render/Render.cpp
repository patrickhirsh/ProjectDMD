////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/Render.h"

/* Statics */
float Render::_globalBrightness = 1.0f;
GPIO* Render::_gpio;
RGBMatrix* Render::_matrix;
FrameCanvas* Render::_buffer;
rgb_matrix::Color* Render::_currentFrame[PANEL_WIDTH*PANEL_COUNT_X][PANEL_HEIGHT*PANEL_COUNT_Y];

////////////////////////////////////////////////////////////////////////////////
// Render

void Render::Initialize(int argc, char* argv[])
{
    // define matrix options
    RGBMatrix::Options options;
    options.hardware_mapping = HARDWARE_MAPPING;
    options.rows = PANEL_HEIGHT;
    options.cols = PANEL_WIDTH;
    options.chain_length = PANEL_COUNT_X;
	options.parallel = PANEL_COUNT_Y;

    // init GPIO settings
    _gpio = new GPIO();
    _gpio->Init(GPIO_SLOWDOWN);

    // create the matrix
    _matrix = new rgb_matrix::RGBMatrix(_gpio, options);
    if (_matrix == NULL) { ErrorHandler::Log("Render", 
        "Couldn't create matrix instance (are matrix options valid?)", ErrorNum::FATAL_INVALID_MATRIX); }

    // create the buffer
    _buffer = _matrix->CreateFrameCanvas();

    // create the current frame
    for (int y = 0; y < _matrix->height(); y++)
        for (int x = 0; x < _matrix->width(); x++)
            _currentFrame[x][y] = new rgb_matrix::Color(0, 0, 0);
}

void Render::FinalizeFrame()
{
    if (_matrix == NULL) { ErrorHandler::Log("Render", "Matrix is null!", ErrorNum::FATAL_INVALID_MATRIX); }
    for (int y = 0; y < _matrix->height(); y++)
    {
        for (int x = 0; x < _matrix->width(); x++)
        {
			_buffer->SetPixel(
				FLIP_HORIZONTAL ? (_matrix->width() - 1) - x : x,
				FLIP_VERTICAL ? (_matrix->height() - 1) - y : y,
				(int)(_currentFrame[x][y]->r * _globalBrightness),
				(int)(_currentFrame[x][y]->g * _globalBrightness),
				(int)(_currentFrame[x][y]->b * _globalBrightness));
        }
    }
    _buffer = _matrix->SwapOnVSync(_buffer);
    Render::Clear();
}

float Render::GetGlobalBrightness()
{
    return _globalBrightness;
}

void Render::SetGlobalBrightness(float brightness)
{
    // brightness must be between 0.0f and 1.0f
    if (brightness > 1.0f)
    {
        _globalBrightness = 1.0f;
        ErrorHandler::Log("Render", std::string("GlobalBrightness set to an invalid value (%f)", brightness), 
            ErrorNum::WARNING_INVALID_RENDER_GLOBAL);
    }
    else if (brightness < 0.0f)
    {
        _globalBrightness = 0.0f;
        ErrorHandler::Log("Render", std::string("GlobalBrightness set to an invalid value (%f)", brightness),
            ErrorNum::WARNING_INVALID_RENDER_GLOBAL);
    }
    else
    {
        _globalBrightness = brightness;
    }
    
}

int Render::GetDisplayWidth()
{
    if (_matrix == NULL) { ErrorHandler::Log("Render", "Matrix is null!", ErrorNum::FATAL_INVALID_MATRIX); return -1; }
    else { return (_matrix->width()); }
}

int Render::GetDisplayHeight()
{
    if (_matrix == NULL) { ErrorHandler::Log("Render", "Matrix is null!", ErrorNum::FATAL_INVALID_MATRIX); return -1; }
    else { return (_matrix->height()); }
}

void Render::Clear()
{
    if (_buffer == NULL) { ErrorHandler::Log("Render", "Buffer is null!", ErrorNum::FATAL_INVALID_BUFFER); }
    _buffer->Clear();
    for (int y = 0; y < _matrix->height(); y++)
    {
        for (int x = 0; x < _matrix->width(); x++)
        {
            _currentFrame[x][y]->r = 0;
            _currentFrame[x][y]->g = 0;
            _currentFrame[x][y]->b = 0;
        }
    }
}

void Render::Text(
    std::string						text,
    const DMDF*						font,
    std::tuple<int, int>			origin,
    const rgb_matrix::Color*		color,
    float                           opacity,
    TextJustification				justification,
    int								horizontalSpacing
)
{
    if (_matrix == NULL) { ErrorHandler::Log("Render", "Matrix is null!", ErrorNum::FATAL_INVALID_MATRIX); return; }
    if (opacity > 1.0f) { opacity = 1.0f; }
    if (opacity <= 0.0f) { return; }
    
    // adjust for text justification
    std::tuple<int, int> currentRenderOrigin = getOriginAfterJustification(text, font, origin, horizontalSpacing, justification);

    // pre-load a color palette for the font color
    DMDColorPalette colorPalette(color);

    for (char& character : text)
    {
        // get DMDFC
        character = toupper(character);

        if (font->GetCharacter(character) != NULL)
        {
            // if DMDFC is valid, get DMDFC data
            std::tuple<int, int> characterDimensions = font->GetCharacter(character)->CharacterDimensions;
            std::vector<std::vector<unsigned char>>* characterRaster = font->GetCharacter(character)->CharacterRaster;

            // render character
            for (int row = 0; row < std::get<1>(characterDimensions); row++)
            {
                for (int col = 0; col < std::get<0>(characterDimensions); col++)
                {
                    // don't render transparent pixels
                    if ((*characterRaster)[col][row] != 255)
                    {
                        if (colorPalette.GetColor((*characterRaster)[col][row]) != NULL)
                        {
                            // don't do unecessary opacity computations
                            if (opacity == 1.0f)
                            {
								SetPixel(
                                    std::get<0>(currentRenderOrigin) + col,
                                    std::get<1>(currentRenderOrigin) + row,
                                    colorPalette.GetColor((*characterRaster)[col][row]));
                            }
                            else
                            {
								SetPixel(
                                    std::get<0>(currentRenderOrigin) + col,
                                    std::get<1>(currentRenderOrigin) + row,
                                    colorPalette.GetColor((*characterRaster)[col][row]),
                                    opacity);
                            }
                        }
                        else
                        {
                            ErrorHandler::Log("Render", 
                                "expected a stern value, got %d", ErrorNum::WARNING_INVALID_STERN_COLOR_VAL);
                        }
                    }
                }
            }
            // move current render position
            std::get<0>(currentRenderOrigin) += std::get<0>(characterDimensions);
        }
        else
        {
            // invalid character for selected font
            std::get<0>(currentRenderOrigin) += 5;

            // space character is fine, if invalid (since we essentially just added a space)
            if (character != ' ')
            {
                ErrorHandler::Log("Render",
                    std::string(std::string("character '") + character + std::string("' not supported by font ") + font->GetName()),
                    ErrorNum::WARNING_UNSUPPORTED_CHARACTER);
            }    
        }
        // add horizontal spacing between characters
        std::get<0>(currentRenderOrigin) += horizontalSpacing;
    }
}

void Render::Rectangle(
    std::tuple<int, int>			origin1,
    std::tuple<int, int>			origin2,                                   
    const rgb_matrix::Color*        color,                              
    float                           opacity,
    bool                            fill,
    int                             borderWidth                          
)
{
    int xLo;
    int yLo;
    int xHi;
    int yHi;
    int x;
    int y;

    // ensure we always draw top-left to bottom-right
    if (std::get<0>(origin1) < std::get<0>(origin2))
    {
        xLo = std::get<0>(origin1);
        xHi = std::get<0>(origin2);
    }
    else
    {
        xLo = std::get<0>(origin2);
        xHi = std::get<0>(origin1);
    }
    if (std::get<1>(origin1) < std::get<1>(origin2))
    {
        yLo = std::get<1>(origin1);
        yHi = std::get<1>(origin2);
    }
    else
    {
        yLo = std::get<1>(origin2);
        yHi = std::get<1>(origin1);
    }
    
    // fill mode
    if (fill)
    {
        for (y = yLo; y <= yLo + yHi; y++)
            if (y < GetDisplayHeight() && y >= 0)
                for (x = xLo; x <= xLo + xHi; x++)
                    if (x < GetDisplayWidth() && x >= 0)
                        SetPixel(x, y, color, opacity);
    }

    // border mode 
    else        
    {
        // top
        y = yLo;
        while (y < yLo + borderWidth && y <= yHi)
        {
            if (y < GetDisplayHeight() && y >= 0)
                for (x = xLo; x <= xLo + xHi; x++)
                    if (x < GetDisplayWidth() && x >= 0)
                        SetPixel(x, y, color, opacity);
            y++;
        }
        // bottom
        y = yHi;
        while (y > yHi - borderWidth && y >= yLo)
        {
            if (y < GetDisplayHeight() && y >= 0)
                for (x = xLo; x <= xLo + xHi; x++)
                    if (x < GetDisplayWidth() && x >= 0)
                        SetPixel(x, y, color, opacity);
            y--;
        }
        // left
        x = xLo;
        while (x < xLo + borderWidth && x <= xHi)
        {
            if (x < GetDisplayWidth() && x >= 0)
                for (y = yLo; y <= yLo + yHi; y++)
                    if (y < GetDisplayHeight() && y >= 0)
                        SetPixel(x, y, color, opacity);
            x++;
        }
        // right
        x = xHi;
        while (x > xHi - borderWidth && x >= xLo)
        {
            if (x < GetDisplayWidth() && x >= 0)
                for (y = yLo; y <= yLo + yHi; y++)
                    if (y < GetDisplayHeight() && y >= 0)
                        SetPixel(x, y, color, opacity);
            x--;
        }
    }    
}

void Render::Notification(
    std::string                     text,
    const DMDF*                     font,
    std::tuple<int, int>            origin,
    const rgb_matrix::Color*        color,
    float                           opacity,
    TextJustification               justification,
    int                             horizontalSpacing,
    int                             outlineSpacing
)
{
    if (_matrix == NULL) { ErrorHandler::Log("Render", "Matrix is null!", ErrorNum::FATAL_INVALID_MATRIX); return; }
    if (opacity > 1.0f) { opacity = 1.0f; }
    if (opacity <= 0.0f) { return; }

    // text performs these for us, but we need to apply justification to box as well.
    std::tuple<int, int> boxOrigin = getOriginAfterJustification(text, font, origin, horizontalSpacing, justification);
    std::get<0>(boxOrigin) -= (outlineSpacing + 1);
    std::get<1>(boxOrigin) -= (outlineSpacing + 1);

    // render notification box
    int boxWidth = getTextWidth(text, font, horizontalSpacing) + (outlineSpacing * 2) + 2;
    int boxHeight = font->GetFontHeight() + (outlineSpacing * 2) + 2;
    int xBoxOrigin = std::get<0>(boxOrigin);
    int yBoxOrigin = std::get<1>(boxOrigin);
    DMDColorPalette colorPalette(color);
    for (int y = yBoxOrigin; y < (yBoxOrigin + boxHeight); y++)
    {
        for (int x = xBoxOrigin; x < (xBoxOrigin + boxWidth); x++)
        {
            bool drawBoarder = (
                ((y == yBoxOrigin) || (y == yBoxOrigin + boxHeight - 1)) ||
                ((x == xBoxOrigin) || (x == xBoxOrigin + boxWidth - 1)));
            
            // don't do unecessary opacity computations
            if (opacity == 1.0f)
            {
                if (drawBoarder) { SetPixel(x, y, colorPalette.GetColor(15)); }
                else { SetPixel(x, y, colorPalette.GetColor(0)); }
            }
            else
            {
                if (drawBoarder) { SetPixel(x, y, colorPalette.GetColor(15)); }
                else { SetPixel(x, y, colorPalette.GetColor(0)); }
            }
        }
    }

    // render notification text
    Text(text, font, origin, color, opacity, justification, horizontalSpacing);
}

void Render::SetPixel(
    int                             xPos,
    int                             yPos,
    const rgb_matrix::Color*        color
)
{
    if (xPos >= 0 && xPos < GetDisplayWidth() && yPos >= 0 && yPos < GetDisplayHeight())
    {
        _currentFrame[xPos][yPos]->r = color->r;
        _currentFrame[xPos][yPos]->g = color->g;
        _currentFrame[xPos][yPos]->b = color->b;
    }
}

void Render::SetPixel(
    int                             xPos,
    int                             yPos,
    const rgb_matrix::Color*        color,
    float                           opacity
)
{
    if (xPos >= 0 && xPos < GetDisplayWidth() && yPos >= 0 && yPos < GetDisplayHeight())
    {
        // alpha blending reference: https://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending
        _currentFrame[xPos][yPos]->r = (int)(color->r*opacity + _currentFrame[xPos][yPos]->r*(1 - opacity));
        _currentFrame[xPos][yPos]->g = (int)(color->g*opacity + _currentFrame[xPos][yPos]->g*(1 - opacity));
        _currentFrame[xPos][yPos]->b = (int)(color->b*opacity + _currentFrame[xPos][yPos]->b*(1 - opacity));
    }
}

std::tuple<int, int> Render::getOriginAfterJustification(
    std::string						text,
    const DMDF*						font,
    std::tuple<int, int>			origin,
    int								horizontalSpacing,
    TextJustification				justification
)
{
    switch (justification)
    {
    case TextJustification::Left:
        return origin;
        break;

    case TextJustification::Center:
        return std::tuple<int, int>(std::get<0>(origin) - (getTextWidth(text, font, horizontalSpacing) / 2), std::get<1>(origin));
        break;

    case TextJustification::Right:
        return std::tuple<int, int>(std::get<0>(origin) - getTextWidth(text, font, horizontalSpacing), std::get<1>(origin));
        break;

    default:
        return origin;
    }
}

int Render::getTextWidth(
    std::string						text,
    const DMDF*						font,
    int								horizontalSpacing
)
{
    int totalWidth = 0;
    for (char& character : text)
    {
        if (font->GetCharacter(character) != NULL) 
        { 
            totalWidth += std::get<0>(font->GetCharacter(character)->CharacterDimensions); 
        }
        else { totalWidth += 5; }
    }
    return (totalWidth + ((text.length() - 1)*horizontalSpacing));
}

////////////////////////////////////////////////////////////////////////////////