//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
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

	// =========== LOAD IMAGES ==========
	sf::Texture odd_tile_image;
	sf::Texture even_tile_image;
	sf::Texture dice_img;
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
    
    int width=600,height=600;
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
	sf::RenderWindow App(sf::VideoMode(width, height, 32), "C00170460 Keith Byrne CA2", sf::Style::Close, Settings);
	App.setFramerateLimit(60);
    sf::Clock Clock; 

	aiVector3D position(0,6,-10);
	Camera camera;
    camera.Init(position);
      
    glClearDepth(1.f); 
    glClearColor(0.3f, 0.3f, 0.6f, 0.f); //background colour
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_TRUE); 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
    // Setup a perspective projection & Camera position 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
     
    //set up a 3D Perspective View volume
    gluPerspective(90.f, (float)width/height, 1.f, 50.0f);//fov, aspect, zNear, zFar 
	
	// DICE CRAP
	GLdouble aaa[] = { 2, 2, 2 };
	GLdouble aab[] = { 2, 2, 0 };
	GLdouble abb[] = { 2, 0, 0 };
	GLdouble aba[] = { 2, 0, 2 };
	GLdouble baa[] = { 0, 2, 2 };
	GLdouble bab[] = { 0, 2, 0 };
	GLdouble bba[] = { 0, 0, 2 };
	GLdouble bbb[] = { 0, 0, 0 };
	double r = 0;
	// END DICE CRAP

	double t = 0.0;
	double tetha;
	double fi_tetha = (2 * M_PI) / 60;

    // Start game loop 
    while (App.isOpen()){ 
        // Process events 
        sf::Event Event; 
        while (App.pollEvent(Event)){ 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                App.close(); 
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                App.close(); 
			//update the camera
			camera.Update(Event);
        } 
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
		static float ang=0.0;
		//ang+=0.01f;
		//glRotatef(ang*2,0,1,0);//spin about y-axis
		
		glEnable(GL_TEXTURE_2D);
		// ============ SURE LETS DRAW A CYLINDER ==========
		double wideBottleY1 = 0;
		double wideBottleY2 = 8;
		double wideRadius = 2;
		double liquidRadius = -1.9;
		double narrowRadius = 1;
		
		glBegin(GL_LINE_LOOP);
		for (double angle = 0; angle < 2 * M_PI; angle += fi_tetha){
			tetha = angle;
			glColor4f(1.0, 1.0, 1.0, 0.15);
			glVertex3d(x(t, tetha, wideRadius), y(t, tetha, wideRadius, wideBottleY1, wideBottleY2), z(t, tetha, wideRadius));
			glVertex3d(x(1, tetha, wideRadius), y(1, tetha, wideRadius, wideBottleY1, wideBottleY2), z(1, tetha, wideRadius));
			glVertex3d(x(1, tetha + fi_tetha, wideRadius), y(1, tetha + fi_tetha, wideRadius, wideBottleY1, wideBottleY2), z(1, tetha + fi_tetha, wideRadius));
			glVertex3d(x(t, tetha + fi_tetha, wideRadius), y(t, tetha + fi_tetha, wideRadius, wideBottleY1, wideBottleY2), z(t, tetha + fi_tetha, wideRadius));
			glVertex3d(x(t, tetha, narrowRadius), y(t, tetha, narrowRadius, wideBottleY2, 12), z(t, tetha, narrowRadius));
			glVertex3d(x(1, tetha, narrowRadius), y(1, tetha, narrowRadius, wideBottleY2, 12), z(1, tetha, narrowRadius));
			glVertex3d(x(1, tetha + fi_tetha, narrowRadius), y(1, tetha + fi_tetha, narrowRadius, wideBottleY2, 12), z(1, tetha + fi_tetha, narrowRadius));
			glVertex3d(x(t, tetha + fi_tetha, narrowRadius), y(t, tetha + fi_tetha, narrowRadius, wideBottleY2, 12), z(t, tetha + fi_tetha, narrowRadius));
			glColor4f(1.0, 0.5, 0.0, 0.7);
			glVertex3d(x(t, tetha, liquidRadius), y(t, tetha, liquidRadius, wideBottleY1, wideBottleY2), z(t, tetha, liquidRadius));
			glVertex3d(x(0.6, tetha, liquidRadius), y(0.6, tetha, liquidRadius, wideBottleY1, wideBottleY2), z(0.6, tetha, liquidRadius));
			glVertex3d(x(0.6, tetha + fi_tetha, liquidRadius), y(0.6, tetha + fi_tetha, liquidRadius, wideBottleY1, wideBottleY2), z(0.6, tetha + fi_tetha, liquidRadius));
			glVertex3d(x(t, tetha + fi_tetha, liquidRadius), y(t, tetha + fi_tetha, liquidRadius, wideBottleY1, wideBottleY2), z(t, tetha + fi_tetha, liquidRadius));
		}
		glEnd;
		glColor3f(1.0f, 1.0f, 1.0f);
		// ======== END SURE LETS DRAW A CYLINDER ==========
		// ============== DRAWING THE FLOOR ================
		int i = 0;
		int j = 0;

		for(i = -100; i < 100; i += 5.0f){
			for (j = -100; j < 100; j += 5.0f){
				int temp = i + j;
				if ((temp % 2) == 0){
					sf::Texture::bind(&odd_tile_image);
				}
				else{
					sf::Texture::bind(&even_tile_image);
				}
				glBegin(GL_QUADS);
					glTexCoord2d(0,0);
					glVertex3i(i, 0, j);

					glTexCoord2d(1, 0);
					glVertex3i(i+5, 0, j);

					glTexCoord2d(1, 1);
					glVertex3i(i+5, 0, j+5);

					glTexCoord2d(0, 1);
					glVertex3i(i, 0, j + 5);
				glEnd();
			}
		}
		// ========== END DRAWING THE FLOOR ================
		/*
		glBegin(GL_QUADS);
			glColor3d(1.0,1.0,1.0);
			glVertex3i(2,2,0); 
			glVertex3i(2,-2,0);
			glVertex3i(-2,-2,0); 
			glVertex3i(-2,2,0);
		glEnd();
		*/
		

		// ================= DICE CRAP =================

		r += 2;
		glEnable(GL_TEXTURE_2D);
		sf::Texture::bind(&dice_img);

		glEnable(GL_DEPTH_TEST);
		glTranslatef(8, 2, 5);
		glRotated(r, 0.5, 1, 0.6);
		
		glBegin(GL_QUADS);
			glTexCoord2f(.25, .66);
			glVertex3dv(aaa);
			glTexCoord2f(0.0, .66);
			glVertex3dv(baa);
			glTexCoord2f(0, .33);
			glVertex3dv(bba);
			glTexCoord2f(.25, .33);
			glVertex3dv(aba);
		glEnd();
		glBegin(GL_QUADS);
			glTexCoord2d(.25, .66);
			glVertex3dv(aaa);
			glTexCoord2d(.5, .66);
			glVertex3dv(aab);
			glTexCoord2d(.5, .33);
			glVertex3dv(abb);
			glTexCoord2d(.25, .33);
			glVertex3dv(aba);
		glEnd();
		glBegin(GL_QUADS);
			glTexCoord2d(.5, .33);
			glVertex3dv(abb);
			glTexCoord2d(.75, .33);
			glVertex3dv(aba);
			glTexCoord2d(.75, .0);
			glVertex3dv(bba);
			glTexCoord2d(.5, .0);
			glVertex3dv(bbb);
		glEnd();
		glBegin(GL_QUADS);
			glTexCoord2d(1, .33);
			glVertex3dv(bba);
			glTexCoord2d(.75, .33);
			glVertex3dv(bbb);
			glTexCoord2d(.75, .66);
			glVertex3dv(bab);
			glTexCoord2d(1, .66);
			glVertex3dv(baa);
		glEnd();
		glBegin(GL_QUADS);
			glTexCoord2d(.75, .66);
			glVertex3dv(bab);
			glTexCoord2d(.75, .33);
			glVertex3dv(bbb);
			glTexCoord2d(.5, .33);
			glVertex3dv(abb);
			glTexCoord2d(.5, .66);
			glVertex3dv(aab);
		glEnd();
		glBegin(GL_QUADS);
			glTexCoord2d(.5, .66);
			glVertex3dv(aaa);
			glTexCoord2d(.75, .66);
			glVertex3dv(baa);
			glTexCoord2d(.75, 1);
			glVertex3dv(bab);
			glTexCoord2d(.5, 1);
			glVertex3dv(aab);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glTranslatef(0, 0, 0);
		// ================= END DICE CRAP =================
		   
        // Finally, display rendered frame on screen 
        App.display(); 
    } 
    return EXIT_SUCCESS; 
}
