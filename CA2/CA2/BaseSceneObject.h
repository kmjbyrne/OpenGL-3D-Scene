

#ifndef CA2_BASE_SCENE_H
#define CA2_BASE_SCENE_H

#include "Vector.h"
#include <vector>


    class BaseSceneObject
    {
    private:
        
        Vector<float> xVector;
        Vector<float> yVector;
        
    public:

		Vector<float> position;

		explicit BaseSceneObject(Vector<float> position = Vector<float>(0, 0, 0),
                           Vector<float> xVector=Vector<float>(1, 0, 0),
                           Vector<float> yVector=Vector<float>(0, 1, 0)):
        position(position), xVector(xVector), yVector(yVector) {}
        
        Vector<float> getPosition() const
        {
            return position;
        }
        
        Vector<float> getXVector() const
        {
            return xVector;
        }
        Vector<float> getYVector() const
        {
            return yVector;
        }
        Vector<float> getZVector() const
        {
            return xVector.crossMultiply(yVector);
        }
        
        void setPosition(const Vector<float>& position)
        {
            this->position = position;
        }
        
        void setXVector(const Vector<float>& xVector)
        {
            this->xVector = xVector;
        }
        void setYVector(const Vector<float>& yVector)
        {
            this->yVector = yVector;
        }
        void setXYVectors(const Vector<float>& xVector, const Vector<float>& yVector)
        {
            setXVector(xVector);
            setYVector(yVector);
        }
        void rotate(const Vector<float>& axisVector, float angle)
        {
            this->xVector = xVector.rotateAroundAxis(axisVector, angle);
            this->yVector = yVector.rotateAroundAxis(axisVector, angle);
        }
        
    };


#endif
