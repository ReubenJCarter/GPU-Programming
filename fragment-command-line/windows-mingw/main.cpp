#include <stdlib.h>
#include "fragment.h"


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

bool FindMarker(int fArgc, char* fArgv[], string fMarker)
{
  string temp;
  for(int i = 1; i < fArgc; i ++)
  {
    temp = fArgv[i];
    if(StringEqual(temp, fMarker))
	{
	  return true;
	}
  }
  return false;
}

string FindParameter(int fArgc, char* fArgv[], string fMarker)
{
  string temp;
  string finalVal;
  
  for(int i = 0; i < fArgc-1; i++)
  {
    temp = fArgv[i];
	if(StringEqual(temp, fMarker))
	{
	  finalVal = fArgv[i + 1];
	  return finalVal;
	}
  }
  return finalVal;
}

void RunKernal(string fFnFragCode, string fFnInputImage, string fFnOutputImage, int fSizeX, int fSizeY, bool fResize)
{
  string code = System::ReadTextFile(fFnFragCode);
  if(code.length() > 0)
  {
    if(fFnInputImage.length() > 0)
	{
      imageProcess kernal(code);
	  Image image(fFnInputImage.c_str());
	  if(fResize)
	  {
	    image = image.PowerTwoConvert();
	  }
	  kernal.PassImage("image", image, 0);
	  kernal.Exicute();
      Image outputImage = kernal.GetOutputImage();
      outputImage.Save(fFnOutputImage.c_str()); 
	}
	else
	{
      imageProcess kernal(code);
      kernal.SetSize(fSizeX, fSizeY);
      kernal.Exicute();
      Image outputImage = kernal.GetOutputImage();
      outputImage.Save(fFnOutputImage.c_str());
	}
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
    string fnFragmentCode = FindParameter(argc, argv, "-i");
	string fnOutputImage = FindParameter(argc, argv, "-o");
	string strSizeX = FindParameter(argc, argv, "-x");
	string strSizeY = FindParameter(argc, argv, "-y");
	string fnInputImage = FindParameter(argc, argv, "-r");
	bool resize = FindMarker(argc, argv, "-z");
	
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
	    RunKernal(fnFragmentCode, fnInputImage, fnOutputImage, sizeX, sizeY, resize);
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