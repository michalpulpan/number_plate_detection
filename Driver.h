/*

NumberPlateRecognition using SVM

Driver.h
Michal Půlpán


Loads image based on arguments
Gets image, detects all possible regions and trains SVM

*/
#include <vector>
#include <string>
#include <iostream>

#include "DetectRegions.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class Driver {

    public:
        Driver(){};

        void Work(const std::vector<std::string> &arguments);

        void ParseArguments(const std::vector<std::string> &arguments);



    private:
        std::string inputFile_;


};