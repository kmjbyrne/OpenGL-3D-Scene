////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
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
#include "SFML/Graphics/Shader.hpp"
#include "SFML/OpenGL.hpp" 
#include <gl/GLU.h>
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>
#include "Camera.h"
#include "Light.h"
#include "Torch.h"
#include "Label.h"
#include "Bottle.h"
#include "Vector.h"
#include "Cube.h"
#include "Floor.h"

double x(double t, double tetha, double rad){
	return rad * cos(tetha);
}
double y(double t, double tetha, double rad, double y1, double y2){
	return (y1*(1 - t)) + (y2*t);
}

double z(double t, double tetha, double rad){
	return rad * sin(tetha);
}

int main(){
	
	int BOTTLE_COUNT = 7;

	// =========== LOAD IMAGES ==========

	sf::Texture odd_tile_image, even_tile_image, dice_img, bottle_label, bottle_two_label;

	bottle_label.loadFromFile("label.png");
	bottle_two_label.loadFromFile("label.png");

	sf::String dice_file = "dice.jpg";
	sf::String odd_tile_file = "odd_tile.jpg";
	sf::String even_tile_file = "even_tile.jpg";
	if (!odd_tile_image.loadFromFile(odd_tile_file)){
		std::cout << "Could not load " << odd_tile_file.getData();
	}
	if (!even_tile_image.loadFromFile(even_tile_file)){
		std::cout << "Could not load " << even_tile_file.getData();
	}
	if (!dice_img.loadFromFile(dice_file)){
		std::cout << "Could not load " << dice_file.getData();
	}
	// =========== END LOAD IMAGES ==========

	int width = 1024, height = 960;
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
	sf::RenderWindow window(sf::VideoMode(width, height, 32), "C00170460 - Keith Byrne CA2", sf::Style::Close, Settings);
	window.setFramerateLimit(60);
	sf::Clock Clock;


	// Help message

	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.TTF");

	

	std::string instructions;
	instructions += "W A S D for Camera Movement\n";
	instructions += "NUMPAD 4 6 8 2 for left/right/up/down Torch Movement\n";
	instructions += "SPACEBAR for torch light switch\n";
	instructions += "LEFT CTRL for torch light colour switch\n";
	instructions += "P for global light switch.";

	sf::Text help;
	help.setFont(font);
	help.setString(instructions);
	help.setColor(sf::Color::White);
	help.setCharacterSize(20);
	help.setPosition(20, 40);

	bool global_switch = true;

	// Camera Setup
	Camera camera(1, // forward speed
		5, // rotation speed
			Vector<float>(0, 5, -15),      // position
				Vector<float>(0, 0, 20),    // forward
					Vector<float>::YAXIS);       // up

	glClearDepth(1.f);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); //background colour
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup a perspective projection & Camera position 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);

	//set up a 3D Perspective View volume
	gluPerspective(90.f, (float)width / height, 1.f, 50.0f);//fov, aspect, zNear, zFar 

	double t = 0.0;
	double tetha;
	double fi_tetha = (2 * M_PI) / 60;

	// Dice
	Dice dice(2,						//3D Cube dimension
		Vector<float>(0, 0, -2));		//Initial positioning

	auto dice_rotation_vector = Vector<float>::YAXIS;
	{
		auto xVector = dice.getXVector();
		auto yVector = dice.getYVector();
		auto zv = dice.getZVector();
		auto xv = xVector.rotateAroundAxis(zv, 45);
		xv = xv.rotateAroundAxis(xVector, -45);
		auto yv = yVector.rotateAroundAxis(zv, 45);
		yv = yv.rotateAroundAxis(xVector, -45);
		dice.setXYVectors(xv, yv);
	}

	dice.initDiceTexture(&dice_img);
	dice.setColor({ 1, 1, 1, 0.2f});

	std::vector<Bottle> bottles;
	int modulo_factor = BOTTLE_COUNT / 3;

#pragma region Bottle Set A
	//Create six packesque bottle arrangement
	for (int i = 0; i < BOTTLE_COUNT - 1; i++){

		//Bottle instantiation as follows
		//Position (x,y,z) global construct
		//Tapering object radius array (bottom to top)
		//Object heights (liquid, etc..)
		//Thickness
		//Liquid measure
		float x_pos = -10;
		float y_pos = 0.5f;
		float z_pos = -10;

		if (i % modulo_factor == 0)
		{
			z_pos = -7.5;
			z_pos += i;
			x_pos = -7.5;
			Bottle bottle(Vector<float>(x_pos, y_pos, z_pos),
			{ 1, 1, .95f, .85f, .7f, .5f, .4f, .4f },
			{ 3.85f, .2f, .2f, .2f, .2f, 2, 1 },
			.1f, 4.2f);

			bottle.label = Label(1, 3, 200, Vector<float>(x_pos, y_pos + 1.0f, z_pos));
			bottle.label.slices = 500;

			//Set label global colour to full opacity 
			bottle.outerColour = { 1.0f, 1.0f, 1.0f, 0.3f };
			bottle.innerColour = { 1.0f, 1.0f, 1.0f, 0.3f };

			bottle.liquidColour = { 1.000f, 0.843f, 0.000f, 0.6f };
			bottle.labelColour = { 0, 0, 0, 1 };

			bottle.label.setTexture(&bottle_label, bottle.label.generateTextureVertices());
			bottle.init();
			bottles.push_back(bottle);
		}
		else
		{
			z_pos += i;
			Bottle bottle(Vector<float>(x_pos, y_pos, z_pos),
			{ 1, 1, .95f, .85f, .7f, .5f, .4f, .4f },
			{ 3.85f, .2f, .2f, .2f, .2f, 2, 1 },
			.1f, 4.2f);     // thickness of bottle, height of liquid

			bottle.label = Label(1, 3, 200, Vector<float>(x_pos, y_pos + 1.0f, z_pos));
			bottle.label.slices = 500;

			//Set label global colour to full opacity 
			bottle.outerColour = { 1.0f, 1.0f, 1.0f, 0.3f };
			bottle.innerColour = { 1.0f, 1.0f, 1.0f, 0.3f };

			bottle.liquidColour = { 1.000f, 0.843f, 0.000f, 0.6f };
			bottle.labelColour = { 0, 0, 0, 1 };

			bottle.label.setTexture(&bottle_label, bottle.label.generateTextureVertices());
			bottle.init();
			bottles.push_back(bottle);
		}

	}
