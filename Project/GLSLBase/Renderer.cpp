#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>

Renderer::Renderer(int windowSizeX, int windowSizeY) : m_Initialized{ false }, m_WindowSizeX{ 0 }, m_WindowSizeY{ 0 }
{
	glClearDepth(1.f);
	Initialize(windowSizeX, windowSizeY);
}

GLuint Renderer::CreatePngTexture(const char* filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);
	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	return temp;
}

GLuint Renderer::CreateBmpTexture(const char* filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char* bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

void Renderer::Render()
{
	//RenderParticle();
	//RenderFullScreenQuad();
	//RenderRadarCircle();
	//RenderLine();
	//RenderTexture();
	//RenderGridRect();
	RenderMultiTexture();
}

void Renderer::RenderParticle()
{
	constexpr GLsizei vertexSize{ sizeof(float) * 15 };
	static float u_time{ 0.0f };

	GLuint shader{ m_shaders["PARTICLE"] };
	glUseProgram(shader);

	// 알파블렌딩
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 정점 데이터 입력 레이아웃
	std::vector<std::tuple<std::string, int, int>> vertexInputLayout
	{
		{ "a_position", 3, 0 },
		{ "a_velocity", 3, 3 },
		{ "a_emitTime", 1, 6 },
		{ "a_lifeTime", 1, 7 },
		{ "a_amp", 1, 8 },
		{ "a_freq", 1, 9 },
		{ "a_value", 1, 10 },
		{ "a_color", 4, 11 },
	};

	// 정점 데이터 셰이더로 넘김
	for (const auto& [name, size, pointer] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glEnableVertexAttribArray(attribLocation);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbos["PARTICLE"]);
		glVertexAttribPointer(attribLocation, size, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * pointer));
	}

	// 유니폼 데이터 셰이더로 넘김
	{
		// 시간
		int uniformLocTime{ glGetUniformLocation(shader, "u_time") };
		glUniform1f(uniformLocTime, u_time);

		// 가속도
		int uniformLocAccel{ glGetUniformLocation(shader, "u_accel") };
		glUniform3f(uniformLocAccel, 0.0f, 0.0f, 0.0f);
	}

	// 렌더링
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCounts["PARTICLE"]);

	// 비활성화
	for (const auto& [name, _, __] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glDisableVertexAttribArray(attribLocation);
	}

	// 시간 증가
	u_time += 0.01f;

	// 알파블렌딩 비활성화
	glDisable(GL_BLEND);
}

void Renderer::RenderFullScreenQuad()
{
	static float u_time{ 0.0f };
	constexpr GLsizei vertexSize{ sizeof(float) * 7 };

	GLuint shader{ m_shaders["FULLQUAD"] };
	glUseProgram(shader);

	// VBO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos["FULLQUAD"]);

	// 정점 데이터 입력 레이아웃
	std::vector<std::tuple<std::string, int, int>> vertexInputLayout
	{
		{ "a_position", 3, 0 },
		{ "a_color",	4, 3 },
	};

	// 정점 데이터 셰이더로 넘김
	for (const auto& [name, size, pointer] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation, size, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * pointer));
	}

	// 유니폼 변수들 셰이더로 넘김
	{
		// 원 위치
		static float points[]
		{
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f
		};

		int uniformLocPoints{ glGetUniformLocation(shader, "u_points") };
		glUniform3fv(uniformLocPoints, _countof(points), points);

		int uniformLocTime{ glGetUniformLocation(shader, "u_time") };
		glUniform1f(uniformLocTime, u_time);
	}

	// 샘플러
	int uniformLocSampler{ glGetUniformLocation(shader, "u_sampler") };
	glUniform1i(uniformLocSampler, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures["TEXTURE0"]);

	// 드로우콜
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCounts["FULLQUAD"]);

	// 비활성화
	for (const auto& [name, _, __] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glDisableVertexAttribArray(attribLocation);
	}

	u_time += 0.01f;
}

