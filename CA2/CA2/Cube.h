//
//  Cube.h
//  3D Scene
//
//  Created by Yu Chen on 4/8/15.
//  Copyright (c) 2015 Yu Chen. All rights reserved.
//

#ifndef _D_Scene_Cube_h
#define _D_Scene_Cube_h

#include "Equilateral.h"
#include "SquareBase.h"

class Dice : public Equilateral
{
public:
	explicit Dice(float side = 0, Vector<float> position = Vector<float>(0, 0, 0)):
    Equilateral(side, position)
    {
        setDrawType(GL_QUADS);
    }
    
	std::vector<SquareBase> getFaces() const
    {
		std::vector<SquareBase> list;
        auto position = getPosition();
        auto xVector = getXVector();
        auto yVector = getYVector();
        auto zVector = getZVector();
		SquareBase bottom(side, position);
        bottom.setXYVectors(xVector, yVector);
		SquareBase top(side, position + zVector.getUnitVector() * side);
        top.setXYVectors(yVector, xVector);
		SquareBase left(side, position);
        left.setXYVectors(yVector, zVector);
		SquareBase right(side, position + xVector.getUnitVector() * side);
        right.setXYVectors(zVector, yVector);
		SquareBase front(side, position);
        front.setXYVectors(zVector, xVector);
		SquareBase back(side, position + yVector.getUnitVector() * side);
        back.setXYVectors(xVector, zVector);
        list.push_back(bottom);
        list.push_back(left);
        list.push_back(front);
        list.push_back(back);
        list.push_back(right);
        list.push_back(top);
        return list;
    }
    
    std::vector<Vector<float>> getVertices() const override {
        std::vector<Vector<float>> list;
        for(auto face: getFaces())
        {
            for(auto vertice: face.getVertices())
            {
                list.push_back(vertice);
            }
        }
        return list;
    } 
};

#endif
