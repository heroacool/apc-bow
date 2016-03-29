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

static bool readVocabulary( const string& filename, Mat& vocabulary )
{
    cout << "Reading vocabulary...";
    FileStorage fs( filename, FileStorage::READ );
    if( fs.isOpened() )
    {
        fs["vocabulary"] >> vocabulary;
        cout << "done" << endl;
        return true;
    }
    return false;
}

static bool writeVocabulary( const string& filename, const Mat& vocabulary )
{
    cout << "Saving vocabulary..." << endl;
    FileStorage fs( filename, FileStorage::WRITE );
    if( fs.isOpened() )
    {
        fs << "vocabulary" << vocabulary;
        return true;
    }
    return false;
}

/*
 * try to read from file, if failed, then train by itself
 *
 *
*/
Mat trainVocabulary( const string& vocabulary_file,
                            const vector<vector<string> > &class_pathes,
                            const Ptr<FeatureDetector>& fdetector){
    Mat vocabulary;
    if(readVocabulary(vocabulary_file, vocabulary)){
       return vocabulary;
    }

    TermCriteria terminate_criterion;
    terminate_criterion.epsilon = FLT_EPSILON;

    BOWKMeansTrainer bowTrainer( 100, terminate_criterion, 3, KMEANS_PP_CENTERS );

    vector<KeyPoint> keypoints;
    Mat current_descriptors;

    for(int i=0; i<class_pathes.size(); i++){
        for(int j=0; j<class_pathes[i].size(); j++){
            Mat src = imread(class_pathes[i][j]);
            cout << "name = " << class_pathes[i][j] <<endl;
            imshow("src", src);

            fdetector->detectAndCompute(src, Mat(), keypoints, current_descriptors);

            bowTrainer.add(current_descriptors);

//            Mat dst;
//            drawKeypoints(src, keypoints, dst);
//            imshow("dst", dst);

//            imshow("sr", current_descriptors);
//            //cout << current_descriptors.size() <<endl;

//            waitKey(0);
        }
    }

    vocabulary = bowTrainer.cluster();

    if(!writeVocabulary(vocabulary_file, vocabulary)){
        cout << "error: ffile " << vocabulary_file<<"can not be opened to write!\n" <<endl;
        exit(-1);
    }

    return vocabulary;
}
