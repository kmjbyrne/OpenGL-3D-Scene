

#ifndef CA2_SQUARE_BASE_H
#define CA2_SQUARE_BASE_H

#include "Equilateral.h"

class SquareBase : public Equilateral
{
public:
	explicit SquareBase(float side = 0, Vector<float> position = Vector<float>(0, 0, 0)) :
        Equilateral(side, position) {}
    
    std::vector<Vector<float>> getVertices() const override {
        std::vector<Vector<float>> list;
        list.push_back(getPosition());
        list.push_back(getPosition() + getXVector().getUnitVector() * side);
        list.push_back(getPosition() + getXVector().getUnitVector() * side + getYVector().getUnitVector() * side);
        list.push_back(getPosition() + getYVector().getUnitVector() * side);
        return list;
    }
};

#endif
