/*
Shader Image 

written by Reuben Carter

A program for processing and generating complex images at high speeds. 
It allows the user to exicute opengl shader language programs on images, or data.
The results can be saved in image format. It also provides a number of pre included 
functions for openGL shader language, making image processing much simpler. This is 
the multi os version, which depends on DevIL, openGL, GLFW, and GLEW. It was hacked together
in a few hours, and as such, it is terrible.
*/

#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <IL/il.h> 
#include <string>


class System
{
  public:
		static void CheckGLErrors() 
		{
		  GLenum errCode;
		  const GLubyte *errStr;
		  if ((errCode = glGetError()) != GL_NO_ERROR) 
		  {
		    errStr = gluErrorString(errCode);
			printf("%s\n", (char*)errStr);
		  }
		}

		static void End()
		{
		  glfwTerminate();
		}

		static void Mapping(unsigned int funcWidth, unsigned int funcHeight)
		{
		  glMatrixMode(GL_PROJECTION);
		  glLoadIdentity();
		  glOrtho(0.0, funcWidth, 0.0, funcHeight, -1000000, 1000000);
		  glViewport(0, 0, funcWidth, funcHeight); 
		  glMatrixMode(GL_MODELVIEW);
		  glLoadIdentity();
		}

		static void Init(int funcWidth, int funcHeight)
		{
		  if(!glfwInit())
			printf("WINDOW: can't instalize glfw.\n");
		  if(!glfwOpenWindow(funcWidth, funcHeight, 0,0,0,0,0,0, GLFW_WINDOW ))
			printf("WINDOW: Cant Open a new window.\n");
		  if(GLEW_OK!=glewInit())
			printf( "GLEW: glew failed to installize.\n%s\n");
		  if(GLEW_OK!=glewInit())
			printf( "GLEW: glew failed to installize.\n%s\n");
		  ilInit();
		  ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		  ilEnable(IL_ORIGIN_SET);
		  Mapping(funcWidth, funcHeight);
		  glDisable(GL_BLEND);
		  glEnable(GL_TEXTURE_2D);
		  glDisable(GL_DEPTH_TEST);
		  glDisable(GL_LIGHTING);
		}

		static int MaxTexSize()
		{
		  int size;
		  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
		  return size;
		}
		
		static std::string ReadTextFile(std::string fFileName)
		{
          FILE* fptr;
          fptr = fopen(fFileName.c_str(), "rb");
		  std::string temp;
		  if(fptr != 0)
		  {
			char c;
			while( c != EOF )
			{
			  c = fgetc(fptr);
			  if(c >= 0 && c<= 127)
			  temp += c;
			}
			fclose(fptr);
		  }
		  else 
		  {
			printf("Failed to open text file");
		  }
		  return temp;
		}
};


class Image
{
  private:
          int width;
          int height;
          unsigned char* data;
          #define PI 3.1415972
          
  public:
         Image()
         {
           data=0;
           width=0;
           height=0;
         }
         
         Image(int funcW, int funcH)
         {
           CreateBlank(funcW, funcH);
         }
         
         Image(char* funcFN)
         {
           Load(funcFN);
         }
         
         Image PowerTwoConvert()
         {
           int nearestPow=1;
           int n;
           if(width >height)
             n= width;
           else 
             n= height;
           while(nearestPow<n)  
             nearestPow<<=1;
             
           Image temp(nearestPow, nearestPow);
           for(int i=0; i<width; i++)
           {
             for(int j=0; j<height; j++)
             {
               temp.SetR(i, j, GetR(i, j));
               temp.SetG(i, j, GetG(i, j));
               temp.SetB(i, j, GetB(i, j));
             }
           }
           
           return temp;
         }
         
         void CopyFrom(Image funcIm)
         {
           width=funcIm.width;
           height=funcIm.height;
           data= new unsigned char[width*height*3];
           for(int i=0; i<width*height*3; i++)
           {
             data[i]=funcIm.data[i];
           }
         }
         
         void operator=(Image funcIm)
         {
           CopyFrom(funcIm);
         }
         
         void Clear()
         {
           for(int i=0; i<width*height*3; i++)
           {
             data[i]=0;
           }      
         }
         
