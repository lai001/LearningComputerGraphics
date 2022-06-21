#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/fast_square_root.hpp>

FCamera::FCamera()
	:ScreenWidth(0), ScreenHeight(0)
{
	View = glm::identity<glm::mat4>();
	Projection = glm::identity<glm::mat4>();
}

FCamera::FCamera(const int WindowWidth, const int WindowHeight)
	:ScreenWidth(WindowWidth), ScreenHeight(WindowHeight)
{
	View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
	Projection = glm::perspective(glm::radians(Fov), (float)WindowWidth / (float)WindowHeight, 0.1f, 1000.0f);
}

FCamera::~FCamera()
{
}

void FCamera::IncreaseCameraSpeed(float Dx)
{
	float X = CameraSpeed + Dx;
	X = glm::max(X, MinCameraSpeed);
	X = glm::min(X, MaxCameraSpeed);
	CameraSpeed = X;
}

void FCamera::SetCameraSpeed(float Speed)
{
	float X = Speed;
	X = glm::max(X, MinCameraSpeed);
	X = glm::min(X, MaxCameraSpeed);
	CameraSpeed = X;
}

glm::vec3 FCamera::GetPosition()
{
	return CameraPos;
}

void FCamera::SetPosition(const glm::vec3 & NewPosition)
{
	CameraPos = NewPosition;
	View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

float FCamera::GetCameraSpeed()
{
	return CameraSpeed;
}

float FCamera::GetFov()
{
	return Fov;
}

void FCamera::SetFov(float Fov)
{
	Fov = Fov * ScrollSensitivity;
	if (Fov >= MinFov && Fov <= MaxFov)

		if (Fov <= MinFov)
			Fov = MinFov;
	if (Fov >= MaxFov)
		Fov = MaxFov;

	this->Fov = Fov;
	Projection = glm::perspective(glm::radians(Fov), (float)ScreenWidth / (float)ScreenHeight, 0.1f, 100.0f);
}

glm::vec3 FCamera::GetCameraFront()
{
	return CameraFront;
}

void FCamera::MoveForward()
{
	CameraPos += CameraFront * CameraSpeed;
	View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

void FCamera::MoveBack()
{
	CameraPos -= CameraFront * CameraSpeed;
	View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

void FCamera::MoveLeft()
{
	CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
	View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

void FCamera::MoveRight()
{
	CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
	View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

void FCamera::MoveUp()
{
	CameraPos += glm::normalize(CameraUp) * CameraSpeed;
	View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

void FCamera::MoveDown()
{
	CameraPos -= glm::normalize(CameraUp) * CameraSpeed;
	View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

glm::mat4 FCamera::GetViewMat()
{
	return View;
}

glm::mat4 FCamera::GetprojectionMat()
{
	return Projection;
}

void FCamera::NotifyKeyInput(Keyboard::KeyType KeyType, Keyboard::ActionType ActionType, Keyboard::ModeFlag ModeFlag)
{
}

void FCamera::NotifyKeyActionChange(std::function<Keyboard::ActionType(Keyboard::KeyType)> select)
{
	if (IsMovable == false)
	{
		return;
	}
	glm::vec3 CameraPos = GetPosition();
	float CameraSpeed = GetCameraSpeed();
	float fixCameraSpeed = CameraSpeed;
	
	if (select(Keyboard::KeyType::KEY_W) == Keyboard::ActionType::Press)
	{
		MoveForward();
	}
	if (select(Keyboard::KeyType::KEY_S) == Keyboard::ActionType::Press)
	{
		MoveBack();
	}
	if (select(Keyboard::KeyType::KEY_A) == Keyboard::ActionType::Press)
	{
		MoveLeft();
	}
	if (select(Keyboard::KeyType::KEY_D) == Keyboard::ActionType::Press)
	{
		MoveRight();
	}
	if (select(Keyboard::KeyType::KEY_Q) == Keyboard::ActionType::Press)
	{
		MoveUp();
	}
	if (select(Keyboard::KeyType::KEY_E) == Keyboard::ActionType::Press)
	{
		MoveDown();
	}
	if (select(Keyboard::KeyType::KEY_KP_DIVIDE) == Keyboard::ActionType::Press)
	{
		IncreaseCameraSpeed(-0.01f);
	}
	if (select(Keyboard::KeyType::KEY_KP_MULTIPLY) == Keyboard::ActionType::Press)
	{
		IncreaseCameraSpeed(0.01f);
	}
}

void FCamera::NotifyMousePositionChange(double Xpos, double Ypos)
{
	if (bFirstMouse)
	{
		LastX = Xpos;
		LastY = Ypos;
		bFirstMouse = false;
	}

	float xoffset = LastX-Xpos;
	float yoffset = LastY-Ypos ;
	LastX = Xpos;
	LastY = Ypos;

	if (!IsMouseMovable)
	{
		return;
	}

	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch > 89.9f)
		Pitch = 89.9f;
	if (Pitch < -89.9f)
		Pitch = -89.9f;

	float FrontVectorLength = glm::fastSqrt(CameraFront.x * CameraFront.x + CameraFront.y * CameraFront.y + CameraFront.z * CameraFront.z);
	float ProjectionyInxyAxisSurface = cos(glm::radians(Pitch)) * FrontVectorLength;

	glm::vec3 Front;
	Front.y = sin(glm::radians(Pitch)) * FrontVectorLength;
	Front.x = cos(glm::radians(Yaw)) * ProjectionyInxyAxisSurface;
	Front.z = sin(glm::radians(Yaw)) * ProjectionyInxyAxisSurface;

	CameraFront = glm::normalize(Front);
	View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

void FCamera::NotifyMouseScroll(double Xoffset, double Yoffset)
{
	float Fov = GetFov();
	Fov -= Yoffset;
	SetFov(Fov);
}