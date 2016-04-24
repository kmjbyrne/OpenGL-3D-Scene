

#ifndef CA2_BOTTLE_H
#define CA2_BOTTLE_H


#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>


#include "BaseSceneObject.h"
#include "TaperedTop.h"
#include "Circle.h"
#include "Label.h"

class Bottle : public BaseSceneObject, public sf::Drawable
{
private:
	std::vector<TaperedTop> outers;
	std::vector<TaperedTop> inners;
	std::vector<TaperedTop> liquids;
	TaperedTop bottom;
    Circle top;
    
public:
	Label label;
    std::vector<float> radius;
    std::vector<float> heights;
    float thickness;
    float liquid;
    sf::Texture* labelTexture = nullptr;
	std::vector<float> labelColour;
    sf::Texture* innerTexture = nullptr;
	std::vector<float> innerColour;
    sf::Texture* outerTexture = nullptr;
	std::vector<float> outerColour;
    sf::Texture* liquidTexture = nullptr;
    std::vector<float> liquidColour ;
    
    explicit Bottle(const Vector<float>& position,
                    const std::vector<float>& radius,
                    const std::vector<float>& heights,
                    float thickness,
                    float liquid):
    BaseSceneObject(position), radius(radius), heights(heights), thickness(thickness), liquid(liquid) {}

	void fillLiquid(){
		auto position = getPosition();
		bottom = TaperedTop(0, radius[0] - thickness, thickness * 2, position);
		bottom.setColor(liquidColour);
		bottom.setTexture(liquidTexture, bottom.generateTextureVertices());
		float height = 0;
		bool filled = false;
		int i;
		for (i = 0; i < radius.size() - 1; i++)
		{
			auto heightNext = heights[i] + height;
			// liquid
			if (!filled)
			{
				if (heightNext > liquid)
				{
					float heightTemp = liquid - height;
					float r = (radius[i + 1] - radius[i]) * heights[i] / heightTemp;
					float radiusTemp = radius[i] + r;
					TaperedTop liq(radius[i] - thickness,
						radiusTemp - thickness,
						heightTemp,
						Vector<float>(0, height, 0) + position);
					liq.setColor(liquidColour);
					liq.setTexture(liquidTexture, liq.generateTextureVertices());
					liquids.push_back(liq);

					top.setDrawType(GL_TRIANGLE_FAN);
					top.setRadius(radiusTemp - thickness);
					top.setPosition(Vector<float>(0, height + heightTemp, 0) + position);
					top.setColor(liquidColour);
					filled = true;
				}
				else
				{
					TaperedTop liq(radius[i] - thickness,
						radius[i + 1] - thickness,
						heights[i],
						Vector<float>(0, height, 0) + position);
					liq.setColor(liquidColour);
					liq.setTexture(liquidTexture, liq.generateTextureVertices());
					liquids.push_back(liq);
				}
			}
			height = heightNext;
		}
	}

	void init()
    {
        auto position = getPosition();

		//Create layered cylinder object
		bottom = TaperedTop(0, radius[0] - thickness, thickness * 2, position);
		bottom.setColor(liquidColour);
        //bottom.setTexture(liquidTexture, bottom.generateTextureVertices());
        float height = 0;
        bool filled = false;
        int i;

        for(i = 0; i < radius.size() - 1; i ++)
        {
            auto heightNext = heights[i] + height;
            
            /*  
				  |   |	
				 /     \
				|       |
				|       |
				|       |
		
			*/

			TaperedTop outer(radius[i], radius[i + 1], heights[i],
                                Vector<float>(0, height, 0) + position);
			outer.setColor(outerColour);
            outer.setTexture(outerTexture, outer.generateTextureVertices());
            outers.push_back(outer);
            
            // inner
			TaperedTop inner(radius[i] - thickness,
                                radius[i + 1] - thickness,
                                heights[i],
                                Vector<float>(0, height, 0) + position);
			inner.setColor(innerColour);
            inner.setTexture(innerTexture, inner.generateTextureVertices());
            inners.push_back(inner);
            
            // liquid
            if(!filled)
            {
                if(heightNext > liquid)
                {
                    float heightTemp = liquid - height;
                    float r = (radius[i + 1] - radius[i]) * heights[i] / heightTemp;
                    float radiusTemp = radius[i] + r;
					TaperedTop liq(radius[i] - thickness,
                                      radiusTemp - thickness,
                                      heightTemp,
                                      Vector<float>(0, height, 0)  + position);
					liq.setColor(liquidColour);
                    liq.setTexture(liquidTexture, liq.generateTextureVertices());
                    liquids.push_back(liq);
                    
                    top.setDrawType(GL_QUAD_STRIP);
                    top.setRadius(radiusTemp - thickness);
                    top.setPosition(Vector<float>(0, height + heightTemp, 0) + position);
                    top.setColor(liquidColour);
                    filled = true;
                }
                else
                {
					TaperedTop liq(radius[i] - thickness,
                                      radius[i + 1] - thickness,
                                      heights[i],
                                      Vector<float>(0, height, 0) + position);
					liq.setColor(liquidColour);
                    liq.setTexture(liquidTexture, liq.generateTextureVertices());
                    liquids.push_back(liq);
                }
            }
            
            height = heightNext;
        }
    }

	void incrementLiquid(){
		this->fillLiquid();
	}

	void decrementLiquid(){
		if (this->liquids.size() > 0){
			this->liquids.pop_back();
		}
	}
 
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
		// label
		target.draw(label);

        glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
        
			// liquid
			target.draw(bottom);
			std::for_each(liquids.begin(), liquids.end(), [&](const TaperedTop& iter)
			{
				target.draw(iter);
			});
			target.draw(top);

        
			// bottle
			std::for_each(inners.begin(), inners.end(), [&](const TaperedTop& iter)
			{
				target.draw(iter);
			});
			std::for_each(outers.begin(), outers.end(), [&](const TaperedTop& iter)
			{
				target.draw(iter);
			});

		
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
        glDisable(GL_BLEND);
    }
};

#endif
