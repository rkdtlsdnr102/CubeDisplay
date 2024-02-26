#include "ObjTestDisplay.h"
#include "ShapeFactory/SolidShapeFactory.h"
#include "ShapeFactory/WireShapeFactory.h"
#include "GLFWAutolink.h"
#include <cassert>
#include <stdio.h>
#include <random>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#define WND_WIDTH 1920
#define WND_HEIGHT 1600
#define TO_RADIAN(x) (x * ( M_PI / 180.0) )

const char* title = "OBJ TEST DISPLAY";

#define DEFAULT_ARRAY_BUFFER 0

const char* vtxShader =
"#version 400\n"
"uniform mat4 u_MVP;\n"
"layout(location = 0) in vec3 vPos;\n"
"void main()\n"
"{\n"
" gl_Position = u_MVP * vec4( vPos, 1.0f );\n"
" gl_PointSize = 10.0;\n"
"}\n";

const char* fragShader =
"#version 400\n"
"uniform vec3 custom_color;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(custom_color.x, custom_color.y, custom_color.z,1.0f);\n"
"}\n";

void _CheckGLError(const char* file, int line);

#define CheckGLError() _CheckGLError(__FILE__, __LINE__)

void _CheckGLError(const char* file, int line)
{
	GLenum err(glGetError());

	while (err != GL_NO_ERROR)
	{
		std::string error;
		switch (err)
		{
		case GL_INVALID_OPERATION:  error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:       error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:      error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:      error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}
		std::cout << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
		err = glGetError();
	}

	return;
}

inline void InitShader(GLuint& shaderHandle)
{
	if (0 != shaderHandle)
		glDeleteShader(shaderHandle);

	shaderHandle = 0;
}

inline void InitProgram(GLuint& programHandle)
{
	if (0 != programHandle)
		glDeleteShader(programHandle);

	programHandle = 0;
}

bool IsShaderCompiled( int shaderHandle)
{
	int isCompiled;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);

	char errLog[512];

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);
		glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, errLog);

		fprintf_s(stderr, errLog);

		return false;
	}

	return true;
}

bool IsShaderLink(int programHandle)
{
	int success;
	char errLog[512];

	glGetProgramiv(programHandle, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(programHandle, 512, NULL, errLog);
		fprintf_s(stderr, errLog);

		return false;
	}

	return true;
}


static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

bool CObjTestDisplay::InitGLFW()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	m_pGFWindow = glfwCreateWindow(WND_WIDTH, WND_HEIGHT, title, nullptr, nullptr);

	if (nullptr == m_pGFWindow)
	{
		glfwTerminate();
		return false;
	}

	CObjDisplayInputManager& mouseInputMgr = CObjDisplayInputManager::GetInstance();
	mouseInputMgr.SetObjDisplay(this);
	glfwSetCursorPosCallback(m_pGFWindow, CObjDisplayInputManager::MouseMoveCallback);
	glfwSetScrollCallback(m_pGFWindow, CObjDisplayInputManager::MouseWheelCallback);
	glfwSetMouseButtonCallback(m_pGFWindow, CObjDisplayInputManager::MouseButtonCallback);

	glfwMakeContextCurrent(m_pGFWindow);
	gladLoadGL();

	return true;
}

void CObjTestDisplay::SetupShader()
{
	InitShader(m_vtxShaderHandle);
	InitShader(m_fragShaderHandle);
	InitProgram(m_programHandle);

	GLint param;

	m_vtxShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vtxShaderHandle, 1, &vtxShader, nullptr);
	glCompileShader(m_vtxShaderHandle);

	if (IsShaderCompiled(m_vtxShaderHandle))
	{

	}

	m_fragShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragShaderHandle, 1, &fragShader, nullptr);
	glCompileShader(m_fragShaderHandle);

	if (IsShaderCompiled(m_fragShaderHandle))
	{

	}

	m_programHandle = glCreateProgram();
	glAttachShader(m_programHandle, m_vtxShaderHandle);
	glAttachShader(m_programHandle, m_fragShaderHandle);
	glLinkProgram(m_programHandle);

	if (IsShaderLink(m_programHandle))
	{

	}
}

unsigned short indices[] = { 0,1,2 };
static float cube_vtx[] = { 0.25,-0.25,0.25 };

