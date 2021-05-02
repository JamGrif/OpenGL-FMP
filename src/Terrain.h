#pragma once
#include "Model.h"
class Terrain :
    public Model
{
public:

    Terrain(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
    ~Terrain();

    void drawPassOne() override;
    void drawPassTwo() override;

    void setShaderTwo(const char* vertexPath, const char* tessellationControlPath, const char* tessellationEvaluationPath, const char* fragmentPath);

private:

    Texture* m_terrainTexture;

};

