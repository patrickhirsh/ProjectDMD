#include "../include/Render.h"

void Render::Text(rgb_matrix::Canvas* canvas, std::string text, DMDF* font, std::tuple<int, int> origin, rgb_matrix::Color color, TextJustification justification, int horizontalSpacing)
{
				std::tuple<int, int> currentRenderOrigin = origin;
				std::tuple<int, int> fontDimensions = font->GetDimensions();
				DMDColorPalette colorPalette(color);

				for (char character : text)
				{
								// validate character
								std::vector<std::vector<unsigned char>>* characterRaster = font->GetCharacter(character);
								bool validCharacter = characterRaster == NULL ? false : true;

								// draw character
								if (validCharacter)
								{
												for (int row = 0; row < std::get<1>(fontDimensions); row++)
												{
																for (int col = 0; col < std::get<0>(fontDimensions); col++)
																{
																				// don't render transparent pixels
																				if ((*characterRaster)[col][row] != 255)
																				{
																								rgb_matrix::Color* renderColor = colorPalette.GetColor((*characterRaster)[col][row]);
																								if (renderColor != NULL) 
																								{ 
																												canvas->SetPixel(col + std::get<0>(currentRenderOrigin), row + std::get<1>(currentRenderOrigin), 
																																renderColor->r, renderColor->g, renderColor->b); 
																								}
																				}
																}
												}
								}
								else
								{
												// TODO: Do something if the character is invalid...
								}			

								// move current render position
								std::get<0>(currentRenderOrigin) = std::get<0>(currentRenderOrigin) + std::get<0>(fontDimensions) + horizontalSpacing;
				}
}


std::tuple<int, int> Render::getOriginAfterJustification(std::string text, DMDF* font, std::tuple<int, int> origin, int horizontalSpacing, TextJustification justification)
{
				int totalWidth = 0;
				switch (justification)
				{
								case TextJustification::Left:
												return origin;
												break;

								case TextJustification::Center:
												totalWidth = text.length() * (std::get<0>(font->GetDimensions())) + (text.length() - 1)*horizontalSpacing;
												return std::tuple<int, int>(std::get<0>(origin) - totalWidth / 2, std::get<1>(origin));
												break;

								case TextJustification::Right:
												totalWidth = text.length() * (std::get<0>(font->GetDimensions())) + (text.length() - 1)*horizontalSpacing;
												return std::tuple<int, int>(std::get<0>(origin) - totalWidth, std::get<1>(origin));
												break;

								default:
												return origin;
				}
}