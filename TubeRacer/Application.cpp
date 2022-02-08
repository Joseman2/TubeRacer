#include "Application.h"

#include "OpenGL-CoreLib/Utils.h"

#include "Utils.h"

constexpr char tubeShader_Vert[] = R"(#version 330
	
    layout (location = 0) in vec3 aPosition;
    layout (location = 1) in vec3 aNormalVertex;
    layout (location = 2) in vec2 aTexCoord;

	//attribute vec4  aColor;
    //uniform vec3    lightPos;

    out vec4 v_color;
    out vec2 v_TexCoord;
    out vec3 v_world;
    out vec3 v_FragPos;
    //out vec3 v_Normal;
    //uniform mat4 mvp_matrix;
    uniform mat4 viewMatrix;
    uniform mat4 projectionMatrix;

    void main()
    {
        gl_Position = projectionMatrix * viewMatrix  * vec4(aPosition.xyz, 1);
        v_color = vec4(1,1,1,1);
        v_world = aPosition;
        v_TexCoord  = aTexCoord;
        v_FragPos = aPosition.xyz;
    }
)";

//char const* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"layout (location = 1) in vec3 aNormal;\n"
//"layout (location = 2) in vec2 aTexCoord;\n"
//
//"uniform mat4 viewMatrix;\n"
//"uniform mat4 projectionMatrix;\n"
//"uniform mat4 modelMatrix;\n"
//
//"uniform sampler2D   Texture1;\n"
//
//"out vec2 texCoord;\n"
//"out vec3 fragPos;\n"
//"out vec3 normal;\n"
//
//"vec4 fragPosTemp;\n"
//
//"void main()\n"
//"{\n"
//"   normal = mat3(transpose(inverse(modelMatrix))) * aNormal;\n"
//"   texCoord = aTexCoord;\n"
//"   fragPosTemp = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);\n"
//"   fragPos = vec3(fragPosTemp.x, fragPosTemp.y, fragPosTemp.z);\n"
//"   gl_Position = fragPosTemp;\n"
//
//"   //gl_Position = vec4(aPos, 1.0);\n"
//"}\0";


//const char* fragmentShaderSource = "#version 330 core\n"
//
//"in vec3             normal;\n"
//"in vec3             fragPos;\n"
//"in vec2             texCoord;\n"
//
//"uniform sampler2D   Texture1;\n"
//
//// Material
//"uniform vec3       materialAmbient;\n"
//"uniform vec3       materialDiffuse;\n"
//"uniform vec3       materialSpecular;\n"
//"uniform int        materialShininess;\n"
//
//// Light
//"uniform vec3       lightPosition;\n"
//"uniform vec3       lightAmbient;\n"
//"uniform vec3       lightDiffuse;\n"
//"uniform vec3       lightSpecular;\n"
//
//"uniform vec3       viewPos;\n"
//
//"uniform bool       hasTexture;\n"
//
////Transparancy
//"uniform float      alpha;\n"
//
//"out vec4           FragColor;\n"
//
//"void main()\n"
//"{\n"
//
//// ambient
//"   vec3 ambient = lightAmbient * materialAmbient;\n"
//
//// diffuse 
//"   vec3 norm = normalize(normal);\n"
//"   vec3 lightDir = normalize(lightPosition - fragPos);\n"
//
//"   float diff = max(dot(norm, lightDir), 0.0);\n"
//"   vec3 diffuse = lightDiffuse * (diff * materialDiffuse);\n"
//
//// specular
//"   vec3 viewDir = normalize(viewPos - fragPos);\n"
//"   vec3 reflectDir = reflect(-lightDir, norm);\n"
//"   float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);\n"
//"   vec3 specular = lightSpecular * (spec * materialSpecular);  // <-- !! (y)\n"
//
//"   vec3 result = ambient + diffuse + specular;\n"
//"   vec4 texCol = texture2D(Texture1, texCoord);\n"
//
//"   if (hasTexture == true)\n"
//"       gl_FragColor = texCol * vec4(result, alpha);\n"
//"   else\n"
//"       gl_FragColor = vec4(result, alpha);\n"
//"}\n\0";

    constexpr char tubeShader_Frag[] = R"(#version 400
    in vec4             v_color;
    in vec2             v_TexCoord;
    in vec3             v_FragPos;

    uniform vec3        lightPos; 
    uniform vec3        lightColor;
    uniform vec3        viewPos;

    layout (binding = 1) uniform sampler2D abdeloTexture;
    layout (binding = 2) uniform sampler2D normalTexture;

    //in vec3 v_world, v_normal;
    //out vec4 f_color;
    //uniform vec3 u_eye;

    vec4 textureCol;
    vec4 normalVal;

    void main()
    {
        vec4 a = vec4(1,0,0,0);
        vec4 b = vec4(0,1,0,0);
        vec4 c = vec4(0,0,1,0);
        vec4 d = vec4(v_FragPos,1);
        mat4 matrix = mat4(a,b,c,d);

        // ambient
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;

        textureCol = texture(abdeloTexture, v_TexCoord.st);
        normalVal = texture(normalTexture, v_TexCoord.st);

        // diffuse 
        vec3 norm = normalize(vec3(normalVal.xyz));
        norm = norm * 0.5 + 0.5;
        //norm = mat3(transpose(inverse(matrix))) * norm;  
        vec3 lightDir = normalize(lightPos - v_FragPos);  
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        vec4 result = (vec4(ambient,1.0) + vec4(diffuse,1.0)) * textureCol;

        gl_FragColor = result;
        //gl_FragColor = vec4(lightColor,1.0); //result; //vec4(1,1,1,1);
    }
)";


bool Application::init()
{
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);

    glClearColor(1.0, 0.0, 0.0, 1.0);

    //GLenum glfwSetInputMode(_GLFWwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    if (auto catmull(Utils::loadBezier("Resources/bezier.bzr")); catmull.has_value())
        _CatmullSpline = catmull.value();
    else
    {
        //Error Handling
    }

    if (auto meshGroup = mycoretools::Utils::loadOBJ("D:/Programmierung/Projekte/TubeRacer/TubeRacer/TubeRacer/Assets/Models/cube.obj"))
    {
        const auto& vertices = meshGroup.value().getModels()[0].getVertices();
        const auto& indices = meshGroup.value().getModels()[0].getIndices();
        _GPUMesh.fromData(vertices, indices, mycoretools::GPUMesh::DrawMode::eGL_QUAD_STRIP);
    }

    if (std::vector<std::string> errLog; !_ShaderTube.makeShaderFromSource(tubeShader_Vert, tubeShader_Frag, errLog))
        return false;

    _Camera.setPos({ 0, 0, 3 });
    _Camera.setDir({ 0, 0, 1 });
    _Camera.setUp({ 0, 1, 0 });
    
    _ShaderTube.bind();
    auto attribLoc = _ShaderTube.getUniformLocation("lightPos");
    _ShaderTube.setUniform3f(attribLoc, {0,5,0});

    attribLoc = _ShaderTube.getUniformLocation("lightColor");
    _ShaderTube.setUniform3f(attribLoc, { 0.8, 0.8, 0.8 });

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
        // benötige ich "TubeSegmentRenderData"? 
        //TubeSegmentRenderData aTubeSegmentRenderData(_TubeSegmentBuilder.getVertices());
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
    //ApplicationBase::render();
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
}

void Application::onMouseMovement(double xPos, double yPos)
{
    ApplicationBase::onMouseMovement(xPos, yPos);
}

void Application::onMouseButton(int button, int action, int mods)
{
    ApplicationBase::onMouseButton(button, action, mods);
}
