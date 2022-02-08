#pragma once

#include "OpenGL-CoreLib/CatmullSpline.h"

#include <string>
#include <vector>
#include <optional>

namespace Utils
{
	std::optional<mycoretools::CatmullSpline> loadBezier(const std::string& string);

	std::vector<std::string> splitString(const std::string_view str, const char separator);

};

