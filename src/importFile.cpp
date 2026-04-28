#include <windows.h>
#include <fstream>
#include <sstream>

std::string openAndReadFile() {
    OPENFILENAMEA ofn;
    CHAR szFile[260] = {0};

    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    
    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = "testcase";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn) == TRUE) {
        std::ifstream file(ofn.lpstrFile);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf(); 
            return buffer.str();    
        }
    }
    
    return ""; 
}