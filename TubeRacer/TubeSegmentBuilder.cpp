#include "TubeSegmentBuilder.h"

#include <glm/gtx/vector_angle.hpp>

void TubeSegmentBuilder::buildTubeSegments(const std::vector<mycoretools::CatmullSpline::SubPath>& subPathes)
{
	_TubeSegments.clear();
	_Vertices.clear();
	std::uint16_t circleStep = 15;

	std::int32_t lastVertexIndex_quadX(0);
	std::int32_t lastVertexIndex_quadY(0);
	for (const auto& subPath : subPathes)
	{
		TubeSegment aTubeSegment;

		aTubeSegment._TextureAbdelo = subPath._TextureAbdeloFilename;
		aTubeSegment._TextureNormal = subPath._TextureNormalFilename;

		const std::uint32_t countQuadsX = circleStep + 1;
		const std::int32_t countQuadsY = !subPath._Points.empty() ? subPath._Points.size() - 1 : 0;

		float circleDelta = glm::radians(360.0f / circleStep);
		auto iterPath = subPath._Points.begin();

		float texCoordS_Step = 1.0f / static_cast<float>(subPath._Points.size());
		float texCoordT_Step = 1.0f / static_cast<float>(circleStep);
		float texCoordS = 0.0f;

		while (iterPath != subPath._Points.end())
		{
			auto pos0 = iterPath->_Pos;
			auto rad0 = iterPath->_Radius;

			++iterPath;
			if (iterPath != subPath._Points.end())
			{
				auto pos1 = iterPath->_Pos;
				//auto rad1 = iterPath->_Radius;

				float texCoordT = 0.0f;
				auto dir = pos1 - pos0;
				auto orthVec = glm::normalize(glm::cross(dir, { dir.x + 5, dir.y + 3, dir.z + 7 }));
				for (std::uint16_t seg(0); seg <= circleStep; ++seg)
				{
					mycoretools::Vertex vert;
					auto pointOnCircel_0 = glm::rotate(orthVec, circleDelta * seg, dir) * rad0 + pos0;
					//auto pointOnCircel_1 = glm::rotate(orthVec, circleDelta * seg, dir) * rad1 + pos1;

					vert._Position = pointOnCircel_0;
					vert._TexCoord.s = texCoordS;
					vert._TexCoord.t = texCoordT;
					vert._Color = { 0.5, 0.3, 0.8, 1.0 };
					_Vertices.push_back(vert);

					texCoordT += texCoordT_Step;
				}
				texCoordS += texCoordS_Step;
			}
		}

		for (std::uint32_t quadY(lastVertexIndex_quadY); quadY < (lastVertexIndex_quadY + countQuadsY - 1); ++quadY)
		{
			for (std::uint32_t quadX(lastVertexIndex_quadX); quadX < (lastVertexIndex_quadX + countQuadsX - 0); ++quadX)
			{
				std::uint32_t Index0 = countQuadsX * quadY + quadX;
				std::uint32_t Index1 = countQuadsX * (quadY + 1) + quadX;

				aTubeSegment._Indices.push_back(Index0);
				aTubeSegment._Indices.push_back(Index1);
			}
		}

		lastVertexIndex_quadX = lastVertexIndex_quadX + countQuadsX - 0;
		lastVertexIndex_quadY = lastVertexIndex_quadY + countQuadsY - 2;

		_TubeSegments.push_back(aTubeSegment);
	}

	for (const auto& aTubeSegment : _TubeSegments)
		calcTangendAndBiTangent(aTubeSegment, _Vertices);
}

void TubeSegmentBuilder::calcTangendAndBiTangent(const TubeSegment& aTubeSegment, std::vector<mycoretools::Vertex>& vertices) const
{
	// GL_TRIANGLE_STRIP
	std::uint32_t index(0);

	while (index < aTubeSegment._Indices.size() - 2 && !aTubeSegment._Indices.empty())
	{
		mycoretools::Face aFace;
		aFace._V0 = aTubeSegment._Indices[index];
		aFace._V1 = aTubeSegment._Indices[index + 1];
		aFace._V2 = aTubeSegment._Indices[index + 2];

		glm::vec3 edge1 = vertices[aFace._V1]._Position - vertices[aFace._V0]._Position;
		glm::vec3 edge2 = vertices[aFace._V2]._Position - vertices[aFace._V1]._Position;
		glm::vec2 deltaUV1 = vertices[aFace._V1]._TexCoord - vertices[aFace._V0]._TexCoord;
		glm::vec2 deltaUV2 = vertices[aFace._V2]._TexCoord - vertices[aFace._V1]._TexCoord;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		vertices[aFace._V0]._Tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		vertices[aFace._V0]._Tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		vertices[aFace._V0]._Tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		vertices[aFace._V0]._BiTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		vertices[aFace._V0]._BiTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		vertices[aFace._V0]._BiTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		++index;
	}

	int test(0);
}


const std::vector<TubeSegmentBuilder::TubeSegment>& TubeSegmentBuilder::getTubeSegments() const
{
	return _TubeSegments;
}

const std::vector<mycoretools::Vertex>& TubeSegmentBuilder::getVertices() const
{
	return _Vertices;
}
