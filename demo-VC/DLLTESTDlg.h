
// DLLTESTDlg.h : 头文件
//

#pragma once
#include "..\SharedLibrary\VmMsoLib.h"
#include "afxwin.h"
#include "AdvPlot.h"
#include "afxcmn.h"
#include "VlgRoundSliderCtrl.h"
#include "TimeRoundSliderCtrl.h"
#include "InfoText.h"
#include "CLedState.h"

// CDLLTESTDlg 对话框
class CDLLTESTDlg : public CDialogEx
{
// 构造
public:
	CDLLTESTDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CDLLTESTDlg(void);

	static void WINAPI UsbDevNoticeAddCallBack(void* ppara);
	static void WINAPI UsbDevNoticeRemoveCallBack(void* ppara);
	static void WINAPI DataReadyCallBack(void* ppara);
	static void WINAPI IOReadStateCallBack(void* ppara, unsigned int state);
// 对话框数据
	enum { IDD = IDD_DLLTEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	LRESULT OnUsbNoticeAddMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnUsbNoticeRemoveMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnDataUpdateMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnIoUpdateMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangeTriggerMode();
	afx_msg void OnCbnSelchangeTriggerStyle();
	afx_msg void OnCbnSelchangeTriggerSource();
	afx_msg void OnEnChangeTriggerLevel();
	afx_msg void OnCbnSelchangeComboSamples();
	afx_msg void OnBnClickedCheckAc1();
	afx_msg void OnBnClickedCheckAc2();
	afx_msg void OnBnClickedDevResetBtn();
	afx_msg void OnBnClickedDllReconnectBtn();
	afx_msg void OnBnClickedCheckCapture();
	afx_msg void OnEnChangeEditAmp();
	afx_msg void OnEnChangeEditBias();
	afx_msg void OnEnChangeEditFreq();
	afx_msg void OnBnClickedCheckWaveCtrl();
	afx_msg void OnCbnSelchangeComboBoxing();
	afx_msg void OnEnChangeWaveDutyCycle();
	afx_msg void OnBnClickedCheckIo0();
	afx_msg void OnBnClickedCheckIo1();
	afx_msg void OnBnClickedCheckIo2();
	afx_msg void OnBnClickedCheckIo3();
	afx_msg void OnBnClickedCheckIo4();
	afx_msg void OnBnClickedCheckIo5();
	afx_msg void OnBnClickedCheckIo6();
	afx_msg void OnBnClickedCheckIo7();
	afx_msg void OnCbnSelchangeComInoutIo0();
	afx_msg void OnCbnSelchangeComInoutIo1();
	afx_msg void OnCbnSelchangeComInoutIo2();
	afx_msg void OnCbnSelchangeComInoutIo3();
	afx_msg void OnCbnSelchangeComInoutIo4();
	afx_msg void OnCbnSelchangeComInoutIo5();
	afx_msg void OnCbnSelchangeComInoutIo6();
	afx_msg void OnCbnSelchangeComInoutIo7();
	afx_msg void OnCbnSelchangeComOutIo0();
	afx_msg void OnCbnSelchangeComOutIo1();
	afx_msg void OnCbnSelchangeComOutIo2();
	afx_msg void OnCbnSelchangeComOutIo3();
	afx_msg void OnCbnSelchangeComOutIo4();
	afx_msg void OnCbnSelchangeComOutIo5();
	afx_msg void OnCbnSelchangeComOutIo6();
	afx_msg void OnCbnSelchangeComOutIo7();
private:
	void NextCapture();
	void LeftDisplayZoomCtrl(bool start = false);
	void RightDisplayZoomCtrl(bool start = false);
	unsigned int DisplayZoomCtrl(double y_min, double y_max);
	
	CString SampleStr(double frequence);
	CString ValueStr(double maxv, double minv);

	//CString iddes;
	AdvPlot m_plot;
	//CButton m_btn_pause;

	CTimeRoundSliderCtrl m_slider_time;
	CVlgRoundSliderCtrl m_slider_left_y;
	CVlgRoundSliderCtrl m_slider_right_y;
	int m_slider_left_move_index;
	int m_slider_right_move_index;
	double m_left_range_min, m_left_range_max;
	double m_right_range_min, m_right_range_max;

	unsigned int* samples;
	int sample_num;
	CComboBox m_samples_combox;
	unsigned int m_sample;

	CComboBox m_trigger_mode;
	CComboBox m_trigger_source;
	CComboBox m_trigger_style;	
	int m_trigger_level;

	double* m_buffer;
	unsigned char* m_logic_buffer;
	int m_capture_length;
	unsigned int m_real_length;
	CButton m_check_capture_btn;
	//
	bool ddssupport;
	int m_wave_duty_cycle;
	CButton m_output_ctrl;
	CComboBox m_boxing_style_ctrl;
	CEdit m_wave_freq_ctrl;
	int m_wave_freq;
	CEdit m_wave_duty_cycle_ctrl;
	int m_wave_amp;
	int m_wave_bias;

	CButton m_ch1_ac;
	CButton m_ch2_ac;

	unsigned short chanenl_mask;
	CButton m_check_ch1;
	CButton m_check_ch2;
	CButton m_check_logic;

	bool iosupport;
	CButton m_enable_io0;
	CButton m_enable_io1;
	CButton m_enable_io2;
	CButton m_enable_io3;
	CButton m_enable_io4;
	CButton m_enable_io5;
	CButton m_enable_io6;
	CButton m_enable_io7;
	CComboBox m_com_inout_io0;
	CComboBox m_com_inout_io1;
	CComboBox m_com_inout_io2;
	CComboBox m_com_inout_io3;
	CComboBox m_com_inout_io4;
	CComboBox m_com_inout_io5;
	CComboBox m_com_inout_io6;
	CComboBox m_com_inout_io7;
	CComboBox m_com_out_io0;
	CComboBox m_com_out_io1;
	CComboBox m_com_out_io2;
	CComboBox m_com_out_io3;
	CComboBox m_com_out_io4;
	CComboBox m_com_out_io5;
	CComboBox m_com_out_io6;
	CComboBox m_com_out_io7;
	CLedState m_in_io0;
	CLedState m_in_io1;
	CLedState m_in_io2;
	CLedState m_in_io3;
	CLedState m_in_io4;
	CLedState m_in_io5;
	CLedState m_in_io6;
	CLedState m_in_io7;

	unsigned short sinc4096[4096];
};
