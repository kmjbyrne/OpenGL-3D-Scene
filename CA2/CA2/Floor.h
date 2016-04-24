

#ifndef CA2_FLOOR_H
#define CA2_FLOOR_H

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "BaseSceneObject.h"

class Floor : public BaseSceneObject, public sf::Drawable
{
private:
    Vector<int> numberOfCells;
    Vector<float> cell_dimensions;
    sf::Texture texture1;
    sf::Texture texture2;
    
    void drawCells(bool flag, const sf::Texture& texture) const
    {
        auto position = getPosition();
        sf::Texture::bind(&texture);
        glBegin(GL_QUADS);
			for(int i = 0; i < numberOfCells.x; i ++)
			{
				for(int j = 0; j < numberOfCells.y; j ++)
				{
					if((i + j) % 2 == flag)
					{
						glTexCoord2f(0.0f,0.0f);
						glVertex3f(position.x + i * cell_dimensions.x + 0.0f, 0.0f, position.y + j * cell_dimensions.y + 0.0f);
						glTexCoord2f(1.0f,0.0f);
						glVertex3f(position.x + i * cell_dimensions.x + 1.0f, 0.0f, position.y + j * cell_dimensions.y + 0.0f);
						glTexCoord2f(1.0f,1.0f);
						glVertex3f(position.x + i * cell_dimensions.x + 1.0f, 0.0f, position.y + j * cell_dimensions.y + 1.0f);
						glTexCoord2f(0.0f,1.0f);
						glVertex3f(position.x + i * cell_dimensions.x + 0.0f, 0.0f, position.y + j * cell_dimensions.y + 1.0f);
					}
				}
			}
        glEnd();
        sf::Texture::bind(nullptr);
    }
public:
    explicit Floor(const Vector<int>& numberOfCells,
                            const Vector<float>& sizeOfCell,
                            const Vector<float>& position,
                            const sf::Texture& texture1,
                            const sf::Texture& texture2):
							numberOfCells(numberOfCells), cell_dimensions(sizeOfCell),
	BaseSceneObject(position),
    texture1(texture1), texture2(texture2) {}
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        drawCells(true, texture1);
        drawCells(false, texture2);
        
    }
};

#endif
