// OpenCV_ZoomInOut.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "OpenCVWindowExt.h"
int main()
{
    //std::cout << "Hello World!\n";
	COpenCVWindowExt window ("Src");
	window.ImRead ("test2.bmp");
	waitKey (0);

}
