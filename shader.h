#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string_view>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    unsigned int ID;

	Shader(std::string_view vertPath, std::string_view fragPath) {
        std::ifstream fs{ vertPath.data() };
        fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        std::string vertTemp{};
        std::string fragTemp{};
        const char* vertSource;
        const char* fragSource;

        try {
            // Vertex shader
            std::stringstream ss{};
            ss << fs.rdbuf();
            vertTemp = ss.str();
            vertSource = vertTemp.c_str();

            // Fragment shader
            fs.close();
            fs.open(fragPath.data());
            ss.str("");
            ss << fs.rdbuf();
            fragTemp = ss.str();
            fragSource = fragTemp.c_str();
        }
        catch (std::ifstream::failure e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
        }

        // Compile vertex
        unsigned int vertShader{ glCreateShader(GL_VERTEX_SHADER) };
        glShaderSource(vertShader, 1, &vertSource, NULL);
        glCompileShader(vertShader);

        int success;
        glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertPath << infoLog << "\n";
        }

        // Compile fragment
        unsigned int fragShader{ glCreateShader(GL_FRAGMENT_SHADER) };
        glShaderSource(fragShader, 1, &fragSource, NULL);
        glCompileShader(fragShader);

        success = 0;
        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << fragPath << infoLog << "\n";
        }

        // Program
        ID = glCreateProgram();
        glAttachShader(ID, vertShader);
        glAttachShader(ID, fragShader);
        glLinkProgram(ID);

        success = 0;
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        }

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
	}

    void use() const {
        glUseProgram(ID);
    }

    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setMatrix4(const std::string& name, const glm::mat4& matrix) {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void setVector3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
    }
    void setVector3(const std::string& name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    ~Shader() {
        glDeleteProgram(ID);
    }
};

#endif