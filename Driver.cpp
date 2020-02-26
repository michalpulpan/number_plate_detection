//NumberPlateRecognition using SVm

//Driver.cpp
//Michal Půlpán

/*

Loads image from arguments 
Gets image, detects all possible regions and trains SVM

*/

#include "Driver.h"

#include <memory>

using namespace std;
using namespace cv;
using namespace cv::ml;
namespace fs = std::filesystem;


bool cmp( vector<Point> contour1, vector<Point> contour2 ) {
        double i = fabs( contourArea(Mat(contour1)) );
        double j = fabs( contourArea(Mat(contour2)) );
        return ( i < j );
    }


Ptr<SVM> Driver::TrainSVM(){
    cout << "Training ML model, please wait..." << endl;
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
    return svm;
}

void Driver::mainLoop(const vector<string> &arguments){
    
    svm_ = TrainSVM();


    if(arguments[1] == "run"){

        /*
        1. load photo from absolute path
        2. load folder from absolute path
        3. show steps during operation
        4. show output in new window
        5. save output -> asks for folder to save output

        */
        cout<<"###################################"<< endl;
        cout<<"### Czech number plate detector ###"<< endl;
        cout<<"###################################"<< endl;

        printHelp();

        cout << "\n\nPlease enter a command:\n" <<endl;
        string input;
        getline(cin, input);

        if(input != "exit")
            loop = true;

        while(loop){

            if(input == "--help"){
                
                printHelp();

            } else if(input == "1"){
                loadPhoto();
            
            } else if(input == "2"){
                loadFolder();
            
            } else if(input == "3"){
                
                showSteps_ = !showSteps_;

                if(showSteps_){
                    cout << "show steps activated" << endl;
                } else {
                    cout << "show steps disabled" << endl;
                }
            
            } else if(input == "4"){
                showOutput = !showOutput;

                if(showOutput){
                    cout << "show output activated" << endl;
                } else {
                    cout << "show output disabled" << endl;
                }
            
            } else if(input == "5"){
                
                saveToFolder();
            
            }  else if(input == "6"){
                run();

            }   
            
            else {
                cout << "Uknown command, use --help to list all possible commands" << endl;
            }
            cout << "\nPlease enter a command:" <<endl;
            getline(cin, input);

        }
        
        exit(10);
        }

    //goes through arguments:

    for(size_t i = 0; i<arguments.size(); i++){

        if(arguments[i] == "-i_file"){

            if(!usingFolder){
                inputFile_ = arguments[i+1];
                usingPhoto = true;
            } else {
                cout << "error, can't use both photo and folder as input" << endl;
                exit(3);
            }
        } else if(arguments[i] == "-i_folder"){

            if(!usingPhoto){
                inputFolder = arguments[i+1];
                usingFolder = true;
            } else {
                cout << "error, can't use both photo and folder as input" << endl;
                exit(3);
            }

        } else if(arguments[i] == "-o"){
            saveOutput = true;
            outputFolder = arguments[i+1];

        } else if(arguments[i] == "-show-steps"){
            showSteps_ = true;

        } else if(arguments[i] == "-show-output"){
            showOutput = true;

        } else if(arguments[i] == "-save-output"){
            saveOutput = true;

        } 

    }

    run();

}

void Driver::run(){

    if(usingPhoto){
        cout << "Starting program, using one photo.." <<endl;
        cout << "Path to photo: " << inputFile_ << endl;
        Mat ret = Work(svm_, loadImg(inputFile_));
        if(showOutput){
            imshow("Output", ret);
            waitKey();
        }

    } else if(usingFolder){
        
        cout << "Starting program, using folder.." <<endl;
        for (const auto & entry : fs::directory_iterator(inputFolder)){
            string path = fs::absolute(entry);


            if(fs::path(entry).extension() == ".jpg" ||
                fs::path(entry).extension() == ".JPG" ||
                fs::path(entry).extension() == ".jpeg" ||
                fs::path(entry).extension() == ".JPEG" ||
                fs::path(entry).extension() == ".png" ||
                fs::path(entry).extension() == ".PNG"){


                Mat ret = Work(svm_, loadImg(path));
                if(showOutput){
                    imshow("Output", ret);
                    waitKey();
                }
                if(saveOutput) {
                    string outputPath = outputFolder + "/" + fs::path(entry).filename().string();
                    imwrite(outputPath, ret);
                }

            }
        }

    }
}

void Driver::printHelp(){

    cout << "__________HELP__________" << endl;
    cout << "1. load photo from absolute path"<< endl;
    cout << "2. load folder from absolute path"<< endl;
    if(!showSteps_){
        cout << "3. show steps during operation"<< endl;
    } else {
        cout << "3. do not show steps during operation"<< endl;
    }
    if(!showOutput){
        cout << "4. show output in new window"<< endl;
    } else {
        cout << "4. do not show output in new window"<< endl;
    }
    if(!saveOutput){
        cout << "5. save output to folder" << endl;
    } else{
        cout << "5. do not save output" << endl;
    }
    cout << "6. run program" << endl;

    cout << "________________________" << endl;


}

void Driver::loadPhoto(){

    if(!usingFolder){
        cout << "Please enter absolute path of the photo: " << endl;
        getline(cin, inputFile_);
        usingPhoto = true;

        cout << "Path to file: "<< inputFile_ << endl;

    } else {
        
        cout << "Error, you already selected folder to use." << endl;
        cout << "Type: (number)" << endl;
        cout << "1   I want to use photo" << endl;
        cout << "2   Quit and continue with using folder" << endl;
        string line;
        getline(cin, line);

        if(line == "1"){
            usingFolder = false;
            loadPhoto();
        } else if( line == "2"){
            
        } else {
            loadPhoto();
        }
    }

}

