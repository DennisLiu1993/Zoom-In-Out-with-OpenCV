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
		x = pParent->ptLButtonDown.x;
		y = pParent->ptLButtonDown.y;
		double dPixelX = (pParent->m_iHorzScrollBarPos + x + pParent->m_dCompensationX) / pParent->m_dNewScale;
		double dPixelY = (pParent->m_iVertScrollBarPos + y + pParent->m_dCompensationY) / pParent->m_dNewScale;

		pParent->m_dNewScale = pParent->m_dInitialScale * pow (pParent->m_dScaleRatio, pParent->m_iScaleTimes);

		if (pParent->m_iScaleTimes != 0)
		{
			
			int iW = pParent->m_iOrgW;
			int iH = pParent->m_iOrgH;
			pParent->m_iHorzScrollBarRange_Max = int (pParent->m_dNewScale * iW - pParent->m_dInitialScale * iW);
			pParent->m_iVertScrollBarRange_Max = int (pParent->m_dNewScale * iH - pParent->m_dInitialScale * iH);
			int iBarPosX = int (dPixelX * pParent->m_dNewScale - x + 0.5);
			int iBarPosY = int (dPixelY * pParent->m_dNewScale - y + 0.5);
			pParent->SetHorzBarPos (iBarPosX);
			pParent->SetVertBarPos (iBarPosY);
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
		pParent->ptRButtonDown.x = x;
		pParent->ptRButtonDown.y = y;
	}
	else if (flag == EVENT_FLAG_RBUTTON)
	{
		int iRButtonOffsetX = x - pParent->ptRButtonDown.x;
		int iRButtonOffsetY = y - pParent->ptRButtonDown.y;


		int iBarPosX = pParent->m_iHorzScrollBarPos_copy - iRButtonOffsetX;
		pParent->SetHorzBarPos (iBarPosX);
		

		int iBarPosY = pParent->m_iVertScrollBarPos_copy - iRButtonOffsetY;
		pParent->SetVertBarPos (iBarPosY);

		

		pParent->RefreshImage ();
	}
	else if (event == EVENT_MOUSEMOVE)
	{
		pParent->ptLButtonDown.x = x;
		pParent->ptLButtonDown.y = y;
		pParent->m_iHorzScrollBarPos_copy = pParent->m_iHorzScrollBarPos;
		pParent->m_iVertScrollBarPos_copy = pParent->m_iVertScrollBarPos;

	}
}
COpenCVWindowExt::COpenCVWindowExt (String strWindowName, int iFlag)
{
	namedWindow (strWindowName, iFlag);
	setMouseCallback (strWindowName, MouseCall, this);
	m_strWindowName = strWindowName;

	//Initial values

	m_iScaleTimes = 0;
	m_iMaxScaleTimes = 5;
	m_iMinScaleTimes = 0;
	m_vecMatResize.resize (m_iMaxScaleTimes + 1);
	m_dCompensationX = 0;
	m_dCompensationY = 0;
	m_dInitialScale = 1;
	m_dNewScale = 1;
	m_dScaleRatio = 1.25;

	m_iOrgW = 0;
	m_iOrgH = 0;

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
	if (m_matSrc.empty ())
		return false;
	m_iOrgW = m_matSrc.cols;
	m_iOrgH = m_matSrc.rows;

	Size sizeInitial (m_matSrc.cols * m_dInitialScale, m_matSrc.rows * m_dInitialScale);
	resize (m_matSrc, m_vecMatResize[0], sizeInitial);

	if (!m_vecMatResize[0].empty ())
		imshow (m_strWindowName, m_vecMatResize[0]);

	return !m_vecMatResize[0].empty ();
}

void COpenCVWindowExt::SetInitailScale (double dScale)
{
	if (dScale <= 0)
		return;

	m_dInitialScale = dScale;
	m_dNewScale = dScale;
}

void COpenCVWindowExt::RefreshImage ()
{
	if (m_matSrc.empty ())
		return;
	if (m_vecMatResize[m_iScaleTimes].empty ())
	{
		Size size (int (m_dNewScale * m_matSrc.cols), int (m_dNewScale * m_matSrc.rows));
		resize (m_matSrc, m_vecMatResize[m_iScaleTimes], size);
	}
	int iW = m_vecMatResize[0].cols - 1, iH = m_vecMatResize[0].rows - 1;//-1: for bar size


	Rect rectShow (Point (m_iHorzScrollBarPos, m_iVertScrollBarPos), Size (iW, iH));
	imshow (m_strWindowName, m_vecMatResize[m_iScaleTimes] (rectShow));
}

void COpenCVWindowExt::SetHorzBarPos (int iPos)
{
	if (iPos > m_iHorzScrollBarRange_Max)
		m_iHorzScrollBarPos = m_iHorzScrollBarRange_Max;
	else if (iPos < 0)
		m_iHorzScrollBarPos = m_iHorzScrollBarRange_Min;
	else
		m_iHorzScrollBarPos = iPos;

}

void COpenCVWindowExt::SetVertBarPos (int iPos)
{
	if (iPos > m_iVertScrollBarRange_Max)
		m_iVertScrollBarPos = m_iVertScrollBarRange_Max;
	else if (iPos < 0)
		m_iVertScrollBarPos = m_iVertScrollBarRange_Min;
	else
		m_iVertScrollBarPos = iPos;
}
