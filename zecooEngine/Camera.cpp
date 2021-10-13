#include "Camera.h"

// Constructor for camera -- initialise with some default values
Camera::Camera(int _SCR_WIDTH, int _SCR_HEIGHT)
{
	SCR_WIDTH = _SCR_WIDTH;
	SCR_HEIGHT = _SCR_WIDTH;

	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_view = glm::vec3(0.0f, 0.0f, 0.0f);
	m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	m_speed = 5.025f;
}
Camera::~Camera()
{}

// Set the camera at a specific position, looking at the view point, with a given up vector
void Camera::Set(glm::vec3& position, glm::vec3& viewpoint, glm::vec3& upVector)
{
	m_position = position;
	m_view = viewpoint;
	m_upVector = upVector;
}

// Respond to mouse movement
void Camera::SetViewByMouse()
{
	int middle_x = SCR_WIDTH >> 1;
	int middle_y = SCR_HEIGHT >> 1;

	float angle_y = 0.0f;
	float angle_z = 0.0f;
	static float rotation_x = 0.0f;

	POINT mouse;
	GetCursorPos(&mouse);

	if (mouse.x == middle_x && mouse.y == middle_y) {
		return;
	}

	SetCursorPos(middle_x, middle_y);

	angle_y = (float)(middle_x - mouse.x) / 1000.0f;
	angle_z = (float)(middle_y - mouse.y) / 1000.0f;

	rotation_x -= angle_z;

	float maxAngle = 1.56f; // Just a little bit below PI / 2

	if (rotation_x > maxAngle) 
	{
		rotation_x = maxAngle;
	}
	else if (rotation_x < -maxAngle)
	{
		rotation_x = -maxAngle;
	}
	else 
	{
		glm::vec3 cross = glm::cross(m_view - m_position, m_upVector);
		glm::vec3 axis = glm::normalize(cross);

		//RotateViewPointMouse(angle_z, axis);
	}

	glm::vec3 y = glm::vec3(0, 1, 0);
	//RotateViewPointMouse(angle_y, y);
}

// Rotate the camera view point -- this effectively rotates the camera since it is looking at the view point
void Camera::RotateViewPointMouse( float radius, float angle )
{
	Set( m_position, m_view, m_upVector );
}

// Rotate the camera view point -- this effectively rotates the camera since it is looking at the view point
void Camera::RotateViewPoint(float radius, float angle)
{
	float camX = sin(angle) * radius;
	float camZ = cos(angle) * radius;

	glm::vec3 pos = glm::vec3(camX, m_position.y, camZ);

	Set(pos, m_view, m_upVector);
}

// Strafe the camera (side to side motion)
void Camera::Strafe(double direction)
{
	float speed = (float)(m_speed * direction);

	m_position.x = m_position.x + m_strafeVector.x * speed;
	m_position.z = m_position.z + m_strafeVector.z * speed;

	m_view.x = m_view.x + m_strafeVector.x * speed;
	m_view.z = m_view.z + m_strafeVector.z * speed;
}

// Advance the camera (forward / backward motion)
void Camera::Advance(double direction)
{
	float speed = (float)(m_speed * direction)*10;

	glm::vec3 view = glm::normalize(m_view - m_position);

	m_position = m_position + view * speed;
	m_view = m_view + view * speed;
}

// Update the camera to respond to mouse motion for rotations and keyboard for translation
void Camera::Update(double dt)
{
	glm::vec3 vector = glm::cross(m_view - m_position, m_upVector);
	m_strafeVector = glm::normalize(vector);

	//SetViewByMouse();
	TranslateByKeyboard(dt);
}

// Update the camera to respond to key presses for translation
void Camera::TranslateByKeyboard(double dt)
{
	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80) {
		Advance(5.0 * dt);
	}

	if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80) {
		Advance(-5.0 * dt);
	}

	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {
		Strafe(-5.0 * dt);
	}

	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {
		Strafe(5.0 * dt);
	}
}

// Return the camera position
glm::vec3 Camera::GetPosition() const
{
	return m_position;
}

// Return the camera view point
glm::vec3 Camera::GetView() const
{
	return m_view;
}

// Return the camera up vector
glm::vec3 Camera::GetUpVector() const
{
	return m_upVector;
}

// Return the camera strafe vector
glm::vec3 Camera::GetStrafeVector() const
{
	return m_strafeVector;
}

// Return the camera perspective projection matrix
glm::mat4 Camera::GetPerspectiveProjectionMatrix()
{
	return m_perspectiveProjectionMatrix;
}

// Return the camera orthographic projection matrix
glm::mat4* Camera::GetOrthographicProjectionMatrix()
{
	return &m_orthographicProjectionMatrix;
}

// Set the camera perspective projection matrix to produce a view frustum with a specific field of view, aspect ratio, 
// and near / far clipping planes
void Camera::SetPerspectiveProjectionMatrix(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane)
{
	m_perspectiveProjectionMatrix = glm::perspective(fov, aspectRatio, nearClippingPlane, farClippingPlane);
}

// The the camera orthographic projection matrix to match the width and height passed in
void Camera::SetOrthographicProjectionMatrix()
{
	m_orthographicProjectionMatrix = glm::ortho(0.0f, float(SCR_WIDTH), 0.0f, float(SCR_HEIGHT));
}

// Get the camera view matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_position, m_view, m_upVector);
}

// The normal matrix is used to transform normals to eye coordinates -- part of lighting calculations
glm::mat3 Camera::ComputeNormalMatrix(const glm::mat4& modelViewMatrix)
{
	return glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
}