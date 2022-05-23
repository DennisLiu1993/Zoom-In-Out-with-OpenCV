# Zoom-In-Out-with-OpenCV

I believe a lot of people is concerned about this issue. So, I implemented a C++ class, containing OpneCV Window.
This project can zoom in / out image showed on OpenCV window without other UI frameworks, like MFC, C#, Java Swing and etc.

#Visual Effect:

![image](https://github.com/DennisLiu1993/Zoom-In-Out-with-OpenCV/blob/main/ZoomInOut.gif)

#Usage:

1.Download Visual Studio and Install
2.Set up your OpenCV include directory, library directory, name of .lib
3.Place OpenCV.dll to the output directory of this project (for me, it's Zoom-In-Out-with-OpenCV\\Debug)
4.Run and Experience

or you can just copy OpenCVWindowEx.h, OpenCVWindowEx.cpp to your own project

#Class Usage:

```
COpenCVWindowExt window ("Src"); // Create an OpenCVWindowEx with name (same as original API)
window.ImRead ("test2.bmp");     // Read an Image with this window and show it
waitKey (0);                     // Wair for user's operation
```
