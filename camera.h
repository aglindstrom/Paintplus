#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <math.h>

class Camera
{
	private:
		struct Vector
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

	private:
		void MatrixRotationYawPitchRoll(float*, float, float, float);
		void TransformCoord(Vector&, float*);
		void BuildViewMatrix(Vector, Vector, Vector);

		float m_posX, m_posY, m_posZ;
		float m_rotX, m_rotY, m_rotZ;
		float m_viewMatrix[16];
};


#endif