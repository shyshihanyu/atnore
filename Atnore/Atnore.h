#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <io.h>
#include <Shlwapi.h>
#include <cstringt.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <shellapi.h>

using namespace std;

class CAtnore
{
public:
	CAtnore();
	~CAtnore();
public:
	void AtCreateDirectory();
	void ReadFile(string filename);
	void CopyFiles( string &srcPath , string &desPath );
	void FindFile1(string path);
	void AtGitLab();
	void DeleteDir( string &srcPath );
public:
	vector<string> m_lfileinfo;
	int m_ncount;
	string m_sdir;
	string m_spath;
};

