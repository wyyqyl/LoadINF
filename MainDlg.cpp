// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	m_infpath = GetDlgItem(IDC_EDIT_INF_PATH);
	m_install = GetDlgItem(IDC_BUTTON_INSTALL);
	m_start = GetDlgItem(IDC_BUTTON_START);
	m_uninstall = GetDlgItem(IDC_BUTTON_UNINSTALL);
	m_stop = GetDlgItem(IDC_BUTTON_STOP);

	return TRUE;
}

LRESULT CMainDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	EndDialog(wID);
	return 0;
}

BOOL executeApp(TCHAR *app, TCHAR *parameters)
{
	SHELLEXECUTEINFO shExecInfo;
#ifdef _DEBUG
	lstrcat(parameters, TEXT(" >> log.txt"));
#endif
	shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shExecInfo.hwnd = NULL;
	shExecInfo.lpVerb = NULL;
	shExecInfo.lpFile = app;
	shExecInfo.lpParameters = parameters;
	shExecInfo.lpDirectory = NULL;
	shExecInfo.nShow = SW_HIDE;
	shExecInfo.hInstApp = NULL;

	BOOL ret = ShellExecuteEx(&shExecInfo);
	WaitForSingleObject(shExecInfo.hProcess, INFINITE);

	return ret;
}

LRESULT CMainDlg::OnBnClickedButtonBrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_SHAREAWARE, TEXT("INF (*.inf)\0*.inf\0All Files (*.*)\0*.*||"));
	TCHAR buffer[MAX_PATH] = {0};
	fileDlg.m_ofn.lpstrFile = buffer;
	fileDlg.m_ofn.nMaxFile = MAX_PATH;
	if (fileDlg.DoModal() == IDOK)
		m_infpath.SetWindowText(buffer);
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonInstall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	TCHAR parameters[2 * MAX_PATH] = TEXT("syssetup,SetupInfObjectInstallAction DefaultInstall 128 ");
	TCHAR infpath[MAX_PATH] = {0};

	m_infpath.GetWindowText(infpath, MAX_PATH);
	lstrcat(parameters, infpath);
	if (!executeApp(TEXT("rundll32.exe"), parameters))
	{
		MessageBox(TEXT("Failed to install INF file"), TEXT("Error"), MB_OK);
		return -1;
	}

	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonUninstall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	TCHAR parameters[2 * MAX_PATH] = TEXT("syssetup,SetupInfObjectInstallAction DefaultUnInstall 128 ");
	TCHAR infpath[MAX_PATH] = {0};

	m_infpath.GetWindowText(infpath, MAX_PATH);
	lstrcat(parameters, infpath);
	if (!executeApp(TEXT("rundll32.exe"), parameters))
	{
		MessageBox(TEXT("Failed to uninstall INF file"), TEXT("Error"), MB_OK);
		return -1;
	}

	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	TCHAR infpath[MAX_PATH] = {0};
	TCHAR filename[MAX_PATH] = {0};
	TCHAR parameters[MAX_PATH] = TEXT("load ");

	m_infpath.GetWindowText(infpath, MAX_PATH);
	_tsplitpath_s(infpath, NULL, 0, NULL, 0, filename, MAX_PATH, NULL, 0);
	lstrcat(parameters, filename);
	if (!executeApp(TEXT("fltmc"), parameters))
	{
		MessageBox(TEXT("Failed to start service"), TEXT("Error"), MB_OK);
		return -1;
	}
	return 0;
}


LRESULT CMainDlg::OnBnClickedButtonStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	TCHAR infpath[MAX_PATH] = {0};
	TCHAR filename[MAX_PATH] = {0};
	TCHAR parameters[MAX_PATH] = TEXT("unload ");

	m_infpath.GetWindowText(infpath, MAX_PATH);
	_tsplitpath_s(infpath, NULL, 0, NULL, 0, filename, MAX_PATH, NULL, 0);
	lstrcat(parameters, filename);
	if (!executeApp(TEXT("fltmc"), parameters))
	{
		MessageBox(TEXT("Failed to start service"), TEXT("Error"), MB_OK);
		return -1;
	}
	return 0;
}


LRESULT CMainDlg::OnEnChangeEditInfPath(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	TCHAR infpath[MAX_PATH] = {0};
	m_infpath.GetWindowText(infpath, MAX_PATH);
	if (lstrlen(infpath) > 0)
	{
		m_install.EnableWindow();
		m_start.EnableWindow();
		m_uninstall.EnableWindow();
		m_stop.EnableWindow();
	}
	else
	{
		m_install.EnableWindow(FALSE);
		m_start.EnableWindow(FALSE);
		m_uninstall.EnableWindow(FALSE);
		m_stop.EnableWindow(FALSE);
	}
	return 0;
}
