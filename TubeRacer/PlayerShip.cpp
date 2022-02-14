#include "PlayerShip.h"

#include "OpenGL-CoreLib/Utils.h"

constexpr char vertShader[] = R"(#version 330
	
    layout (location = 0) in vec3 aPosition;
    layout (location = 1) in vec3 aNormalVertex;
    layout (location = 2) in vec2 aTexCoord;
    layout (location = 3) in vec4 aColor;
    layout (location = 4) in vec3 aTangent;
    layout (location = 5) in vec3 aBiTangent;

    out vec4 v_color;
    out vec2 v_TexCoord;
    out vec3 v_world;
    out vec3 v_FragPos;
    out mat3 TBN;

    uniform mat4 modelMatrix;
    uniform mat4 viewMatrix;
    uniform mat4 projectionMatrix;

    void main()
    {
        gl_Position = projectionMatrix * viewMatrix  * modelMatrix * vec4(aPosition.xyz, 1);
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

constexpr char fragShader[] = R"(#version 400
    in vec4             v_color;
    in vec2             v_TexCoord;
    in vec3             v_FragPos;
    in mat3             TBN;

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
        float ambientStrength = 0.7;
        vec3 ambient = ambientStrength * lightColor;

        textureCol = texture(abdeloTexture, v_TexCoord.st);
        //textureCol = texture(abdeloTexture, vec2(0.5, 0.5));
        normalVal = texture(normalTexture, v_TexCoord.st);

        // diffuse 
        vec3 normal = normalize(vec3(normalVal.xyz));
        normal = normalize(normal * 2.0 - 1.0);   
        normal = normalize(TBN * normal);   

        vec3 lightDir = normalize(lightPos - v_FragPos);  
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        vec4 result = (vec4(ambient,1.0) + vec4(diffuse,1.0)) * textureCol;

        gl_FragColor = result;
        //gl_FragColor = textureCol;
    }
)";

PlayerShip::PlayerShip(const mycoretools::Timer& timer)
    :_Timer(timer)
{

}

bool PlayerShip::load(const std::string_view fileName)
{
    std::vector<std::string> errLog;
    if (!_Shader.makeShaderFromSource(vertShader, fragShader, errLog))
    {
        // Error Handling
        return false;
    }


    if (auto model = mycoretools::Utils::loadOBJ(std::string(fileName)); model.has_value())
        _MeshGroup = model.value();
    else
    {
        // Error Handling
        return false;
    }

    for (const auto& mesh : _MeshGroup.getSubMeshes())
        _GPUMeshes.emplace_back().fromMesh(mesh);

    _MatrixModel = glm::translate(glm::mat4(1.0), _Pos);

    _Shader.bind();
    auto attribLoc = _Shader.getUniformLocation("lightPos");
    _Shader.setUniform3f(attribLoc, { 5, 10, 0 });

    attribLoc = _Shader.getUniformLocation("lightColor");
    _Shader.setUniform3f(attribLoc, { 0.9, 0.9, 0.9 });
    _Shader.release();

    _BoundingBox = calcBoundingBox(_MeshGroup);

    return true;
}

void PlayerShip::scale(const glm::vec3& scale)
{
    // scale _MatrixMode

    // scale BoundingBox
}


void PlayerShip::render(const mycoretools::Camera& camera)
{
    _Shader.bind();
    // Matrices
    
    auto locModelMatrix = _Shader.getUniformLocation("modelMatrix");
    _Shader.setUniformMat4(locModelMatrix, _MatrixModel);

    auto locViewMatrix = _Shader.getUniformLocation("viewMatrix");
    _Shader.setUniformMat4(locViewMatrix, camera.getViewMatrix());

    auto locProjMatrix = _Shader.getUniformLocation("projectionMatrix");
    _Shader.setUniformMat4(locProjMatrix, camera.getProjectionMatrix());

    for (auto& gpuMesh : _GPUMeshes)
        gpuMesh.render(_Shader);

    _Shader.release();
}

void PlayerShip::translate(const glm::vec3& vec)
{
    _Pos += vec;
    _BoundingBox.translate(vec);
    _MatrixModel = glm::translate(glm::mat4(1.0), _Pos);
}

void PlayerShip::setDir(const glm::vec3& vec)
{
    _Dir = glm::normalize(vec);
}



const glm::vec3& PlayerShip::getPosition() const
{
    return _Pos;
}

const glm::vec3& PlayerShip::getDirection() const
{
    return _Dir;
}

void PlayerShip::onKeyEvent(int key, int scancode, int action, int mode)
{
    if (action == 1)
        _KeysPressed[key] = _Timer.elapsedMilliseconds();
    else if (action == 0)
        _KeysPressed.erase(key);

    int test(0);
}

