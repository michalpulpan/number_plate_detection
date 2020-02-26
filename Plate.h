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
    Plate(cv::Mat img, cv::Rect pos, cv::RotatedRect rotRect): img_(img), pos_(pos), rotRect_(rotRect) {};

    cv::Rect pos_;
    cv::Mat img_;
    cv::RotatedRect rotRect_;

};