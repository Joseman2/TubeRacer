#pragma once
#include "OpenGL-CoreLib/Camera.h"


class CameraDeveloper : public mycoretools::Camera
{
public:
	CameraDeveloper() = default;
	~CameraDeveloper() override = default;

	void onMouseWheel(bool forward) override;
	void onMouseButtonPressed(int keyCode, const glm::vec2& pos) override;
	void onMouseButtonReleased(int keyCode) override;
	void onMouseMovement(double xPos, double yPos) override;

//private:
//	void calcViewMatrix() override;

private:
	glm::vec3       _Pivot{ 0,0,0 };
	float           _DistToPivot{ 1.0 };
	float           _DistanceOnMouseWheel{ 0.5 };

};

