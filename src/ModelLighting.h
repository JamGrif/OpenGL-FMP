#pragma once

#include "Model.h"

class ModelLighting :
    public Model
{
public:
    ModelLighting(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
    ~ModelLighting();

    void drawPassOne() override;
    void drawPassTwo() override;

    void setDiffuseTexture(const char* texturePath);
    void setSpecularTexture(const char* texturePath);
    void setEmissionTexture(const char* texturePath);
    void setNormalTexture(const char* texturePath, bool normalize);
    void setHeightTexture(const char* texturePath);

private:

    

    Texture* m_modelDiffuseTexture;
    Texture* m_modelSpecularTexture;
    Texture* m_modelEmissionTexture;
    Texture* m_modelNormalTexture;
    Texture* m_modelHeightTexture;

    bool m_normalizeTexture;
    bool m_usingEmission;
    bool m_usingNormal;
    bool m_usingHeight;

    std::vector<glm::vec3> m_tangents;
    std::vector<glm::vec3> m_bitangents;

};

