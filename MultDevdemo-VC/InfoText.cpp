// InfoText.cpp : implementation file
//

#include "stdafx.h"
#include "InfoText.h"

// CInfoText

IMPLEMENT_DYNAMIC(CInfoText, CStatic)

CInfoText::CInfoText()
: str_value("")
{

}

CInfoText::~CInfoText()
{
}


BEGIN_MESSAGE_MAP(CInfoText, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CInfoText message handlers




void CInfoText::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	CRect m_clientRect;
	GetClientRect(m_clientRect);

	int nBkMode =  dc.SetBkMode(TRANSPARENT);

	CBrush brush;
	//brush.CreateSolidBrush(SIMPLE_PLOT_OUTBACKCLR);  
	brush.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	dc.FillRect(m_clientRect, &brush);

	//dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	COLORREF clrText = dc.SetTextColor(RGB(0,0,0));
	
	CFont TitleFont;
	TitleFont.CreateFont(GetGlobalData()->m_sizeSmallIcon.cy*2, 0, 0, 0, FW_SEMIBOLD, false, false, false, DEFAULT_CHARSET,
			 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, _T("Times New Roman"));
	CFont * pOldFont = dc.SelectObject(&TitleFont);
	
	//CString text;
	//GetWindowText(text);
	dc.DrawText(str_value, m_clientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	dc.SelectObject(pOldFont);
	dc.SetTextColor(clrText);
	dc.SetBkMode(nBkMode);
}


BOOL CInfoText::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CStatic::OnEraseBkgnd(pDC);  //FALSE;//
}