void Renderer::RenderRadarCircle()
{
	static float u_time{ 0.0f };
	constexpr GLsizei vertexSize{ sizeof(float) * 7 };

	GLuint shader{ m_shaders["FULLQUAD"] };
	glUseProgram(shader);

	// VBO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos["FULLQUAD"]);

	// 정점 데이터 입력 레이아웃
	std::vector<std::tuple<std::string, int, int>> vertexInputLayout
	{
		{ "a_position", 3, 0 },
		{ "a_color", 4, 3 }
	};

	// 정점 데이터 셰이더로 넘김
	for (const auto& [name, size, pointer] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation, size, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * pointer));
	}

	// 유니폼 변수들 셰이더로 넘김
	{
		// 원 위치
		static float points[]
		{
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f,
			(float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 0.0f 
		};
		int uniformLocPoints{ glGetUniformLocation(shader, "u_points") };
		glUniform3fv(uniformLocPoints, _countof(points), points);

		// 수명
		static float lifeTimes[]
		{
			(float)rand() / (float)RAND_MAX * 2.0f + 1.0f,
			(float)rand() / (float)RAND_MAX * 2.0f + 1.0f,
			(float)rand() / (float)RAND_MAX * 2.0f + 1.0f,
			(float)rand() / (float)RAND_MAX * 2.0f + 1.0f,
			(float)rand() / (float)RAND_MAX * 2.0f + 1.0f,
			(float)rand() / (float)RAND_MAX * 2.0f + 1.0f,
			(float)rand() / (float)RAND_MAX * 2.0f + 1.0f,
			(float)rand() / (float)RAND_MAX * 2.0f + 1.0f,
			(float)rand() / (float)RAND_MAX * 2.0f + 1.0f,
			(float)rand() / (float)RAND_MAX * 2.0f + 1.0f
		};
		int uniformLocLifetimes{ glGetUniformLocation(shader, "u_lifeTimes") };
		glUniform1fv(uniformLocLifetimes, _countof(lifeTimes), lifeTimes);

		// 시간
		int uniformLocTime{ glGetUniformLocation(shader, "u_time") };
		glUniform1f(uniformLocTime, u_time);
	}

	// 드로우콜
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCounts["FULLQUAD"]);

	// 비활성화
	for (const auto& [name, _, __] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glDisableVertexAttribArray(attribLocation);
	}

	u_time += 0.005f;
}

void Renderer::RenderLine()
{
	static float u_time{ 0.0f };
	constexpr GLsizei vertexSize{ sizeof(float) * 3 };

	GLuint shader{ m_shaders["LINE"] };
	glUseProgram(shader);

	std::vector<std::tuple<std::string, int, int>> vertexInputLayout
	{
		{ "a_position", 3, 0 }
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos["LINE"]);
	for (const auto& [name, size, pointer] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation, size, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * pointer));
	}

	{
		int uniformLocTime{ glGetUniformLocation(shader, "u_time") };
		glUniform1f(uniformLocTime, u_time);
	}

	glDrawArrays(GL_LINE_STRIP, 0, m_vertexCounts["LINE"]);

	// 비활성화
	for (const auto& [name, _, __] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glDisableVertexAttribArray(attribLocation);
	}

	u_time += 0.005f;
}

void Renderer::RenderTexture()
{
	constexpr GLsizei vertexSize{ sizeof(float) * 5 };

	GLuint shader{ m_shaders["TEXTURE"] };
	glUseProgram(shader);

	std::vector<std::tuple<std::string, int, int>> vertexInputLayout
	{
		{ "a_position", 3, 0 },
		{ "a_uv",		2, 3 }
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos["TEXTURE"]);
	for (const auto& [name, size, pointer] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation, size, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * pointer));
	}

	// 텍스쳐 바인딩
	{
		int uniLocSampler{ glGetUniformLocation(shader, "u_sampler") };
		glUniform1i(uniLocSampler, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textures["TEXTURE0"]);
	}

	glDrawArrays(GL_TRIANGLES, 0, m_vertexCounts["TEXTURE"]);

	for (const auto& [name, _, __] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glDisableVertexAttribArray(attribLocation);
	}
}

void Renderer::RenderGridRect()
{
	static float u_time{ 0.0f };
	constexpr GLsizei vertexSize{ sizeof(float) * 3 };
	GLuint shader{ m_shaders["GRID"] };
	glUseProgram(shader);

	std::vector<std::tuple<std::string, int, int>> vertexInputLayout
	{
		{ "a_position", 3, 0 }
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos["GRID"]);
	for (const auto& [name, size, pointer] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation, size, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * pointer));
	}

	{
		int uniformLocTime{ glGetUniformLocation(shader, "u_time") };
		glUniform1f(uniformLocTime, u_time);
	}

	glDrawArrays(GL_LINE_STRIP, 0, m_vertexCounts["GRID"]);

	for (const auto& [name, _, __] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glDisableVertexAttribArray(attribLocation);
	}
	u_time += 0.02f;
}

void Renderer::RenderFullTexture(GLuint fbo)
{
	constexpr GLsizei vertexSize{ sizeof(float) * 7 };

	GLuint shader{ m_shaders["FULL_TEXTURE"] };
	glUseProgram(shader);

	// VBO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos["TEXTURE"]);

	// 정점 데이터 입력 레이아웃
	std::vector<std::tuple<std::string, int, int>> vertexInputLayout
	{
		{ "a_position", 3, 0 },
		{ "a_color",	4, 3 }
	};

	// 정점 데이터 셰이더로 넘김
	for (const auto& [name, size, pointer] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glEnableVertexAttribArray(attribLocation);
		glVertexAttribPointer(attribLocation, size, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * pointer));
	}

	// 샘플러
	int uniformLocSampler{ glGetUniformLocation(shader, "u_sampler") };
	glUniform1i(uniformLocSampler, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo);

	// 드로우콜
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCounts["FULLQUAD"]);

	// 비활성화
	for (const auto& [name, _, __] : vertexInputLayout)
	{
		int attribLocation{ glGetAttribLocation(shader, name.c_str()) };
		glDisableVertexAttribArray(attribLocation);
	}
}

