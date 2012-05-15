#include "fragment.h"

#define MODE_1 1
#define MODE_2 2
#define MODE_3 3

using namespace std;

void Mode1(char* fArgv[])
{
  printf("No Input!");
}

void Mode2(char* fArgv[])
{
  string code = System::ReadTextFile(fArgv[1]);
  imageProcess kernal(code);
  kernal.SetSize(System::MaxTexSize(), System::MaxTexSize());
  kernal.Exicute();
  Image outputImage = kernal.GetOutputImage();
  outputImage.Save("default.jpg");
}

void Mode3(char* fArgv[])
{
  
}

int main(int argc, char* argv[])
{
  System::Init(1,1);
  
  switch(argc)
  {
    case MODE_1:
	  Mode1(argv);
	break;
	
	case MODE_2:
	  Mode2(argv);
	break;
	
	case MODE_3:
	  Mode3(argv);
	break;
	
    default:
	break;
  }
  
  System::End();
  return 0;
}