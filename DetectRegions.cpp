/*

Czech number plate detection 

DetectRegions.cpp
Michal Půlpán


- Function "segment" tries to find vertical edges because car number plate has high density of them -> first step of heuristic approach
                     uses floodfill algorithm to connect close regions and make few that might be plates
                     controlles if these floodfill masks have approx size and ration that could signalise a number plate
                     crops possible number plates out from photo
                     returns vector of possible number plates

*/



#include "DetectRegions.h"

using namespace std;
//using namespace cv;

vector<Plate> DetectRegions::run(const cv::Mat &input){

    //segment image by white and return detected possible regions
    vector<Plate> tmp = segment(input);
    return tmp;

}

vector<Plate> DetectRegions::segment(const cv::Mat &input){

    vector<Plate> plates; //return value

    //converting to gray, no need for colors when detecting edges...
    cv::Mat imgGray;
    cv::cvtColor(input, imgGray, cv::COLOR_BGR2GRAY);
    cv::blur(imgGray, imgGray, cv::Size(5,5));

    //Find vertical lines, since car plate has high density of them
    cv::Mat imgSobel;
    cv::Sobel(imgGray, imgSobel, CV_8U, 1,0,3,1,0, cv::BORDER_DEFAULT);
    if(showSteps_){
        cv::imshow("Vertical_lines", imgSobel);
        cv::waitKey();
    }

    //Thresholding -> segmenting from grayscale
    cv::Mat imgThreshold;
    cv::threshold(imgSobel, imgThreshold, 0,255, cv::THRESH_OTSU+cv::THRESH_BINARY);
    if(showSteps_){
        cv::imshow("Threshold", imgThreshold);
        cv::waitKey();

    }

    //Getting structural rectangular element with 17x3 dimensions -> we'll have
    //regions in the image that COULD contain a plate

    //THIS MIGHT NEED SOME ADJUSTING, MAYBE SCALING INPUT IMAGE? 
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(17,3));
    cv::morphologyEx(imgThreshold, imgThreshold, cv::MORPH_CLOSE, element);
    if(showSteps_){
        cv::imshow("Morph_close", imgThreshold);
        cv::waitKey();
    }

    //Find contours of possible plates
    vector<vector<cv::Point> > contours;
    cv::findContours(imgThreshold, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    vector<cv::RotatedRect> rects;
    for(auto& contour: contours){
        cv::RotatedRect mbr = cv::minAreaRect(cv::Mat(contour));
        if(verifySizes(mbr)){
            //it's ok, could be a plate
            rects.push_back(mbr);
        } else {
            //probably not a plate, don't do anything
            continue;
        }
    }

    //Draw green contours on a white image
    cv::Mat result;
    input.copyTo(result);
    cv::drawContours(result,contours,
                     -1, //all contours
                     cv::Scalar(0,255,0), //green
                     1); //thickness of 1

    if(showSteps_){
        cv::imshow("Green_contours", result);
        cv::waitKey();
    }

    for(auto &rect: rects){
        //floodfill algorithm -> plate has white background
        //then we can get contour box more clearly


        float minSize;
        cv::circle(result, rect.center, 3, cv::Scalar(0,255,0), -1);
        if(rect.size.width < rect.size.height){
            minSize = rect.size.width;
        } else {
            minSize = rect.size.height;
        }
        minSize = minSize -minSize*0.5;

        srand(time(NULL));
        //floodfill params
        cv::Mat mask;
        mask.create(input.rows + 2, input.cols + 2, CV_8UC1);
        mask = cv::Scalar::all(0);
        int loDiff = 30;
        int upDiff = 30;
        int connectivity = 4;
        int newMaskVal = 255;
        int numSeeds = 10;
        cv::Rect ccomp;
        int flags = connectivity + (newMaskVal<<8) + cv::FLOODFILL_FIXED_RANGE + cv::FLOODFILL_MASK_ONLY;
        
        for (size_t i = 0; i < numSeeds; i++)
        {
            cv::Point seed;
            seed.x = rect.center.x+rand()%(int)minSize-(minSize*0.5);
            seed.y = rect.center.y+rand()%(int)minSize-(minSize*0.5);
            cv::circle(result, seed, 1, cv::Scalar(0,255,255), -1);
            int area = cv::floodFill(input, mask, seed, cv::Scalar(255,0,0), &ccomp, cv::Scalar(loDiff, loDiff, loDiff), cv::Scalar(upDiff, upDiff, upDiff), flags);
        }

        if(showSteps_){
            cv::imshow("MASK", mask);
            cv::waitKey();
        }

        //check every floodfill mask for a correct plate
        //get all points detected for get Minimal rotated rect

        vector<cv::Point> interestPoints;
        /*
        cv::Mat_<uchar>::iterator itMask= mask.begin<uchar>();
        cv::Mat_<uchar>::iterator end= mask.end<uchar>();
        for (auto it = mask.begin(); it != mask.end(); it++)
        {
            if(*it == 255){
                interestPoints.push_back(it.pos());
            }
        }
        */
        /*
        unsigned char *maskIt = (unsigned char*)(mask.data);
        
        for (size_t i = 0; i < mask.cols; i++)
        {
            for (size_t j = 0; j < mask.rows; j++)
            {
                
                if()

            }
            
        }
        */
       cv::MatIterator_<uchar> it;
       for (it = mask.begin<uchar>();it != mask.end<uchar>();++it){
           if(*it == 255){
                interestPoints.push_back(it.pos());
           }
       }

        cv::RotatedRect mbr = cv::minAreaRect(interestPoints);

        if(verifySizes(mbr)){
            //draw rotated rectangle
            cv::Point2f rect_points[4];
            mbr.points(rect_points);
            for (size_t j = 0; j < 4; j++)
            {
                cv::line(result, rect_points[j], rect_points[(j+1)%4], cv::Scalar(0,0,255), 1, 8);
            }

            cv::Mat imgCrop2;
            cv::Mat imHelp = result;
            cv::getRectSubPix(imHelp, mbr.size, mbr.center, imgCrop2);
            if(debug_){
                cv::imshow("verifySizes", imgCrop2);
                cv::waitKey();
            }

            //get rot. matrix
            float r = (float)mbr.size.width / (float)mbr.size.height;
            if(showSteps_)
                cout << "first r: " << r << endl;
            float angle = mbr.angle;
            if(r<1){
                mbr.angle = 90+mbr.angle;
                cv::swap(mbr.size.width, mbr.size.height);
            //    r = (float)mbr.size.width / (float)mbr.size.height;
            }
            if(showSteps_)
                cout << "second r: " << r << endl;

            cv::Mat rotationMatrix = cv::getRotationMatrix2D(mbr.center, mbr.angle, 1);

        
            //create and rotate image
            cv::Mat imgRotated;
            cv::warpAffine(input, imgRotated, rotationMatrix, input.size(), cv::INTER_CUBIC);
            if(debug_){
                cv::imshow("imgRotated", imgRotated);
                cv::waitKey();
            }

            //crop img
            cv::Size rectSize = mbr.size;
            /*
            if(r<1){
                cv::swap(rectSize.width, rectSize.height);
            }*/
            cv::Mat imgCrop;
            cv::getRectSubPix(imgRotated, rectSize, mbr.center, imgCrop);
            if(debug_){
                cv::imshow("imgCrop", imgCrop);
                cv::waitKey();
            }
            cv::Mat resultRes;
            resultRes.create(33,144,CV_8UC3);
            cv::resize(imgCrop, resultRes, resultRes.size(), 0, 0, cv::INTER_CUBIC);

            //Equalize cropped img
            cv::Mat resultGray;
            cv::cvtColor(resultRes, resultGray, cv::COLOR_BGR2GRAY); 
            cv::blur(resultGray, resultGray, cv::Size(3,3));

            resultGray = histeq(resultGray);
            
            plates.push_back(Plate(resultGray, mbr.boundingRect(), mbr));
        }
    }

    if(showSteps_){
        cv::imshow("MASK", result);
        cv::waitKey();
    }

    return plates;
}


