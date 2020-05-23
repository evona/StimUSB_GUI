
// Sitimulation Interface GUIDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Sitimulation Interface GUI.h"
#include "Sitimulation Interface GUIDlg.h"
#include "afxdialogex.h"
#include <atlstr.h>
#include <iostream>

#include <windows.h>  
#include <winsvc.h>  

#include "stdafx.h"
#include "UsbDkHelper.h"
#include "UsbDkHelperHider.h"
#include "UsbDkDataHider.h"
#include "time.h"
//#include "clock.h"
#include <chrono>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// constants related to DevBoard Timer Settings
#define CPU_CLK_FREQ 168000000 // STM32F407VG clock frequency
#define PERIPHERAL_CLK_DIVIDER 4 // Peripheral clock down-scale factor
#define TIM2_PS 1
#define TIM3_PS 1
#define TIM12_PS 1
#define TIM13_PS 1

#define TIM2_ARR 65535
#define TIM3_ARR 65535
#define TIM12_ARR 1000
#define TIM13_ARR 1000

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnBnClickedButtonLeftPhoticWrite();
	//afx_msg void OnBnClickedButtonLeftPhoticRead();
	//afx_msg void OnBnClickedButtonRightPhoticWrite();
	//afx_msg void OnBnClickedButtonRightPhoticRead();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	/*ON_BN_CLICKED(IDC_BUTTON_LEFT_PHOTIC_WRITE, &CAboutDlg::OnBnClickedButtonLeftPhoticWrite)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_PHOTIC_READ, &CAboutDlg::OnBnClickedButtonLeftPhoticRead)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_PHOTIC_WRITE, &CAboutDlg::OnBnClickedButtonRightPhoticWrite)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_PHOTIC_READ, &CAboutDlg::OnBnClickedButtonRightPhoticRead)*/
END_MESSAGE_MAP()


// CSitimulationInterfaceGUIDlg dialog



CSitimulationInterfaceGUIDlg::CSitimulationInterfaceGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SITIMULATIONINTERFACEGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_LeftPhoticFrequency = 0.0;
	m_LeftPhoticIntensity = 0.0;
	m_rightPhoticFrequency = 0.0;
	m_rightPhoticIntensity = 0.0;
	Vulcan24Found = false;
	Vulcan24DeviceHandle = NULL;
}

void CSitimulationInterfaceGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	//UpdateData(TRUE);
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LEFT_PHOTIC_FREQ, mEditLeftPhoticFreqControl);
	DDX_Control(pDX, IDC_EDIT_LEFT_PHOTIC_INTE, mEditLeftPhoticInteControl);
	DDX_Control(pDX, IDC_EDIT_RIGHT_PHOTIC_FREQ, mEditRightPhoticFreqControl);
	DDX_Control(pDX, IDC_EDIT_RIGHT_PHOTIC_INTE, mEditRightPhoticInteControl);
	//DDX_Control(pDX, IDC_STATIC_PROGRESS_REPORT, mStaticReportControl);
	DDX_Control(pDX, IDC_STATIC_REPORT, mStaticReportControl);
	//  DDX_Control(pDX, IDC_LIST_STATUS_MESSAGES, m);
	DDX_Control(pDX, IDC_LIST_STATUS_MESSAGES, mStatusMessageBoxControl);
	DDX_Text(pDX, IDC_EDIT_RIGHT_PHOTIC_FREQ, m_rightPhoticFrequency);
	DDX_Text(pDX, IDC_EDIT_RIGHT_PHOTIC_INTE, m_rightPhoticIntensity);
	DDX_Text(pDX, IDC_EDIT_LEFT_PHOTIC_FREQ, m_LeftPhoticFrequency);
	DDX_Text(pDX, IDC_EDIT_LEFT_PHOTIC_INTE, m_LeftPhoticIntensity);
}

BEGIN_MESSAGE_MAP(CSitimulationInterfaceGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LEFT_PHOTIC_WRITE, &CSitimulationInterfaceGUIDlg::OnBnClickedButtonLeftPhoticWrite)


	ON_BN_CLICKED(IDC_BUTTON_LEFT_PHOTIC_WRITE, &CSitimulationInterfaceGUIDlg::OnBnClickedButtonLeftPhoticWrite)
	ON_BN_CLICKED(IDC_BUTTON_LEFT_PHOTIC_READ, &CSitimulationInterfaceGUIDlg::OnBnClickedButtonLeftPhoticRead)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_PHOTIC_WRITE, &CSitimulationInterfaceGUIDlg::OnBnClickedButtonRightPhoticWrite)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_PHOTIC_READ, &CSitimulationInterfaceGUIDlg::OnBnClickedButtonRightPhoticRead)



	ON_BN_CLICKED(IDC_BUTTON_CONNECT_USB, &CSitimulationInterfaceGUIDlg::OnBnClickedButtonConnectUsb)
	ON_BN_CLICKED(IDC_BUTTON_READ_BULK_DATA, &CSitimulationInterfaceGUIDlg::OnBnClickedButtonReadBulkData)
	//ON_BN_CLICKED(IDC_BUTTON1, &CSitimulationInterfaceGUIDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_ALL_SETTINGS, &CSitimulationInterfaceGUIDlg::OnBnClickedButtonWriteAllSettings)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_USB, &CSitimulationInterfaceGUIDlg::OnBnClickedButtonDisconnectUsb)
	ON_BN_CLICKED(IDOK, &CSitimulationInterfaceGUIDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSitimulationInterfaceGUIDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_MESSAGES, &CSitimulationInterfaceGUIDlg::OnBnClickedButtonClearMessages)
END_MESSAGE_MAP()


// CSitimulationInterfaceGUIDlg message handlers

BOOL CSitimulationInterfaceGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	/*CString defaultValues;
	defaultValues.FormatMessage("10.0");*/
	this->m_LeftPhoticFrequency = 1280.0;
	this->m_LeftPhoticIntensity = 50.0;
	this->m_rightPhoticFrequency = 1280.0;
	this->m_rightPhoticIntensity = 50.0;
	//this->mEditLeftPhoticFreqControl.SetWindowTextW(_T("1280.0"));
	//this->mEditRightPhoticFreqControl.SetWindowTextW(_T("1280.0"));
	//this->mEditLeftPhoticInteControl.SetWindowTextW(_T("50.0"));
	//this->mEditRightPhoticInteControl.SetWindowTextW(_T("50.0"));
	UpdateData(FALSE);
	//GetDlgItem(IDC_EDIT1)->SetWindowTextA("100");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSitimulationInterfaceGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSitimulationInterfaceGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSitimulationInterfaceGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//
