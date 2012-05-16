#include <stdlib.h>
#include "fragment.h"

#define FRAGMENT_CODE 0
#define OUTPUT_IMAGE 1
#define OUTPUT_BIN 2
#define GPU_SIZE_X 3
#define GPU_SIZE_Y 4

using namespace std;


bool StringEqual(string fStr1, string fStr2)
{
  if(fStr1.length() == fStr2.length())
  {
    for(int i = 0; i < fStr1.length(); i++)
	{
	  if(fStr1[i] != fStr2[i])
	    return false;
    }
  }
  else
  {
    return false;
  }
  return true;
}

int StringToInt(string fStr)
{
  return atoi(fStr.c_str());
}

string FindParameter(int argc, char* fArgv[], int fMarker)
{
  string temp;
  string finalVal;
  string testStr;
  
  for(int i = 0; i < argc-1; i++)
  {
    temp = fArgv[i];
	switch(fMarker)
	{
	  case FRAGMENT_CODE:
	    testStr = "-i";
	    if(StringEqual(temp, testStr))
		{
		  finalVal = fArgv[i+1];
		  return  finalVal;
		}
	  break;
	  
	  case OUTPUT_IMAGE:
	    testStr = "-o";
	    if(StringEqual(temp, testStr))
		{
		  finalVal = fArgv[i+1];
		  return  finalVal;
		}
	  break;
	  
	  case GPU_SIZE_X:
	    testStr = "-x";
	    if(StringEqual(temp, testStr))
		{
		  finalVal = fArgv[i+1];
		  return  finalVal;
		}
	  break;
	  
	  case GPU_SIZE_Y:
	    testStr = "-y";
	    if(StringEqual(temp, testStr))
		{
		  finalVal = fArgv[i+1];
		  return  finalVal;
		}
	  break;
    }
  }
  
  return finalVal;
}

void RunKernal(string fFnFragCode, string fFnOutputImage, int fSizeX, int fSizeY)
{
  string code = System::ReadTextFile(fFnFragCode);
  if(code.length() > 0)
  {
    imageProcess kernal(code);
    kernal.SetSize(fSizeX, fSizeY);
    kernal.Exicute();
    Image outputImage = kernal.GetOutputImage();
    outputImage.Save(fFnOutputImage.c_str());
	printf("Done!");
  }
  else 
  {
    printf("couldnt load code file");
  }
}

int main(int argc, char* argv[])
{
  System::Init(1,1);
  
  if(argc>1)
  { 
    string fnFragmentCode = FindParameter(argc, argv, FRAGMENT_CODE);
	string fnOutputImage = FindParameter(argc, argv, OUTPUT_IMAGE);
	string strSizeX = FindParameter(argc, argv, GPU_SIZE_X);
	string strSizeY = FindParameter(argc, argv, GPU_SIZE_Y);
    int sizeX = System::MaxTexSize();
    int sizeY = System::MaxTexSize();
	
	if(fnFragmentCode.length() > 0)
	{
	  if(fnOutputImage.length() == 0)
	  {
	    fnOutputImage = "default.jpg";
	  }
	  
	  if(strSizeX.length() != 0)
	  {
	    sizeX = StringToInt(strSizeX);
	  }
	  
	  if(strSizeY.length() != 0)
	  {
	    sizeY = StringToInt(strSizeY);
	  }
	  
	  if(sizeX != 0 && sizeY != 0)
	  {
	    RunKernal(fnFragmentCode, fnOutputImage, sizeX, sizeY);
	  }
	  else 
	  {
	    printf("size values not correct");
	  }
    }
	else
	{
	  printf("No GPU fragment code provided");
    }
  }
  else
  {
    printf("not enough arguments!");
  }
  
  System::End();
  return 0;
}