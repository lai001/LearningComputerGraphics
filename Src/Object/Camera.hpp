#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Interface/Input.hpp"

class FCamera: public IInput
{
public:
	FCamera();
	FCamera(const int WindowWidth, const int WindowHeight);
	~FCamera();

private:
	int ScreenWidth = 800;
	int ScreenHeight = 600;
	float Sensitivity = 0.2f;
	float ScrollSensitivity = 1.0f;
	float MaxFov = 90.0f;
	float MinFov = 1.0f;
	float MaxCameraSpeed = 10.0f;
	float MinCameraSpeed = 0.01f;
	float CameraSpeed = 0.05f;
	bool bFirstMouse = true;

	glm::mat4 View = glm::mat4(1.0f);
	glm::mat4 Projection = glm::mat4(1.0f);

	float Yaw = 90.0f;
	float Pitch = 0.0f;
	float LastX = 0.0f;
	float LastY = 0.0f;
	float Fov = 45.0f;

	glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
	glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

public:
	bool IsMouseMovable = false;
	bool IsMovable = true;
	std::string Name;

public:
	void IncreaseCameraSpeed(float Dx);
	void SetCameraSpeed(float Speed);
	float GetCameraSpeed();
	float GetFov();
	void SetFov(float Fov);
	glm::vec3 GetCameraFront();

	void MoveForward();
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	glm::mat4 GetViewMat();
	glm::mat4 GetprojectionMat();
	glm::vec3 GetPosition();
	void SetPosition(const glm::vec3& NewPosition);

	virtual void NotifyKeyInput(Keyboard::KeyType KeyType, Keyboard::ActionType ActionType, Keyboard::ModeFlag ModeFlag) override;
	virtual void NotifyKeyActionChange(std::function<Keyboard::ActionType(Keyboard::KeyType)> select) override;
	virtual void NotifyMousePositionChange(double Xpos, double Ypos) override;
	virtual void NotifyMouseScroll(double Xoffset, double Yoffset) override;
};
