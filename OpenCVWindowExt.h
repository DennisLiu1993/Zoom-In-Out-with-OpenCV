#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
#pragma once
class COpenCVWindowExt
{
public:
	COpenCVWindowExt (String strWindowName, int iFlag = 1);
	~COpenCVWindowExt ();

	bool ImRead (String strFileName);
	void RefreshImage ();
	Mat m_matSrc;
	String m_strWindowName;

	int m_iScaleTimes;
	int m_iMaxScaleTimes;
	int m_iMinScaleTimes;
	double m_dCompensationX;
	double m_dCompensationY;
	double m_dInitialScale;
	double m_dNewScale;
	double m_dScaleRatio;

	int m_iMouseX;
	int m_iMouseY;

	int m_iHorzScrollBarPos;
	int m_iVertScrollBarPos;

	int m_iHorzScrollBarRange_Min;
	int m_iHorzScrollBarRange_Max;
	int m_iVertScrollBarRange_Min;
	int m_iVertScrollBarRange_Max;
};

