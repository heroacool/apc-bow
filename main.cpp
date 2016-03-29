#include "opencv2/opencv_modules.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/ml.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <functional>

#if defined WIN32 || defined _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef min
#undef max
#include "sys/types.h"
#endif
#include <sys/stat.h>

#include "utils.h"

#define DEBUG_DESC_PROGRESS

using namespace cv;
using namespace cv::xfeatures2d;
using namespace cv::ml;
using namespace std;

const string paramsFile = "params.xml";
const string vocabularyFile = "../model/vocabulary.xml.gz";
const string bowImageDescriptorsDir = "/bowImageDescriptors";
const string svmsDir = "/svms";
const string plotsDir = "/plots";




int main(int argc, char** argv)
{
    string train_folder = "../data/train/";
    string test_folder = "../data/test/";




    vector<string> class_names = getFiles(train_folder);
    vector<vector<string> > class_files;    //different class files, may need to randomlize
    class_files.resize(class_names.size());

    for(int i=0; i<class_names.size(); i++){
        string current_folder = train_folder + class_names[i];
        class_files[i] = getFiles(current_folder, ".jpg", true);

        cout << "class_name " << class_names[i] <<endl;
        for(int j=0; j<class_files[i].size(); j++){
            cout << class_files[i][j] <<endl;
        }
    }

    Mat descriptors;
    Mat current_descriptors;

    Ptr<FeatureDetector> feature_descriptor = SURF::create();
    vector<cv::KeyPoint> keypoints;

    Ptr<DescriptorMatcher> descMatcher = DescriptorMatcher::create("FlannBased");

    Ptr<BOWImgDescriptorExtractor> bowExtractor = makePtr<BOWImgDescriptorExtractor>( feature_descriptor, descMatcher );


//    for(int i=0; i<class_files.size(); i++){
//        for(int j=0; j<class_files[i].size(); j++){
//            Mat src = imread(class_files[i][j]);
//            cout << "name = " << class_files[i][j] <<endl;
//            imshow("src", src);

//            feature_descriptor->detectAndCompute(src, Mat(), keypoints, current_descriptors);
//            descriptors.push_back(current_descriptors);

//            bowTrainer.add(current_descriptors);

//            Mat dst;
//            drawKeypoints(src, keypoints, dst);
//            imshow("dst", dst);

//            imshow("sr", current_descriptors);
//            //cout << current_descriptors.size() <<endl;

//            waitKey(0);
//        }
//    }

    clock_t t1,t2;
    t1 = clock();
    Mat vocabulary = trainVocabulary(vocabularyFile, class_files, feature_descriptor);
    t2 = clock();
    cout << " train time = " << t2 - t1 << "ms " <<endl;

    bowExtractor->setVocabulary(vocabulary);

    Mat trainMat, trainLabel;

    string svm_xml_file = "../model/svm.xml.gz";



    return 0;

}