void Renderer::RenderMultiTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_textures["FRAME_BUFFER_0"]);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderParticle();

	// 메인 프레임버퍼
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 250, 250);
	RenderFullTexture(m_textures["FRAME_TEXTURE_0"]);
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_shaders["PARTICLE"] = CompileShaders("./Shaders/particle.vs", "./Shaders/particle.fs");
	m_shaders["FULLQUAD"] = CompileShaders("./Shaders/sandbox.vs", "./Shaders/sandbox.fs");
	m_shaders["LINE"] = CompileShaders("./Shaders/line.vs", "./Shaders/line.fs");
	m_shaders["TEXTURE"] = CompileShaders("./Shaders/texture.vs", "./Shaders/texture.fs");
	m_shaders["GRID"] = CompileShaders("./Shaders/grid.vs", "./Shaders/grid.fs");
	m_shaders["FULL_TEXTURE"] = CompileShaders("./Shaders/fullTexture.vs", "./Shaders/fullTexture.fs");

	// Load Texture
	m_textures["TEXTURE0"] = CreatePngTexture("./Textures/texture0.png");

	// Create VBOs
	CreateVertexBufferObjects();

	// Create FBOs
	CreateFrameBufferObjects();

	//Initialize camera settings
	m_v3Camera_Position = glm::vec3(0.f, 0.f, 1000.f);
	m_v3Camera_Lookat = glm::vec3(0.f, 0.f, 0.f);
	m_v3Camera_Up = glm::vec3(0.f, 1.f, 0.f);
	m_m4View = glm::lookAt(
		m_v3Camera_Position,
		m_v3Camera_Lookat,
		m_v3Camera_Up
	);

	//Initialize projection matrix
	m_m4OrthoProj = glm::ortho(
		-(float)windowSizeX / 2.f, (float)windowSizeX / 2.f,
		-(float)windowSizeY / 2.f, (float)windowSizeY / 2.f,
		0.0001f, 10000.f);
	m_m4PersProj = glm::perspectiveRH(45.f, 1.f, 1.f, 1000.f);

	//Initialize projection-view matrix
	m_m4ProjView = m_m4OrthoProj * m_m4View; //use ortho at this time
	//m_m4ProjView = m_m4PersProj * m_m4View;

	//Initialize model transform matrix :; used for rotating quad normal to parallel to camera direction
	m_m4Model = glm::rotate(glm::mat4(1.0f), glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
}

void Renderer::CreateVertexBufferObjects()
{
	CreateParticle(1000);
	CreateFullScreenQuad();
	CreateLinePoints(2000);
	CreateFullScreenTextureQuad();
	CreateGridMesh();
}

