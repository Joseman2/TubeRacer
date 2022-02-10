#include "Application.h"

#include "OpenGL-CoreLib/Utils.h"

constexpr char tubeShader_Vert[] = R"(#version 330
	
    layout (location = 0) in vec3 aPosition;
    layout (location = 1) in vec3 aNormalVertex;
    layout (location = 2) in vec2 aTexCoord;
    layout (location = 3) in vec4 aColor;
    layout (location = 4) in vec3 aTangent;
    layout (location = 5) in vec3 aBiTangent;

	//attribute vec4  aColor;
    //uniform vec3    lightPos;

    out vec4 v_color;
    out vec2 v_TexCoord;
    out vec3 v_world;
    out vec3 v_FragPos;
    out vec3 v_Tangent;
    out vec3 v_BiTangent;
    out mat3 TBN;


    //out vec3 v_Normal;
    //uniform mat4 mvp_matrix;
    uniform mat4 viewMatrix;
    uniform mat4 projectionMatrix;

    void main()
    {
        gl_Position = projectionMatrix * viewMatrix  * vec4(aPosition.xyz, 1);
        v_color = aColor;
        v_world = aPosition;
        v_TexCoord  = aTexCoord;
        v_FragPos = aPosition.xyz;

        vec3 T = normalize(aTangent);
        vec3 B = normalize(aBiTangent);
        vec3 N = normalize(aNormalVertex);
        TBN = mat3(T, B, N);
    }
)";

constexpr char tubeShader_Frag[] = R"(#version 400
    in vec4             v_color;
    in vec2             v_TexCoord;
    in vec3             v_FragPos;
    in mat3             TBN;

//debug
    in vec3 v_Tangent;
    in vec3 v_BiTangent;

    uniform vec3        lightPos; 
    uniform vec3        lightColor;
    uniform vec3        viewPos;

    layout (binding = 1) uniform sampler2D abdeloTexture;
    layout (binding = 2) uniform sampler2D normalTexture;

    vec4 textureCol;
    vec4 normalVal;

    void main()
    {
        // ambient
        float ambientStrength = 0.8;
        vec3 ambient = ambientStrength * lightColor;

        textureCol = texture(abdeloTexture, v_TexCoord.st);
        normalVal = texture(normalTexture, v_TexCoord.st);

        // diffuse 
        vec3 normal = normalize(vec3(normalVal.xyz));
        normal = normalize(normal * 2.0 - 1.0);   
        normal = normalize(TBN * normal);   

        vec3 lightDir = normalize(lightPos - v_FragPos);  
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        vec4 result = (vec4(ambient,1.0) + vec4(diffuse,1.0)) * textureCol;

        //result = vec4(v_BiTangent, 1.0);
        gl_FragColor = result;
    }
)";


bool Application::init()
{
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.4, 0.3, 0.7, 1.0);
  
    if (auto catmull(mycoretools::CatmullSpline::loadCatmull("Resources/bezier_1.bzr")); catmull.has_value())
        _CatmullSpline = catmull.value();
    else
    {
        //Error Handling
    }

    if (std::vector<std::string> errLog; !_ShaderTube.makeShaderFromSource(tubeShader_Vert, tubeShader_Frag, errLog))
        return false;

    _Camera.setPos({ 0, 0, 3 });
    _Camera.setDir({ 0, 0, 1 });
    _Camera.setUp({ 0, 1, 0 });
    
    _ShaderTube.bind();
    auto attribLoc = _ShaderTube.getUniformLocation("lightPos");
    _ShaderTube.setUniform3f(attribLoc, {0, 10, 0});

    attribLoc = _ShaderTube.getUniformLocation("lightColor");
    _ShaderTube.setUniform3f(attribLoc, { 0.9, 0.9, 0.9 });

    _ShaderTube.release();
    return true;
}

bool Application::start()
{
    //if (!_AudioManager.playSound())
    //    _ErrorLog.push_back("Could not start sound!");

    _TubeSegmentBuilder.buildTubeSegments(_CatmullSpline.getPathes());

    for (const auto& aTubeSegment : _TubeSegmentBuilder.getTubeSegments())
    {
        auto aTubeSegmentRenderData = TubeSegmentRenderData::makeFromTubeSegmentBuilder(aTubeSegment, _TubeSegmentBuilder.getVertices(), _TextureContainer);
        _TubeSegmentRenderDatas.emplace_back(std::move(aTubeSegmentRenderData));
    }

    return true;
}

void Application::update()
{
    ApplicationBase::update();
}

void Application::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& aTubeSegmentRenderData : _TubeSegmentRenderDatas)
        aTubeSegmentRenderData.render(_ShaderTube, _Camera);

    glfwSwapBuffers(_GLFWwindow);
    glfwPollEvents();
}

void Application::reshape(int width, int height)
{
    ApplicationBase::reshape(width, height);
    _Camera.setPerspective({ width, height }, 45.0, float(width) / float(height), 1, 100);
}


std::vector<std::string> Application::getErrorLog() const
{
    return _ErrorLog;
}

void Application::onKeyEvent(int key, int scancode, int action, int mods)
{
    ApplicationBase::onKeyEvent(key, scancode, action, mods);

    _Camera.onKeyEvent(key, scancode, action, mods);
}

void Application::onMouseMovement(double xPos, double yPos)
{
    ApplicationBase::onMouseMovement(xPos, yPos);

    _MousePose = { xPos,yPos };

    _Camera.onMouseMovement(xPos, yPos);
}

void Application::onMouseButton(int button, int action, int mods)
{
    ApplicationBase::onMouseButton(button, action, mods);

    if (action == 1)
        _Camera.onMouseButtonPressed(button, _MousePose);
    else if (action == 0)
        _Camera.onMouseButtonReleased(button);
}

void Application::onMouseWheel(double xoffset, double yoffsets)
{
    _Camera.onMouseWheel(yoffsets < 0 ? false : true);
}