//void CAboutDlg::OnBnClickedButtonLeftPhoticWrite()
//{
//	// TODO: Add your control notification handler code here
//}
//
//
//void CAboutDlg::OnBnClickedButtonLeftPhoticRead()
//{
//	// TODO: Add your control notification handler code here
//	this->mStaticReportControl.SetWindowTextW(_T("Left Photic Read Values..."));
//}
//
//
//void CAboutDlg::OnBnClickedButtonRightPhoticWrite()
//{
//	// TODO: Add your control notification handler code here
//	this->mStaticReportControl.SetWindowTextW(_T("Right Photic Write Values..."));
//}
//
//
//void CAboutDlg::OnBnClickedButtonRightPhoticRead()
//{
//	// TODO: Add your control notification handler code here
//	this->mStaticReportControl.SetWindowTextW(_T("Right Photic Read Values..."));
//}


void CSitimulationInterfaceGUIDlg::OnBnClickedButtonLeftPhoticWrite()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//String freqString,intensityString;
	//this->mEditLeftPhoticFreqControl.GetWindowTextW(&freqString);
	//this->m_LeftPhoticFrequency = 

	this->mStaticReportControl.SetWindowTextW(_T("Left Photic Write Values..."));

	UCHAR dataToWrite[10];
	UINT dataLengthToWrite = 10;

	double TIM2_ARR_InitialValue = 66; //65535
	double TIM2_ARR_Value;
	TIM2_ARR_Value = 640.0 * TIM2_ARR_InitialValue / this->m_LeftPhoticFrequency * 2;
	UINT32 TIM2_ARR_INT = (UINT32)TIM2_ARR_Value;


	double TIM12_CCR1 = 1000 - (this->m_LeftPhoticIntensity * 10);
	UINT32 TIM12_CCR1_INT = (UINT32)TIM12_CCR1;

	dataToWrite[0] = 0;
	dataToWrite[1] = TIM2_ARR_INT & 0xFF;  //xFF;
	dataToWrite[2] = (TIM2_ARR_INT & 0xFF00) >> 8;// 0x7F;
	dataToWrite[3] = 0x00;
	dataToWrite[4] = 0x00;

	dataToWrite[5] = 1;
	dataToWrite[6] = TIM12_CCR1_INT & 0xFF;;//0x80;
	dataToWrite[7] = (TIM12_CCR1_INT & 0xFF00) >> 8;//0x03;
	dataToWrite[8] = 0x00;
	dataToWrite[9] = 0x00;


	//this->writeDataToUSB(dataToWrite, dataLengthToWrite);

	this->writeDataToUSBWithoutEnumeration(dataToWrite, dataLengthToWrite);




}

void CSitimulationInterfaceGUIDlg::OnBnClickedButtonRightPhoticWrite()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->mStaticReportControl.SetWindowTextW(_T("Right Photic Write Values..."));

	UCHAR dataToWrite[10];
	UINT dataLengthToWrite = 10;

	double TIM3_ARR_InitialValue = 66; //65535
	double TIM3_ARR_Value;
	TIM3_ARR_Value = 640.0 * TIM3_ARR_InitialValue / this->m_rightPhoticFrequency * 2;
	UINT32 TIM3_ARR_INT = (UINT32)TIM3_ARR_Value;


	double TIM13_CCR1 = 1000 - (this->m_rightPhoticIntensity * 10);
	UINT32 TIM13_CCR1_INT = (UINT32)TIM13_CCR1;

	dataToWrite[0] = 2;
	dataToWrite[1] = TIM3_ARR_INT & 0xFF;  //xFF;
	dataToWrite[2] = (TIM3_ARR_INT & 0xFF00) >> 8;// 0x7F;
	dataToWrite[3] = 0x00;
	dataToWrite[4] = 0x00;

	dataToWrite[5] = 3;
	dataToWrite[6] = TIM13_CCR1_INT & 0xFF;;//0x80;
	dataToWrite[7] = (TIM13_CCR1_INT & 0xFF00) >> 8;//0x03;
	dataToWrite[8] = 0x00;
	dataToWrite[9] = 0x00;

	this->writeDataToUSBWithoutEnumeration(dataToWrite, dataLengthToWrite);

}

void CSitimulationInterfaceGUIDlg::OnBnClickedButtonLeftPhoticRead()
{
	// TODO: Add your control notification handler code here
	this->mStaticReportControl.SetWindowTextW(_T("Left Photic Read Values..."));
}


void CSitimulationInterfaceGUIDlg::OnBnClickedButtonRightPhoticRead()
{
	// TODO: Add your control notification handler code here
	this->mStaticReportControl.SetWindowTextW(_T("Right Photic Read Values..."));
}


void CSitimulationInterfaceGUIDlg::OnBnClickedButtonConnectUsb()
{
	// TODO: Add your control notification handler code here
	int Result;
	this->mStaticReportControl.SetWindowTextW(_T("Connecting USB..."));

	Result = this->checkUSBActivity();

}

