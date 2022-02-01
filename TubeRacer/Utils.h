#pragma once

#include "OpenGL-CoreLib/BezierNPoints.h"

#include <string_view>

namespace Utils
{
	mycoretools::BezierNPoints loadBezier(const std::string_view strView);

};

