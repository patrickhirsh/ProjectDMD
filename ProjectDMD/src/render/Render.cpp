////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/Render.h"

/* Statics */
float Render::_globalBrightness = 1.0f;
Canvas* Render::_canvas;
rgb_matrix::Color* Render::_currentFrame[PANEL_WIDTH*PANEL_COUNT_X][PANEL_HEIGHT*PANEL_COUNT_Y];

////////////////////////////////////////////////////////////////////////////////
// Render

void Render::Initialize(int argc, char* argv[])
{
    // define matrix options
    RGBMatrix::Options defaults;
    defaults.hardware_mapping = "adafruit-hat";
    defaults.rows = PANEL_HEIGHT;
    defaults.cols = PANEL_WIDTH;
    defaults.chain_length = PANEL_COUNT_X;
    defaults.show_refresh_rate = false;

    // create the canvas
    _canvas = rgb_matrix::CreateMatrixFromFlags(&argc, &argv, &defaults);
    if (_canvas == NULL) { ErrorHandler::Log("Render", 
        "Couldn't create canvas (are runtime arguments valid?)", ErrorNum::FATAL_INVALID_CANVAS); }

    // create the current frame
    for (int y = 0; y < _canvas->height(); y++)
        for (int x = 0; x < _canvas->width(); x++)
            _currentFrame[x][y] = new rgb_matrix::Color(0, 0, 0);
}

void Render::FinalizeFrame()
{
    if (_canvas == NULL) { ErrorHandler::Log("Render", "Canvas is null!", ErrorNum::FATAL_INVALID_CANVAS); }
    for (int y = 0; y < _canvas->height(); y++)
    {
        for (int x = 0; x < _canvas->width(); x++)
        {
            _canvas->SetPixel(x, y,
                (int)(_currentFrame[x][y]->r * _globalBrightness),
                (int)(_currentFrame[x][y]->g * _globalBrightness),
                (int)(_currentFrame[x][y]->b * _globalBrightness));
        }
    }
}

float Render::GetGlobalBrightness()
{
    return _globalBrightness;
}

void Render::SetGlobalBrightness(float brightness)
{
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
    if (_canvas == NULL) { ErrorHandler::Log("Render", "Canvas is null!", ErrorNum::FATAL_INVALID_CANVAS); return -1; }
    else { return (_canvas->width()); }
}

int Render::GetDisplayHeight()
{
    if (_canvas == NULL) { ErrorHandler::Log("Render", "Canvas is null!", ErrorNum::FATAL_INVALID_CANVAS); return -1; }
    else { return (_canvas->height()); }
}

void Render::Clear()
{
    if (_canvas == NULL) { ErrorHandler::Log("Render", "Canvas is null!", ErrorNum::FATAL_INVALID_CANVAS); }
    _canvas->Clear();
    for (int y = 0; y < _canvas->height(); y++)
    {
        for (int x = 0; x < _canvas->width(); x++)
        {
            _currentFrame[x][y]->r = 0;
            _currentFrame[x][y]->g = 0;
            _currentFrame[x][y]->b = 0;
        }
    }
}

void Render::Text(
    std::string													        text,
    const DMDF*																			  font,
    std::tuple<int, int>				        origin,
    const rgb_matrix::Color*							 color,
    float                           opacity,
    TextJustification							        justification,
    int																					        horizontalSpacing
)
{
    if (_canvas == NULL) { ErrorHandler::Log("Render", "Canvas is null!", ErrorNum::FATAL_INVALID_CANVAS); }
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
                                setPixel(
                                    std::get<0>(currentRenderOrigin) + col,
                                    std::get<1>(currentRenderOrigin) + row,
                                    colorPalette.GetColor((*characterRaster)[col][row]));
                            }
                            else
                            {
                                setPixel(
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

void Render::Notification(
    std::string                     text,
    const DMDF*                     font,
    std::tuple<int, int>            origin,
    rgb_matrix::Color               color,
    float                           opacity,
    TextJustification               justification,
    int                             horizontalSpacing,
    int                             outlineSpacing
)
{
    if (_canvas == NULL) { ErrorHandler::Log("Render", "Canvas is null!", ErrorNum::FATAL_INVALID_CANVAS); }
    if (opacity > 1.0f) { opacity = 1.0f; }
    if (opacity <= 0.0f) { return; }

    // notif box is rendered outlineSpacing + 1 pixels outside text (move text origin inward)
    std::tuple<int, int> textOrigin(
        std::get<0>(origin) + outlineSpacing + 1, 
        std::get<1>(origin) + outlineSpacing + 1);

    // text performs these for us, but we need to apply justification to box as well.
    std::tuple<int, int> boxOrigin = getOriginAfterJustification(text, font, origin, horizontalSpacing, justification);

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
                setPixel(x, y, drawBoarder ? colorPalette.GetColor(15) : colorPalette.GetColor(0));
            }
            else
            {
                setPixel(x, y, drawBoarder ? colorPalette.GetColor(15) : colorPalette.GetColor(0), opacity);
            }
        }
    }
}

void Render::setPixel(
    int                             xPos,
    int                             yPos,
    const rgb_matrix::Color*        color
)
{
    _currentFrame[xPos][yPos]->r = color->r;
    _currentFrame[xPos][yPos]->g = color->g;
    _currentFrame[xPos][yPos]->b = color->b;
}

void Render::setPixel(
    int                             xPos,
    int                             yPos,
    const rgb_matrix::Color*        color,
    float                           opacity
)
{
    // alpha blending reference: https://en.wikipedia.org/wiki/Alpha_compositing#Alpha_blending
    _currentFrame[xPos][yPos]->r = (int)(color->r*opacity + _currentFrame[xPos][yPos]->r*(1 - opacity));
    _currentFrame[xPos][yPos]->g = (int)(color->g*opacity + _currentFrame[xPos][yPos]->g*(1 - opacity));
    _currentFrame[xPos][yPos]->b = (int)(color->b*opacity + _currentFrame[xPos][yPos]->b*(1 - opacity));
}

std::tuple<int, int> Render::getOriginAfterJustification(
    std::string													        text,
    const DMDF*																			  font,
    std::tuple<int, int>				        origin,
    int																					        horizontalSpacing,
    TextJustification							        justification
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
    std::string													        text,
    const DMDF*																			  font,
    int																					        horizontalSpacing
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