void PlayerShip::update(const mycoretools::CatmullSpline& catmullSpline)
{
    processKeyForwardAndBackward();
    //processKeyBackward();
    processKeyRightLeft();

    moveAndRotateShip();

    int test(0);
}

const mycoretools::BoundingBox& PlayerShip::getBoundingBox() const
{
    return _BoundingBox;
}


mycoretools::BoundingBox PlayerShip::calcBoundingBox(const mycoretools::MeshGroup& meshGroup) const
{
    mycoretools::BoundingBox bb;

    for (const auto& subMesh : meshGroup.getSubMeshes())
        for (const auto& vert : subMesh.getVertices())
            bb.expandBy(vert._Position);

    return bb;
}

void PlayerShip::moveAndRotateShip()
{
    auto newPos = _Pos + _Dir * (float)_Velocity;
    _MatrixModel = glm::translate(glm::mat4(1.0), newPos) * glm::rotate(glm::mat4(1), (float)glm::radians(_AngleHorz), _Up);
    auto transVec = newPos - _Pos;
    _BoundingBox.translate(transVec);
    _Pos = newPos;
}

void PlayerShip::processKeyForwardAndBackward()
{
    if (auto findKeyUp = _KeysPressed.find(static_cast<int>(Keys::eForward)); findKeyUp != _KeysPressed.end())
    {
        auto lastTimePressed = findKeyUp->second;
        _Velocity += _AccelerationPerMS * (_Timer.elapsedMilliseconds() - lastTimePressed);

        _Velocity = std::clamp(_Velocity, 0.0, _VelocityMax);
    }
    else if (auto findKeyBack = _KeysPressed.find(static_cast<int>(Keys::eBack)); findKeyBack != _KeysPressed.end())
    {
        auto lastTimePressed = findKeyBack->second;
        _Velocity -= _AccelerationPerMS * (_Timer.elapsedMilliseconds() - lastTimePressed);

        _Velocity = std::clamp(_Velocity, -_VelocityMax, 0.0);
    }
    else
    {
        if (_LastTimeVelocityReduced < 0)
            _LastTimeVelocityReduced = _Timer.elapsedMilliseconds();

        if (_Velocity < 0)
        {
            _Velocity += _AccelerationPerMS * 15.0 * (_Timer.elapsedMilliseconds() - _LastTimeVelocityReduced);
            _LastTimeVelocityReduced = _Timer.elapsedMilliseconds();
        }
        else if (_Velocity > 0)
        {
            _Velocity -= _AccelerationPerMS * 15.0 * (_Timer.elapsedMilliseconds() - _LastTimeVelocityReduced);
            _LastTimeVelocityReduced = _Timer.elapsedMilliseconds();
        }

    }
}

void PlayerShip::processKeyBackward()
{
    
    if (auto findKeyUp = _KeysPressed.find(static_cast<int>(Keys::eBack)); findKeyUp != _KeysPressed.end())
    {
        auto lastTimePressed = findKeyUp->second;
        _Velocity -= _AccelerationPerMS * (_Timer.elapsedMilliseconds() - lastTimePressed);

        _Velocity = std::clamp(_Velocity, -_VelocityMax, 0.0);
    }
    else
    {
        _Velocity += _AccelerationPerMS * 15.0 * (_Timer.elapsedMilliseconds() - _LastTimeVelocityReduced);
        _LastTimeVelocityReduced = _Timer.elapsedMilliseconds();
        if (_Velocity > 0.0)
            _Velocity = 0.0;
    }
}


void PlayerShip::processKeyRightLeft()
{
    if (auto findKeyRight = _KeysPressed.find(static_cast<int>(Keys::eRight)); findKeyRight != _KeysPressed.end())
    {
        _AngleHorz -= _RotationDegreePerMS * (_Timer.elapsedMilliseconds() - findKeyRight->second);
        findKeyRight->second = _Timer.elapsedMilliseconds();
    }

    if (auto findKeyLeft = _KeysPressed.find(static_cast<int>(Keys::eLeft)); findKeyLeft != _KeysPressed.end())
    {
        _AngleHorz += _RotationDegreePerMS * (_Timer.elapsedMilliseconds() - findKeyLeft->second);
        findKeyLeft->second = _Timer.elapsedMilliseconds();
    }

    glm::vec3 direction(cos(0) * sin(glm::radians(_AngleHorz)), sin(0), cos(0) * cos(glm::radians(_AngleHorz)));
    _Dir = direction;

    moveAndRotateShip();
}


