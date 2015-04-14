#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <math.h>

class Camera
{
	private:
		struct VectorType
		{
			float x, y, z;
		};

	public:
		Camera();
		Camera(const Camera&);
		~Camera();

		void SetPosition(float, float, float);
		void SetRotation(float, float, float);

		void Render();
		void GetViewMatrix(float*);
		void Rotate(float, float, float);

	private:
		void MatrixRotationYawPitchRoll(float*, float, float, float);
		void TransformCoord(VectorType&, float*);
		void BuildViewMatrix(VectorType, VectorType, VectorType);

		float m_posX, m_posY, m_posZ;
		float m_rotX, m_rotY, m_rotZ;
		float m_viewMatrix[16];
};


#endif