int CSitimulationInterfaceGUIDlg::checkUSBActivity(void)
{
	PUSB_DK_DEVICE_INFO devicesArray;
	ULONG               numberDevices;
	bool successResetDevice = false;
	bool successResetPipe = false;
	bool successGetConfigurationDescriptor = false;
	//bool Vulcan24Found = false;
	bool successStop = false;
	bool successGetDeviceList = false;
	bool successGetDescriptorRequest = false;
	CString messageString;
	CStringW tempStringW;
	CString tempString;

	this->mStatusMessageBoxControl.ResetContent();
	messageString.Format(_T("Enumerate USB devices\n"));

	// wprintf(L"Enumerate USB devices\n");
	this->mStatusMessageBoxControl.SetWindowTextW(messageString);

	this->mStatusMessageBoxControl.GetWindowTextW(tempStringW);

	successGetDeviceList = UsbDk_GetDevicesList(&devicesArray, &numberDevices);

	if (successGetDeviceList)
	{
		this->mStatusMessageBoxControl.AddString(_T("Enumerate USB devices succeeded"));

		//  wprintf(L"Enumerate USB devices succeeded\n");
		messageString.Format(_T("Found %d USB devices...\n"), numberDevices);

		this->mStatusMessageBoxControl.AddString(messageString);
		//    wprintf(L"Found %d USB devices...\n", numberDevices);

		for (ULONG deviceIndex = 0; deviceIndex < numberDevices; ++deviceIndex)
		{
			//   HANDLE Vulcan24DeviceHandle;
			auto& Dev = devicesArray[deviceIndex];

			if (Dev.DeviceDescriptor.idProduct == 0x01 && Dev.DeviceDescriptor.idVendor == 0x01)
			{
				USB_DK_TRANSFER_REQUEST requestVulcan24;
				long int dataToSend[12] = { 0xAAAAAAAAAAAAAAAA, 0x5555555555555555,0xAAAAAAAAAAAAAAAA, 0x5555555555555555,0xAAAAAAAAAAAAAAAA, 0x5555555555555555,0xAAAAAAAAAAAAAAAA, 0x5555555555555555,0xAAAAAAAAAAAAAAAA, 0x5555555555555555,0xAAAAAAAAAAAAAAAA, 0x5555555555555555 };
				requestVulcan24.EndpointAddress = 0;
				requestVulcan24.BufferLength = 12;
				requestVulcan24.Buffer = &dataToSend;
				TransferResult transferResultVulcan24_ReadPipe;
				TransferResult transferResultVulcan24_WritePipe;

				this->Vulcan24Found = true;
				/*wprintf(L"Vulcan-24 USB device found...\n");
				wprintf(L"==============================\n");
				wprintf(L"Filter ID: %d\n", Dev.FilterID);
				wprintf(L"Port: %d\n", Dev.Port);
				wprintf(L"Speed : %d\n", Dev.Speed);
				wprintf(L"Vendor ID : 0x%X\n", Dev.DeviceDescriptor.idVendor);
				wprintf(L"Product ID : 0x%X\n\n", Dev.DeviceDescriptor.idProduct);*/
				//this-> VulcanDeviceInfo = devicesArray[deviceIndex];
				messageString.Format(_T("Vulcan-24 USB device found..."));
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("=============================="));
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Filter ID: %d"), Dev.FilterID);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Port ID: %d"), Dev.Port);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Speed ID: %d"), Dev.Speed);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Vendor ID : 0x%X"), Dev.DeviceDescriptor.idVendor);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Product ID :0x%X"), Dev.DeviceDescriptor.idProduct);
				this->mStatusMessageBoxControl.AddString(messageString);







				//Create the OVERLAPPED structure for asynch transfers
				OVERLAPPED* overlappedVulcan24 = NULL; // initialize as null; to be prepared...
				overlappedVulcan24 = new OVERLAPPED;// Create overlapped structure for each thread
				overlappedVulcan24->Internal = 0;
				overlappedVulcan24->InternalHigh = 0;
				overlappedVulcan24->Offset = 0;
				overlappedVulcan24->OffsetHigh = 0;
				overlappedVulcan24->Pointer = 0;
				overlappedVulcan24->Offset = 0;
				HANDLE hEvent;
				hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
				if (hEvent)
				{
					overlappedVulcan24->hEvent = hEvent;
				}
				else
				{
					//wprintf(L"\nCreate event failed with error:%d", GetLastError());
					messageString.Format(_T("Create event failed with error : % d"), GetLastError());
					this->mStatusMessageBoxControl.AddString(messageString);
				}
				// OVERLAPPED structure creation complete

				// Create the transfer request structure to be used during sending data (WritePipe)                    
				USB_DK_TRANSFER_REQUEST Vulcan24TransferRequest;
				Vulcan24TransferRequest.EndpointAddress = 0x1;// 0x0;
				UCHAR bufferToSend[13] = "Hello World!";
				UCHAR bufferToSend55[13] = "UUUUUUUUUUUU";
				//UCHAR bufferToSendEE[16] = "Yes This is It!";
				UCHAR bufferToSendEE[16] = "Test Message...";
				Vulcan24TransferRequest.Buffer = &bufferToSendEE;
				Vulcan24TransferRequest.BufferLength = 16;
				Vulcan24TransferRequest.TransferType = BulkTransferType; //BulkTransferType
				/*Vulcan24TransferRequest.IsochronousPacketsArray = bufferToSend55;
				Vulcan24TransferRequest.IsochronousPacketsArraySize = 15;*/
				// BulkTransferType;
				// ControlTransferType;
				// InterruptTransferType;
				// IsochronousTransferType;
				/*typedef enum
				{
					ControlTransferType,
					BulkTransferType,
					InterruptTransferType,
					IsochronousTransferType
				} USB_DK_TRANSFER_TYPE;*/

				// TransferRequest structure created
				// create counters to count number of failures and success during transfers.
				int successCount = 0;
				int failureCount = 0;
				USB_DK_CONFIG_DESCRIPTOR_REQUEST Vulcan24ConfigDescriptorRequest;
				PUSB_CONFIGURATION_DESCRIPTOR Vulcan24ConfigDescriptor;
				ULONG Vulcan24ConfigDescriptorLength = 20;
				Vulcan24ConfigDescriptorRequest.Index = 0x1;

				// START OPERATING ON THE DEVICE
				//============================================================
				//1. Get the Handle to the Device by detaching it from Windows Kernel
				this->Vulcan24DeviceHandle = UsbDk_StartRedirect(&Dev.ID); // sometimes we have issues getting the handle! Why?

				//2. Reset the device
				successResetDevice = UsbDk_ResetDevice(this->Vulcan24DeviceHandle);
				//transferResultVulcan24_ReadPipe =   UsbDk_ReadPipe(Vulcan24DeviceHandle, &requestVulcan24, overlappedVulcan24);

				//3. Reset the pipe (==ENDP in HW, we will use ENDP0 for default operation during test)
				successResetPipe = UsbDk_ResetPipe(this->Vulcan24DeviceHandle, 0x0);

				//successGetConfigurationDescriptor = UsbDk_GetConfigurationDescriptor(&Vulcan24ConfigDescriptorRequest,
				//    &Vulcan24ConfigDescriptor,
				//    &Vulcan24ConfigDescriptorLength);

				//this->wait(1000); // wait 1 sec. after reset

				//4. To test the pipe, write to the ENDP0 100 times
				int numberOfInitialMessages = 1;
				int index;
				for (index = 0; index < numberOfInitialMessages; index++)
				{
					/*  if (index == 9)*/
					{
						UCHAR startMessage[17] = "Start Operation!";
						//UCHAR startMessage[17] = "123456789ABCDEF!";
						Vulcan24TransferRequest.BufferLength = 17;

						//UCHAR startMessage[2] = "S";// tart Operation!";
						//Vulcan24TransferRequest.BufferLength = 2;//17;

						//UCHAR startMessage[17] = "OPERATION START";
						//Vulcan24TransferRequest.BufferLength = 16;

						Vulcan24TransferRequest.Buffer = &startMessage;
					}

					transferResultVulcan24_WritePipe = UsbDk_WritePipe(this->Vulcan24DeviceHandle, &Vulcan24TransferRequest, overlappedVulcan24);

					if (transferResultVulcan24_WritePipe == 0)
						failureCount++;
					else
						successCount++;

				}

				//                wprintf(L"Writing to Pipe is Complete. Total Count: %d | Success Count: %d | Failure Count: %d\n", index, successCount, failureCount);

				messageString.Format(_T("Writing to Pipe is Complete. Total Count: %d | Success Count: %d | Failure Count: %d\n"), index, successCount, failureCount);
				this->mStatusMessageBoxControl.AddString(messageString);



				if (failureCount > 0)
				{
					int errorCode = GetLastError();
					//                    wprintf(L"Error Code is: %d\n", errorCode);

					messageString.Format(_T("Error Code is: %d\n"), errorCode);
					this->mStatusMessageBoxControl.AddString(messageString);


				}



				// 5. Start closing the interface by resetting the pipe
				//successResetPipe = UsbDk_ResetPipe(this->Vulcan24DeviceHandle, 0x0);
				// 6. Reset the device
				//successResetDevice = UsbDk_ResetDevice(this->Vulcan24DeviceHandle);

				// 7. Give the driver back to Windows Kernel
				//successStop = UsbDk_StopRedirect(this->Vulcan24DeviceHandle);

			}
			else
			{
				/* HANDLE otherDeviceHandle;
				 bool successResetOtherDevice = false;
				 bool successResetPipe = false;
				 wprintf(L"DEVICE #: %d\n", deviceIndex);
				 wprintf(L"==============================\n");
				 wprintf(L"Filter ID: %d\n", Dev.FilterID);
				 wprintf(L"Port: %d\n", Dev.Port);
				 wprintf(L"Speed : %d\n", Dev.Speed);
				 wprintf(L"Vendor ID : 0x%X\n", Dev.DeviceDescriptor.idVendor);
				 wprintf(L"Product ID : 0x%X\n\n", Dev.DeviceDescriptor.idProduct);*/

				 /* CRASHED WINDOWS!!!
					 otherDeviceHandle = UsbDk_StartRedirect(&Dev.ID);

					 successResetOtherDevice = UsbDk_ResetDevice(otherDeviceHandle);
					 successResetPipe = UsbDk_ResetPipe(otherDeviceHandle, 0x0);
				 */



			}

		}
		if (Vulcan24Found == false)
		{
			// wprintf(L"Vulcan-24 USB device NOT found...\n");
			messageString.Format(_T("Vulcan-24 USB device NOT found..."));
			this->mStatusMessageBoxControl.AddString(messageString);


		}

		return 0;

	}
	else
	{
		//wprintf(L"Enumerate USB devices failed\n");
		messageString.Format(_T("Enumerate USB devices failed"));
		this->mStatusMessageBoxControl.AddString(messageString);

		return -1;
	}

}


