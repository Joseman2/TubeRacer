#pragma once

#include "OpenGL-CoreLib/ApplicationBase.h"
#include "OpenGL-CoreLib/Shader.h"
#include "OpenGL-CoreLib/CatmullSpline.h"
#include "OpenGL-CoreLib/Texture.h"
#include "OpenGL-CoreLib/Camera.h"
#include "OpenGL-CoreLib/MeshGroup.h"
#include "OpenGL-CoreLib/SkyBox.h"
#include "OpenGL-CoreLib/Timer.h"

#include "TubeSegmentBuilder.h"
#include "TubeSegmentRenderData.h"
#include "CameraDeveloper.h"
#include "PlayerShip.h"

#include <map>

class Application : public mycoretools::ApplicationBase
{
public:
	Application();
	~Application() override = default;

    bool init();
    bool start();
    void update() override;
    void render() override;

    void reshape(int width, int height) override;

    std::vector<std::string> getErrorLog() const;

protected:
    void onKeyEvent(int key, int scancode, int action, int mods) override;
    void onMouseMovement(double xPos, double yPos) override;
    void onMouseButton(int button, int action, int mods) override;
    void onMouseWheel(double xoffset, double yoffsets) override;

private:
    void setCameraFromPlayerShip();

private:
    glm::uvec2                                  _MousePose{ 0,0 };
    CameraDeveloper                             _Camera;
    mycoretools::Shader                         _ShaderTube;
    std::vector<std::string>                    _ErrorLog;
    mycoretools::CatmullSpline                  _CatmullSpline;
    TubeSegmentBuilder                          _TubeSegmentBuilder;
    std::map<std::string, mycoretools::Texture> _TextureContainer;
    std::vector<TubeSegmentRenderData>          _TubeSegmentRenderDatas;

    std::vector<mycoretools::Vertex>            _Vertices;
    std::vector<std::uint32_t>                  _Indices;
    mycoretools::SkyBox                         _SkyBox;
    PlayerShip                                  _PlayerShip;
    
    mycoretools::Timer                          _Timer;
    bool                                        _isTimerAllreadyStarted = false;
};
