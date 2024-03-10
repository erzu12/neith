#include "framebuffer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdbool.h>

#include "log.h"
#include "window/window.h"
namespace neith {
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    struct CallbackContext *cbc = (struct CallbackContext *)glfwGetWindowUserPointer(window);
    cbc->win->SetResize(true);
    cbc->win->SetWidth(width);
    cbc->win->SetHeight(height);
}

unsigned int CreatFrameBuffer(int width, int height)
{
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    unsigned int colorRBO;

    glGenRenderbuffers(1, &colorRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRBO);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        NT_INTER_CRITICAL("ERROR: Framebuffer not Complete %i\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return fbo;
}

unsigned int CreatIntermediateFrameBuffer(int width, int height, unsigned int *screenTexture, unsigned int *screenDepthMap)
{
    unsigned int intermediateFBO;
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

    glGenTextures(1, screenTexture);
    glBindTexture(GL_TEXTURE_2D, *screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *screenTexture, 0);


    glGenTextures(1, screenDepthMap);
    glBindTexture(GL_TEXTURE_2D, *screenDepthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *screenDepthMap, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("ERROR: Framebuffer not Complete\n");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return intermediateFBO;
}

unsigned int CreatDepthMapFrameBuffer(unsigned int *depthMap)
{
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, depthMap);
    glBindTexture(GL_TEXTURE_2D, *depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return depthMapFBO;
}
}  // namespace neith
