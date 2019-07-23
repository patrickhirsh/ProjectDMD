////////////////////////////////////////////////////////////////////////////////
/* ProjectDMD Written by: Patrick Hirsh, 2019 */
////////////////////////////////////////////////////////////////////////////////

#include "../../include/Render.h"

/* Statics */
float Render::GlobalBrightness = 1.0f;
Canvas* Render::_canvas;

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
    std::string													text,
    DMDF*																			font,
    std::tuple<int, int>				origin,
    rgb_matrix::Color							color,
    TextJustification							justification,
    int																					horizontalSpacing
)
{
    if (_canvas == NULL) { ErrorHandler::Log("Render", 
        "Canvas is null!", ErrorNum::FATAL_INVALID_CANVAS); }
    
    std::tuple<int, int> currentRenderOrigin = origin;
    DMDColorPalette colorPalette(color);

    for (char& character : text)
    {
        // get DMDFC
        character = toupper(character);
        DMDFC* characterDMDFC = font->GetCharacter(character);

        if (characterDMDFC != NULL)
        {
            // if DMDFC is valid, get DMDFC data
            std::tuple<int, int> characterDimensions = characterDMDFC->CharacterDimensions;
            std::vector<std::vector<unsigned char>>* characterRaster = characterDMDFC->CharacterRaster;

            // render character
            for (int row = 0; row < std::get<1>(characterDimensions); row++)
            {
                for (int col = 0; col < std::get<0>(characterDimensions); col++)
                {
                    // don't render transparent pixels
                    if ((*characterRaster)[col][row] != 255)
                    {
                        rgb_matrix::Color* renderColor = colorPalette.GetColor((*characterRaster)[col][row]);
                        if (renderColor != NULL)
                        {
                            _canvas->SetPixel(std::get<0>(currentRenderOrigin) + col, std::get<1>(currentRenderOrigin) + row,
                                (int)(renderColor->r * GlobalBrightness),
                                (int)(renderColor->g * GlobalBrightness),
                                (int)(renderColor->b * GlobalBrightness));
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
    std::string             text,
    DMDF*                   font,
    std::tuple<int, int>    origin,
    rgb_matrix::Color       color,
    TextJustification       justification,
    int                     horizontalSpacing
)
{

}

/* Determines where to begin rendering based on selected Justification and text length */
std::tuple<int, int> Render::getOriginAfterJustification(
    std::string													text,
    DMDF*																			font,
    std::tuple<int, int>				origin,
    int																					horizontalSpacing,
    TextJustification							justification
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
    std::string													text,
    DMDF*																			font,
    int																					horizontalSpacing
)
{
    int totalWidth = 0;
    for (char& character : text)
    {
        DMDFC* characterDMDFC = font->GetCharacter(character);
        if (characterDMDFC != NULL) { totalWidth += std::get<0>(characterDMDFC->CharacterDimensions); }
        else { totalWidth += 5; }
    }
    return (totalWidth + ((text.length() - 1)*horizontalSpacing));
}

////////////////////////////////////////////////////////////////////////////////