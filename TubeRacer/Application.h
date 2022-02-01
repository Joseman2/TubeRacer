#pragma once

#include "OpenGL-CoreLib/ApplicationBase.h"
#include "OpenGL-CoreLib/Shader.h"

class Application : public mycoretools::ApplicationBase
{
public:
	Application() = default;
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

private:
    mycoretools::Shader             _Shader;
    std::vector<std::string>        _ErrorLog;
};