#pragma endregion

	// Torch
	Torch torch(0.5f, 1,      // radius 1, radius 2, height
		Vector<float>(-5, 10, 0), // position
		GL_LIGHT1); // light
	torch.setYVector(Vector<float>(0, -1, 0));
	torch.addColor({ 1, 1, 1, 1 });
	torch.addColor({ 1, 1, 0, 1 });
	torch.addColor({ 0, 1, 1, 1 });
	torch.addColor({ 0, 1, 0, 1 });
	torch.setColor(0);

	torch.setfv(GL_DIFFUSE, { .8f, .8f, .8f, .8f });
	torch.setfv(GL_SPECULAR, { 1, 1, 1, 1 });
	torch.setf(GL_SPOT_CUTOFF, 30);
	torch.setf(GL_SPOT_EXPONENT, 5);
	torch.setf(GL_CONSTANT_ATTENUATION, .001);
	torch.setf(GL_LINEAR_ATTENUATION, .001);
	torch.setf(GL_QUADRATIC_ATTENUATION, .01);

	// Odd/Even tiled floor setup. 200x200
	Floor floor(Vector<int>(200, 200),
		Vector<float>(1, 1),          
		Vector<float>(-100, -100),    
		odd_tile_image,		//Map odd floor texture
		even_tile_image);	//Map even floor texture


	//Setup HSR
	glClearDepth(1.0f);
	glClearColor(0.00f, 0.00f, 0.1f, 1.0f); //background colour
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glShadeModel(GL_SMOOTH);

	//Lighting Config
	glEnable(GL_LIGHTING);
	GLfloat global_ambient[] = { 0.2f, 0.2f, 0.2f, 0.2f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	Light globalLight(GL_LIGHT0);
	globalLight.setfv(GL_AMBIENT, { .0f, .0f, .0f, 0.05f });
	globalLight.setfv(GL_DIFFUSE, { .0f, .0f, .0f, 0.05f });
	globalLight.setfv(GL_SPECULAR, { .0f, .0f, .0f, 0.05f });
	globalLight.setfv(GL_POSITION, { -50, -50, 0, 1 });
	//globalLight.enable();

	Light masterLight(GL_LIGHT2);
	masterLight.setfv(GL_AMBIENT, { .5f, .5f, 1.0f, 0.4f });
	masterLight.setfv(GL_DIFFUSE, { .1f, .1f, .1f, .0f });
	masterLight.setfv(GL_SPECULAR, { 1, 1, 1, 0.1f });
	masterLight.setfv(GL_POSITION, { 0, -50, 0, 1 });
	//masterLight.enable();
	global_switch = false;

	// Material
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	GLfloat materialSpecular[4] = { 1, 1, 1, 1 };
	GLfloat materialAmbient[4] = { .1f, .1f, .1f, .1f };
	GLfloat materialDiffuse[4] = { .8f, .8f, .8f, .8f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMateriali(GL_FRONT, GL_SHININESS, 100);

	// Start game loop 
	while (window.isOpen()){
		// Process events 
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit 
			if (Event.type == sf::Event::Closed)
				window.close();
			// Escape key : exit 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();

			//update the camera
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::P)){
				if (global_switch == true)
				{
					masterLight.disable();
					global_switch = false;
				}
				else{
					masterLight.enable();
					global_switch = true;
				}
			}

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::PageUp)){
				for (int i = 0; i < BOTTLE_COUNT - 1; i++)
				{
					float liquid = bottles[i].liquid;
					liquid += 0.5f;
					bottles[i].incrementLiquid();
				}
			}

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::PageDown)){
				for (int i = 0; i < BOTTLE_COUNT - 1; i++)
				{
					float liquid = bottles[i].heights[0];
					liquid -= 0.5f;
					bottles[i].decrementLiquid();
				}
			}

			torch.update(Event);

			
		}

		camera.updateKeyboardEvent();
		torch.updateKeyboardEvent();
		
		//Prepare for drawing
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Apply some transformations
		//initialise the worldview matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//get the viewing transform from the camera
		camera.ViewingTransform();

		//make the world spin
		//TODO:probably should remove this in final
		static float ang = 0.0;
		//ang+=0.01f;
		//glRotatef(ang*2,0,1,0);//spin about y-axis

		auto light_position = torch.getPosition();
		auto yVector = torch.getYVector();

		//make sure light object follows absolute position and direction of torch source
		torch.setfv(GL_POSITION, { light_position.x, light_position.y, light_position.z, 1 });
		torch.setfv(GL_SPOT_DIRECTION, { yVector.x, yVector.y, yVector.z });

		
		window.draw(floor);
		dice.rotate(dice_rotation_vector, 5);
		window.draw(dice);
		

		for (int i = 0; i < BOTTLE_COUNT-1; i++){
			bottles[i].rotate(Vector<float>(1, 1, 2), 2);
			window.draw(bottles[i]);
		}

		window.draw(torch);
		

		window.pushGLStates();
		window.draw(help);
		window.popGLStates();

			//Render all objects to display
			window.display();
	}
	return EXIT_SUCCESS;
}