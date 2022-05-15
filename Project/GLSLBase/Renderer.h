#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer() = default;

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);
	   
	void Render();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	void CreateVertexBufferObjects();

	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

	unsigned char* loadBMPRaw(const char* imagepath, unsigned int& outWidth, unsigned int& outHeight);

private:
	bool			m_Initialized;
	unsigned int	m_WindowSizeX;
	unsigned int	m_WindowSizeY;

	glm::vec3		m_v3Camera_Position;
	glm::vec3		m_v3Camera_Lookat;
	glm::vec3		m_v3Camera_Up;

	glm::mat4		m_m4OrthoProj;
	glm::mat4		m_m4PersProj;
	glm::mat4		m_m4Model;
	glm::mat4		m_m4View;
	glm::mat4		m_m4ProjView;

	GLuint			m_VBORect;
	GLuint			m_SolidRectShader;

	GLuint			m_VBOLecture;
	GLuint			m_testShader;
};

