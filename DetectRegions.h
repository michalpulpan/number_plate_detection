//NumberPlateRecognition using SVM

//DetectRegions.h
//Michal Půlpán

/*

main class for detecing regions on image,
returns vector<Plate> from public function run which calls segment()

*/

#include "Plate.h"

#include <vector>
#include <string>


class DetectRegions{

    public:
        DetectRegions(bool showSteps=false):showSteps_(showSteps){};
        std::vector<Plate> run(const cv::Mat &input);
        //void setFilename(const std::string &str);

    private:
        std::string filename_;
        bool showSteps_;
        std::vector<Plate> segment(const cv::Mat &input);
        bool verifySizes(const cv::RotatedRect &mr);
        cv::Mat histeq(const cv::Mat &in);
};