#pragma once
#include <Windows.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Dense"
#include <memory>
#include "ShapeFactory/ShapeData.h"
#include "Camera.h"

/// <summary>
/// 직육면체를 마우스로 회전시키고, 삼각형 색상을 매초마다 바뀌게 표현한다.
/// 색상 변경은 Fragment Shader에서 진행한다.
/// 
/// 
/// </summary>

class CObjTestDisplay
{
private:
	class CObjDisplayInputManager
	{
	private:

		int m_prev_x;
		int m_prev_y;

		// 카메라 translate하기 위한 초기값
		Eigen::Vector4f m_startRightVec;
		Eigen::Vector4f m_startUpVec;

		bool m_bMove;
		CObjTestDisplay* m_pObjDisplay;
		
	//private
	private:
		CObjDisplayInputManager();
		CObjDisplayInputManager(CObjTestDisplay* pObjDisplay);

	//public
	public:
		static CObjDisplayInputManager& GetInstance();
		static void MouseMoveCallback(GLFWwindow* pWnd, double x, double y);
		static void MouseWheelCallback(GLFWwindow* pWnd, double xoff, double yoff);
		static void MouseButtonCallback(GLFWwindow* pWnd, int button, int action, int mod);
		void SetObjDisplay(CObjTestDisplay* pObjDisplay);
		void SetMove(bool bStart);


	};

private:
	//glfw
	GLFWwindow* m_pGFWindow;
	
	// camera
	Camera m_Camera;

	// shader
	GLuint m_vtxShaderHandle;
	GLuint m_fragShaderHandle;
	GLuint m_programHandle;

	// vao, vbo, ibo
	GLuint m_vtxArray;
	GLuint m_vtxBuffer;
	GLuint m_vtxIndexBuffer;

	//shape
	CShape m_Shape;

	bool m_bShapeChange;
	
private:
	bool InitGLFW();
	void SetupShader();
	void SetupBuffersAndObject();
	void SetupCamera();
	void UpdateObject();
	void UpdateDisplay();

public:
	// CModuleRunner을(를) 통해 상속됨
	CObjTestDisplay();
	void Run(int argc, char** argv);
	~CObjTestDisplay();

	friend CObjTestDisplay::CObjDisplayInputManager;
};