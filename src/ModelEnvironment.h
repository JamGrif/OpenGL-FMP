#pragma once
#include "Model.h"
class ModelEnvironment :
    public Model
{
public:
    ModelEnvironment(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
    ~ModelEnvironment();

    void drawPassOne() override;
    void drawPassTwo() override;

private:


};

