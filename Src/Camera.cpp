#include "Camera.h"
#include "ThirdParty/glm.h"

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
	CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
	View = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}

void FCamera::MoveRight()
{
	CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
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

void FCamera::MouseCallback(GLFWwindow * Window, double Xpos, double Ypos)
{
	if (bFirstMouse)
	{
		LastX = Xpos;
		LastY = Ypos;
		bFirstMouse = false;
	}

	float xoffset = Xpos - LastX;
	float yoffset = LastY - Ypos;
	LastX = Xpos;
	LastY = Ypos;

	if (!bIsMovable)
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

void FCamera::ProcessInput(GLFWwindow * Window, int Key, int Scancode, int Action, int Mods)
{
}

void FCamera::ContiguousInput(GLFWwindow * Window, int Key)
{
	glm::vec3 CameraPos = GetPosition();
	float CameraSpeed = GetCameraSpeed();
	float fixCameraSpeed = CameraSpeed;

	switch (Key)
	{
	case GLFW_KEY_W:
		MoveForward();
		break;

	case GLFW_KEY_S:
		MoveBack();
		break;

	case GLFW_KEY_A:
		MoveLeft();
		break;

	case GLFW_KEY_D:
		MoveRight();
		break;

	case GLFW_KEY_Q:
		MoveUp();
		break;

	case GLFW_KEY_E:
		MoveDown();
		break;

	case GLFW_KEY_KP_DIVIDE:
		IncreaseCameraSpeed(-0.01f);
		break;

	case GLFW_KEY_KP_MULTIPLY:
		IncreaseCameraSpeed(0.01f);
		break;

	default:
		break;
	}
}

void FCamera::ScrollCallback(GLFWwindow * Window, double Xoffset, double Yoffset)
{
	float Fov = GetFov();
	Fov -= Yoffset;
	SetFov(Fov);
}