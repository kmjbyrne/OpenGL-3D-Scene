

#ifndef CA2_LABEL_H
#define CA2_LABEL_H

#include "TaperedTop.h"

class Label : public TaperedTop
{
public:
    float angle;
	explicit Label(float radius = 0, float height = 0, float angle = 0,
                        Vector<float> position=Vector<float>(0, 0, 0)):
    TaperedTop(radius, radius, height, position), angle(angle) {}
    
    std::vector<Vector<float>> getVertices() const
    {
        std::vector<Vector<float>> list;
        float rotateSpeed = angle / slices;
        auto axis = getYVector();
        auto position_1 = getPosition();
        auto position_2 = position_1 + axis.getUnitVector() * height;
        auto unitVector = getXVector().getUnitVector();
        auto vector_1 = unitVector * lower_boundary_radius;
        auto vector_2 = unitVector * upper_boundary_radius;
        list.push_back(vector_1 + position_1);
        list.push_back(vector_2 + position_2);
        for (int i = 1; i < slices; i ++) {
            auto v1 = vector_1.rotateAroundAxis(axis, rotateSpeed * i);
            auto v2 = vector_2.rotateAroundAxis(axis, rotateSpeed * i);
            auto p1 = position_1 + v1;
            auto p2 = position_2 + v2;
            list.push_back(p1);
            list.push_back(p2);
        }
        return list;
    }
};

#endif
