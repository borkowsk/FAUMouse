//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------
USEFORM("SDIMAIN.CPP", SDIAppForm);
USEFORM("About.cpp", AboutBox);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application->Initialize();
	Application->CreateForm(__classid(TSDIAppForm), &SDIAppForm);
		Application->CreateForm(__classid(TAboutBox), &AboutBox);
		Application->Run();

	return 0;
}
//---------------------------------------------------------------------
