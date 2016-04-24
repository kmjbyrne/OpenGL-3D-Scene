//	Reference:
//  Extend source from Ken Power I.T Carlow Graphics
//  3D Lighting Scene in OpenGL & SFML
//


#ifndef CA2_CAMERA_H
#define CA2_CAMERA_H

#include <SFML/Graphics.hpp>
#include "BaseSceneObject.h"
#include "Movable.h"
#include "Rotatable.h"

class Camera :
	public BaseSceneObject,
	public Interaction::Movable,
	public Interaction::Rotatable
{
public:
	float forwardSpeed;
	float rotationSpeed;

	const float MaxSightAngle = 170;
	const float MinSightAngle = 10;
	const float Maxheight = 30;
	const float Minheight = 5;
	const float LimitForward = -98;
	const float LimitBack = 98;
	const float LimitLeft = -98;
	const float LimitRight = 98;

	explicit Camera(float forwardSpeed = 1, float rotationSpeed = 1) :
		forwardSpeed(forwardSpeed), rotationSpeed(rotationSpeed), BaseSceneObject() {}

	explicit Camera(float forwardSpeed, float rotationSpeed,
		const Vector<float>& position,
		const Vector<float>& forward,
		const Vector<float>& up) :
		forwardSpeed(forwardSpeed), rotationSpeed(rotationSpeed),
		BaseSceneObject(position, forward, up) {}

	void updateKeyboardEvent()
	{
		updateMoveEvent();
		updateRotateEvent();
	}

	void move(Vector<float>& position, Vector<float>& forward)
	{
		auto oldPosition = position;

		/*if (position.x < LimitLeft) position.x = LimitLeft;
		if (position.x > LimitRight) position.x = LimitRight;
		if (position.y < Minheight) position.y = Minheight;
		if (position.y > Maxheight) position.y = Maxheight;
		if (position.z < LimitForward) position.z = LimitForward;
		if (position.z > LimitBack) position.z = LimitBack;*/

		forward = forward + position - oldPosition;
		setPosition(position);
		setXVector(forward);
	}

	void moveLeftRight(int dir) override
	{
		auto forward = getXVector();
		auto up = getYVector();
		auto position = getPosition();
		auto distance = forwardSpeed * dir;
		auto direction = (forward - position).crossMultiply(up);
		auto velocity = direction.getUnitVector() * distance;
		forward += velocity;
		position += velocity;
		move(position, forward);
	}

	void moveUpDown(int dir) override
	{
		auto forward = getXVector();
		auto up = getYVector();
		auto position = getPosition();
		auto distance = forwardSpeed * dir;
		auto velocity = up * distance;
		forward += velocity;
		position += velocity;
		move(position, forward);
	}

	void moveForwardBack(int dir) override
	{
		auto forward = getXVector();
		auto up = getYVector();
		auto position = getPosition();
		auto distance = forwardSpeed * dir;
		auto direction = up.crossMultiply((forward - position).crossMultiply(up));
		auto velocity = direction.getUnitVector() * distance;
		forward += velocity;
		position += velocity;
		move(position, forward);
	}

	void turnRightLeft(int dir) override
	{
		auto forward = getXVector();
		auto up = getYVector();
		auto position = getPosition();
		auto angle = rotationSpeed * dir;
		auto direction = forward - position;
		auto newDirection = direction.rotateAroundAxis(up, -angle);
		forward = position + newDirection;
		setXVector(forward);
	}

	void turnUpDown(int dir) override
	{
		auto forward = getXVector();
		auto up = getYVector();
		auto position = getPosition();
		auto angle = rotationSpeed * dir;
		auto direction = forward - position;
		auto axisVector = direction.crossMultiply(up);
		auto newDirection = direction.rotateAroundAxis(axisVector, angle);
		auto newForward = position + newDirection;
		auto newAngle = up.getAngleBetween(newDirection);
		if (newAngle >= MinSightAngle && newAngle <= MaxSightAngle)
			setXVector(newForward);
	}

	void ViewingTransform()
	{
		auto forward = getXVector();
		auto up = getYVector();
		auto position = getPosition();
		gluLookAt(position.x, position.y, position.z,// camera position
			forward.x, forward.y, forward.z, //look at this point //TODO: BUG here!! what is it ??
			up.x, up.y, up.z); //camera up
	}

};

#endif