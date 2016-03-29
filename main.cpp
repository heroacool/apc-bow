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

    Ptr<FeatureDetector> feature_descriptor = SIFT::create();
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
    if(bowExtractor->getVocabulary().empty()){
        cout <<"the vocabulary is empty " <<endl;
        exit(-1);
    }

    Mat_<float> trainMat;
    Mat_<int>trainLabel;

    string svm_xml_file = "../model/svm.xml.gz";

    Ptr<SVM> svm;
    FileStorage fs(svm_xml_file, FileStorage::READ);
    if(fs.isOpened()){
        cout << "load svm xml file" <<endl;
        svm = StatModel::load<SVM>(svm_xml_file);
    }else{
        cout << "train svm" <<endl;

        for(int i=0; i<class_files.size(); i++){
            for(int j=0; j<class_files[i].size(); j++){
                Mat src = imread(class_files[i][j]);
                cout << "name = " << class_files[i][j] <<endl;
                imshow("src", src);
                feature_descriptor->detect(src, keypoints);

                bowExtractor->compute(src, keypoints, current_descriptors);
                trainMat.push_back(current_descriptors);

                Mat_<int> t(1,1);
                t.at<int>(0,0) = i;
                trainLabel.push_back(t);

//                imshow("current_descriptors", current_descriptors);
//                cout << current_descriptors.size() <<endl;
//                waitKey(0);

            }
        }
        cout << trainLabel <<endl;
        svm = SVM::create();
        svm->setType(SVM::C_SVC);
        svm->setKernel(SVM::RBF);
        svm->setDegree(1);
        svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

        svm->trainAuto(TrainData::create(trainMat, ROW_SAMPLE, trainLabel), 10);
        svm->save(svm_xml_file);
        cout << "saved xml file" <<endl;
    }


    for(int i=0; i<class_files.size(); i++){
        for(int j=0; j<class_files[i].size(); j++){
            Mat src = imread(class_files[i][j]);
            cout << "name = " << class_files[i][j] <<endl;
            imshow("src", src);
            feature_descriptor->detect(src, keypoints);

            bowExtractor->compute(src, keypoints, current_descriptors);
            double sum = 0;
            for(int k=0; k<current_descriptors.cols; k++){
                sum += current_descriptors.at<float>(0,k);
            }

            cout << "sum = " << sum << endl;

            float response = svm->predict(current_descriptors);
            cout << "response = " << response <<endl;
            cout <<"class = " << class_names[response] <<endl;

            string current_name = class_names[response];

            putText(src, current_name, Point(10, 50), CV_FONT_NORMAL, 0.5, CV_RGB(255,0,0));
            imshow("src", src);
            waitKey(0);

        }
    }


    return 0;

}
