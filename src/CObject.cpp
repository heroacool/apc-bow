#include "CObject.h"


class CObject{
public:
    CObject();
    ~CObject();

    vector<string> classes;
    vector<string> train_class_filenames;
    vector<string> train_filenames;
    string train_folder;
    string test_folder;


};
