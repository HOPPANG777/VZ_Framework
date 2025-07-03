#include "Framework.h"
#include "Path.h"

// 파일 존재 여부
bool Path::ExistFile(std::wstring path)
{
	// 파일이나 폴더의 속성을 알아보는 함수 DWORD를 반환
	// 실패하면 INVALID_FILE_ATTRIBUTES를 반환
	DWORD fileValue = GetFileAttributes(path.c_str());

	// 파일이 존재할경우 True 없을경우 False
	return fileValue != INVALID_FILE_ATTRIBUTES;
}

bool Path::ExistFile(std::string path)
{
	return ExistFile(String::ToWstring(path));
}

// 폴더 존재 여부
bool Path::ExistDirectory(std::wstring path)
{
	DWORD attribute = GetFileAttributes(path.c_str());

	BOOL temp = (attribute != INVALID_FILE_ATTRIBUTES &&
		attribute & FILE_ATTRIBUTE_DIRECTORY);	// 이 조건문이 참이면 검사 중인 항목이 폴더

	return temp == TRUE;
}

bool Path::ExistDirectory(std::string path)
{
	return ExistDirectory(String::ToWstring(path));
}

// 병합
std::string Path::Combine(std::string path1, std::string path2)
{
	return path1 + path2;
}

std::wstring Path::Combine(std::wstring path1, std::wstring path2)
{
	return path1 + path2;
}

std::string Path::Combine(std::vector<std::string> paths)
{
	std::string temp = "";
	for (std::string path : paths)
		temp += path;

	return temp;
}

std::wstring Path::Combine(std::vector<std::wstring> paths)
{
	std::wstring temp = L"";
	for (std::wstring path : paths)
		temp += path;

	return temp;
}

// 입력된 경로에서 폴더 이름을 반환
std::string Path::GetDirectoryName(std::string path)
{
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

std::wstring Path::GetDirectoryName(std::wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

// 파일의 확장자를 반환
std::string Path::GetExtension(std::string path)
{
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());;
}

std::wstring Path::GetExtension(std::wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());;
}

