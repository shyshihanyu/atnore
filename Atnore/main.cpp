#include "Atnore.h"


int main()
{
	CAtnore gt;
	string path = gt.m_spath;
	char szPath [100] = { 0 };
	GetModuleFileNameA( NULL , szPath , 100);
	char dir [100];
	char drive [100];
	_splitpath_s( szPath , drive , 100 , dir , 100 , NULL , 0 , NULL , 0);
	strcat_s(drive,dir);
	char *file = "conf.txt";
	strcat_s(drive,file);
	gt.ReadFile(drive);
	gt.AtCreateDirectory();

	//gt.AtGitLab();
	return 0;
}