#include "OpenCVWindowExt.h"

void MouseCall (int event, int x, int y, int flag, void* pUserData)
{
	COpenCVWindowExt* pParent = (COpenCVWindowExt*)pUserData;
	if (event == EVENT_MOUSEWHEEL)
	{
		if (getMouseWheelDelta (flag) > 0 && pParent->m_iScaleTimes != pParent->m_iMaxScaleTimes)
			pParent->m_iScaleTimes++;
		else if (getMouseWheelDelta (flag) < 0 && pParent->m_iScaleTimes != pParent->m_iMinScaleTimes)
			pParent->m_iScaleTimes--;

		if (pParent->m_iScaleTimes == 0)
			pParent->m_dCompensationX = pParent->m_dCompensationY = 0;

		//Pixel value = mouse offset (v.s. window's left top position)
		//But using x, y is not correct in Wheel Event. So, use pre recorded value
		x = pParent->m_iMouseX;
		y = pParent->m_iMouseY;
		double dPixelX = (pParent->m_iHorzScrollBarPos + x + pParent->m_dCompensationX) / pParent->m_dNewScale;
		double dPixelY = (pParent->m_iVertScrollBarPos + y + pParent->m_dCompensationY) / pParent->m_dNewScale;

		pParent->m_dNewScale = pParent->m_dInitialScale * pow (pParent->m_dScaleRatio, pParent->m_iScaleTimes);

		if (pParent->m_iScaleTimes != 0)
		{
			int iWidth = pParent->m_matSrc.cols;
			int iHeight = pParent->m_matSrc.rows;

			pParent->m_iHorzScrollBarRange_Max = int (pParent->m_dNewScale * iWidth - pParent->m_dInitialScale * iWidth) - 1;
			pParent->m_iVertScrollBarRange_Max = int (pParent->m_dNewScale * iHeight - pParent->m_dInitialScale * iHeight) - 1;
			int iBarPosX = pParent->m_iHorzScrollBarPos = int (dPixelX * pParent->m_dNewScale - x + 0.5);
			int iBarPosY = pParent->m_iVertScrollBarPos = int (dPixelY * pParent->m_dNewScale - y + 0.5);
			pParent->m_dCompensationX = -iBarPosX + (dPixelX * pParent->m_dNewScale - x);
			pParent->m_dCompensationY = -iBarPosY + (dPixelY * pParent->m_dNewScale - y);
		}
		else
		{
			pParent->m_iHorzScrollBarPos = 0;
			pParent->m_iVertScrollBarPos = 0;
		}
		pParent->RefreshImage ();
	}
	else if (event == EVENT_RBUTTONDOWN)
	{

	}
	else if (flag == EVENT_FLAG_RBUTTON)
	{

	}
	else if (event == EVENT_MOUSEMOVE)
	{
		pParent->m_iMouseX = x;
		pParent->m_iMouseY = y;

	}
}
COpenCVWindowExt::COpenCVWindowExt (String strWindowName, int iFlag)
{
	namedWindow (strWindowName, iFlag);
	setMouseCallback (strWindowName, MouseCall, this);
	m_strWindowName = strWindowName;

	//Initial values
	m_strWindowName = "Src";

	m_iScaleTimes = 0;
	m_iMaxScaleTimes = 10;
	m_iMinScaleTimes = 0;
	m_dCompensationX = 0;
	m_dCompensationY = 0;
	m_dInitialScale = 1;
	m_dNewScale = 1;
	m_dScaleRatio = 1.25;

	m_iMouseX = 0;
	m_iMouseY = 0;

	m_iHorzScrollBarPos = 0;
	m_iVertScrollBarPos = 0;

	m_iHorzScrollBarRange_Min = 0;
	m_iHorzScrollBarRange_Max = 1;
	m_iVertScrollBarRange_Min = 0;
	m_iVertScrollBarRange_Max = 1;
}

COpenCVWindowExt::~COpenCVWindowExt ()
{
	setMouseCallback (m_strWindowName, NULL);
}

bool COpenCVWindowExt::ImRead (String strFileName)
{
	m_matSrc = imread (strFileName);

	if (!m_matSrc.empty ())
		imshow (m_strWindowName, m_matSrc);

	return !m_matSrc.empty ();
}

void COpenCVWindowExt::RefreshImage ()
{
	if (m_matSrc.empty ())
		return;
	Mat matResize;
	Size size (int (m_dNewScale * m_matSrc.cols), int (m_dNewScale * m_matSrc.rows));
	resize (m_matSrc, matResize, size);
	int iW = int (m_matSrc.cols * m_dInitialScale), iH = int (m_matSrc.rows * m_dInitialScale);

	Rect rectShow (Point (m_iHorzScrollBarPos, m_iVertScrollBarPos), Size (iW, iH));
	imshow (m_strWindowName, matResize (rectShow));
}
