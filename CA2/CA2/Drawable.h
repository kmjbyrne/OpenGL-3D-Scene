
//
//	2D Robot Arm
//	CA1 Graphics
//	Modified for CA2 3D Scene
//


#ifndef CA2_DRAWABLE_H
#define CA2_DRAWABLE_H

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class Drawable : public sf::Drawable
{
private:
    GLenum type;
    std::vector<float> color;
    sf::Texture* texture = nullptr;
    std::vector<Vector<float>> texture_vertices;
public:
    explicit Drawable(GLenum type=GL_QUAD_STRIP): type(type) {}
    void setDrawType(GLenum type)
    {
        this->type = type;
    }
    void setColor(const std::vector<float>& color)
    {
        this->color = color;
    }
    void setTexture(sf::Texture* texture,
                    const std::vector<Vector<float>>& texture_vertices)
    {
        this->texture = texture;
        this->texture_vertices = texture_vertices;
    }

	void initDiceTexture(sf::Texture * texture){
		this->texture = texture;
		this->texture_vertices.push_back(Vector<float>(0.25, 0.66));
		this->texture_vertices.push_back(Vector<float>(0.0, 0.66));
		this->texture_vertices.push_back(Vector<float>(0.0, 0.33));
		this->texture_vertices.push_back(Vector<float>(0.25, 0.33));

		this->texture_vertices.push_back(Vector<float>(0.25, 0.66));
		this->texture_vertices.push_back(Vector<float>(0.5, 0.66));
		this->texture_vertices.push_back(Vector<float>(0.5, 0.33));
		this->texture_vertices.push_back(Vector<float>(0.25, 0.33));

		this->texture_vertices.push_back(Vector<float>(0.25, 0.66));
		this->texture_vertices.push_back(Vector<float>(0.5, 0.66));
		this->texture_vertices.push_back(Vector<float>(0.5, 0.33));
		this->texture_vertices.push_back(Vector<float>(0.25, 0.33));

		this->texture_vertices.push_back(Vector<float>(1, 0.33));
		this->texture_vertices.push_back(Vector<float>(0.75, 0.33));
		this->texture_vertices.push_back(Vector<float>(0.75, 0.66));
		this->texture_vertices.push_back(Vector<float>(1, 0.66));

		this->texture_vertices.push_back(Vector<float>(0.75, 0.66));
		this->texture_vertices.push_back(Vector<float>(0.75, 0.33));
		this->texture_vertices.push_back(Vector<float>(0.5, 0.33));
		this->texture_vertices.push_back(Vector<float>(0.5, 0.66));

		this->texture_vertices.push_back(Vector<float>(0.5, 0.66));
		this->texture_vertices.push_back(Vector<float>(0.75, 0.66));
		this->texture_vertices.push_back(Vector<float>(0.75, 1.0));
		this->texture_vertices.push_back(Vector<float>(0.50, 1.0));
	}

	void bindTexture(sf::Texture* texture){
		this->texture->bind(texture);
	}

    virtual std::vector<Vector<float>> getVertices() const = 0;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        if(texture)
        {
            auto vertices = getVertices();
            sf::Texture::bind(texture);
            glBegin(type);

			if (!(this->color.size() == 0)){
				glColor4f(this->color[0], this->color[1], this->color[2], this->color[3]);
			}
			else{
				glColor4f(1, 1, 1, 1);
			}

            for(int i = 0; i < vertices.size(); i ++)
            {
                glNormal3f(vertices[i].x, vertices[i].y, vertices[i].z);
                glTexCoord2f(texture_vertices[i].x, texture_vertices[i].y);
                glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
            }
            glEnd();
            sf::Texture::bind(nullptr);
        }
        else
        {
            auto vertices = getVertices();
            glBegin(type);

			if (!(this->color.size() == 0)){
				glColor4f(this->color[0], this->color[1], this->color[2], this->color[3]);
			}
			else{
				glColor4f(1, 1, 1, 1);
			}

            for(int i = 0; i < vertices.size(); i ++)
            {
                glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
            }
            glEnd();
        }
    }
};

#endif
