//NumberPlateRecognition using SVm

//Driver.cpp
//Michal Půlpán

/*

Loads image from arguments 
Gets image, detects all possible regions and trains SVM

*/

#include "Driver.h"
#include "opencv2/ml/ml.hpp"
#include <memory>

using namespace std;
using namespace cv;
using namespace cv::ml;

void Driver::Work(const vector<string> &arguments){

  cout << "Training ML model" << endl;

    //Train SVM, read file storage and 
    cv::FileStorage fileStorage;
    fileStorage.open("SVM.xml", FileStorage::READ);
    Mat SVM_TrainingData;
    Mat SVM_Classes;
    fileStorage["TrainingData"] >> SVM_TrainingData;
    fileStorage["classes"] >>SVM_Classes;
    //SVM parameters
    
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR); //CvSVM::LINEAR;
    svm->setDegree(0);
    svm->setGamma(1);
    svm->setCoef0(0);
    svm->setC(1);
    svm->setNu(0);
    svm->setP(0);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 0.01));
    //Train SVM
    svm->train(SVM_TrainingData,ROW_SAMPLE, SVM_Classes);


    ParseArguments(arguments);
    
    //detect possible regions that could present a plate
    DetectRegions detectRegions(showSteps_);
    //detectRegions.setFilename(inputFile_);
    Mat input_img = imread(inputFile_);
    
    if(input_img.empty()){
        cout << "error: Image was not found" <<endl;
        exit(0);
    }
    vector<Plate> possible_regions_detected = detectRegions.run(input_img);

    
  
    cout << "Detecting plates using ML" <<"    currently have: "<<possible_regions_detected.size()<<endl;
    //imshow("ssss", possible_regions_detected[0].img_);
    //waitKey();

    vector<Plate> finalPlates;
    for(auto& region: possible_regions_detected){

        Mat imgPossRegion = region.img_;
        Mat p = imgPossRegion.reshape(1,1);

        //cout << p.cols <<"    "<<p.type()<<endl;
        p.convertTo(p, CV_32F);
        Size size(14256,1);
        //resultRes.create(33,144,CV_8UC3);
        resize(p, p, size);
        //cout << resultRes.rows <<"  x  "<<resultRes.cols<< "       "<<resultRes<<endl;

        

        int response = (int)svm->predict(p);
        if(response == 1){
            finalPlates.push_back(region);
            //rectangle(input_img,region.pos_, (0,255,0),5);
            GaussianBlur(input_img(region.pos_), input_img(region.pos_), Size(51,51), 0);
            cout<<"drawing"<<endl;
        } 




    }

    cout << "Number of detected plates: "<< finalPlates.size() << endl;
    imshow("final", input_img);
    waitKey(0);
    
}

void Driver::ParseArguments(const vector<string> &arguments){

    if(arguments[1] == "-o"){
        inputFile_ = arguments[2];
        
    } else {
        cout << "error: Wrong argument" <<endl;
        exit(0);
    }
    if(arguments.size() >3 &&arguments[3] == "--show-steps"){
        showSteps_ = true;
    } else {
        showSteps_ = false;
    }
}