#include "framework.h"
#include <fstream>

int WINAPI WinMain(HINSTANCE hInstance, 
						 HINSTANCE hPrevInstance,
						 LPSTR lpCmdline,
						 int showCmd
						)
{
	Framework* framework_Inst;
	bool result = true;
	std::ofstream fout;
	
	fout.open("error.log");
	
	framework_Inst = new Framework;
	if(!framework_Inst){
		fout << "Framework Not Created /n";
		return 0;
	}
	
	result = framework_Inst->Initialize(&fout);
	if(result)
	{
		fout << "It Worked!";
		framework_Inst->Run();
	}
	else
	{
		fout << "Framework Failed" << std::endl;
	}
	
	framework_Inst->Shutdown();
	delete framework_Inst;
	framework_Inst = 0;
	
	fout << "closing";
	fout.close();
	
	return 0;	
}














