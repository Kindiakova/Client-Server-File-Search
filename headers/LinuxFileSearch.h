#pragma once
#include<string>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
bool FileSearch(const std::string &FileName, std::string &result_path);
