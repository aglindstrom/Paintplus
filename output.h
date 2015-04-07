#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "ogl.h"
#include "camera.h"
#include "model.h"
#include "colorShader.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const bool VSYNC_ENABLED = false;

class output
{
	public:
		output();
		output(const output&);
		~output();

		bool Initialize(OGL* ogl, HWND hwnd);
		bool Frame();
		void Shutdown();

	private:
		bool Render();

		OGL* m_openGL;
		Camera* m_camera;
		Model* m_model;
		ColorShader* m_shader;
};

#endif