bool CSitimulationInterfaceGUIDlg::writeDataToUSB(UCHAR* dataToWrite, UINT bufferLength)
{

	PUSB_DK_DEVICE_INFO devicesArray;
	ULONG               numberDevices;
	bool successResetDevice = false;
	bool successResetPipe = false;
	bool successGetConfigurationDescriptor = false;
	bool Vulcan24Found = false;
	bool successStop = false;
	bool successGetDeviceList = false;
	bool successGetDescriptorRequest = false;
	CString messageString;


	this->mStatusMessageBoxControl.ResetContent();


	successGetDeviceList = UsbDk_GetDevicesList(&devicesArray, &numberDevices);

	if (successGetDeviceList)
	{
		this->mStatusMessageBoxControl.AddString(_T("Enumerate USB devices succeeded"));

		//  wprintf(L"Enumerate USB devices succeeded\n");
		messageString.Format(_T("Found %d USB devices...\n"), numberDevices);

		this->mStatusMessageBoxControl.AddString(messageString);
		//    wprintf(L"Found %d USB devices...\n", numberDevices);

		for (ULONG deviceIndex = 0; deviceIndex < numberDevices; ++deviceIndex)
		{
			HANDLE Vulcan24DeviceHandle;
			auto& Dev = devicesArray[deviceIndex];

			if (Dev.DeviceDescriptor.idProduct == 0x01 && Dev.DeviceDescriptor.idVendor == 0x01)
			{

				TransferResult transferResultVulcan24_WritePipe;

				Vulcan24Found = true;

				messageString.Format(_T("Vulcan-24 USB device found..."));
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("=============================="));
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Filter ID: %d"), Dev.FilterID);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Port ID: %d"), Dev.Port);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Speed ID: %d"), Dev.Speed);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Vendor ID : 0x%X"), Dev.DeviceDescriptor.idVendor);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Product ID :0x%X"), Dev.DeviceDescriptor.idProduct);
				this->mStatusMessageBoxControl.AddString(messageString);



				//Create the OVERLAPPED structure for asynch transfers
				OVERLAPPED* overlappedVulcan24 = NULL; // initialize as null; to be prepared...
				overlappedVulcan24 = new OVERLAPPED;// Create overlapped structure for each thread
				overlappedVulcan24->Internal = 0;
				overlappedVulcan24->InternalHigh = 0;
				overlappedVulcan24->Offset = 0;
				overlappedVulcan24->OffsetHigh = 0;
				overlappedVulcan24->Pointer = 0;
				overlappedVulcan24->Offset = 0;
				HANDLE hEvent;
				hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
				if (hEvent)
				{
					overlappedVulcan24->hEvent = hEvent;
				}
				else
				{
					//wprintf(L"\nCreate event failed with error:%d", GetLastError());
					messageString.Format(_T("Create event failed with error : % d"), GetLastError());
					this->mStatusMessageBoxControl.AddString(messageString);
				}
				// OVERLAPPED structure creation complete

				// Create the transfer request structure to be used during sending data (WritePipe)                    
				USB_DK_TRANSFER_REQUEST Vulcan24TransferRequest;
				Vulcan24TransferRequest.EndpointAddress = 0x1;// 0x0;

				Vulcan24TransferRequest.Buffer = dataToWrite;
				Vulcan24TransferRequest.BufferLength = bufferLength;
				Vulcan24TransferRequest.TransferType = BulkTransferType; //BulkTransferType

				// TransferRequest structure created
				// create counters to count number of failures and success during transfers.
				int successCount = 0;
				int failureCount = 0;


				// START OPERATING ON THE DEVICE
				//============================================================
				//1. Get the Handle to the Device by detaching it from Windows Kernel
				Vulcan24DeviceHandle = UsbDk_StartRedirect(&Dev.ID); // sometimes we have issues getting the handle! Why?

				//successGetDescriptorRequest = UsbDk_GetConfigurationDescriptor(&Vulcan24ConfigDescriptorRequest,
				//    &Vulcan24ConfigDescriptor,
				//    &Vulcan24ConfigDescriptorLength);

				//2. Reset the device
				successResetDevice = UsbDk_ResetDevice(Vulcan24DeviceHandle);
				//transferResultVulcan24_ReadPipe =   UsbDk_ReadPipe(Vulcan24DeviceHandle, &requestVulcan24, overlappedVulcan24);

				//3. Reset the pipe (==ENDP in HW, we will use ENDP0 for default operation during test)
				successResetPipe = UsbDk_ResetPipe(Vulcan24DeviceHandle, 0x0);


				//4. To test the pipe, write to the ENDP0 100 times
				/*int index;
				for (index = 0; index <= 9; index++)
				{*/

				transferResultVulcan24_WritePipe = UsbDk_WritePipe(Vulcan24DeviceHandle, &Vulcan24TransferRequest, overlappedVulcan24);
				if (transferResultVulcan24_WritePipe == 0)
					failureCount++;
				else
					successCount++;

				messageString.Format(_T("Writing to Pipe is Complete. Total Count: %d | Success Count: %d | Failure Count: %d\n"), 1, successCount, failureCount);
				this->mStatusMessageBoxControl.AddString(messageString);



				if (failureCount > 0)
				{
					int errorCode = GetLastError();

					messageString.Format(_T("Error Code is: %d\n"), errorCode);
					this->mStatusMessageBoxControl.AddString(messageString);


				}



				// 5. Start closing the interface by resetting the pipe
				successResetPipe = UsbDk_ResetPipe(Vulcan24DeviceHandle, 0x0);
				// 6. Reset the device
				successResetDevice = UsbDk_ResetDevice(Vulcan24DeviceHandle);

				// 7. Give the driver back to Windows Kernel
				successStop = UsbDk_StopRedirect(Vulcan24DeviceHandle);

			}
			else
			{


			}

		}
		if (Vulcan24Found == false)
		{
			// wprintf(L"Vulcan-24 USB device NOT found...\n");
			messageString.Format(_T("Vulcan-24 USB device NOT found..."));
			this->mStatusMessageBoxControl.AddString(messageString);


		}

		return 0;

	}
	else
	{
		this->mStatusMessageBoxControl.ResetContent();
		messageString.Format(_T("Enumerate USB devices failed"));
		this->mStatusMessageBoxControl.AddString(messageString);

		return -1;
	}

}