         void CreateBlank(int funcW, int funcH)
         {
           width=funcW;
           height=funcH;
           data= new unsigned char[width*height*3]; 
           for(int i=0; i<width*height*3; i++)
           {
             data[i]=0;
           }      
         }
         
         unsigned char GetR(int funcX,  int funcY)
         {
           if((funcX<width)&&(funcY<height))
             return data[(funcY*width+funcX)*3];
         }
         
         unsigned char GetG(int funcX,  int funcY)
         {
           if((funcX<width)&&(funcY<height))
             return data[(funcY*width+funcX)*3+1];
         }
         
         unsigned char GetB(int funcX,  int funcY)
         {
           if((funcX<width)&&(funcY<height))
             return data[(funcY*width+funcX)*3+2];
         }
         
         void SetR(int funcX,  int funcY, unsigned char funcValue)
         {
           if((funcX<width)&&(funcY<height))
             data[(funcY*width+funcX)*3]=funcValue;
         }
         
         void SetG(int funcX,  int funcY, unsigned char funcValue)
         {
           if((funcX<width)&&(funcY<height))
             data[(funcY*width+funcX)*3+1]=funcValue;
         }
         
         void SetB(int funcX,  int funcY, unsigned char funcValue)
         {
           if((funcX<width)&&(funcY<height))
             data[(funcY*width+funcX)*3+2]=funcValue;
         }
         
         int GetWidth()
         {
           return width;
         }
         
         int GetHeight()
         {
           return height;
         }
         
         unsigned char* GetData()
         {
           return data;
         }

         void Load(const char* funcFN)
         {
           ILboolean able;  
           ILuint ilimage;
           ilGenImages(1, &ilimage);
           ilBindImage(ilimage);
           able=ilLoadImage(funcFN);
           if(able==true)
           {            
             able=ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
             if(able!=true)
             {
               printf("IMAGE LOADING: failed to convert image to unsigned byte\n");
             }
             data=ilGetData(); 
             width=ilGetInteger(IL_IMAGE_WIDTH);
             height=ilGetInteger(IL_IMAGE_HEIGHT);
           }
           else 
           {
             printf("IMAGE LOADING: failed to load image\n");
           }
         } 
         
         void Save(const char* funcFN)
         {
           ILboolean able;  
           ILuint ilimage;
           ilGenImages(1, &ilimage);
           ilBindImage(ilimage);
           able=ilTexImage(width, height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, data);
           
           if(able!=true)
           {
             printf("IMAGE SAVING: failed to copy data to a bound image for saving\n");
           }
  
           ilEnable(IL_FILE_OVERWRITE);
           ilSaveImage(funcFN);
           ilDisable(IL_FILE_OVERWRITE);
           ilDeleteImages(1, &ilimage);
         }      
};


class shader
{
  private:
          unsigned int vertexShader;
          unsigned int fragmentShader;
          unsigned int programObject;
          
  public:
         shader()
         {
         }
         
         shader(const char** funcVerCode, int fVertexNumber, const char** funcFragCode, int fFragmentNumber)
         {
           Load(funcVerCode, fVertexNumber, funcFragCode, fFragmentNumber);
         }
         
         void Load(const char** funcVerCode, int fVertexNumber, const char** funcFragCode, int fFragmentNumber)
         {  
           glDeleteObjectARB(vertexShader);
           glDeleteObjectARB(fragmentShader);
           glDeleteObjectARB(programObject);
           const char** vCode= funcVerCode;
           const char** fCode= funcFragCode;
           vertexShader=glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
           fragmentShader=glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
           glShaderSourceARB(vertexShader, fVertexNumber, vCode, NULL);
           glShaderSourceARB(fragmentShader, fFragmentNumber, fCode, NULL);
           glCompileShaderARB(vertexShader);
           glCompileShaderARB(fragmentShader);
           programObject=glCreateProgramObjectARB();
           glAttachObjectARB(programObject, vertexShader);
           glAttachObjectARB(programObject, fragmentShader);
           glLinkProgramARB(programObject);
           Debug();
         }
         
