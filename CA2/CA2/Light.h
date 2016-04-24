
#include <SFML/OpenGL.hpp>

#ifndef CA2_LIGHT_H
#define CA2_LIGHT_H

class Light
{
	
public:
    GLenum source;

    Light(GLenum source)
    {
        this->source = source;
    }
    
    void setfv(GLenum pname, const GLfloat *params)
    {
        glLightfv(source, pname, params);
    }
    
    void setfv(GLenum pname, const std::initializer_list<GLfloat>& list)
    {
        std::vector<GLfloat> vector = list;
        setfv(pname, &vector[0]);
    }
    
    void setf(GLenum pname, GLfloat param)
    {
        glLightf(source, pname, param);
    }
    
    void enable()
    {
        glEnable(source);
    }
    
    void disable()
    {
        glDisable(source);
    }
};

#endif
