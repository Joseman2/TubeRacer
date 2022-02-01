#include "Application.h"

bool Application::init()
{
    //if (!_Scene.init(_ErrorLog))
    //    return false;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);

    //glfwSetInputMode(_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return true;
}

bool Application::start()
{
    //if (!_AudioManager.playSound())
    //    _ErrorLog.push_back("Could not start sound!");

    return true;
}

void Application::update()
{
    ApplicationBase::update();

    //_Scene.update();
    //_AudioManager.update();
//    auto& spectrumRight = _AudioManager.getSpektrumRight();
//    auto& spectrumLeft = _AudioManager.getSpektrumLeft();
}

void Application::render()
{
    ApplicationBase::render();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //_Scene.render();

    glfwSwapBuffers(_GLFWwindow);
    glfwPollEvents();
}

void Application::reshape(int width, int height)
{
    ApplicationBase::reshape(width, height);
    //_Scene.onResize(glm::vec2(width, height), 45.0, 1.0, 100.0);
}


std::vector<std::string> Application::getErrorLog() const
{
    return _ErrorLog;
}

void Application::onKeyEvent(int key, int scancode, int action, int mods)
{
    ApplicationBase::onKeyEvent(key, scancode, action, mods);
    //_Scene.onKeyEvent(key, scancode, action, mods);
}

void Application::onMouseMovement(double xPos, double yPos)
{
    ApplicationBase::onMouseMovement(xPos, yPos);
    //_Scene.onMouseMovement(xPos, yPos);
}

void Application::onMouseButton(int button, int action, int mods)
{
    ApplicationBase::onMouseButton(button, action, mods);
}