void Renderer::CreateParticle(int particleCount)
{
	// 파티클 크기
	constexpr float particleSize = 0.1f;

	// 정점의 데이터 개수
	// (위치(3), 속도(3), 생성 시각(1), 수명(1), 진폭(1), 주기(1), 랜덤값(1), 색깔(4)
	constexpr int vertexDataCount{ 3 + 3 + 1 + 1 + 1 + 1 + 1 + 4 };

	// 총 float 개수
	// 정점 데이터 개수 * 삼각형을 이루는 정점의 개수 * 삼각형 개수 * 파티클 개수
	int floatCount{ (vertexDataCount * 3 * 2) * particleCount };
	float* vertices{ new float[floatCount] };

	int index = 0;
	for (int i = 0; i < particleCount; i++)
	{
		float randomValueX = 0.0f;// ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		float randomValueY = 0.0f;// ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		float randomValueZ = 0.0f;
		float randomValueVX = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.1f;
		float randomValueVY = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.1f;
		float randomValueVZ = 0.0f;
		float randomEmitTime = ((float)rand() / (float)RAND_MAX) * 5.0f;
		float randomLifeTime = ((float)rand() / (float)RAND_MAX) * 2.0f;
		float randomAmp = ((float)rand() / (float)RAND_MAX) * 0.2f - 0.1f;
		float randomFreq = ((float)rand() / (float)RAND_MAX) * 2.0f;
		float randomValue = ((float)rand() / (float)RAND_MAX) * 1.0f;
		float color[4] = { (float)rand() / (float)RAND_MAX * 1.0f,
						   (float)rand() / (float)RAND_MAX * 1.0f,
						   (float)rand() / (float)RAND_MAX * 1.0f,
						   1.0f };

		for (int j = 0; j <= 5; ++j)
		{
			if (j == 0)
			{
				vertices[index++] = -particleSize / 2.f + randomValueX;
				vertices[index++] = -particleSize / 2.f + randomValueY;
			}
			else if (j == 1)
			{
				vertices[index++] = particleSize / 2.f + randomValueX;
				vertices[index++] = -particleSize / 2.f + randomValueY;
			}
			else if (j == 2)
			{
				vertices[index++] = particleSize / 2.f + randomValueX;
				vertices[index++] = particleSize / 2.f + randomValueY;
			}
			else if (j == 3)
			{
				vertices[index++] = -particleSize / 2.f + randomValueX;
				vertices[index++] = -particleSize / 2.f + randomValueY;
			}
			else if (j == 4)
			{
				vertices[index++] = particleSize / 2.f + randomValueX;
				vertices[index++] = particleSize / 2.f + randomValueY;
			}
			else if (j == 5)
			{
				vertices[index++] = -particleSize / 2.f + randomValueX;
				vertices[index++] = particleSize / 2.f + randomValueY;
			}
			vertices[index++] = 0.0f;

			vertices[index++] = randomValueVX;
			vertices[index++] = randomValueVY;
			vertices[index++] = 0.0f;

			vertices[index++] = randomEmitTime;
			vertices[index++] = randomLifeTime;

			vertices[index++] = randomAmp;
			vertices[index++] = randomFreq;

			vertices[index++] = randomValue;

			vertices[index++] = color[0];
			vertices[index++] = color[1];
			vertices[index++] = color[2];
			vertices[index++] = color[3];
		}
	}
	glGenBuffers(1, &m_vbos["PARTICLE"]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos["PARTICLE"]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices, GL_STATIC_DRAW);

	// 총 파티클들의 정점 개수
	m_vertexCounts["PARTICLE"] = particleCount * 3 * 2;

	delete[] vertices;
}

void Renderer::CreateFullScreenQuad()
{
	// 위치(3), 색깔(4)
	constexpr float vertices[]
	{
		-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	};
	glGenBuffers(1, &m_vbos["FULLQUAD"]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos["FULLQUAD"]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	m_vertexCounts["FULLQUAD"] = _countof(vertices) / 7;
}

void Renderer::CreateLinePoints(int vertexCount)
{
	std::vector<float> vertices;
	for (int i = 0; i < vertexCount; ++i)
	{
		vertices.push_back(std::lerp(-1.0f, 1.0f, static_cast<float>(i) / (vertexCount - 1)));
		vertices.push_back(0.0f);
		vertices.push_back(0.0f);
	}
	glGenBuffers(1, &m_vbos["LINE"]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos["LINE"]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	m_vertexCounts["LINE"] = vertexCount / 3;
}

void Renderer::CreateFullScreenTextureQuad()
{
	constexpr float vertices[]
	{
		// 위치(3)				// 텍스쳐좌표(2)
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,		1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,		1.0f, 1.0f
	};
	glGenBuffers(1, &m_vbos["TEXTURE"]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos["TEXTURE"]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	m_vertexCounts["TEXTURE"] = _countof(vertices) / 5;
}

void Renderer::CreateFrameBufferObjects()
{
	glGenTextures(1, &m_textures["FRAME_TEXTURE_0"]);
	glBindTexture(GL_TEXTURE_2D, m_textures["FRAME_TEXTURE_0"]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glGenRenderbuffers(1, &m_textures["DEPTH_BUFFER_0"]);
	glBindRenderbuffer(GL_RENDERBUFFER, m_textures["DEPTH_BUFFER_0"]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glGenFramebuffers(1, &m_textures["FRAME_BUFFER_0"]);
	glBindRenderbuffer(GL_RENDERBUFFER, m_textures["FRAME_BUFFER_0"]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures["FRAME_TEXTURE_0"], 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_textures["DEPTH_BUFFER_0"]);
}

void Renderer::CreateGridMesh()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	m_vertexCounts["GRID"] = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	// Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_vbos["GRID"]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbos["GRID"]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);
}

GLuint Renderer::CompileShaders(const std::string& filenameVS, const std::string& filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// 쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);
	if (!ShaderObj)
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);

	// 쉐이더 코드를 쉐이더 오브젝트에 할당
	GLint length = (GLint)strlen(pShaderText);
	glShaderSource(ShaderObj, 1, &pShaderText, &length);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// OpenGL 의 shader log 데이터를 가져옴
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(const std::string& filename, std::string& target)
{
	std::ifstream file{ filename };
	if (!file)
	{
		std::cout << filename << " file loading failed.. \n";
		return false;
	}

	std::string line;
	while (std::getline(file, line))
		target.append(line + "\n");
	return true;
}

unsigned char* Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}