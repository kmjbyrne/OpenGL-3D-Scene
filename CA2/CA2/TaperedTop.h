

#ifndef CA2_TAPERED_TOP
#define CA2_TAPERED_TOP

#include "BaseSceneObject.h"
#include "Drawable.h"

class TaperedTop : public BaseSceneObject, public Drawable
{
public:
    float height;
    float lower_boundary_radius;
	float upper_boundary_radius;
    int slices = 24;

	explicit TaperedTop(float radius_1 = 0, float radius_2 = 0, float height = 0,
                           Vector<float> position=Vector<float>(0, 0, 0)):
						   BaseSceneObject(position), lower_boundary_radius(radius_1), upper_boundary_radius(radius_2), height(height)
    {
        setDrawType(GL_QUAD_STRIP);
    }
    
    std::vector<Vector<float>> getVertices() const
    {
        std::vector<Vector<float>> list;
        float rotateSpeed = 360.0f / slices;
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
        list.push_back(vector_1 + position_1);
        list.push_back(vector_2 + position_2);
        return list;
    }
    
    std::vector<Vector<float>> generateTextureVertices() const
    {
        std::vector<Vector<float>> list;
        auto vertices = getVertices();
        auto texture_coordinate = 1.0f / slices;
        for(int i = 0; i < vertices.size(); i ++)
            list.push_back(Vector<float>(i / 2 * texture_coordinate, i % 2));
        return list;
    }

};

#endif
