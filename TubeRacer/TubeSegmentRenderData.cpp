#include "TubeSegmentRenderData.h"

TubeSegmentRenderData TubeSegmentRenderData::makeFromTubeSegmentBuilder(const TubeSegmentBuilder::TubeSegment& aTubeSegment, const std::vector<mycoretools::Vertex>& vertices, std::map<std::string, mycoretools::Texture>& textureContainer)
{
	TubeSegmentRenderData retData(vertices);

    retData._Indices.insert(std::end(retData._Indices), std::begin(aTubeSegment._Indices), std::end(aTubeSegment._Indices));

    if (!aTubeSegment._TextureAbdelo.empty())
    {
        if (auto found = textureContainer.find(aTubeSegment._TextureAbdelo); found != textureContainer.end())
            retData._AbdeloTexture = &(found->second);
        else
        {
            mycoretools::Texture aTexture;
            aTexture.load(aTubeSegment._TextureAbdelo);
            auto [iter, succeed] = textureContainer.try_emplace(aTubeSegment._TextureAbdelo, std::move(aTexture));
            assert(succeed);
        }
    }

    if (!aTubeSegment._TextureNormal.empty())
    {
        if (auto found = textureContainer.find(aTubeSegment._TextureNormal); found != textureContainer.end())
            retData._NormalTexture = &(found->second);
        else
        {
            mycoretools::Texture aTexture;
            aTexture.load(aTubeSegment._TextureNormal);
            auto [iter, succeed] = textureContainer.try_emplace(aTubeSegment._TextureNormal,  std::move(aTexture));
            assert(succeed);
        }
    }
    //retData._GPUMesh.fromData(retData._Vertices, retData._Indices, mycoretools::GPUMesh::DrawMode::eGL_TRIANGLE_STRIP);
    retData._GPUMesh.fromData(retData._Vertices, retData._Indices, mycoretools::GPUMesh::DrawMode::eGL_TRIANGLES);
    return retData;
}

void TubeSegmentRenderData::render(mycoretools::Shader& shader, const mycoretools::Camera& camera)
{
    shader.bind();

    if (_AbdeloTexture)
    {
        glActiveTexture(GL_TEXTURE1);
        _AbdeloTexture->bind();
    }

    if (_NormalTexture)
    {
        glActiveTexture(GL_TEXTURE2);
        _NormalTexture->bind();
    }

    // Matrices
    auto locViewMatrix = shader.getUniformLocation("viewMatrix");
    shader.setUniformMat4(locViewMatrix, camera.getViewMatrix());

    auto locProjMatrix = shader.getUniformLocation("projectionMatrix");
    shader.setUniformMat4(locProjMatrix, camera.getProjectionMatrix());

    _GPUMesh.render(shader);

    if (_AbdeloTexture)
        _AbdeloTexture->release();

    if (_NormalTexture)
        _NormalTexture->release();

    shader.release();
}