void CSitimulationInterfaceGUIDlg::OnBnClickedButtonReadBulkData()
{
	// TODO: Add your control notification handler code here
	//UCHAR dataRead[1024];
	UCHAR dataReadLocal[1024];

	UINT16 dataLength = sizeof(dataReadLocal);
	ULONG64 EndpointAddress = 0x1; // 0x2;
	bool isSuccess;
	CFile myFile;

	for (int resetIndex = 0; resetIndex < dataLength; resetIndex++)
	{
		dataReadLocal[resetIndex] = 0xAA;
	}


	//isSuccess = this->readDataFromUSB(this->dataRead, dataLength);
	//isSuccess = this->readDataFromUSB(EndpointAddress,dataReadLocal, dataLength);
	isSuccess = this->readDataFromUSBWithoutEnumeration(EndpointAddress, dataReadLocal, dataLength);
	//myFile.Open()
	myFile.Open(_T("dataRead.dat"), CFile::modeCreate | CFile::modeWrite, NULL); //"w+");
	//myFile.Write(this->dataRead,dataLength);
	myFile.Write(dataReadLocal, dataLength);
	myFile.Flush();
	myFile.Close();
	//myFile = fopen("dataRead.txt", "w+");
	//bool successStop;
	//successStop = UsbDk_StopRedirect(this->Vulcan24DeviceHandle);
	////successStart = UsbDk_StartRedirect(this->Vulcan24DeviceHandle);

	//int Result = this->checkUSBActivity();

	//


}

bool CSitimulationInterfaceGUIDlg::readDataFromUSB(ULONG64 EndpointAddress, UCHAR* dataBuffer, UINT dataBufferLength)
{

	PUSB_DK_DEVICE_INFO devicesArray;
	ULONG               numberDevices;
	bool successResetDevice = false;
	bool successResetPipe = false;
	bool successGetConfigurationDescriptor = false;
	bool Vulcan24Found = false;
	bool successStop = false;
	bool successGetDeviceList = false;
	bool successGetDescriptorRequest = false;
	CString messageString;


	this->mStatusMessageBoxControl.ResetContent();


	successGetDeviceList = UsbDk_GetDevicesList(&devicesArray, &numberDevices);

	if (successGetDeviceList)
	{
		this->mStatusMessageBoxControl.AddString(_T("Enumerate USB devices succeeded"));

		//  wprintf(L"Enumerate USB devices succeeded\n");
		messageString.Format(_T("Found %d USB devices...\n"), numberDevices);

		this->mStatusMessageBoxControl.AddString(messageString);
		//    wprintf(L"Found %d USB devices...\n", numberDevices);

		for (ULONG deviceIndex = 0; deviceIndex < numberDevices; ++deviceIndex)
		{
			HANDLE Vulcan24DeviceHandle;
			auto& Dev = devicesArray[deviceIndex];

			if (Dev.DeviceDescriptor.idProduct == 0x01 && Dev.DeviceDescriptor.idVendor == 0x01)
			{

				TransferResult transferResultVulcan24_ReadPipe;

				Vulcan24Found = true;

				messageString.Format(_T("Vulcan-24 USB device found..."));
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("=============================="));
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Filter ID: %d"), Dev.FilterID);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Port ID: %d"), Dev.Port);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Speed ID: %d"), Dev.Speed);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Vendor ID : 0x%X"), Dev.DeviceDescriptor.idVendor);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Product ID :0x%X"), Dev.DeviceDescriptor.idProduct);
				this->mStatusMessageBoxControl.AddString(messageString);



				//Create the OVERLAPPED structure for asynch transfers
				OVERLAPPED* overlappedVulcan24 = NULL; // initialize as null; to be prepared...
				overlappedVulcan24 = new OVERLAPPED;// Create overlapped structure for each thread
				overlappedVulcan24->Internal = 0;
				overlappedVulcan24->InternalHigh = 0;
				overlappedVulcan24->Offset = 0;
				overlappedVulcan24->OffsetHigh = 0;
				overlappedVulcan24->Pointer = 0;
				overlappedVulcan24->Offset = 0;
				HANDLE hEvent;
				hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
				if (hEvent)
				{
					overlappedVulcan24->hEvent = hEvent;
				}
				else
				{
					//wprintf(L"\nCreate event failed with error:%d", GetLastError());
					messageString.Format(_T("Create event failed with error : % d"), GetLastError());
					this->mStatusMessageBoxControl.AddString(messageString);
				}
				// OVERLAPPED structure creation complete

				// Create the transfer request structure to be used during sending data (WritePipe)                    
				USB_DK_TRANSFER_REQUEST Vulcan24TransferRequest;
				Vulcan24TransferRequest.EndpointAddress = EndpointAddress; // 0x2;// 0x0;

				Vulcan24TransferRequest.Buffer = dataBuffer;
				Vulcan24TransferRequest.BufferLength = dataBufferLength;
				Vulcan24TransferRequest.TransferType = BulkTransferType; //BulkTransferType

				// TransferRequest structure created
				// create counters to count number of failures and success during transfers.
				int successCount = 0;
				int failureCount = 0;


				// START OPERATING ON THE DEVICE
				//============================================================
				//1. Get the Handle to the Device by detaching it from Windows Kernel
				Vulcan24DeviceHandle = UsbDk_StartRedirect(&Dev.ID); // sometimes we have issues getting the handle! Why?

				//successGetDescriptorRequest = UsbDk_GetConfigurationDescriptor(&Vulcan24ConfigDescriptorRequest,
				//    &Vulcan24ConfigDescriptor,
				//    &Vulcan24ConfigDescriptorLength);

				//2. Reset the device
				successResetDevice = UsbDk_ResetDevice(Vulcan24DeviceHandle);
				//transferResultVulcan24_ReadPipe =   UsbDk_ReadPipe(Vulcan24DeviceHandle, &requestVulcan24, overlappedVulcan24);

				//3. Reset the pipe (==ENDP in HW, we will use ENDP0 for default operation during test)
				successResetPipe = UsbDk_ResetPipe(Vulcan24DeviceHandle, 0x0);


				//4. To test the pipe, write to the ENDP0 100 times
				int index;
				int totalCount = 1;
				int totalBytesRead = 0;
				double timeElapsed = 0.0;
				double bitRateMbps = 0.0;


				//high_resol
				//high_resolution_clock::now

				auto start = std::chrono::high_resolution_clock::now();


				for (index = 0; index <= totalCount - 1; index++)
				{

					transferResultVulcan24_ReadPipe = UsbDk_ReadPipe(Vulcan24DeviceHandle, &Vulcan24TransferRequest, overlappedVulcan24);
					if (transferResultVulcan24_ReadPipe == 0)
						failureCount++;
					else
					{
						successCount++;
						totalBytesRead += Vulcan24TransferRequest.BufferLength;
					}
				}
				auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = finish - start; //nanoseconds?

				timeElapsed = elapsed.count(); // in seconds

				//timeElapsed = 1; //finish - start;

				bitRateMbps = (((double)totalBytesRead) * 8 / (timeElapsed)) * 1e-6;

				this->mStatusMessageBoxControl.ResetContent();
				messageString.Format(_T("Reading from Pipe is Complete. Total Count: %d | Success Count: %d | Failure Count: %d\n"), totalCount, successCount, failureCount);
				this->mStatusMessageBoxControl.AddString(messageString);




				messageString.Format(_T("Total Bytes Read: %d | Total Duration: %3.3f us | Data Rate: %4.3f Mbps\n"), totalBytesRead, timeElapsed * 1e6, bitRateMbps);
				this->mStatusMessageBoxControl.AddString(messageString);



				bool allDataPrinted = false;
				int dataToPrint;
				index = 0;
				int printLength = Vulcan24TransferRequest.BufferLength;

				messageString.Format(_T("The first %d 32-bit of the data read is as below:\n"), printLength);

				this->mStatusMessageBoxControl.AddString(messageString);
				this->mStatusMessageBoxControl.AddString(_T("=================================================\n"));

				while (!allDataPrinted)
				{

					dataToPrint = *(((int*)(Vulcan24TransferRequest.Buffer)) + index);// Buffer[index];


					messageString.Format(_T("%d. 0x%X"), index + 1, dataToPrint);

					this->mStatusMessageBoxControl.AddString(messageString);

					index++;

					if (index == printLength)
						allDataPrinted = true;

				}

				this->mStatusMessageBoxControl.AddString(_T("=================================================\n"));

				if (failureCount > 0)
				{
					int errorCode = GetLastError();

					messageString.Format(_T("Error Code is: %d\n"), errorCode);
					this->mStatusMessageBoxControl.AddString(messageString);


				}



				// 5. Start closing the interface by resetting the pipe
				successResetPipe = UsbDk_ResetPipe(Vulcan24DeviceHandle, 0x0);
				// 6. Reset the device
				successResetDevice = UsbDk_ResetDevice(Vulcan24DeviceHandle);

				// 7. Give the driver back to Windows Kernel
				successStop = UsbDk_StopRedirect(Vulcan24DeviceHandle);

			}
			else
			{


			}

		}
		if (Vulcan24Found == false)
		{
			// wprintf(L"Vulcan-24 USB device NOT found...\n");
			messageString.Format(_T("Vulcan-24 USB device NOT found..."));
			this->mStatusMessageBoxControl.AddString(messageString);


		}

		return 0;

	}
	else
	{
		this->mStatusMessageBoxControl.ResetContent();
		messageString.Format(_T("Enumerate USB devices failed"));
		this->mStatusMessageBoxControl.AddString(messageString);

		return -1;
	}

}

