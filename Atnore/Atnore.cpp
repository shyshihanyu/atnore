#include "Atnore.h"

#include <time.h>

#include <sstream>
#include <iostream>

CAtnore::CAtnore()
{
}


CAtnore::~CAtnore()
{
	m_lfileinfo.clear();
}

void CAtnore::ReadFile( string filename )
{
	ifstream in( filename );
	string line;

	if (in) // 有该文件  
	{
		while (!in.eof()) // line中不包括每行的换行符  
		{
			getline( in , line );
			cout << line << endl;
			m_lfileinfo.push_back( line );
			m_ncount++;
		}
	}
	else // 没有该文件  
	{
		cout << "no such file" << endl;
	}
	m_spath = m_lfileinfo.at( 0 );
	m_sdir = m_lfileinfo.at( 1 );
	DeleteDir( m_sdir );
}

void CAtnore::AtCreateDirectory()
{
	/**********文件夹加时间******************/
	SYSTEMTIME st = { 0 };
	GetLocalTime( &st );
	int day = st.wDay;
	int yue = st.wMonth;
	string timeday;
	stringstream ss;
	ss << yue << day;
	timeday = ss.str();

	string directory = m_lfileinfo.at( 1 ) + timeday;
	/************************************************/
	if (0 != _access( directory.c_str() , 0 ))
	{
		// if this folder not exist, create a new one.
		_mkdir( directory.c_str() );

	}
	string srcpath = m_spath;
	string despath = directory;
	CopyFiles(srcpath,despath);
	FindFile1(despath);

}

void CAtnore::DeleteDir( string &srcPath )
{
	if (srcPath == "")
	{
		return;
	}
	char buf [100] = { 0 };
	_finddata_t FileInfo;
	string startFindPath = srcPath + "\\*";
	intptr_t handle = _findfirst( startFindPath.c_str() , &FileInfo );  //	
	if (handle == -1L) { return; }
	do {
		if (FileInfo.attrib == _A_SUBDIR) //判断是否为子文件夹		
		{
			if (( strcmp( FileInfo.name , "." ) != 0 ) && ( strcmp( FileInfo.name , ".." ) != 0 ))  //过滤掉本代表本目录的.和上一级目录的..			
			{
				string newPath = srcPath + "\\" + FileInfo.name;
				DeleteDir(newPath);
			}
		}
		else
		{
			string srcFilePath = srcPath + "\\" + FileInfo.name;
			remove(srcFilePath.c_str());
		}
	} while (_findnext( handle , &FileInfo ) == 0);
	_findclose( handle );
	size_t origsize = srcPath.length() + 1;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc( sizeof( wchar_t )*( srcPath.length() - 1 ) );
	mbstowcs_s( &convertedChars , wcstring , origsize , srcPath.c_str() , _TRUNCATE );
	RemoveDirectory( wcstring );
}

