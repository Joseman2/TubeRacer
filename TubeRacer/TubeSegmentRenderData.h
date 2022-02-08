#pragma once
#include "OpenGL-CoreLib/MathStructs.h"
#include "OpenGL-CoreLib/Texture.h"
#include "OpenGL-CoreLib/Shader.h"
#include "OpenGL-CoreLib/GPUMesh.h"
#include "OpenGL-CoreLib/Camera.h"

#include "TubeSegmentBuilder.h"

#include <map>
class TubeSegmentRenderData
{
public:
	TubeSegmentRenderData() = delete;
	explicit TubeSegmentRenderData(const std::vector<mycoretools::Vertex>& vertices)
		:_Vertices(vertices)
	{}
	TubeSegmentRenderData(TubeSegmentRenderData&& rhs) = default;
	~TubeSegmentRenderData() = default;

	TubeSegmentRenderData& operator=(TubeSegmentRenderData&& rhs) = default;

	static TubeSegmentRenderData makeFromTubeSegmentBuilder(const TubeSegmentBuilder::TubeSegment& aTubeSegment, const std::vector<mycoretools::Vertex>& vertices, std::map<std::string, mycoretools::Texture>& textureContainer);

	void render(mycoretools::Shader& shader, const mycoretools::Camera& camera);

private:
	const std::vector<mycoretools::Vertex>&	_Vertices;
	std::vector<std::uint32_t>				_Indices;
	mycoretools::Texture*					_AbdeloTexture = nullptr;
	mycoretools::Texture*					_NormalTexture = nullptr;

	//mycoretools::Shader						_Shader;
	mycoretools::GPUMesh					_GPUMesh;
};

