#ifndef SHADER_H
#define SHADER_H

#include <string_view>
#include <glm/glm.hpp>
#include <string>
#include <glad/glad.h>

class Scene;
class Camera;
class Model;

class Shader {

protected:
    unsigned int mID;
    Shader(std::string_view vertPath, std::string_view fragPath);
    virtual void use() const final;

    void setBool   (const std::string& name, bool  value)                const;
    void setInt    (const std::string& name, int   value)                 const;
    void setVector3(const std::string& name, const glm::vec3& value)    const;
    void setVector3(const std::string& name, float x, float y, float z) const;
    void setMatrix4(const std::string& name, const glm::mat4& matrix)   const;
    void setFloat  (const std::string& name, float value)               const;

public:
    ~Shader() {
        glDeleteProgram(mID);
    }
};

#endif