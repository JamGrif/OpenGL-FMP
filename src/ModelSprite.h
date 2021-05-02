#pragma once
#include "Model.h"

class ModelSprite :
    public Model
{
public:

    ModelSprite(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f));
    ~ModelSprite();

    void drawPassOne() override;
    void drawPassTwo() override;

    void setSprite(const char* filePath);

    void setPosData(std::vector<glm::vec3> data);

private:

    Texture* m_modelSprite;


    std::vector<glm::vec3> m_PosData;
};

