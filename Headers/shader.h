#ifndef SHADER_H
#define SHADER_H

#include <string_view>
#include <glm/glm.hpp>
#include "scene.h"
#include "camera.h"
#include "model.h"

class Shader {

protected:
    unsigned int ID;
    Shader(std::string_view vertPath, std::string_view fragPath);
    virtual void renderModel(const Model& model) {}
    virtual void use() const final;

    void setBool   (const std::string& name, bool value)                const;
    void setInt    (const std::string& name, int value)                 const;
    void setVector3(const std::string& name, const glm::vec3& value)    const;
    void setVector3(const std::string& name, float x, float y, float z) const;
    void setMatrix4(const std::string& name, const glm::mat4& matrix)   const;
    void setFloat  (const std::string& name, float value)               const;

public:
    virtual void render(const Scene& scene, const Camera& camera) {} //=0
    ~Shader();
};

#endif