// 파일 이름과 확장자를 반환
std::string Path::GetFileName(std::string path)
{
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

std::wstring Path::GetFileName(std::wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

// 파일 이름만 반환
std::string Path::GetFileNameWithoutExtension(std::string path)
{
	std::string fileName = GetFileName(path);

	size_t index = fileName.find_last_of('.');
	return fileName.substr(0, index);
}

std::wstring Path::GetFileNameWithoutExtension(std::wstring path)
{
	std::wstring fileName = GetFileName(path);

	size_t index = fileName.find_last_of('.');
	return fileName.substr(0, index);
}

// 특정 확장자 파일의 필터
const WCHAR* Path::ImageFilter = L"Image\0*.png;*.bmp;*.jpg";
const WCHAR* Path::ShaderFilter = L"HLSL file\0*.hlsl";
const WCHAR* Path::TextFilter = L"Text file\0*.txt";
const WCHAR* Path::TileFilter = L"Tile file\0*.png;*.bmp;*.jpg;*.json;*.data";
const WCHAR* Path::TMapFilter = L"TMap file\0*.tmap";

// 파일 선택 대화 상자를 열어 사용자가 파일을 선택하도록 해줌
void Path::OpenFileDialog(std::wstring defaultFileName, const WCHAR* fileTypeFilter, const std::wstring defaultFolder, const std::function<void(std::wstring)> callback, HWND parentWindowHandle)
{
	WCHAR name[255];
	wcscpy_s(name, defaultFileName.c_str());

	std::wstring tempFolder = defaultFolder;
	
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = parentWindowHandle;			// 부모창 핸들
	ofn.lpstrFilter = fileTypeFilter;			// 파일 유형 필터
	ofn.lpstrFile = name;						// 파일 이름
	ofn.lpstrTitle = L"불러오기";				// 대화 상자 제목
	ofn.nMaxFile = 255;							// 파일 이름 최대 크기
	ofn.lpstrInitialDir = tempFolder.c_str();	// 기본 폴더 경로
	ofn.Flags = OFN_NOCHANGEDIR;				// 대화 상자 옵션

	if (GetOpenFileName(&ofn) == TRUE)			// 대화 상자 열고 사용자가 파일을 선택하면
	{
		if (callback != NULL)					// 콜백 함수가 지정되어 있으면
		{
			std::wstring loadName = name;			// 선택된 파일의 경로를 저장
			String::Replace(&loadName, L"\\", L"/"); // 경로에서 역슬래시를 슬래시로 변환

			callback(loadName);					// 콜백 함수 호출
		}
	}
}

/*
typedef struct tagOFN {
  DWORD         lStructSize;		//구조체의 크기지정, 버전확인을 위해 사용됨
  HWND          hwndOwner;			//핸들의 주인을 지정, 주인이 될 윈도우가 없을경우 NULL
  HINSTANCE     hInstance;			//인스턴스 핸들을 지정
  LPCTSTR       lpstrFilter;		//파일 형식들을 구분해줄 필터들을 지정 여러개의 필터를 동시에 지정가능
  LPTSTR        lpstrCustomFilter;	//사용자가 만든 커스텀 필터를 지정
  DWORD         nMaxCustFilter;		//커스텀 필터의 버퍼의 길이를 지정 최소 40자
  DWORD         nFilterIndex;		//필터들의 목록중 사용할 필터의 인덱스를 지정
  LPTSTR        lpstrFile;			//파일이름 지정에 첫번째로 나타날 파일명을 지정, 필요없을시 NULL
  DWORD         nMaxFile;
  LPTSTR        lpstrFileTitle;
  DWORD         nMaxFileTitle;
  LPCTSTR       lpstrInitialDir;
  LPCTSTR       lpstrTitle;
  DWORD         Flags;
  WORD          nFileOffset;
  WORD          nFileExtension;
  LPCTSTR       lpstrDefExt;
  LPARAM        lCustData;
  LPOFNHOOKPROC lpfnHook;
  LPCTSTR       lpTemplateName;
#if (_WIN32_WINNT >= 0x0500)
  void *        pvReserved;
  DWORD         dwReserved;
  DWORD         FlagsEx;
#endif // (_WIN32_WINNT >= 0x0500)
} OPENFILENAME, *LPOPENFILENAME;
*/


// 파일 대화 상자를 열어 사용자가 파일을 저장하도록 함
void Path::SaveFileDialog(std::wstring defaultFileName, const WCHAR* fileTypeFilter, const std::wstring defaultFolder, const std::function<void(std::wstring)> callback, HWND parentWindowHandle)
{
	WCHAR name[255];
	wcscpy_s(name, defaultFileName.c_str());

	std::wstring tempFolder = defaultFolder;

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = parentWindowHandle;
	ofn.lpstrFilter = fileTypeFilter;
	ofn.lpstrFile = name;
	ofn.lpstrTitle = L"저장하기";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		if (callback != NULL)
		{
			std::wstring saveName = name;
			String::Replace(&saveName, L"\\", L"/");

			callback(saveName);
		}
	}
}

// 지정된 폴더에서 특정 유형의 파일들을 찾아 반환하는 함수
void Path::GetFiles(std::vector<std::string>* files, std::string path, std::string filter, bool bFindSubFolder)
{
	std::vector<std::wstring> wFiles;
	std::wstring wPath = String::ToWstring(path);
	std::wstring wFilter = String::ToWstring(filter);

	GetFiles(&wFiles, wPath, wFilter, bFindSubFolder);

	for (std::wstring str : wFiles)
		files->push_back(String::ToString(str));
}

//path : ../Temp/
//filter : *.txt
// 지정된 폴더에서 특정 파일들을 찾아 반환하는 함수
void Path::GetFiles(std::vector<std::wstring>* files, std::wstring path, std::wstring filter, bool bFindSubFolder/*하위폴더 검색 옵션*/)
{
	std::wstring file = path + filter; // 검색할 파일의 경로와 필터를 결합하여 검색할 파일의 전체 경로 생성

	WIN32_FIND_DATA findData;
	HANDLE handle = FindFirstFile(file.c_str(), &findData);	// 첫 번째 파일 검색
	if (handle != INVALID_HANDLE_VALUE)	// 파일이 발견되면
	{
		do
		{
			// 발견된 파일이 디렉토리인 경우
			if (findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				// 하위 폴더 검색 옵션 활성화 & 발견된 디렉토리가 '.'으로 시작하지 않는 경우
				if (bFindSubFolder == true && findData.cFileName[0] != '.')
				{
					// 발견된 디렉토리의 전체 경로 설정
					std::wstring folder = path + std::wstring(findData.cFileName) + L"/";
					// 재귀적으로 하위 폴더에서 파일을 검색
					GetFiles(files, folder, filter, bFindSubFolder);
				}
			}
			else // 발견된 파일이 디렉토리가 아닌 경우
			{
				// 발견된 파일의 전체 경로 생성
				std::wstring fileName = path + std::wstring(findData.cFileName);
				// 벡터에 발견된 파일의 전체 경로를 추가
				files->push_back(fileName);
			}
		} while (FindNextFile(handle, &findData)); // 다음 파일 검색
		// 파일 검색 종료
		FindClose(handle);
	}
}

// 주어진 경로에 폴더가 존재하지 않는 경우 생성하는 함수
// 주어진 경로의 폴더만 생성
void Path::CreateFolder(std::wstring path)
{
	// 주어진 경로에 폴더가 없을 경우
	if (ExistDirectory(path) == false)
		// 폴더 생성
		CreateDirectory(path.c_str(), NULL);
}

void Path::CreateFolder(std::string path)
{
	CreateFolder(String::ToWstring(path));
}

// 폴더가 없을 경우 폴더를 생성하는 함수
// 주어진 경로의 모든 하위 폴더를 생성
void Path::CreateFolders(std::wstring path)
{
	String::Replace(&path, L"\\", L"/");

	std::vector<std::wstring> folders;
	String::SplitString(&folders, path, L"/");

	std::wstring temp = L"";
	for (std::wstring folder : folders)
	{
		temp += folder + L"/";

		CreateFolder(temp);
	}
}

void Path::CreateFolders(std::string path)
{
	CreateFolders(String::ToWstring(path));
}