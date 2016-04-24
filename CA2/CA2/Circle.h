
#ifndef CA2_SCENE_CIRCLE
#define CA2_SCENE_CIRCLE

#include "Equilateral.h"

class Circle : public Equilateral
{
public:
    int slices = 24;
    explicit Circle(float radius=0, Vector<float> position=Vector<float>(0, 0, 0)):
    Equilateral(radius, position)
    {
        setDrawType(GL_TRIANGLE_FAN);
    }
    
    float getRadius() const {
        return side;
    }
    
    void setRadius(float radius)
    {
        side = radius;
    }
    
    std::vector<Vector<float>> getVertices() const override {
        std::vector<Vector<float>> list;
        float rotateSpeed = 360.0f / slices;
        auto axis = getYVector();
        auto vector = getXVector().getUnitVector() * getRadius();
        auto position = getPosition();
        list.push_back(position);
        list.push_back(vector + position);
        for (int i = 1; i < slices; i ++)
            list.push_back(position + vector.rotateAroundAxis(axis, rotateSpeed * i));
        list.push_back(vector + position);
        return list;
    }
};

#endif
