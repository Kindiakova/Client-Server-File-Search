#include"headers/LinuxFileSearch.h"

using namespace std;

bool LinuxFileSearch(const string &FileName, string &result_path, const string &current_path)
{
    DIR *dir = opendir(current_path.c_str());
    if (!dir) {
        return false;
    }

    struct dirent *dptr;
    while(dptr = readdir(dir)){
        string cur_filename = dptr->d_name;
        string cur_filepath = current_path + "/" + cur_filename;

        struct stat statStruct;
        stat(cur_filepath.c_str(), &statStruct);
        if( S_ISDIR(statStruct.st_mode) ){
            if ( cur_filename.compare(".") == 0 || cur_filename.compare("..") == 0 ){
                continue;
            }

            if (LinuxFileSearch(FileName, result_path, cur_filepath))
            {
                closedir(dir);
                return true;
            }
        }
        else if( S_ISREG(statStruct.st_mode)){
            if( cur_filename.compare(FileName) == 0){
                result_path = cur_filepath;
                closedir(dir);
                return true;
            }
        }
    }

    closedir(dir);
    return false;
}

bool FileSearch(const string &FileName, string &result_path)
{
    char tmp[256];
    getcwd(tmp, 256);
    string current_path(tmp);
    return LinuxFileSearch(FileName, result_path, current_path);
}