void CObjTestDisplay::SetupBuffersAndObject()
{
	/*CShapeFactory& shapeFactory = CShapeFactory::GetInstance<CWireShapeFactory>();
	m_Shape = shapeFactory.CreateCube();*/
	
	CShapeFactory& shapeFactory = CShapeFactory::GetInstance<CSolidShapeFactory>();
	m_Shape = shapeFactory.CreateCube();
	

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	{
		glGenVertexArrays(1, &m_vtxArray);
		glBindVertexArray(m_vtxArray);

		// 버퍼 생성 및 데이터 전달
		glGenBuffers(1, &m_vtxBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vtxBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_Shape.GetByteSize(), m_Shape.data.get(), GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, 12, cube_vtx, GL_STATIC_DRAW);

		glGenBuffers(1, &m_vtxIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vtxIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Shape.GetIndexByteSize(), m_Shape.pIndices, GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12, indices, GL_STATIC_DRAW);
	}
	

	glVertexAttribPointer(DEFAULT_ARRAY_BUFFER, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	glEnableVertexAttribArray(DEFAULT_ARRAY_BUFFER);
}

void CObjTestDisplay::SetupCamera()
{
	m_Camera.Init();
}

void CObjTestDisplay::UpdateObject()
{
	if (!m_bShapeChange)
		return;

	glVertexAttribPointer(DEFAULT_ARRAY_BUFFER, 1, GL_FLOAT, false, sizeof(GLfloat) * 3, 0);

	m_bShapeChange = false;
}

void CObjTestDisplay::UpdateDisplay()
{
	GLuint mvp_loc = glGetUniformLocation(m_programHandle, "u_MVP");
	GLuint custom_color_loc = glGetUniformLocation(m_programHandle, "custom_color");
	
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<double> dist(0, 1);

	const float* mvpMat = m_Camera.GetMVPMat();

	glUseProgram(m_programHandle);

	glBindVertexArray(m_vtxArray);

	while (!glfwWindowShouldClose(m_pGFWindow))
	{
		int width, height;
		//float ratio;
		
		glfwGetFramebufferSize(m_pGFWindow, &width, &height);

		//ratio = width / (float)height;

		//glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		//MVP matrix를 구성하여 vertex shader로 넘겨준다.
		glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, (const GLfloat*)mvpMat);

		//Color값을 시간에 따라 다르게 하여 넘겨준다
		glUniform3f(custom_color_loc, dist(rng), dist(rng), dist(rng));

		//glDrawElements(m_SolidCubeShape.drawmode, m_SolidCubeShape.nIndices, GL_UNSIGNED_INT, 0);
		glDrawElements(m_Shape.drawmode, m_Shape.nIndices, GL_UNSIGNED_INT, 0);

		//UpdateObject();
		
		glfwSwapBuffers(m_pGFWindow);
		glfwPollEvents();
	}

	glfwDestroyWindow(m_pGFWindow);
}

CObjTestDisplay::CObjTestDisplay() : m_pGFWindow(nullptr), 
									m_vtxShaderHandle( 0 ),
									m_fragShaderHandle( 0 ),
									m_programHandle(0),
									m_vtxArray(0),
									m_vtxBuffer(0),
									m_vtxIndexBuffer(0),
									m_bShapeChange(true)
{
}

void CObjTestDisplay::Run(int argc, char** argv)
{
	// GLFW Window 초기화 및 Mouse Event 생성
	if (false == InitGLFW())
		return;

	// Fragment Shader 생성
	SetupShader();

	SetupBuffersAndObject();

	SetupCamera();

	// 화면 Update를 돌린다.
	UpdateDisplay();
}

CObjTestDisplay::~CObjTestDisplay()
{
}



CObjTestDisplay::CObjDisplayInputManager& CObjTestDisplay::CObjDisplayInputManager::GetInstance()
{
	static CObjTestDisplay::CObjDisplayInputManager objDisplayInputMgr;

	return objDisplayInputMgr;
}

CObjTestDisplay::CObjDisplayInputManager::CObjDisplayInputManager() : m_pObjDisplay(nullptr), m_prev_x( 0 ), m_prev_y(0), m_bMove(false)
{
}

