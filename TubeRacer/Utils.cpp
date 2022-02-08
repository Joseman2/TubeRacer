#include "Utils.h"
#include "OpenGL-CoreLib/MathStructs.h"

#include <fstream>
#include <iostream>
#include <string>

namespace Utils
{
	std::optional<mycoretools::CatmullSpline> loadBezier(const std::string& fileName)
	{
		std::ifstream inStream;

		inStream.open(fileName, std::ios::in);
		if (!inStream.is_open())
			return std::nullopt;

		mycoretools::CatmullSpline retCatmull;
		std::string line;
		while (std::getline(inStream, line))
		{
			auto subStrings = splitString(line, ';');

			if (subStrings.size() < 3)
				return std::nullopt;

			glm::vec3 coord(std::atof(subStrings[0].c_str()), std::atof(subStrings[1].c_str()), std::atof(subStrings[2].c_str()));
			
			mycoretools::BezierControlPoint controlPoint{ ._Pos = coord, ._Radius = 1.0, ._TextureAbdeloFilename = "", ._TextureNormalFilename = "" };
			if (subStrings.size() == 5)
			{
				controlPoint._TextureAbdeloFilename = subStrings[3];
				controlPoint._TextureNormalFilename = subStrings[4];
			}

			retCatmull.addControlPoint(controlPoint);
		}

		inStream.close();

		return retCatmull;

	}

	std::vector<std::string> splitString(const std::string_view strView, const char separator)
	{
		std::vector<std::string> retStrings;

		size_t start;
		size_t end = 0;

		while ((start = strView.find_first_not_of(separator, end)) != std::string::npos)
		{
			end = strView.find(separator, start);
			retStrings.emplace_back(strView.substr(start, end - start));
		}


		return retStrings;
	}

};
