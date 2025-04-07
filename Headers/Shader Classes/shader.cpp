#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string_view>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../scene.h"
#include "../camera.h"
#include "shader.h"

Shader::Shader(std::string_view vertPath, std::string_view fragPath) {
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
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << vertPath << " : " << fragPath << "\n";
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
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragPath << infoLog << "\n";
    }

    // Program
    mID = glCreateProgram();
    glAttachShader(mID, vertShader);
    glAttachShader(mID, fragShader);
    glLinkProgram(mID);

    success = 0;
    glGetProgramiv(mID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(mID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

Shader::Shader(std::string_view vertPath, std::string_view geomPath, std::string_view fragPath) {
    std::ifstream fs{ vertPath.data() };
    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string vertTemp{};
    std::string geomTemp{};
    std::string fragTemp{};
    const char* vertSource;
    const char* geomSource;
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

        // Fragment shader
        fs.close();
        fs.open(geomPath.data());
        ss.str("");
        ss << fs.rdbuf();
        geomTemp = ss.str();
        geomSource = geomTemp.c_str();
    }
    catch (std::ifstream::failure e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << vertPath << " : " << geomPath << " : " << fragPath << "\n";
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

    // Compile geometry
    unsigned int geomShader{ glCreateShader(GL_GEOMETRY_SHADER) };
    glShaderSource(geomShader, 1, &geomSource, NULL);
    glCompileShader(geomShader);

    success = 0;
    glGetShaderiv(geomShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(geomShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << geomPath << infoLog << "\n";
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
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragPath << infoLog << "\n";
    }

    // Program
    mID = glCreateProgram();
    glAttachShader(mID, vertShader);
    glAttachShader(mID, geomShader);
    glAttachShader(mID, fragShader);
    glLinkProgram(mID);

    success = 0;
    glGetProgramiv(mID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(mID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
    }

    glDeleteShader(vertShader);
    glDeleteShader(geomShader);
    glDeleteShader(fragShader);
}

void Shader::use() const {
    glUseProgram(mID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
}
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
}
void Shader::setVector3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(mID, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::setVector3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(mID, name.c_str()), x, y, z);
}
void Shader::setMatrix4(const std::string& name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(mID, name.c_str()), value);
}