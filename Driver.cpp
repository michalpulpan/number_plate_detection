//NumberPlateRecognition using SVm

//Driver.cpp
//Michal Půlpán

/*

Loads image from arguments 
Gets image, detects all possible regions and trains SVM

*/

#include "Driver.h"

using namespace std;
using namespace cv;

void Driver::Work(const vector<string> &arguments){

    ParseArguments(arguments);
    
    //detect possible regions that could present a plate
    DetectRegions detectRegions(true);
    //detectRegions.setFilename(inputFile_);
    Mat input_img = imread(inputFile_);
    
    if(input_img.empty()){
        cout << "error: Image was not found" <<endl;
        exit(0);
    }

    vector<Plate> possible_regions_detected = detectRegions.run(input_img);

    //Train SVM, read file storage and 

    
}

void Driver::ParseArguments(const vector<string> &arguments){

    if(arguments[1] == "-o"){
        inputFile_ = arguments[2];
        
    } else {
        cout << "error: Wrong argument" <<endl;
        exit(0);
    }
}