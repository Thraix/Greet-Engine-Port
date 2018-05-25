#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>
#include <string.h>

BYTE* loadImage(const char* filepath, unsigned int* width, unsigned int* height, unsigned int* bpp)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

		FIBITMAP *dib = NULL;

		fif = FreeImage_GetFileType(filepath, 0);

		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filepath);

		if (fif == FIF_UNKNOWN)
		{
      std::cout << "Something went wrong" << std::endl;
			return NULL;
    }


		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filepath);
		if (!dib)
		{
      std::cout << "Something went wrong" << std::endl;
			return NULL;
    }


		BYTE* bits = FreeImage_GetBits(dib);

		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);
		*bpp = FreeImage_GetBPP(dib);
		if (*bpp != 24 && *bpp != 32)
		{
      std::cout << "Something went wrong" << std::endl;
			delete[] bits;
			return NULL; 
		}

		//printImage(bits,*width, *height, *bpp);
		//return ImageFactory::getBadBPPImage(width, height, bpp);

		int size = (*width) * (*height) * (*bpp >> 3);
		BYTE* result = new BYTE[size];
		memcpy(result,bits,size);
		FreeImage_Unload(dib);
		//delete[] bits;
		return result;
	}
struct Texture
{
  unsigned int m_texId;
  Texture(const std::string& filename)
  {
    unsigned int w,h,bpp;
    BYTE* bits = loadImage(filename.c_str(), &w, &h, &bpp);
    if (m_texId != 0)
			glDeleteTextures(1,&m_texId);
		glGenTextures(1, &m_texId);

		glBindTexture(GL_TEXTURE_2D, m_texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, bpp == 32 ? GL_RGBA : GL_RGB, w,h, 0, bpp == 32 ? GL_BGRA : GL_BGR, GL_UNSIGNED_BYTE, bits);
		glBindTexture(GL_TEXTURE_2D, 0);
  }

  void Bind()
  {
    glBindTexture(GL_TEXTURE_2D, m_texId);
  }
};


int main()
{
  GLFWwindow* window;

  if(!glfwInit())
    exit(1);

  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if(!window)
  {
    glfwTerminate();
    exit(1);
  }

  glfwMakeContextCurrent(window);
  glEnable(GL_TEXTURE_2D);
  Texture texture("image.png");
  texture.Bind();

  while(!glfwWindowShouldClose(window))
  {
    int ww,wh;
    glfwGetFramebufferSize(window,&ww,&wh);
    glViewport(0,0,ww,wh);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.5, 1.0);
    glVertex2f(0, 0.5);
    glTexCoord2f(0.0, 0.0);
    glVertex2f(-0.5, -0.5);
    glTexCoord2f(1.0, 0.0);
    glVertex2f(0.5, -0.5);
    glEnd();

    glfwSwapBuffers(window);
    glfwPollEvents();
    
  }

  std::cout << "Hello freetype :)" << std::endl;
  return 0;
}
