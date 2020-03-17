/*

NumberPlateRecognition using SVM

Driver.h
Michal Půlpán


Loads image based on arguments
Gets image, detects all possible regions and trains SVM

*/
#include <vector>
#include <string>
//#include <iostream>
#include <algorithm>
#include <filesystem>


#include "DetectRegions.h"

#include "opencv2/ml/ml.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



class Driver {

    public:
        Driver(){};

        void mainLoop(const std::vector<std::string> &arguments);




    private:
        cv::Mat Work( cv::Ptr<cv::ml::SVM> svm, const cv::Mat& input_img);
        cv::Mat loadImg(const std::string& path);

        std::string inputFile_;
        std::string inputFolder;
        std::string outputFolder;
        bool showSteps_;
        bool loop;
        bool showOutput;
        bool saveOutput;
        bool usingPhoto;
        bool usingFolder;
        bool debug;

        cv::Ptr<cv::ml::SVM> svm_;

        cv::Mat DrawRotatedRect(const cv::Mat& mat, const cv::RotatedRect& rect);
        void run();
        bool isItReallyAPlate(const Plate& plate);
        void printHelp();
        void loadPhoto();
        void loadFolder();
        void saveToFolder();
        cv::Ptr<cv::ml::SVM> TrainSVM();

     

};