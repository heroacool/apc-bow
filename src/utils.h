#ifndef _UTILS_H_
#define _UTILS_h_

#include <iostream>
#include <dirent.h>
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>

#include "opencv2/opencv_modules.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/ml.hpp"


using namespace std;
using namespace cv;

vector<string> ListDirectories(string dirName);
vector<string> getFiles(string dir_name, string extension = "", bool connect_folder = false);

Mat trainVocabulary( const string& vocabulary_file,
                            const vector<vector<string> > &class_pathes,
                            const Ptr<FeatureDetector>& fdetector);

static bool readVocabulary( const string& filename, Mat& vocabulary );
static bool writeVocabulary( const string& filename, const Mat& vocabulary );

#endif
