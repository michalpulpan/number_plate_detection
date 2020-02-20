//NumberPlateRecognition using SVM

//Plate.h
//Michal Půlpán

/*

Struct for storing plateImg, Rect position

*/
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


struct Plate{

    Plate(){};
    Plate(cv::Mat img, cv::Rect pos): img_(img), pos_(pos){};

    cv::Rect pos_;
    cv::Mat img_;

};