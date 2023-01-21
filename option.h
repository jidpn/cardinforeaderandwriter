#pragma once
#include "resource.h"
#include "windows.h"
#include <iostream>
#include<string>
extern HMENU menu;
extern char OPTION_FILE_NAME[];
class Option
{
private: int halfAuto;
private: int intermediate;
public: std::string language;
public: TCHAR base_dir[256];
public:TCHAR inputPath[MAX_PATH] ;
public:std::wstring metaPath ;
public:double resize;
public: std::string template_file_name;
   
public:Option() {
    halfAuto = 0;
    resize = 1.0;
    intermediate = 0;
    template_file_name = "C:\\";
    language = "jpn";
    GetCurrentDirectory(255, base_dir);
    int ret = WideCharToMultiByte(
                    CP_ACP,
                    0,
                    base_dir,
                    -1,
                    OPTION_FILE_NAME,
                    200 + 1,
                    NULL,
                    NULL);
    strcat_s(OPTION_FILE_NAME,255, "\\option.txt");
};
public:TCHAR* getBaseDir() {
    return base_dir;
}
public: std::string getLanguage() {
    return language;
}
public: void setLanguage(std::string initial_language ) {
    language = initial_language;
}

public: void setHalfAuto(char opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_HALFAUTO, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_HALFAUTO, MFS_UNCHECKED);
	  }
    halfAuto = opt;
};
public: char getIntermediate() {
    return intermediate ;
}


public: void setIntermediate(char opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_INTERMEDIATE, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_INTERMEDIATE, MFS_UNCHECKED);
	  }
    intermediate = opt;
}
public: void toggleHalfAuto() {
    setHalfAuto(!halfAuto);
};
public: void toggleIntermediate() {
    setIntermediate(!intermediate);
};

public: std::string getAllOptionString() {
    char char_base_dir[255];
    std::string result_string = "";
    result_string += language + "\n";
    result_string +=  std::to_string(resize)+"\n";
    result_string += template_file_name+"\n";
    int ret = WideCharToMultiByte(
        CP_ACP,
        0,
        base_dir,
        -1,
        char_base_dir,
        200 + 1,
        NULL,
        NULL);
    result_string +=  char_base_dir;
    result_string +=   "\n";
                ret = WideCharToMultiByte(
                    CP_ACP,
                    0,
                    inputPath,
                    -1,
                    char_base_dir,
                    200 + 1,
                    NULL,
                    NULL);
    result_string += char_base_dir;
    result_string +=   "\n";
                ret = WideCharToMultiByte(
                    CP_ACP,
                    0,
                    metaPath.c_str(),
                    -1,
                    char_base_dir,
                    200 + 1,
                    NULL,
                    NULL);
    result_string += char_base_dir;
    result_string +=   "\n";
    if (intermediate) {
        result_string += "intermediate\n";
    }
    if (halfAuto) {
        result_string += "HALF_AUTO\n";
    }
    return result_string;
};
};
