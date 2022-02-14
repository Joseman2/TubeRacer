#include "CameraDeveloper.h"

void CameraDeveloper::onMouseWheel(bool forward)
{
    if (forward)
        _DistToPivot -= _DistanceOnMouseWheel;
    else
        _DistToPivot += _DistanceOnMouseWheel;

    if (_DistToPivot < _DistanceOnMouseWheel)
        _DistToPivot += _DistanceOnMouseWheel;

    calcViewMatrix();
    //calcViewMatrix_EgoPerspective();
}

void CameraDeveloper::onMouseButtonPressed(int keyCode, const glm::vec2& pos)
{
    // Qt Button Value
    // LeftButton = 0x00000001,
    // RightButton = 0x00000002,
    // MiddleButton = 0x00000004,

    if (keyCode == 2 && !_MiddleMouseButtonPressed)
    {
        _MiddleMouseButtonPressed = true;
        _PosLastTimeWhenMiddleMouseButtonPressed = pos;
    }

    if (keyCode == 0x00000001 && !_RightMouseButtonPressed)
    {
        _RightMouseButtonPressed = true;
        _PosLastTimeWhenRightMouseButtonPressed = pos;
    }
}

void CameraDeveloper::onMouseButtonReleased(int keyCode)
{
    if (keyCode == 2)
        _MiddleMouseButtonPressed = false;

    if (keyCode == 0x00000001)
        _RightMouseButtonPressed = false;
}


void CameraDeveloper::onMouseMovement(double xPos, double yPos)
{
    const glm::vec2 pos((float)xPos, (float)yPos);

    // Rotate around Pivot
    if (_MiddleMouseButtonPressed)
    {
        float deltaX = pos.x - _PosLastTimeWhenMiddleMouseButtonPressed.x;
        float degreeMovementX = (3.14f/* / 2.0f*/ * deltaX) / _ViewportSize.x;       // this is maybe not correct => but it works.Maybe one day do it correct
        _HorizontalAngle -= glm::radians(degreeMovementX * 20);

        float deltaY = pos.y - _PosLastTimeWhenMiddleMouseButtonPressed.y;
        float degreeMovementY = (3.14f / 2.0 * deltaY) / _ViewportSize.y;       // this is maybe not correct => but it works. Maybe one day do it correct
        _VerticalAngle += glm::radians(degreeMovementY * 20);

        if (_VerticalAngle < -3.14f / 2.0)
            _VerticalAngle = -3.14f / 2.0;

        if (_VerticalAngle >= 3.14f / 2.0)
            _VerticalAngle = 3.14f / 2.0;

        calcViewMatrix();
        //calcViewMatrix_EgoPerspective();

        _PosLastTimeWhenMiddleMouseButtonPressed = pos;
    }

    // move Pivot
    if (_RightMouseButtonPressed)
    {
        glm::vec2 delta = (pos - _PosLastTimeWhenRightMouseButtonPressed);
        delta.x = delta.x / 200.0f;
        delta.y = delta.y / 600.0f;

        glm::vec3 dirToPivot(_Pivot - _Pos);

        glm::vec3 cameraRight = glm::inverse(_ViewMatrix) * glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 cameraUp = glm::cross(dirToPivot, cameraRight);

        auto newPosRight = cameraRight * delta.x;
        auto newPosUp = cameraUp * delta.y;

        auto newDelta = glm::vec3((newPosRight.x + newPosUp.x) / 2.0, (newPosRight.y + newPosUp.y) / 2.0, (newPosRight.z + newPosUp.z) / 2.0);

        _Pos += newDelta;
        _Pivot += newDelta;

        calcViewMatrix();

        _PosLastTimeWhenRightMouseButtonPressed = pos;
    }
}

//void CameraDeveloper::calcViewMatrix()
//{
//    glm::vec3 pos(cos(_VerticalAngle) * sin(_HorizontalAngle), sin(_VerticalAngle), cos(_VerticalAngle) * cos(_HorizontalAngle));
//    _Pos = pos * glm::vec3(_DistToPivot, _DistToPivot, _DistToPivot) + _Pivot;
//
//    _Dir = glm::normalize(_Pivot - _Pos);
//    _ViewMatrix = glm::lookAt(_Pos, _Pivot, { 0,1,0 });
//}
