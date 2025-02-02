#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string_view>
#include <iostream>

class Texture {
public:
    unsigned int ID;
    int textureUnit;

	Texture(std::string_view imagePath, int textureUnitOffset) {
        textureUnit = GL_TEXTURE0 + textureUnitOffset;
        glActiveTexture(textureUnit);
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        int width, height, channelCount;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(imagePath.data(), &width, &height, &channelCount, 0);

        if (data) {
            auto internalFormat{ channelCount == 3 ? GL_RGB : GL_RGBA };
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
        else {
            std::cerr << "Failed to load data\n";
        }
	}

    void use() {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
};

#endif