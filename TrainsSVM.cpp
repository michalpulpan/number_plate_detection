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
#include <vector>
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

    numPlates= atoi(argv[1]);
    numNoPlates= atoi(argv[2]);
    pathPlates= argv[3];
    pathNoPlates= argv[4];



}