         void Debug()
         {
           int infologLength=0;
           int charsWritten=0;
           char* infoLog;
           glGetObjectParameterivARB(programObject, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);
           if (infologLength>1)
           {
             infoLog=new char[infologLength];
             glGetInfoLogARB(programObject, infologLength, &charsWritten, infoLog);
             printf("%s\n",infoLog);
             delete [] infoLog;
           }
         }
         
         void Use()
         {
           glUseProgramObjectARB(programObject);
         }
         
         void Pass(char* funcName, int funcVal)
         {
           glUseProgramObjectARB(programObject); 
           int location=glGetUniformLocation(programObject, funcName);
           if (location!=-1)
           {
             glUniform1i(location, funcVal);
           }  
           glUseProgramObjectARB(0);           
         }
         
         void Pass(char* funcName, float funcVal)
         {
           glUseProgramObjectARB(programObject); 
           int location=glGetUniformLocation(programObject, funcName);
           if (location!=-1)
           {
             glUniform1f(location, funcVal);
           }  
           glUseProgramObjectARB(0);          
         }
         
         void PassTexture(char* funcName, unsigned int funcTextureId, int funcTextureUnit)
         {
           glUseProgramObjectARB(programObject); 
           glActiveTexture(GL_TEXTURE0+funcTextureUnit);
           int location= glGetUniformLocationARB(programObject, funcName);
           glBindTexture(GL_TEXTURE_2D, funcTextureId);
           glUniform1iARB(location, funcTextureUnit);
           glUseProgramObjectARB(0); 
         }
};

class GPUcode
{
  public:
		 static std::string defaultVertexCode;
		 static std::string fragmentShaderHeader;
};

std::string GPUcode::defaultVertexCode = 
"void main()"
"{"
"  gl_TexCoord[0] = gl_MultiTexCoord0;"
"  gl_TexCoord[1] = gl_MultiTexCoord1;"
"  gl_TexCoord[2] = gl_MultiTexCoord2;"
"  gl_Position = ftransform();"
"}";

std::string GPUcode::fragmentShaderHeader =
"vec4 TexturePixel(sampler2D funcTexture, int funcTexUnit)\n"
"{\n"
"  	vec4 col= texture2D(funcTexture, gl_TexCoord[funcTexUnit].st);\n"
"	return col;\n"
"}\n"
"vec4 TexturePixel(sampler2D funcTexture, int funcTexUnit, vec2 funcCoord)\n"
"{\n"
" 	vec4 col= texture2D(funcTexture, funcCoord);\n"
"	return col;\n"
"}\n"
"vec2 Coord()\n"
"{ \n"
"	return gl_TexCoord[0].st;\n"
"}\n"
"int CoordIntX(int funcWidth)\n"
"{"
"  	return int(float(funcWidth)*Coord().x);\n"
"}\n"
"int CoordIntY(int funcHeight)\n"
"{\n"
"  	return int(float(funcHeight)*Coord().y);\n"
"}\n"
"vec2 Coord(int funcX, int funcY, int funcTextureWidth, int funcTextureHeight)\n"
"{\n"
"	vec2 result;\n"
"	result.x=1.0/(float(funcTextureWidth)*2.0)+float(funcX)*(1.0/float(funcTextureWidth));\n"
"	result.y=1.0/(float(funcTextureHeight)*2.0)+float(funcY)*(1.0/float(funcTextureHeight));\n"
"	return result;\n"
"}\n";


class imageProcess: public shader
{
  private:
          #define MAXTEXNUM 3
          int width;
          int height;
          unsigned int texture[MAXTEXNUM];
          unsigned int outputTexture;
          unsigned int frameBuffer;
          
