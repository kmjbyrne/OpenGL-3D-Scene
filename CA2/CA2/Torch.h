
#ifndef CA2_TORCH_H
#define CA2_TORCH_H

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "TaperedTop.h"
#include "Movable.h"
#include "Rotatable.h"
#include "Light.h"

class Torch :

	public TaperedTop,
	public Interaction::Movable,
	public Interaction::Rotatable,
	public Light

{

private:

	TaperedTop base;
	TaperedTop handle;

    bool switchOn = true;
    std::vector<std::vector<GLfloat>> colors;
    unsigned int colorIndex = 0;
    sf::Keyboard::Key SWITCH = sf::Keyboard::T;
    sf::Keyboard::Key CHANGE = sf::Keyboard::LControl;

	sf::Keyboard::Key forward = sf::Keyboard::Numpad2;
	sf::Keyboard::Key backwards = sf::Keyboard::Numpad8;
	sf::Keyboard::Key left = sf::Keyboard::Numpad6;
	sf::Keyboard::Key right = sf::Keyboard::Numpad4;
	sf::Keyboard::Key tilt_up = sf::Keyboard::Numpad7;
	sf::Keyboard::Key tilt_down = sf::Keyboard::Numpad9;

	sf::Keyboard::Key ascend = sf::Keyboard::Add;
	sf::Keyboard::Key descend = sf::Keyboard::Subtract;

    void move(Vector<float>& direction, float distance)
    {
		distance *= 1.1;
        auto position = getPosition();
        auto velocity = direction.getUnitVector() * distance;
        position += velocity;
        setPosition(position);
    }


public:
    GLdouble slices = 30;
    GLdouble stack = 3.0;
    float forwardSpeed = .1;
    float rotationSpeed = 1;
 
    explicit Torch(float radius_1, float height, const Vector<float>& position, GLenum light):
		TaperedTop(radius_1, radius_1, height, position), Light(light) {}
    
    ~Torch() {}

    void updateKeyboardEvent()
    {
        updateMoveEvent();
        updateRotateEvent();
    }

	void updateMoveEvent()
	{
		
	}
    
    void updateSwitchEvent(const sf::Event& event)
    {
        if ((event.type == sf::Event::KeyPressed) && (event.key.code == SWITCH))
        {
            if(!switchOn)
                enable();
            else
                disable();
            switchOn = !switchOn;
        }
    }
    
    void moveLeftRight(int dir) override
    {
        move(Vector<float>::XAXIS,
             forwardSpeed * dir);
    }
    
    void moveUpDown(int dir) override
    {
        move(Vector<float>::YAXIS,
             forwardSpeed * dir);
    }
    
    void moveForwardBack(int dir) override
    {
        move(Vector<float>::ZAXIS,
             -forwardSpeed * dir);
    }


	void turnUpDown(int dir) override{
		rotate(Vector<float>::ZAXIS,
			-forwardSpeed * dir);
	}
	void turnRightLeft(int dir) override{

	}



    void addColor(std::vector<GLfloat> color)
    {
        colors.push_back(color);
    }
    
    void changeColor()
    {
        if(colors.size())
        {
            if(colorIndex >= colors.size() - 1)
                colorIndex = 0;
            else
                colorIndex ++;
            setColor(colorIndex);
        }
    }
    
    void setColor(unsigned int index)
    {
        setfv(GL_AMBIENT, colors[index].data());
    }
    

	void update(sf::Event e){
		if ((e.type == sf::Event::KeyPressed))
		{
			if (e.key.code == left)
				moveLeftRight(-1);

			if (e.key.code == right)
				moveLeftRight(1);

			if (e.key.code == forward)
				moveForwardBack(1);
			if (e.key.code == backwards)
				moveForwardBack(-1);

			if (e.key.code == tilt_up)
				turnUpDown(1);
			if (e.key.code == tilt_down)
				turnUpDown(-1);

			if (e.key.code == ascend)
				moveUpDown(1);

			if (e.key.code == descend)
				moveUpDown(-1);

			if (e.key.code == CHANGE)
				this->changeColor();

			if (e.key.code == SWITCH)
			{
				this->updateSwitchEvent(e);
			}
				
		}
	}
};

#endif
