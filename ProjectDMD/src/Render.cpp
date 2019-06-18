#include "../include/Render.h"

void Render::Text(rgb_matrix::Canvas* canvas, std::string text, DMDF* font, std::tuple<int, int> origin, rgb_matrix::Color color, TextJustification justification, int horizontalSpacing)
{
				std::tuple<int, int> currentRenderOrigin = origin;
				DMDColorPalette colorPalette(color);

				for (char& character : text)
				{
								// get DMDFC
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
																												canvas->SetPixel(std::get<0>(currentRenderOrigin) + col, std::get<1>(currentRenderOrigin) + row, 
																																renderColor->r, renderColor->g, renderColor->b); 
																								}
																				}
																}
												}
												// move current render position
												std::get<0>(currentRenderOrigin) += std::get<0>(characterDimensions);
								}
								else
								{
												// TODO: Do something if the character is invalid... (for now, move render position by 5..)
												std::get<0>(currentRenderOrigin) += 5;
								}			
								// add horizontal spacing between characters
								std::get<0>(currentRenderOrigin) += horizontalSpacing;				
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
												for (char& character : text)
												{
																DMDFC* characterDMDFC = font->GetCharacter(character);
																if (characterDMDFC != NULL) { totalWidth += std::get<0>(characterDMDFC->CharacterDimensions); }
																else { totalWidth += 5; }
												}
												totalWidth += (text.length() - 1)*horizontalSpacing;
												return std::tuple<int, int>(std::get<0>(origin) - totalWidth / 2, std::get<1>(origin));
												break;

								case TextJustification::Right:
												for (char& character : text)
												{
																DMDFC* characterDMDFC = font->GetCharacter(character);
																if (characterDMDFC != NULL) { totalWidth += std::get<0>(characterDMDFC->CharacterDimensions); }
																else { totalWidth += 5; }
												}
												totalWidth += (text.length() - 1)*horizontalSpacing;
												return std::tuple<int, int>(std::get<0>(origin) - totalWidth, std::get<1>(origin));
												break;

								default:
												return origin;
				}
}