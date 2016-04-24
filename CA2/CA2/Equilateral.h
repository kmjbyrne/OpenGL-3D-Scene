
#ifndef CA2_BASE_EQUI
#define CA2_BASE_EQUI

#include "BaseSceneObject.h"
#include "Drawable.h"

class Equilateral : public BaseSceneObject, public Drawable
{
public:
    float side;
    
    explicit Equilateral(float side=0, Vector<float> position=Vector<float>(0, 0, 0)):
		BaseSceneObject(position), side(side) {}
    
    virtual std::vector<Vector<float>> getVertices() const = 0;
};

#endif
