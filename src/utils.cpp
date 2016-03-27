#include "utils.h"

vector<string> ListDirectories(string dirName) {
    DIR *dir;
    struct dirent *ent;
    vector<string> result;
    dir = opendir(dirName.c_str());
    // cout << "folder name = " << dirName <<endl;

    dirName+="/";
    if (dir != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_name[0] >= '0'){
        result.push_back(dirName+(string)ent->d_name);
//		printf("%s\n", ent->d_name);
        }
    }

    closedir(dir);
    sort(result.begin(),result.end());

//	cout << "result.size = " << result.size() <<endl;
//	for(int i=0; i<result.size(); i++){
//	    cout << result[i] <<endl;
//	}

    return result;
    } else {
    /* could not open directory */
    perror("no valid dir");
    return result;
    }

}
vector<string> getFiles(string dir_name, string extension, bool connect_folder){
    DIR *dir;
    struct dirent *ent;
    vector<string> result;
    dir = opendir(dir_name.c_str());
    // cout << "folder name = " << dirName <<endl;
    if(dir_name.size() <= 0){
        return result;
    }
    if(dir_name[dir_name.size()-1] != '/'){
        dir_name += "/";
    }

    bool filter_file = true;
    if(extension.size() == 0){
        filter_file = false;
    }
    if(extension[0] != '.'){
        extension = "." + extension;
    }


//    cout << "extension = " << extension <<endl;

    if (dir != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_name[0] >= '0'){
            if(filter_file){
                string current_name = ent->d_name;
                if(current_name.size() < extension.size())
                    continue;
                string sub_name = current_name.substr(current_name.size()-extension.size(), extension.size());
               // cout << "sub_name = " << sub_name <<endl;
                if(sub_name == extension){
                    if(connect_folder)
                        result.push_back(dir_name + current_name);
                    else
                        result.push_back(current_name);
                }
            }else{
                if(connect_folder)
                    result.push_back(dir_name + (string)ent->d_name);
                else{
                    result.push_back((string)ent->d_name);
                }
            }
        }
    }

        closedir(dir);
        sort(result.begin(),result.end());


        return result;
    } else {
    /* could not open directory */
        perror("no valid dir");
        return result;
    }
}