bool DetectRegions::verifySizes(const cv::RotatedRect &mr){

    //Czech number plates are 520x110, so aspect ratio is 4.7272
    float error = 0.6;
    float aspect = 4.7272;
    //Setting minimal and maximal area that could be taken by number plate, all other rects are ignored
    int minArea = 15*aspect*15;
    int maxArea = 125*aspect*125;

    //only patches matching area
    float rectMin = aspect-aspect*error;
    float rectMax = aspect+aspect*error;

    int area = mr.size.height*mr.size.width;

    float r = (float)mr.size.width / (float)mr.size.height;
    if(r<1){ //it's rotated 90° 
        r= (float)mr.size.height / (float)mr.size.width;
    }
    if((area < minArea || area > maxArea) || (r<rectMin || r>rectMax)){
        return false;
    } else {
        return true;
    }
}


cv::Mat DetectRegions::histeq(const cv::Mat &in){

    cv::Mat out(in.size(), in.type());

    if(in.channels()==3){
        cv::Mat hsv;
        vector<cv::Mat> hsvSplit;
        cv::cvtColor(in, hsv, cv::COLOR_BGR2HSV);
        split(hsv, hsvSplit);
        cv::equalizeHist(hsvSplit[2], hsvSplit[2]);
        cv::merge(hsvSplit, hsv);
        cv::cvtColor(hsv, out, cv::COLOR_HSV2BGR);
    } else if(in.channels()==1){
        cv::equalizeHist(in,out);
    }

    return out;


}