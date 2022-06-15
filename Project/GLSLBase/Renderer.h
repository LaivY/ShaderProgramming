#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_map>

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

	GLuint CreatePngTexture(const char* filePath);
	GLuint CreateBmpTexture(const char* filePath);

	void Render();
	void RenderParticle();
	void RenderFullScreenQuad();
	void RenderRadarCircle();
	void RenderLine();
	void RenderTexture();
	void RenderGridRect();
	void RenderFullTexture(GLuint fbo);
	void RenderMultiTexture();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	void CreateVertexBufferObjects();
	void CreateParticle(int count);
	void CreateFullScreenQuad();
	void CreateLinePoints(int vertexCount);
	void CreateFullScreenTextureQuad();
	void CreateGridMesh();
	void CreateFrameBufferObjects();

	GLuint CompileShaders(const std::string& filenameVS, const std::string& filenameFS);
	bool ReadFile(const std::string& filename, std::string& target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

	unsigned char* loadBMPRaw(const char* imagepath, unsigned int& outWidth, unsigned int& outHeight);

private:
	bool			m_Initialized;
	UINT			m_WindowSizeX;
	UINT			m_WindowSizeY;

	glm::vec3		m_v3Camera_Position;
	glm::vec3		m_v3Camera_Lookat;
	glm::vec3		m_v3Camera_Up;

	glm::mat4		m_m4OrthoProj;
	glm::mat4		m_m4PersProj;
	glm::mat4		m_m4Model;
	glm::mat4		m_m4View;
	glm::mat4		m_m4ProjView;

	std::unordered_map<std::string, GLuint> m_shaders;
	std::unordered_map<std::string, GLuint> m_vbos;
	std::unordered_map<std::string, GLuint> m_vertexCounts;
	std::unordered_map<std::string, GLuint> m_textures;
	std::unordered_map<std::string, GLuint> m_fbos;
};