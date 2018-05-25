#ifndef _GREETGL_H_
#define _GREETGL_H_

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif // !GLEW_STATIC

#include <logging/Log.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype-gl.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>

#define GLFW_JOYSTICKS				0x04

#define GLFW_JOYSTICK_A				0x00
#define GLFW_JOYSTICK_B				0x01
#define GLFW_JOYSTICK_X				0x02
#define GLFW_JOYSTICK_Y				0x03
#define GLFW_JOYSTICK_LEFT_BUMPER	0x04
#define GLFW_JOYSTICK_RIGHT_BUMPER	0x05
#define GLFW_JOYSTICK_SELECT		0x06
#define GLFW_JOYSTICK_START			0x07

#define GLFW_JOYSTICK_LEFT_STICK	0x08
#define GLFW_JOYSTICK_RIGHT_STICK	0x09

#define GLFW_JOYSTICK_PAD_UP		0x0A
#define GLFW_JOYSTICK_PAD_RIGHT		0x0B
#define GLFW_JOYSTICK_PAD_DOWN		0x0C
#define GLFW_JOYSTICK_PAD_LEFT		0x0D

#define GLFW_JOYSTICK_BUTTONS		14
#define GLFW_JOYSTICK_AXES			6

static void GLClearError()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR);
}

static void GLLogError(const char* glCall, const char* file, int line)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
		Greet::Log::Error("OpenGL error(",err,")", " at ", file,":",line," using ", glCall);
}



#if defined _DEBUG
	#define GLCall(x) GLClearError(); x; GLLogError(#x,__FILE__,__LINE__)
#else
	#define GLCall(x) x
#endif

#endif // _GREETGL_H_
