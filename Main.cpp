/*

NumberPlateRecognition using SVM

Main.cpp
Michal Půlpán

Reads arguments from cmd line

*/

#include <iostream>
#include <vector>
#include <string>

#include "Driver.h"

using namespace std;

int main(int argc, char** argv)
{

    vector<string> args;


    for (size_t i = 0; i < argc; i++)
    {
        cout << argv[i] << endl;
        args.push_back(argv[i]);
    }


    Driver driver;
    driver.Work(args);

}

