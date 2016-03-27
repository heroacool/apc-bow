#ifndef _UTILS_H_
#define _UTILS_h_

#include <iostream>
#include <dirent.h>
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>

using namespace std;

vector<string> ListDirectories(string dirName);
vector<string> getFiles(string dir_name, string extension = "", bool connect_folder = false);


#endif
