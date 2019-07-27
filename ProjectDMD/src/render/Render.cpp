////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/Render.h"

/* Statics */
float Render::GlobalBrightness = 1.0f;
Canvas* Render::_canvas;
rgb_matrix::Color Render::_currentFrame[DISPLAY_WIDTH][DISPLAY_HEIGHT];

////////////////////////////////////////////////////////////////////////////////
// Render

/* Initializes the panel canvas using application arguments. Render should be
initialized before any other systems. */
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
}

void Render::Text(
    const std::vector<Modifier*>&   activeModifiers,
    std::string													        text,
    const DMDF*																			  font,
    std::tuple<int, int>				        origin,
    const rgb_matrix::Color*							 color,
    TextJustification							        justification,
    int																					        horizontalSpacing
)
{
    if (_canvas == NULL) { ErrorHandler::Log("Render", 
        "Canvas is null!", ErrorNum::FATAL_INVALID_CANVAS); }
    
    // adjust for text justification
    std::tuple<int, int> currentRenderOrigin = getOriginAfterJustification(text, font, origin, horizontalSpacing, justification);
    
    // apply modifier transforms
    for (Modifier* modifier : activeModifiers) { modifier->Transform(currentRenderOrigin); }

    // pre-load a color palette for the font color
    DMDColorPalette colorPalette(color);

    bool beginText = false;
    for (char& character : text)
    {
        // ignore whitespace prefix
        if (!beginText && (character == ' ')) { continue; }
        else if (!beginText) { beginText = true; }

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
                            setPixel(
                                std::get<0>(currentRenderOrigin) + col,
                                std::get<1>(currentRenderOrigin) + row,
                                colorPalette.GetColor((*characterRaster)[col][row]),
                                activeModifiers);
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
    const std::vector<Modifier*>&   activeModifiers,
    std::string                     text,
    const DMDF*                     font,
    std::tuple<int, int>            origin,
    rgb_matrix::Color               color,
    TextJustification               justification,
    int                             horizontalSpacing
)
{

}

/* Apply activeModifiers to all pixels in the panel. NOTE: Transforms don't apply 
to panel-wide modifiers */
void Render::ModifyPanel(
    const std::vector<Modifier*>&   activeModifiers
)
{
    if (_canvas == NULL) { ErrorHandler::Log("Render", 
        "Canvas is null!", ErrorNum::FATAL_INVALID_CANVAS); }

    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < DISPLAY_WIDTH; x++)
        {
            modifyPixel(x, y, activeModifiers);
        }
    }
}

/* internal setPixel method, which automatically applies any active transitions and gloabl modifiers */
void Render::setPixel(
    int                             xPos,
    int                             yPos,
    const rgb_matrix::Color*        color,
    const std::vector<Modifier*>&   activeModifiers
)
{
    // apply active transition modifiers
    rgb_matrix::Color finalColor;
    finalColor.r = color->r;
    finalColor.g = color->g;
    finalColor.b = color->b;
    for (const Modifier* modifier : activeModifiers)
    {
        modifier->Modify(finalColor);
    }

    // apply global modifiers
    _canvas->SetPixel(xPos, yPos,
        (int)(finalColor.r * GlobalBrightness),
        (int)(finalColor.g * GlobalBrightness),
        (int)(finalColor.b * GlobalBrightness));

    // store current frame data
    _currentFrame[xPos][yPos].r = (int)(finalColor.r * GlobalBrightness);
    _currentFrame[xPos][yPos].g = (int)(finalColor.g * GlobalBrightness);
    _currentFrame[xPos][yPos].b = (int)(finalColor.b * GlobalBrightness);
}

/* internal setPixel method, which automatically applies any active transitions and gloabl modifiers */
void Render::setPixel(
    int                             xPos,
    int                             yPos,
    const rgb_matrix::Color*        color
)
{
    // apply global modifiers
    _canvas->SetPixel(xPos, yPos,
        (int)(color->r * GlobalBrightness),
        (int)(color->g * GlobalBrightness),
        (int)(color->b * GlobalBrightness));

    // store current frame data
    _currentFrame[xPos][yPos].r = (int)(color->r * GlobalBrightness);
    _currentFrame[xPos][yPos].g = (int)(color->g * GlobalBrightness);
    _currentFrame[xPos][yPos].b = (int)(color->b * GlobalBrightness);
}

/* Apply modifiers to the pixel at <xPos,yPos> */
void Render::modifyPixel(
    int                             xPos,
    int                             yPos,
    const std::vector<Modifier*>&   activeModifiers
)
{
    rgb_matrix::Color finalColor;
    finalColor.r = _currentFrame[xPos][yPos].r;
    finalColor.g = _currentFrame[xPos][yPos].g;
    finalColor.b = _currentFrame[xPos][yPos].b;
    for (Modifier* modifier : activeModifiers)
    {
        modifier->Modify(finalColor);
    }
    _canvas->SetPixel(xPos, yPos, finalColor.r, finalColor.g, finalColor.b);
}

/* Determines where to begin rendering based on selected Justification and text length */
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

/* Get the total width of "text" rendered in "font" with "horizontalSpacing" */
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