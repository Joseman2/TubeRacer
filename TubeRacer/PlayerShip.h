#pragma once

#include "OpenGL-CoreLib/MeshGroup.h"
#include "OpenGL-CoreLib/GPUMesh.h"
#include "OpenGL-CoreLib/Shader.h"
#include "OpenGL-CoreLib/Camera.h"
#include "OpenGL-CoreLib/Timer.h"
#include "OpenGL-CoreLib/BoundingBox.h"
#include "OpenGL-CoreLib/CatmullSpline.h"

#include <string>

/*
* KeyCodes:
*
Key:
	w : 87
	s : 83
	a : 65
	d : 68

Scancode:
	w : 17
	s : 31
	a : 30
	d : 32

action : 1 => pressed
action : 0 => released
*/

class PlayerShip
{
private:
	enum class Keys
	{
		eForward = 87,
		eBack = 83,
		eLeft = 65,
		eRight = 68,

		eUnknown = -1
	};
public:
	explicit PlayerShip(const mycoretools::Timer& timer);
	~PlayerShip() = default;

	bool load(const std::string_view fileName);
	void scale(const glm::vec3& scale);
	void render(const mycoretools::Camera& camera);

	void translate(const glm::vec3& vec);
	void setDir(const glm::vec3& vec);

	const glm::vec3& getPosition() const;
	const glm::vec3& getDirection() const;
	void onKeyEvent(int key, int scancode, int action, int mode);

	void update(const mycoretools::CatmullSpline& catmullSpline);

	const mycoretools::BoundingBox& getBoundingBox() const;

private:
	mycoretools::BoundingBox calcBoundingBox(const mycoretools::MeshGroup& meshGroup) const;
	void moveAndRotateShip();
	void processKeyForwardAndBackward();
	void processKeyBackward();
	void processKeyRightLeft();

private:
	std::vector<mycoretools::GPUMesh>	_GPUMeshes;
	mycoretools::MeshGroup				_MeshGroup;
	mycoretools::Shader                 _Shader;
	glm::mat4							_MatrixModel{ 1.0 };
	glm::vec3							_Pos{0, 0, 0};
	glm::vec3							_Dir{0, 0, 1};
	glm::vec3							_Up{0, 1, 0};
	mycoretools::BoundingBox			_BoundingBox;

	double								_TimeLastKeyDown = 0.0;
	std::map<int, double>				_KeysPressed;
	const mycoretools::Timer&			_Timer;
	double								_Velocity = 0.0;
	const double						_VelocityMax = 0.5;
	double								_LastTimeVelocityReduced = -1.0;
	const double						_AccelerationPerMS = 0.00001f;

	const double						_RotationDegreePerMS = 0.05;

	double								_AngleHorz = 0.0;
};

