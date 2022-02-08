#pragma once
#include "OpenGL-CoreLib/BezierNPoints.h"
#include "OpenGL-CoreLib/CatmullSpline.h"
#include "OpenGL-CoreLib/MathStructs.h"

#include <vector>

class TubeSegmentBuilder
{
public:
	struct TubeSegment
	{
		std::vector<std::uint32_t>			_Indices;
		std::string							_TextureAbdelo;
		std::string							_TextureNormal;
	};

public:
	TubeSegmentBuilder() = default;
	~TubeSegmentBuilder() = default;

	// build tubeSegments from <path> with the length of <length>
	void buildTubeSegments(const std::vector<mycoretools::CatmullSpline::SubPath>& subPathes);
	const std::vector<TubeSegment>& getTubeSegments() const;
	const std::vector<mycoretools::Vertex>& getVertices() const;

private:
	std::vector<TubeSegment>			_TubeSegments;
	std::vector<mycoretools::Vertex>	_Vertices;
};

