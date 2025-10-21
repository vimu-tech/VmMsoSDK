#pragma once


// CInfoText

class CInfoText : public CStatic
{
	DECLARE_DYNAMIC(CInfoText)

public:
	CInfoText();
	virtual ~CInfoText();

	void ChangeText(CString v)
	{
		str_value = v;
		Invalidate();
	}

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	CString str_value;
};


