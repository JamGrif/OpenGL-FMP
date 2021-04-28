#pragma once

#include "Model.h"

class ModelBasic :
    public Model
{
public:
    ModelBasic(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
    ~ModelBasic();

    void drawPassOne() override;
    void drawPassTwo() override;

    void copyPointLight(int index);

private:

    glm::vec3 m_defaultColour;

    int m_pointLightToCopy;
};

