#include "shaders.h"
#include "camera.h"
#include "textures.h"
#include "mesh.h"
#include "meshloader.h"
#include "staticrenderer.h"

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/affine.h>

#define ASSET_DIR "/media/ssd2/dev/neith/assets/"
//#define ASSET_DIR "E:/dev/neith/assets/"

unsigned int CreatDepthMapFrameBuffer(unsigned int *depthMap);
unsigned int CreatIntermediateFrameBuffer(int width, int height, unsigned int *screenTexture);
unsigned int CreatFrameBuffer(int width, int height, unsigned int *texColorBuffer);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow *window, double posX, double posY);
void processInput(GLFWwindow *window);


struct CallbackContext {
	bool resize;
	int width, height;
	vec3 moveVec;
	bool firstMouse;
	float lastX, lastY;
	struct CameraData *cd;
};

int main () {

	//Mesh Data
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
	     1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};	
	
    const char *modelPaths[] = {
        ASSET_DIR "TestScene/TestScene2.gltf"
    };

    struct StaticPrimitives *sp = LoadModels(modelPaths, 1);

	//GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(1800, 1000, "Neith", NULL, NULL);
	if(window == NULL) {
		printf("faild to crate window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    	{
        	return -1;
    	}    
	struct CallbackContext cbc;
	struct CameraData *cd = CameraInit();
	cbc.resize = false;
	cbc.width = 1800;
	cbc.height = 1000;
	cbc.lastX = 900;
	cbc.lastY = 500;
	cbc.firstMouse = true;
	cbc.cd = cd;
	glfwSetWindowUserPointer(window, &cbc);

	glfwSetCursorPosCallback(window, mouse_callback);
	
	//Shaders
	unsigned int shaderProgram = LoadAndCompileShaders(ASSET_DIR "shader.vert", ASSET_DIR "shader.frag");
	unsigned int lightShader = LoadAndCompileShaders(ASSET_DIR "light.vert", ASSET_DIR "light.frag");
	unsigned int screenShader = LoadAndCompileShaders(ASSET_DIR "screen.vert", ASSET_DIR "screen.frag");
	unsigned int cubeMapShader = LoadAndCompileShaders(ASSET_DIR "cubeMap.vert", ASSET_DIR "cubeMap.frag");
	unsigned int shadowMapShader = LoadAndCompileShaders(ASSET_DIR "shadowMap.vert", 
													     ASSET_DIR "shadowMap.frag"); 
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

    struct RenderContext *rc = InitStaticRender(sp);

	float screenVertices[] = {
		-1.0f, -1.0f,   0.0f,  0.0f,
		 1.0f,  1.0f,   1.0f,  1.0f,
		-1.0f,  1.0f,   0.0f,  1.0f,
		-1.0f, -1.0f,   0.0f,  0.0f,
		 1.0f, -1.0f,   1.0f,  0.0f,
		 1.0f,  1.0f,   1.0f,  1.0f,
	};

	unsigned int screenVAO, screenVBO;
	glGenBuffers(1, &screenVBO);
	glGenVertexArrays(1, &screenVAO);

	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

	glBindVertexArray(screenVAO);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int cubeMapVAO, cubeMapVBO;
	glGenBuffers(1, &cubeMapVBO);
	glGenVertexArrays(1, &cubeMapVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeMapVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Textures
	const char *cubeMapPaths[] = {
		ASSET_DIR "skybox/right.jpg",
		ASSET_DIR "skybox/left.jpg",
		ASSET_DIR "skybox/top.jpg",
		ASSET_DIR "skybox/bottom.jpg",
		ASSET_DIR "skybox/front.jpg",
		ASSET_DIR "skybox/back.jpg",
	};
    unsigned int cubeMap = LoadCubeMap(cubeMapPaths);
    
    unsigned int texture1 = LoadTexture(ASSET_DIR "Rocks004/Rocks004_2K_Color.jpg", GL_SRGB, GL_RGB);
    unsigned int texture2 = LoadTexture(ASSET_DIR "Rocks004/Rocks004_2K_Roughness.jpg", GL_RED, GL_RED);
    unsigned int normalMap = LoadTexture(ASSET_DIR "Rocks004/Rocks004_2K_Normal.jpg", GL_RGB, GL_RGB);

	unsigned int texColorBuffer;
	unsigned int fbo = CreatFrameBuffer(1800, 1000, &texColorBuffer);

	unsigned int screenTexture;
	unsigned int intermediateFBO = CreatIntermediateFrameBuffer(1800, 1000, &screenTexture);
	
	unsigned int depthMap;
	unsigned int depthMapFBO = CreatDepthMapFrameBuffer(&depthMap);

	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.roughness"), 1);
	glUniform1i(glGetUniformLocation(shaderProgram, "material.normal"), 2);
	glUniform1i(glGetUniformLocation(shaderProgram, "cubeMap"), 3);
	glUniform1i(glGetUniformLocation(shaderProgram, "shadowMap"), 4);

	glUseProgram(cubeMapShader);
	glUniform1i(glGetUniformLocation(cubeMapShader, "skybox"), 0);

	//Main Loop
	while(!glfwWindowShouldClose(window)) {
		FrameTime();
		if(cbc.resize) {
			glDeleteFramebuffers(1, &fbo);
			fbo = CreatFrameBuffer(cbc.width, cbc.height, &texColorBuffer);
			intermediateFBO = CreatIntermediateFrameBuffer(cbc.width, cbc. height, &screenTexture);
		}

		processInput(window);
		glEnable(GL_DEPTH_TEST);

		mat4 model = GLM_MAT4_IDENTITY_INIT;

		mat4 lightProjection;
		glm_ortho(-10.0, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f, lightProjection);

		mat4 lightView = GLM_MAT4_ZERO_INIT;
		glm_lookat((vec3){-1.6f, 4.0f, 1.6f}, (vec3){0.0f, 0.0f, 0.0f},
			   (vec3){0.0f, 1.0f, 0.0f}, lightView);

		mat4 lightSpaceMatrix;
		glm_mat4_mul(lightProjection, lightView, lightSpaceMatrix);

		glUseProgram(shadowMapShader);
		UniformMat4v(shadowMapShader, "lightSpaceMatrix", lightSpaceMatrix[0]);
		//UniformMat4v(shadowMapShader, "model", model[0]);
		glViewport(0, 0, 4096, 4096);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		//glCullFace(GL_FRONT);
		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, dataIndCount, GL_UNSIGNED_INT, 0);
        RenderStatic(rc, shadowMapShader);
		//glCullFace(GL_BACK);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glViewport(0, 0, cbc.width, cbc.height);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, normalMap);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		
		//MVP Matrix

		mat4 view = GLM_MAT4_IDENTITY_INIT;
		mat4 viewCubeMap = GLM_MAT4_IDENTITY_INIT;
		mat3 view3 = GLM_MAT3_IDENTITY_INIT;
		CameraGetViewMat(cd, view);
		glm_mat4_pick3(view, view3);
		glm_mat4_ins3(view3, viewCubeMap);
			
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		mat4 projection;
		glm_perspective(GLM_PI / 2.5f, (float)width / (float)height, 0.1f, 1000.0f, projection);

		vec3 lightPos = {1.2f, 4.0f, 2.0f};
		glUseProgram(shaderProgram);
		UniformVec3v(shaderProgram, "viewPos", cd->cameraPos);
		UniformVec3(shaderProgram, "light.direction", 0.4, -1.0, -0.4);
		UniformVec3(shaderProgram, "light.color", 3.0f, 3.0f, 3.0f);
		glUniform1f(glGetUniformLocation(shaderProgram, "material.metallic"), 0.0f);
		glUniform1f(glGetUniformLocation(shaderProgram, "material.specular"), 0.1f);
	
		//CubeMVPuniforms
		UniformMat4v(shaderProgram, "lightSpaceMatrix", lightSpaceMatrix[0]);
		//UniformMat4v(shaderProgram, "model", model[0]);
		UniformMat4v(shaderProgram, "view", view[0]);
		UniformMat4v(shaderProgram, "projection", projection[0]);

		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, dataIndCount, GL_UNSIGNED_INT, 0);
        RenderStatic(rc, shaderProgram);

		glUseProgram(cubeMapShader);

		glDepthFunc(GL_LEQUAL);

		UniformMat4v(cubeMapShader, "view", viewCubeMap[0]);
		UniformMat4v(cubeMapShader, "projection", projection[0]);
		glBindVertexArray(cubeMapVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, cbc.width, cbc.height, 0, 0, cbc.width, cbc.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if(cbc.resize){
			glViewport(0, 0, cbc.width, cbc.height);
			cbc.resize = false;
		}

		glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(screenShader);
		glBindVertexArray(screenVAO);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	DeleteStaticRender(rc, sp);

	glDeleteProgram(shaderProgram);
	
	free(cd);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window) {
	struct CallbackContext *callbackContext = (struct CallbackContext *)glfwGetWindowUserPointer(window);

	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		//glfwSetWindowShouldClose(window, true);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	CameraKeyInput(callbackContext->cd, window);
}

void mouse_callback(GLFWwindow *window, double posX, double posY){
	struct CallbackContext *cbc = (struct CallbackContext *)glfwGetWindowUserPointer(window);
	
	if(cbc->firstMouse) {
		cbc->lastX = posX;
		cbc->lastY = posY;
		cbc->firstMouse = false;
	}

	float offestX = posX - cbc->lastX;
	float offestY = posY - cbc->lastY;
	cbc->lastX = posX;
	cbc->lastY = posY;

	CameraMouseInput(cbc->cd, offestX, offestY);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	struct CallbackContext *cbc = (struct CallbackContext *)glfwGetWindowUserPointer(window);
	cbc->resize = true;
	cbc->width = width;
	cbc->height = height;

}

unsigned int CreatFrameBuffer(int width, int height, unsigned int *texColorBuffer) {
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, texColorBuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, *texColorBuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGB, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, *texColorBuffer, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR: Framebuffer not Complete %i\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return fbo;
}

unsigned int CreatIntermediateFrameBuffer(int width, int height, unsigned int *screenTexture) {
	unsigned int intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
	
	glGenTextures(1, screenTexture);
	glBindTexture(GL_TEXTURE_2D, *screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *screenTexture, 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    	printf("ERROR: Framebuffer not Complete\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return intermediateFBO;
}

unsigned int CreatDepthMapFrameBuffer(unsigned int *depthMap) {
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, depthMap);
	glBindTexture(GL_TEXTURE_2D, *depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO); 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return depthMapFBO;
}
