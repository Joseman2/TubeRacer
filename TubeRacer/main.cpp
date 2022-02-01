#include "Application.h"


void initAndStart(Application& app)
{
    mycoretools::ApplicationBase::AppSettings setting;
    setting._Title = "Tube Racer";

    if (!app.instantiate(setting))
    {
        glfwTerminate();

        // Exit program
        exit(EXIT_FAILURE);
    }

    if (!app.init())
    {
        glfwTerminate();

        // Exit program
        exit(EXIT_FAILURE);
    }

    if (!app.start())
    {
        glfwTerminate();

        // Exit program
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char* argv[])
{
    Application app;
    initAndStart(app);

    while (!app.close())
    {
        app.update();
        app.render();
    }

    // Terminate GLFW
    glfwTerminate();

    // Exit program
    exit(EXIT_SUCCESS);
}
