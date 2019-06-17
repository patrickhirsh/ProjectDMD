#pragma once
#include "Libraries.h"
#include "DMDF.h"
#include "ResourceManager.h"

enum TextJustification 
{
				Left,
				Center,
				Right
};

class Render
{
				public:
								static void Text(
												rgb_matrix::Canvas*					canvas,
												std::string													text,
												DMDF*																			font,
												std::tuple<int, int>				origin,
												rgb_matrix::Color							color,
												TextJustification							justification = TextJustification::Left,
												int																					horizontalSpacing = 2);

				private:
								static std::tuple<int, int> getOriginAfterJustification(
												std::string													text, 
												DMDF*																			font, 
												std::tuple<int, int>				origin, 
												int																					horizontalSpacing, 
												TextJustification							justification);
};