void Driver::loadFolder(){

    if(!usingPhoto){
        cout << "Please enter absolute path of the folder and make sure it contains : " << endl;
        getline(cin, inputFolder);
        usingFolder = true;

        cout << "Path to folder: "<< inputFolder << endl;

    } else {
        
        cout << "Error, you already selected image to use." << endl;
        cout << "Type: (number)" << endl;
        cout << "1   I want to use folder" << endl;
        cout << "2   Quit and continue with using image" << endl;
        string line;
        getline(cin, line);

        if(line == "1"){
            usingPhoto = false;
            loadFolder();
        } else if( line == "2"){
            
        } else {
            loadFolder();
        }
    }

}

void Driver::saveToFolder(){
    if(!saveOutput){
        
        cout << "Please enter absolute path of folder to save photo: " << endl;
        getline(cin, outputFolder);
        outputFolder = true;

        cout << "Path to output folder: "<< outputFolder << endl;

    } else {
        outputFolder = false;
    }
}

Mat Driver::loadImg(const string& path){

    Mat input_img = imread(path);
    
    if(input_img.empty()){
        cout << "error: Image: "<< path <<" was not found" <<endl;
        exit(0);
    }
    return input_img;

}

Mat Driver::Work( Ptr<SVM> svm, const Mat& input_img){


    debug = false;
    //ParseArguments(arguments);
    
    //detect possible regions that could present a plate
    DetectRegions detectRegions(showSteps_, debug);
    //detectRegions.setFilename(inputFile_);
    //Mat input_img = imread(inputFile_);
    /*
    if(input_img.empty()){
        cout << "error: Image was not found" <<endl;
        exit(0);
    }
    */

    vector<Plate> possible_regions_detected = detectRegions.run(input_img);

    
  
    cout << "Detecting plates using ML" <<"    currently have: "<<possible_regions_detected.size()<<endl;
    if(showSteps_){
        for(auto& possibleRegion: possible_regions_detected){
            imshow("possible region", possibleRegion.img_);
            waitKey();
        }
    }

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
            //RotatedRect rectPlate = RotatedRect(region.rotRect_.center, region.rotRect_.size, region.rotRect_.angle);
            //GaussianBlur(input_img(region.rotRect_.boundingRect2f()), input_img(region.rotRect_.boundingRect2f()), Size(51,51), 0);
            
            
            //input_img = DrawRotatedRect(input_img, region.rotRect_);
            //cout<<"drawing"<<endl;
        } 

        //If there are multiple (final) plates detected, choose one with closer ratio to 4.7272

       

    }

    cout << "Number of detected plates: "<< finalPlates.size() << "    Choosing the closest one."<< endl;
    
    Plate final;
    if(finalPlates.size()>0){

        float minDistFromOptimalRatio = INT8_MAX;
        for(auto& plate: finalPlates){
            
            float plateRatioDist = plate.pos_.width/plate.pos_.height-4.722;
            if(plateRatioDist<0){
                plateRatioDist=-plateRatioDist;
            }
            if(plateRatioDist<minDistFromOptimalRatio){
                minDistFromOptimalRatio = plateRatioDist;
                final = plate;
            }

        }
    } else {

        //ML found 0 plates
        float minDistFromOptimalRatio = INT8_MAX;
        for(auto& plate: possible_regions_detected){



            float plateRatioDist = plate.pos_.width/plate.pos_.height-4.722;
            if(plateRatioDist<0){
                plateRatioDist=-plateRatioDist;
            }
            if(plateRatioDist<minDistFromOptimalRatio){
                minDistFromOptimalRatio = plateRatioDist;
                final = plate;
            }

        }
    }

    DrawRotatedRect(input_img, final.rotRect_).copyTo(input_img);

    return input_img;

    if(showOutput){
        imshow("final", input_img);
        waitKey(0);
    }
    
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

Mat Driver::DrawRotatedRect(const Mat& mat, const RotatedRect& rect ){

    Scalar color = cv::Scalar(0, 0, 0); // white

    // We take the edges that OpenCV calculated for us
    Point2f vertices2f[4];
    rect.points(vertices2f);

    // Convert them so we can use them in a fillConvexPoly
    Point vertices[4];    
    for(int i = 0; i < 4; ++i){
        vertices[i] = vertices2f[i];
    }

    // Now we can fill the rotated rectangle with our specified color
    fillConvexPoly(mat,vertices,4,color);
    return mat;

}

bool Driver::isItReallyAPlate(const Plate& plate){
    
    Mat gray = plate.img_;
    Mat bilGray;

    //cvtColor(imgPlate, gray, COLOR_BGR2GRAY);
    bilateralFilter(gray, bilGray, 11, 17, 17);

    //find edges of grayscale plate
    Mat edged;
    vector<vector<Point> > contours;
    Canny(bilGray, edged, 170, 200);
    findContours(edged, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    sort(contours.begin(), contours.end(), cmp);
  
    vector<vector<Point> > contoursGreaterThen30;

    for(auto& contour : contours){
        if(contourArea(contour) > 30){
            Mat approx;
            //contoursGreaterThen30.push_back(contour);
            double peri = arcLength(contour,true);

            approxPolyDP(contour, approx, 0.02*peri, true);

            if(approx.total() == 4){
                return true;
            }
        }
    }
    return false;
}