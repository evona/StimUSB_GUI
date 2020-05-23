
// Sitimulation Interface GUIDlg.h : header file
//

#pragma once
//#include "UsbDkHelper.h"
//#include "UsbDkHelperHider.h"
//#include "UsbDkDataHider.h"


// CSitimulationInterfaceGUIDlg dialog
class CSitimulationInterfaceGUIDlg : public CDialogEx
{
// Construction
public:
	CSitimulationInterfaceGUIDlg(CWnd* pParent = nullptr);	// standard constructor
	int checkUSBActivity(void);
	bool writeDataToUSB(UCHAR* dataToWrite, UINT bufferLength);
	bool readDataFromUSB(ULONG64 EndpointAddress,UCHAR* dataBuffer, UINT dataBufferLength);
	bool readDataFromUSBWithoutEnumeration(ULONG64 EndpointAddress, UCHAR* dataBuffer, UINT dataBufferLength);
	bool writeDataToUSBWithoutEnumeration(UCHAR* dataToWrite, UINT bufferLength);
	HANDLE Vulcan24DeviceHandle;
	bool Vulcan24Found;
	DWORD dataBufferRead[1024];
	//PUSB_DK_DEVICE_INFO VulcanDeviceInfo;


	afx_msg void OnBnClickedButtonLeftPhoticWrite();
	afx_msg void OnBnClickedButtonLeftPhoticRead();
	afx_msg void OnBnClickedButtonRightPhoticWrite();
	afx_msg void OnBnClickedButtonRightPhoticRead();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SITIMULATIONINTERFACEGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit mEditLeftPhoticFreqControl;
	double m_LeftPhoticFrequency;
	double m_LeftPhoticIntensity;
	double m_rightPhoticFrequency;
	double m_rightPhoticIntensity;
	CEdit mEditLeftPhoticInteControl;
	CEdit mEditRightPhoticFreqControl;
	CEdit mEditRightPhoticInteControl;
//	CStatic mStaticReportControl;
	//afx_msg void OnBnClickedButtonLeftPhoticWrite();
	CStatic mStaticReportControl;
	afx_msg void OnBnClickedButtonConnectUsb();
//	CListBox m;
	CListBox mStatusMessageBoxControl;
	afx_msg void OnBnClickedButtonReadBulkData();
	//afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonWriteAllSettings();
	void wait(unsigned timeout);
	UCHAR dataRead[1024];
	afx_msg void OnBnClickedButtonDisconnectUsb();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonClearMessages();
};