//void CSitimulationInterfaceGUIDlg::OnBnClickedButton1()
//{
//    // TODO: Add your control notification handler code here
//}


void CSitimulationInterfaceGUIDlg::OnBnClickedButtonWriteAllSettings()
{
	// TODO: Add your control notification handler code here
	this->UpdateData(TRUE);
	this->OnBnClickedButtonRightPhoticWrite();
	this->wait(500);
	this->OnBnClickedButtonLeftPhoticWrite();

	this->mStaticReportControl.SetWindowTextW(_T("Right & Left Photic Write Values Done..."));




}

void CSitimulationInterfaceGUIDlg::wait(unsigned timeout)
{
	timeout += std::clock();
	while (std::clock() < timeout) continue;
}



bool CSitimulationInterfaceGUIDlg::readDataFromUSBWithoutEnumeration(ULONG64 EndpointAddress, UCHAR* dataBuffer, UINT dataBufferLength)
{

	PUSB_DK_DEVICE_INFO devicesArray;
	ULONG               numberDevices;
	bool successResetDevice = false;
	bool successResetPipe = false;
	bool successGetConfigurationDescriptor = false;
	bool successStop = false;
	bool successGetDeviceList = false;
	bool successGetDescriptorRequest = false;
	CString messageString;

	this->mStatusMessageBoxControl.ResetContent();

	if (this->Vulcan24DeviceHandle != NULL)
	{
		if (this->Vulcan24Found)
		{

			TransferResult transferResultVulcan24_ReadPipe;

			//Create the OVERLAPPED structure for asynch transfers
			OVERLAPPED* overlappedVulcan24 = NULL; // initialize as null; to be prepared...
			overlappedVulcan24 = new OVERLAPPED;// Create overlapped structure for each thread
			overlappedVulcan24->Internal = 0;
			overlappedVulcan24->InternalHigh = 0;
			overlappedVulcan24->Offset = 0;
			overlappedVulcan24->OffsetHigh = 0;
			overlappedVulcan24->Pointer = 0;
			overlappedVulcan24->Offset = 0;
			HANDLE hEvent;
			hEvent = CreateEvent(NULL, //default security attributes
				TRUE, //manual reset event
				FALSE,  // Initial State is Non-signaled
				TEXT("USBReadEvent")); // Event Name
			if (hEvent)
			{
				overlappedVulcan24->hEvent = hEvent;
			}
			else
			{
				//wprintf(L"\nCreate event failed with error:%d", GetLastError());
				messageString.Format(_T("Create event failed with error : % d"), GetLastError());
				this->mStatusMessageBoxControl.AddString(messageString);
			}
			// OVERLAPPED structure creation complete

			// Create the transfer request structure to be used during sending data (WritePipe)                    
			USB_DK_TRANSFER_REQUEST Vulcan24TransferRequest;
			Vulcan24TransferRequest.EndpointAddress = EndpointAddress; // 0x2;// 0x0;

			Vulcan24TransferRequest.Buffer = dataBuffer;
			Vulcan24TransferRequest.BufferLength = dataBufferLength;
			Vulcan24TransferRequest.TransferType = BulkTransferType; //BulkTransferType

			// initializing
			Vulcan24TransferRequest.IsochronousPacketsArraySize = dataBufferLength;
			Vulcan24TransferRequest.IsochronousPacketsArray = dataBuffer;

			// TransferRequest structure created
			// create counters to count number of failures and success during transfers.
			int successCount = 0;
			int failureCount = 0;

			// START OPERATING ON THE DEVICE
			//============================================================
			//1. Get the Handle to the Device by detaching it from Windows Kernel
			//Vulcan24DeviceHandle = UsbDk_StartRedirect(&Dev.ID); // sometimes we have issues getting the handle! Why?

			//successGetDescriptorRequest = UsbDk_GetConfigurationDescriptor(&Vulcan24ConfigDescriptorRequest,
			//    &Vulcan24ConfigDescriptor,
			//    &Vulcan24ConfigDescriptorLength);

			//2. Reset the device
			//successResetDevice = UsbDk_ResetDevice(this->Vulcan24DeviceHandle);
			//transferResultVulcan24_ReadPipe =   UsbDk_ReadPipe(Vulcan24DeviceHandle, &requestVulcan24, overlappedVulcan24);

			//3. Reset the pipe (==ENDP in HW, we will use ENDP0 for default operation during test)
			//successResetPipe = UsbDk_ResetPipe(this->Vulcan24DeviceHandle, 0x1);


			//4. To test the pipe, write to the ENDP0 100 times
			int index;
			int totalCount = 1;
			int totalBytesRead = 0;
			double timeElapsed = 0.0;
			double bitRateMbps = 0.0;

			auto start = std::chrono::high_resolution_clock::now();

			bool operationSuccessful = false;

			for (index = 0; index <= totalCount - 1; index++)
			{
				DWORD i, dwWait, cbRet, dwErr;

				messageString.Format(_T("USB IN Request-# %d\n"), index + 1);
				this->mStatusMessageBoxControl.AddString(messageString);

				if (!SetEvent(overlappedVulcan24->hEvent))
				{
					messageString.Format(_T("SetEvent failed (%d)\n", GetLastError()));
					this->mStatusMessageBoxControl.AddString(messageString);
					operationSuccessful = false;
					break;
				}
				else
				{
					messageString.Format(_T("SetEvent Succeded\n\n"));
					this->mStatusMessageBoxControl.AddString(messageString);
					messageString.Format(_T("======================\n"));
					this->mStatusMessageBoxControl.AddString(messageString);
				}

				transferResultVulcan24_ReadPipe = UsbDk_ReadPipe(this->Vulcan24DeviceHandle, &Vulcan24TransferRequest, overlappedVulcan24);

				int errorCodeCheck = GetLastError();

				while (errorCodeCheck == 0x3e5) // Error Code 997 | 0x3e5 = IO Pending
				{
					messageString.Format(_T("Last Error Code Received: %X\n"), errorCodeCheck);
					this->mStatusMessageBoxControl.AddString(messageString);
					errorCodeCheck = GetLastError();
				}


				bool isOperationComplete = false;
				unsigned int trials = 0;
				while ((isOperationComplete == false) && (trials < 1000))
				{
					isOperationComplete = GetOverlappedResult(this->Vulcan24DeviceHandle, overlappedVulcan24, this->dataBufferRead, TRUE);
					trials++;
				}

				if (isOperationComplete == FALSE)
				{
					int errorCode = GetLastError();
					messageString.Format(_T("USB Read Failed...\n"), errorCode);
					this->mStatusMessageBoxControl.AddString(messageString);
					messageString.Format(_T("==================\n"), errorCode);
					this->mStatusMessageBoxControl.AddString(messageString);
					messageString.Format(_T("Error Code is: %d\n\n"), errorCode);
					this->mStatusMessageBoxControl.AddString(messageString);
					operationSuccessful = false;
				}
				else
				{
					operationSuccessful = true;

				}

				if (transferResultVulcan24_ReadPipe == 0)
					failureCount++;
				else
				{
					successCount++;
					totalBytesRead += Vulcan24TransferRequest.BufferLength;
				} // here I only start the transmission, so I should go to the overlapped asynch read thread for the end result!!!
			}
			auto finish = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed = finish - start; //nanoseconds?

			timeElapsed = elapsed.count(); // in seconds

			bitRateMbps = (((double)totalBytesRead) * 8 / (timeElapsed)) * 1e-6;

			if (operationSuccessful)
			{
				this->mStatusMessageBoxControl.ResetContent();
				messageString.Format(_T("Reading from Pipe is Complete. Total Count: %d | Success Count: %d | Failure Count: %d\n"), totalCount, successCount, failureCount);
				this->mStatusMessageBoxControl.AddString(messageString);
				messageString.Format(_T("Total Bytes Read: %d | Total Duration: %3.3f us | Data Rate: %4.3f Mbps\n"), totalBytesRead, timeElapsed * 1e6, bitRateMbps);
				this->mStatusMessageBoxControl.AddString(messageString);

				bool allDataPrinted = false;
				UCHAR dataToPrint;
				index = 0;
				int printLength = Vulcan24TransferRequest.BufferLength;

				messageString.Format(_T("The first %d byte of the data read is as below:\n"), printLength);

				this->mStatusMessageBoxControl.AddString(messageString);
				this->mStatusMessageBoxControl.AddString(_T("=================================================\n"));

				while (!allDataPrinted)
				{

					dataToPrint = *((UCHAR*)(Vulcan24TransferRequest.Buffer) + index);

					messageString.Format(_T("%d. \t 0x%X"), index + 1, dataToPrint);

					this->mStatusMessageBoxControl.AddString(messageString);

					index++;

					if (index == printLength)
						allDataPrinted = true;

				}

				this->mStatusMessageBoxControl.AddString(_T("=================================================\n"));

				if (failureCount > 0)
				{
					int errorCode = GetLastError();

					messageString.Format(_T("Error Code is: %d\n"), errorCode);
					this->mStatusMessageBoxControl.AddString(messageString);


				}
			}


			// 5. Start closing the interface by resetting the pipe
			//successResetPipe = UsbDk_ResetPipe(Vulcan24DeviceHandle, 0x0);
			//// 6. Reset the device
			//successResetDevice = UsbDk_ResetDevice(Vulcan24DeviceHandle);

			//// 7. Give the driver back to Windows Kernel
			//successStop = UsbDk_StopRedirect(Vulcan24DeviceHandle);

		}
		else
		{
			messageString.Format(_T("Vulcan-24 USB device NOT found..."));
			this->mStatusMessageBoxControl.AddString(messageString);

		}

		return 0;

	}
	else
	{
		this->mStatusMessageBoxControl.ResetContent();
		messageString.Format(_T("Vulcan24 Device is Not Yet Connected. Try Connecting USB First!"));
		this->mStatusMessageBoxControl.AddString(messageString);

		return -1;
	}

}


