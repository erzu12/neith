#include "textures.h"

#include <stdbool.h>
#include <stb_image.h>
#include <glad/glad.h>

#include "log.h"

namespace neith {
    unsigned int CreatValueTextureF(float value) {
        
        unsigned int texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1, 1, 0, GL_RED, GL_FLOAT, &value);
            
        return texture;
    }

    unsigned int CreatValueTextureV3(float* value) {
        
        unsigned int texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, value);
            
        return texture;
    }

    unsigned int CreatValueTextureV3v(float red, float green, float blue) {
        float value[3];
        value[0] = red;
        value[1] = green;
        value[2] = blue;
        return CreatValueTextureV3(value);
    }

    unsigned int LoadTexture(const char *path, GLint colorSpace, GLint internalColorSpace) {
        unsigned int texture;
        int texWidth, texHeight, nrChannels;
        unsigned char *data;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);
        data = stbi_load(path, &texWidth, &texHeight, &nrChannels, 0);

        if(data){
            glTexImage2D(GL_TEXTURE_2D, 0, colorSpace, texWidth, texHeight, 0, internalColorSpace, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            NT_INTER_ERROR("faild to load texture: {}", path);
        }
        stbi_image_free(data);

        return texture;
    }
}