void CAtnore::CopyFiles( string &srcPath , string &desPath )
{

	if (srcPath == "")
	{
		return;
	}
	char buf [100] = { 0 };
	_finddata_t FileInfo;
	string startFindPath = srcPath + "\\*";
	intptr_t handle = _findfirst( startFindPath.c_str() , &FileInfo );  //	
	if (handle == -1L) { return; }
	do {
		if (FileInfo.attrib == _A_SUBDIR) //判断是否为子文件夹		
		{
			if (( strcmp( FileInfo.name , "." ) != 0 ) && ( strcmp( FileInfo.name , ".." ) != 0 ))  //过滤掉本代表本目录的.和上一级目录的..			
			{
				cout << "subdir:" << FileInfo.name << endl;
				string newPath = srcPath + "\\" + FileInfo.name;
				string newdesPath = desPath + "\\" + FileInfo.name;
				if (_access( newdesPath.c_str() , 0 ) == -1)  //判断组合好的目录是否已经存在，不存在则创建				
				{
					_mkdir( newdesPath.c_str() );
				}
				CopyFiles( newPath , newdesPath );
			}
		}
		else
		{
			string srcFilePath = srcPath + "\\" + FileInfo.name;
			string desFilePath = desPath + "\\" + FileInfo.name;
			ifstream fin( srcFilePath , ios::_Nocreate | ios::binary );
			ofstream fout( desFilePath , ios::binary );
			if (!fin)
			{
				cout << "源文件路径没有找到:" << srcFilePath << endl;
				continue;
			}
			if (!fout)
			{
				cout << "打开或者创建文件失败:" << desFilePath << endl;
				continue;
			}

			/*
				fin.read( buf , sizeof( buf ) );
				fout.write( buf , sizeof( buf ) );*/
			size_t origsize = srcFilePath.length() + 1;
			//const size_t newsize = 100;
			size_t convertedChars = 0;
			wchar_t *wcstring = (wchar_t *)malloc( sizeof( wchar_t )*( srcFilePath.length() - 1 ) );
			mbstowcs_s( &convertedChars , wcstring , origsize , srcFilePath.c_str() , _TRUNCATE );
			size_t des = desFilePath.length() + 1;
			size_t converteddesChars = 0;
			wchar_t *wdstring = (wchar_t *)malloc( sizeof( wchar_t )*( desFilePath.length() - 1 ) );
			mbstowcs_s( &converteddesChars , wdstring , des , desFilePath.c_str() , _TRUNCATE );
			CopyFile( wcstring , wdstring , FALSE );
			cout << srcFilePath << endl;
		}
	} while (_findnext( handle , &FileInfo ) == 0);
	_findclose( handle );
}

void CAtnore::FindFile1( string path )
{
	intptr_t hFile = 0;
	struct _finddata_t fileInfo;
	string pathName , exdName;

	if (( hFile = _findfirst( pathName.assign( path ).append( "\\*" ).c_str() , &fileInfo ) ) == -1)
	{
		return;
	}
	do
	{
		if (fileInfo.attrib&_A_SUBDIR)
		{
			string fname = string( fileInfo.name );
			if (fname != ".." && fname != ".")
			{
				FindFile1( path + "\\" + fname );
			}
		}
		else
		{

			char ext [10];
			_splitpath_s( fileInfo.name , NULL , 0 , NULL , 0 , NULL , 0 , ext , 10 );
			for (int i = 3; i < m_ncount; i++)
			{
				if (strcmp( ext , m_lfileinfo.at( i ).c_str() ) == 0)
				{
					char newname [100] = { 0 };
					string newpath = path + "\\";
					string newdir = m_sdir + "\\";
					newpath += fileInfo.name;
					sprintf_s( newname , sizeof( newname ) , "%s%s" , newdir.c_str() , fileInfo.name );

					int  err = remove( newpath.data() );
					if (err == 0)
					{
						cout << GetLastError() << endl;
					}
					cout << fileInfo.name << endl;

					break;
				}

			}

		}
	} while (_findnext( hFile , &fileInfo ) == 0);
	_findclose( hFile );
}

void CAtnore::AtGitLab()
{
	string a = m_sdir;
	SetCurrentDirectoryA( a.c_str() );
	/*string exe1 = "git init";
	WinExec(exe1.c_str(), SW_HIDE);
	string exe7 = "git branch Dll";
	WinExec(exe7.c_str(), SW_HIDE);
	string exe8 = "git checkout Dll";
	WinExec(exe8.c_str(), SW_HIDE);


	string exe2 = "git add .";
	WinExec(exe2.c_str(), SW_HIDE);

	string exe3 = "git commit -m 'update' ";
	WinExec(exe3.c_str(), SW_HIDE);
	string exe5 = "git remote add origin https://192.168.16.235/jinguanglu/ATDS_release.git ";
	WinExec(exe5.c_str(), SW_HIDE);*/
	/*string exe6 = "git pull --rebase origin master ";
	WinExec(exe6.c_str(), SW_HIDE);*/
	string exe9 = "git tag -a 'v0.2' -m 'v0.2'";
	WinExec( exe9.c_str() , SW_HIDE );
	string exe4 = "git push  origin master ";
	WinExec( exe4.c_str() , SW_HIDE );

}