          void CreateFrameBuffer()
          {
            glDeleteTextures(1, &frameBuffer);
            glGenTextures (1, &outputTexture);
            glBindTexture(GL_TEXTURE_2D, outputTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA , GL_FLOAT, 0); 
  
            glGenFramebuffersEXT(1, &frameBuffer);
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, outputTexture, 0);
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
          }
          
          void RenderQuad(int funcWidth, int funcHeight)
          {
               
            for(int i=0; i<MAXTEXNUM; i++)
            {
              glActiveTexture(GL_TEXTURE0+i);
              glBindTexture(GL_TEXTURE_2D, texture[i] );
            }
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
            System::Mapping(width, height);
            glLoadIdentity();
            glClear(GL_COLOR_ATTACHMENT0_EXT);
            glPolygonMode(GL_FRONT,GL_FILL);
            glBegin(GL_QUADS);
              glTexCoord2f(0, 0);
              glVertex3f(0, 0, 0);
              glTexCoord2f(0, 1);
              glVertex3f(0, funcHeight, 0);
              glTexCoord2f(1, 1);
              glVertex3f(funcWidth, funcHeight, 0);
              glTexCoord2f(1, 0);
              glVertex3f(funcWidth, 0, 0);
            glEnd();  
            glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);    
          }
          
  public:        
         imageProcess( std::string funcFragCode)
         {
           LoadFrag(funcFragCode);
         }
         
         imageProcess()
         {
           width=0;
           height=0;
           frameBuffer=0;
           outputTexture=0; 
         }
         
         void LoadFrag(std::string funcFragCode)
         {
		   std::string tempString;
           const char* temp1[1];
           temp1[0] = GPUcode::defaultVertexCode.c_str();
		   const char* temp2[2];
           temp2[0] = GPUcode::fragmentShaderHeader.c_str();	   
		   temp2[1] = funcFragCode.c_str();	
		   
		   Load(temp1, 1, temp2, 2);
           width=0;
           height=0;
           frameBuffer=0;
           outputTexture=0; 
         }
         
         void SetSize(int funcWidth, int funcHeight)
         {
           width=funcWidth; 
           height=funcHeight; 
         }
         
         void RenderOutput(int funcWidth, int funcHeight)
         {
           glUseProgramObjectARB(0);
           glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
           glPolygonMode(GL_FRONT,GL_FILL);
           glActiveTexture(GL_TEXTURE0);    
           glBindTexture(GL_TEXTURE_2D, outputTexture);
           glBegin(GL_QUADS);
             glTexCoord2f(0, 0);
             glVertex3f(0, 0, 0);
             glTexCoord2f(0, 1);
             glVertex3f(0, funcHeight, 0);
             glTexCoord2f(1, 1);
             glVertex3f(funcWidth, funcHeight, 0);
             glTexCoord2f(1, 0);
             glVertex3f(funcWidth, 0, 0);
           glEnd();   
           glBindTexture(GL_TEXTURE_2D, 0);
         }
         
         void RenderOutput()
         {
           RenderOutput(width, height);
         }
         
         void PassImage(char* funcName, Image funcIm, int funcTexUnitIndex)
         {
           glGenTextures(1, &texture[funcTexUnitIndex]);
           glBindTexture(GL_TEXTURE_2D, texture[funcTexUnitIndex]);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
           glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
           glTexImage2D(GL_TEXTURE_2D, 0, 3, funcIm.GetWidth(), funcIm.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
           glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, funcIm.GetWidth(), funcIm.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, funcIm.GetData());
           glBindTexture(GL_TEXTURE_2D, 0);   
           
           PassTexture(funcName, texture[funcTexUnitIndex], funcTexUnitIndex);
           if(funcTexUnitIndex==0)
           {
             SetSize(funcIm.GetWidth(), funcIm.GetHeight());
             CreateFrameBuffer();
           }
         }
         
         void PassPrevOutput(char* funcName, int funcTexUnitIndex)
         {
           PassTexture(funcName, outputTexture, funcTexUnitIndex);
         }
         
         void CleanUpTexture()
         {
           glDeleteTextures(MAXTEXNUM, texture);
         }
         
         Image Exicute()
         {
		   CreateFrameBuffer();
           Use();
           RenderQuad(width, height);
           glUseProgramObjectARB(0);
         }
         
         unsigned int GetOutputTexture()
         {
           return outputTexture;
         }
         
         Image GetOutputImage()
         {
           Image temp(width, height);
           glBindTexture(GL_TEXTURE_2D, outputTexture);
           glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, temp.GetData());
           glBindTexture(GL_TEXTURE_2D, 0);
           return temp;
         }
};