void CSitimulationInterfaceGUIDlg::OnBnClickedButtonDisconnectUsb()
{
	// TODO: Add your control notification handler code here
	bool successStop;
	CString messageString;

	if (this->Vulcan24DeviceHandle == NULL)
	{
		this->mStatusMessageBoxControl.ResetContent();
		messageString.Format(_T("Vulcan24 Device is Not Yet Connected. Try Connecting USB First!"));
		this->mStatusMessageBoxControl.AddString(messageString);
	}
	else
	{
		int errorCode = GetLastError();

		this->mStatusMessageBoxControl.ResetContent();

		successStop = UsbDk_StopRedirect(this->Vulcan24DeviceHandle);

		if (successStop)
		{

			messageString.Format(_T("Vulcan24 Device is successfully stopped!"));
			this->Vulcan24DeviceHandle = NULL;

		}
		else
		{

			messageString.Format(_T("Something went wrong during USB Operation.Error Code: 0x%X", errorCode));

		}
		this->mStatusMessageBoxControl.AddString(messageString);

	}
}





bool CSitimulationInterfaceGUIDlg::writeDataToUSBWithoutEnumeration(UCHAR* dataToWrite, UINT bufferLength)
{

	PUSB_DK_DEVICE_INFO devicesArray;
	ULONG               numberDevices;
	bool successResetDevice = false;
	bool successResetPipe = false;
	bool successGetConfigurationDescriptor = false;
	bool successStop = false;
	bool successGetDeviceList = false;
	bool successGetDescriptorRequest = false;
	CString messageString;


	this->mStatusMessageBoxControl.ResetContent();


	if (this->Vulcan24DeviceHandle != NULL)
	{
		if (this->Vulcan24Found)
		{

			TransferResult transferResultVulcan24_WritePipe;

			//Create the OVERLAPPED structure for asynch transfers
			OVERLAPPED* overlappedVulcan24 = NULL; // initialize as null; to be prepared...
			overlappedVulcan24 = new OVERLAPPED;// Create overlapped structure for each thread
			overlappedVulcan24->Internal = 0;
			overlappedVulcan24->InternalHigh = 0;
			overlappedVulcan24->Offset = 0;
			overlappedVulcan24->OffsetHigh = 0;
			overlappedVulcan24->Pointer = 0;
			overlappedVulcan24->Offset = 0;
			HANDLE hEvent;
			hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			if (hEvent)
			{
				overlappedVulcan24->hEvent = hEvent;
			}
			else
			{
				messageString.Format(_T("Create event failed with error : % d"), GetLastError());
				this->mStatusMessageBoxControl.AddString(messageString);
			}
			// OVERLAPPED structure creation complete

			// Create the transfer request structure to be used during sending data (WritePipe)                    
			USB_DK_TRANSFER_REQUEST Vulcan24TransferRequest;
			Vulcan24TransferRequest.EndpointAddress = 0x1;// 0x0;

			Vulcan24TransferRequest.Buffer = dataToWrite;
			Vulcan24TransferRequest.BufferLength = bufferLength;
			Vulcan24TransferRequest.TransferType = BulkTransferType; //BulkTransferType

			// TransferRequest structure created
			// create counters to count number of failures and success during transfers.
			int successCount = 0;
			int failureCount = 0;


			// START OPERATING ON THE DEVICE
			//============================================================
			//1. Get the Handle to the Device by detaching it from Windows Kernel
			//Vulcan24DeviceHandle = UsbDk_StartRedirect(&Dev.ID); // sometimes we have issues getting the handle! Why?

			//successGetDescriptorRequest = UsbDk_GetConfigurationDescriptor(&Vulcan24ConfigDescriptorRequest,
			//    &Vulcan24ConfigDescriptor,
			//    &Vulcan24ConfigDescriptorLength);

			//2. Reset the device
			//successResetDevice = UsbDk_ResetDevice(Vulcan24DeviceHandle);
			//transferResultVulcan24_ReadPipe =   UsbDk_ReadPipe(Vulcan24DeviceHandle, &requestVulcan24, overlappedVulcan24);

			//3. Reset the pipe (==ENDP in HW, we will use ENDP0 for default operation during test)
			//successResetPipe = UsbDk_ResetPipe(Vulcan24DeviceHandle, 0x0);


			//4. To test the pipe, write to the ENDP0 100 times

			// trying write data 16.05.2020
			/*UCHAR testMessage[10] = "Test 1234";
			Vulcan24TransferRequest.Buffer = &testMessage;*/
			// end trying write data 16.05.2020

			int index;
			int writeCount = 1000;
			for (index = 0; index < writeCount; index++)
			{
				transferResultVulcan24_WritePipe = UsbDk_WritePipe(this->Vulcan24DeviceHandle, &Vulcan24TransferRequest, overlappedVulcan24);
				if (transferResultVulcan24_WritePipe == 0)
					failureCount++;
				else
					successCount++;
			}

			messageString.Format(_T("Writing to Pipe is Complete. Total Count: %d | Success Count: %d | Failure Count: %d\n"), 1, successCount, failureCount);
			this->mStatusMessageBoxControl.AddString(messageString);



			if (failureCount > 0)
			{
				int errorCode = GetLastError();

				messageString.Format(_T("Error Code is: %d\n"), errorCode);
				this->mStatusMessageBoxControl.AddString(messageString);


			}



			// 5. Start closing the interface by resetting the pipe
			//successResetPipe = UsbDk_ResetPipe(Vulcan24DeviceHandle, 0x0);
			//// 6. Reset the device
			//successResetDevice = UsbDk_ResetDevice(Vulcan24DeviceHandle);

			//// 7. Give the driver back to Windows Kernel
			//successStop = UsbDk_StopRedirect(Vulcan24DeviceHandle);
			return 0;

		}
		else
		{
		messageString.Format(_T("Vulcan-24 USB device NOT found..."));
		this->mStatusMessageBoxControl.AddString(messageString);
		return -1;

		}

	}

	else
	{
	this->mStatusMessageBoxControl.ResetContent();
	messageString.Format(_T("Vulcan24 Device is Not Yet Connected. Try Connecting USB First!"));
	this->mStatusMessageBoxControl.AddString(messageString);
	return -1;
	}

}


void CSitimulationInterfaceGUIDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	bool successStop;

	if (this->Vulcan24DeviceHandle == NULL)
	{

	}
	else
	{
		successStop = UsbDk_StopRedirect(this->Vulcan24DeviceHandle);

	}


	CDialogEx::OnOK();
}


void CSitimulationInterfaceGUIDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	bool successStop;

	if (this->Vulcan24DeviceHandle == NULL)
	{

	}
	else
	{
		successStop = UsbDk_StopRedirect(this->Vulcan24DeviceHandle);

	}


	CDialogEx::OnCancel();
}


void CSitimulationInterfaceGUIDlg::OnBnClickedButtonClearMessages()
{
	// TODO: Add your control notification handler code here
	this->mStatusMessageBoxControl.ResetContent();

}