void CObjTestDisplay::CObjDisplayInputManager::MouseMoveCallback(GLFWwindow* pWnd, double x, double y)
{
	CObjDisplayInputManager& inputMgr = CObjTestDisplay::CObjDisplayInputManager::GetInstance();
	CObjTestDisplay* objDisplay = inputMgr.m_pObjDisplay;

	if (nullptr == objDisplay)
		return;

	if (false == inputMgr.m_bMove)
		return;

	if (GLFW_PRESS == glfwGetMouseButton(objDisplay->m_pGFWindow, GLFW_MOUSE_BUTTON_LEFT))
	{
		constexpr float sensitivity = 0.0006;

		float dx = (x - inputMgr.m_prev_x) * sensitivity;
		float dy = (inputMgr.m_prev_y - y) * sensitivity;

		objDisplay->m_Camera.Translate(dx * inputMgr.m_startRightVec, dy * inputMgr.m_startUpVec);

		inputMgr.m_prev_x = x;
		inputMgr.m_prev_y = y;
	}
	else if (GLFW_PRESS == glfwGetMouseButton(objDisplay->m_pGFWindow, GLFW_MOUSE_BUTTON_RIGHT))
	{
		constexpr float sensitivity = 0.5f;

		// 현재 마우스 움직임으로 역산하여 theta, phi 값에 더해준다.
		// 참고 : https://learnopengl.com/Getting-started/Camera
		int dx = (x - inputMgr.m_prev_x) * sensitivity;
		int dy = (inputMgr.m_prev_y - y) * sensitivity;

		objDisplay->m_Camera.Rotate(dx, dy);

		inputMgr.m_prev_x = x;
		inputMgr.m_prev_y = y;


		objDisplay->m_bShapeChange = true;
	}
}

void CObjTestDisplay::CObjDisplayInputManager::MouseWheelCallback(GLFWwindow* pWnd, double xoff, double yoff)
{
	CObjDisplayInputManager& inputMgr = CObjTestDisplay::CObjDisplayInputManager::GetInstance();
	CObjTestDisplay* pObjDisplay = inputMgr.m_pObjDisplay;

	const float sensitivity = 0.1f;

	pObjDisplay->m_Camera.Zoom(sensitivity * yoff);
	
}

void CObjTestDisplay::CObjDisplayInputManager::MouseButtonCallback(GLFWwindow* pWnd, int button, int action, int mod)
{
	switch (button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
		{
			if (GLFW_PRESS == action)
			{
				CObjDisplayInputManager& inputMgr = CObjTestDisplay::CObjDisplayInputManager::GetInstance();
				Camera& cam = inputMgr.m_pObjDisplay->m_Camera;

				inputMgr.SetMove(true);

				double xPos, yPos;

				glfwGetCursorPos(pWnd, &xPos, &yPos);

				inputMgr.m_prev_x = xPos;
				inputMgr.m_prev_y = yPos;

				inputMgr.m_startRightVec = cam.GetRightVec();
				inputMgr.m_startUpVec = cam.GetUpVec();

			}

			if (GLFW_RELEASE == action)
			{
				CObjDisplayInputManager& inputMgr = CObjTestDisplay::CObjDisplayInputManager::GetInstance();

				inputMgr.SetMove(false);
			}
		}
		break;

		case GLFW_MOUSE_BUTTON_RIGHT:
		{
			// 오른쪽 마우스 처음 누른 경우
			if (GLFW_PRESS == action)
			{
				CObjDisplayInputManager& inputMgr = CObjTestDisplay::CObjDisplayInputManager::GetInstance();

				inputMgr.SetMove(true);

				double xPos, yPos;

				glfwGetCursorPos(pWnd, &xPos, &yPos);

				inputMgr.m_prev_x = xPos;
				inputMgr.m_prev_y = yPos;

			}

			// 오른쪽 마우스 떼어낸 경우
			if (GLFW_RELEASE == action)
			{
				CObjDisplayInputManager& inputMgr = CObjTestDisplay::CObjDisplayInputManager::GetInstance();

				inputMgr.SetMove(false);
			}
		}
		break;

		default:
			break;
	}
}

void CObjTestDisplay::CObjDisplayInputManager::SetObjDisplay(CObjTestDisplay* pObjDisplay)
{
	m_pObjDisplay = pObjDisplay;
}

void CObjTestDisplay::CObjDisplayInputManager::SetMove(bool bStart)
{
	m_bMove = bStart;
}
