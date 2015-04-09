#include "camera.h"

Camera::Camera()
{

	m_posX = 0.0f;
	m_posY = 0.0f;
	m_posZ = 0.0f;

	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_rotZ = 0.0f;
}

Camera::Camera(const Camera& other)
{

}

Camera::~Camera()
{

}

void Camera::SetPosition(float x, float y, float z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
	return;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotX = x;
	m_rotY = y;
	m_rotZ = z;
	return;
}

void Camera::Render()
{
	VectorType up, position, lookAt;
	float yaw, pitch, roll;
	float rotationMatrix[9];

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = m_posX;
	position.y = m_posY;
	position.z = m_posZ;

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	pitch = (m_rotX * 3.14159265359f) / 180.0f;
	yaw = (m_rotY* 3.14159265359f) / 180.0f;
	roll = (m_rotZ* 3.14159265359f) / 180.0f;

	MatrixRotationYawPitchRoll(rotationMatrix, yaw, pitch, roll);

	TransformCoord(lookAt, rotationMatrix);
	TransformCoord(up, rotationMatrix);

	lookAt.x = position.x + lookAt.x;
	lookAt.y = position.y + lookAt.y;
	lookAt.z = position.z + lookAt.z;

	BuildViewMatrix(position, lookAt, up);

	return;
}

void Camera::MatrixRotationYawPitchRoll(float* matrix, float yaw, float pitch, float roll)
{
	float cYaw, cPitch, cRoll, sYaw, sPitch, sRoll;

	cYaw   = cosf(yaw);
	cPitch = cosf(pitch);
	cRoll  = cosf(roll);

	sYaw   = sinf(yaw);
	sPitch = sinf(pitch);
	sRoll  = sinf(roll);

	matrix[0] = (cRoll * cYaw) + (sRoll * sPitch * sYaw);
	matrix[1] = (sRoll * cPitch);
	matrix[2] = (cRoll * -sYaw) + (sRoll * sPitch * cYaw);

	matrix[3] = (-sRoll * cYaw) + (cRoll * sPitch * sYaw);
	matrix[4] = (cRoll * cPitch);
	matrix[5] = (sRoll * sYaw) + (cRoll * sPitch * cYaw);

	matrix[6] = (cPitch * sYaw);
	matrix[7] = -sPitch;
	matrix[8] = (cPitch * cYaw);


	return;
}

void Camera::TransformCoord(VectorType& vector, float* matrix)
{
	float x, y, z;

	x = (vector.x * matrix[0]) + (vector.y * matrix[3]) + (vector.z * matrix[6]);
	y = (vector.x * matrix[1]) + (vector.y * matrix[4]) + (vector.z * matrix[7]);
	z = (vector.x * matrix[2]) + (vector.y * matrix[5]) + (vector.z * matrix[8]);

	vector.x = x;
	vector.y = y;
	vector.z = z;

	return;
}

void Camera::BuildViewMatrix(VectorType position, VectorType lookAt, VectorType up)
{
	VectorType zAxis, xAxis, yAxis;
	float length, result1, result2, result3;

	zAxis.x = lookAt.x - position.x;
	zAxis.y = lookAt.y - position.y;
	zAxis.z = lookAt.z - position.z;
	length = sqrt((zAxis.x * zAxis.x) + (zAxis.y * zAxis.y) + (zAxis.z * zAxis.z));
	zAxis.x = zAxis.x / length;
	zAxis.y = zAxis.y / length;
	zAxis.z = zAxis.z / length;


	xAxis.x = (up.y * zAxis.z) - (up.z * zAxis.y);
	xAxis.y = (up.z * zAxis.x) - (up.x * zAxis.z);
	xAxis.z = (up.x * zAxis.y) - (up.y * zAxis.x);
	length = sqrt((xAxis.x * xAxis.x) + (xAxis.y * xAxis.y) + (xAxis.z * xAxis.z));
	xAxis.x = xAxis.x / length;
	xAxis.y = xAxis.y / length;
	xAxis.z = xAxis.z / length;


	yAxis.x = (zAxis.y * xAxis.z) - (zAxis.z * xAxis.y);
	yAxis.y = (zAxis.z * xAxis.x) - (zAxis.x * xAxis.z);
	yAxis.z = (zAxis.x * xAxis.y) - (zAxis.y * xAxis.x);

	result1 = ((xAxis.x * position.x) + (xAxis.y * position.y) + (xAxis.z * position.z)) * -1.0f;

	result2 = ((yAxis.x * position.x) + (yAxis.y * position.y) + (yAxis.z * position.z)) * -1.0f;

	result3 = ((zAxis.x * position.x) + (zAxis.y * position.y) + (zAxis.z * position.z)) * -1.0f;
	
	m_viewMatrix[0]  = xAxis.x;
	m_viewMatrix[1]  = yAxis.x;
	m_viewMatrix[2]  = zAxis.x;
	m_viewMatrix[3]  = 0.0f;

	m_viewMatrix[4]  = xAxis.y;
	m_viewMatrix[5]  = yAxis.y;
	m_viewMatrix[6]  = zAxis.y;
	m_viewMatrix[7]  = 0.0f;

	m_viewMatrix[8]  = xAxis.z;
	m_viewMatrix[9]  = yAxis.z;
	m_viewMatrix[10] = zAxis.z;
	m_viewMatrix[11] = 0.0f;

	m_viewMatrix[12] = result1;
	m_viewMatrix[13] = result2;
	m_viewMatrix[14] = result3;
	m_viewMatrix[15] = 1.0f;

	return;
}

void Camera::GetViewMatrix(float* matrix)
{

	matrix[0] = m_viewMatrix[0];
	matrix[1] = m_viewMatrix[1];
	matrix[2] = m_viewMatrix[2];
	matrix[3] = m_viewMatrix[3];

	matrix[4] = m_viewMatrix[4];
	matrix[5] = m_viewMatrix[5];
	matrix[6] = m_viewMatrix[6];
	matrix[7] = m_viewMatrix[7];

	matrix[8] = m_viewMatrix[8];
	matrix[9] = m_viewMatrix[9];
	matrix[10] = m_viewMatrix[10];
	matrix[11] = m_viewMatrix[11];

	matrix[12] = m_viewMatrix[12];
	matrix[13] = m_viewMatrix[13];
	matrix[14] = m_viewMatrix[14];
	matrix[15] = m_viewMatrix[15];

	return;
}
