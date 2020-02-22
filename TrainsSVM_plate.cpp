//NumberPlateRecognition using SVM

//TrainSVM.cpp
//Michal Půlpán

/*

standalone program for creating XML file used for training SVM.


*/

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

int main ( int argc, char** argv )
{

    cout << "Creating XML for plate detection" << endl;

    vector<string> args;

    for (size_t i = 0; i < argc; i++)
    {
        cout << argv[i] << endl;
        args.push_back(argv[i]);
    }

    string pathPlates;
    string pathNoPlates;
    int numPlates;
    int numNoPlates;
    int imageWidth=144;
    int imageHeight=33;

    if(argc >= 5){
        numPlates= atoi(argv[1]);
        numNoPlates= atoi(argv[2]);
        pathPlates= argv[3];
        pathNoPlates= argv[4];

    } else {
        cout << "no enough arguments" << endl;
    }

    Mat classes;
    Mat trainingData;
    Mat trainingImages;

    vector<int> trainingLabels;

    for(int i=1; i<= numPlates; i++)
    {

        stringstream ss(stringstream::in | stringstream::out);
        ss << pathPlates << i << ".jpg";
        Mat img=imread(ss.str());
        if (img.empty()){
            cout <<"emptyimg"<<endl;
           return 0;
        }
        img= img.reshape(1, 1);

        trainingImages.push_back(img);
        trainingLabels.push_back(1);
        //cout << "Plate was pushed" <<endl;
    }

    for(int i=1; i<= numNoPlates; i++)
    {
        stringstream ss(stringstream::in | stringstream::out);
        ss << pathNoPlates << i << ".jpg";
        Mat img=imread(ss.str());
        if (img.empty()){
            cout <<"emptyimg"<<endl;
            cout<<"image: " << i <<endl;
            return 0;
        }
        img= img.reshape(1, 1);
        trainingImages.push_back(img);
        trainingLabels.push_back(0);
        //cout << "no_plate was pushed" <<endl;

    }

    Mat(trainingImages).copyTo(trainingData);
    trainingData.convertTo(trainingData, CV_32F);
    Mat(trainingLabels).copyTo(classes);

    FileStorage fs("SVM.xml", FileStorage::WRITE);
    fs << "TrainingData" << trainingData;
    fs << "classes" << classes;
    fs.release();

}