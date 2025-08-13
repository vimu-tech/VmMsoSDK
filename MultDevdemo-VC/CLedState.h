#pragma once
#include <afxwin.h>

class CLedState :
    public CStatic
{
public:
    DECLARE_DYNAMIC(CLedState)

    CLedState();
    ~CLedState();

    bool IsLenOn() { return m_led_on; };
    void ChangeLed(bool on) { m_led_on = on; Invalidate();};
public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();

private:
    bool m_led_on;
public:
    afx_msg void OnEnable(BOOL bEnable);
};

