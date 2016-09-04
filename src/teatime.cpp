/*
 * Teatime, the custom filetime application.
 * A user can select a file and change its creation, last access and last write
 * time.
 */
#include <Windows.h>
#include <string>
#include "resource.h"


/* Global file handle of the user selected file */
HANDLE hFile = NULL;


/*
 * Creates a file browser dialog.
 */
BOOL SelectFile(HWND hWnd, wchar_t *szFilename)
{
    /* Superseded by Common Item Dialog */
	OPENFILENAME ofn;

    /* Fill allocated Memory with zeros to avoid side-effects */
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFilename;
	ofn.nMaxFile = MAX_PATH;

    /* Creates an Explorer-styled window showing existing modifiable files */
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

	return GetOpenFileName(&ofn);
}

/*
 * Gets the creation, last access and last write time of a specified file.
 */
void GetFT(HANDLE hFile, HWND hDlg)
{
	wchar_t creation[256], access[256], write[256];
	SYSTEMTIME creationST, accessST, writeST;
	FILETIME creationFT, accessFT, writeFT;

    /* Get the filetime */
	GetFileTime(hFile, &creationFT, &accessFT, &writeFT);

    /* Convert to readable timeformat */
	FileTimeToSystemTime(&creationFT, &creationST);
	FileTimeToSystemTime(&accessFT, &accessST);
	FileTimeToSystemTime(&writeFT, &writeST);

    /* Write the creation time to buffer */
	wsprintf(creation, L"%02d:%02d:%02d %02d.%02d.%d", creationST.wHour,
		creationST.wMinute, creationST.wSecond, creationST.wDay,
		creationST.wMonth, creationST.wYear);

    /* Write the last access time to buffer */
	wsprintf(access, L"%02d:%02d:%02d %02d.%02d.%d", accessST.wHour,
		accessST.wMinute, accessST.wSecond, accessST.wDay, accessST.wMonth,
		accessST.wYear);

    /* Write the last write time to buffer*/
	wsprintf(write, L"%02d:%02d:%02d %02d.%02d.%d", writeST.wHour,
		writeST.wMinute, writeST.wSecond, writeST.wDay, writeST.wMonth,
		writeST.wYear);

    /* Display the three filetimes */
	SetDlgItemText(hDlg, IDC_EDIT2, creation);
	SetDlgItemText(hDlg, IDC_EDIT3, access);
	SetDlgItemText(hDlg, IDC_EDIT4, write);
}

/*
 * Sets the custom filetime.
 */
bool SetFT(HANDLE hFile, HWND hDlg)
{
    bool success = TRUE;
    wchar_t creation[256], access[256], write[256], *cPos = 0, *aPos = 0,
        *wPos = 0, *cHour, *cMinute, *cSecond, *cDay, *cMonth, *cYear, *aHour,
        *aMinute, *aSecond, *aDay, *aMonth, *aYear, *wHour, *wMinute, *wSecond,
        *wDay, *wMonth, *wYear;
    SYSTEMTIME creationST, accessST, writeST;
	FILETIME creationFT, accessFT, writeFT;

    /* Sets the creation time for a valid user input */
    if (GetDlgItemText(hDlg, IDC_EDIT5, creation, 128))
    {
        cHour = wcstok_s(creation, L" :./-", &cPos);
        cMinute = wcstok_s(NULL, L" :./-", &cPos);
        cSecond = wcstok_s(NULL, L" :./-", &cPos);
        cDay = wcstok_s(NULL, L" :./-", &cPos);
        cMonth = wcstok_s(NULL, L" :./-", &cPos);
        cYear = wcstok_s(NULL, L" :./-", &cPos);

        creationST.wHour = _wtoi(cHour);
        creationST.wMinute = _wtoi(cMinute);
        creationST.wSecond = _wtoi(cSecond);
        creationST.wMilliseconds = 0;
        creationST.wDay = _wtoi(cDay);
        creationST.wMonth = _wtoi(cMonth);
        creationST.wYear = _wtoi(cYear);

        if (!SystemTimeToFileTime(&creationST, &creationFT))
        {
            MessageBox(hDlg, L"Couldn't convert creation time", L"Error", NULL);
            success = FALSE;
        }
        if (!SetFileTime(hFile, &creationFT, NULL, NULL))
        {
            MessageBox(hDlg, L"Couldn't set file time", L"Error", NULL);
            success = FALSE;
        }
    }

    /* Sets the last access time for a valid user input */
    if (GetDlgItemText(hDlg, IDC_EDIT6, access, 128))
    {
        aHour = wcstok_s(access, L" :./-", &aPos);
        aMinute = wcstok_s(NULL, L" :./-", &aPos);
        aSecond = wcstok_s(NULL, L" :./-", &aPos);
        aDay = wcstok_s(NULL, L" :./-", &aPos);
        aMonth = wcstok_s(NULL, L" :./-", &aPos);
        aYear = wcstok_s(NULL, L" :./-", &aPos);

        accessST.wHour = _wtoi(aHour);
        accessST.wMinute = _wtoi(aMinute);
        accessST.wSecond = _wtoi(aSecond);
        accessST.wMilliseconds = 0;
        accessST.wDay = _wtoi(aDay);
        accessST.wMonth = _wtoi(aMonth);
        accessST.wYear = _wtoi(aYear);

        if (!SystemTimeToFileTime(&accessST, &accessFT))
        {
            MessageBox(hDlg, L"Couldn't convert access time", L"Error", NULL);
            success = FALSE;
        }
        if (!SetFileTime(hFile, NULL, &accessFT, NULL))
        {
            MessageBox(hDlg, L"Couldn't set file time", L"Error", NULL);
            success = FALSE;
        }
    }

    /* Sets the last write time for a valid user input */
    if (GetDlgItemText(hDlg, IDC_EDIT7, write, 128))
    {
        wHour = wcstok_s(write, L" :./-", &wPos);
        wMinute = wcstok_s(NULL, L" :./-", &wPos);
        wSecond = wcstok_s(NULL, L" :./-", &wPos);
        wDay = wcstok_s(NULL, L" :./-", &wPos);
        wMonth = wcstok_s(NULL, L" :./-", &wPos);
        wYear = wcstok_s(NULL, L" :./-", &wPos);

        writeST.wHour = _wtoi(wHour);
        writeST.wMinute = _wtoi(wMinute);
        writeST.wSecond = _wtoi(wSecond);
        writeST.wMilliseconds = 0;
        writeST.wDay = _wtoi(wDay);
        writeST.wMonth = _wtoi(wMonth);
        writeST.wYear = _wtoi(wYear);

        if (!SystemTimeToFileTime(&writeST, &writeFT))
        {
            MessageBox(hDlg, L"Couldn't convert write time", L"Error", NULL);
            success = FALSE;
        }
        if (!SetFileTime(hFile, NULL, NULL, &writeFT))
        {
            MessageBox(hDlg, L"Couldn't set file time", L"Error", NULL);
            success = FALSE;
        }
    }
    return success;
}

/*
 * Processing messages to the dialog.
 * Returns TRUE if message could be processed, otherwise FALSE.
 */
BOOL CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    /* FALSE passes message to dialog, TRUE discards */
    bool fail = FALSE;

	switch (msg)
    {
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
                /* File selection button */
				case IDC_FILE:
				{
                    /* Stores the filename */
					wchar_t filename[MAX_PATH] = L"";

                    /* Create a filebrowser dialog */
					if (!SelectFile(hWnd, filename))
                    {
                        MessageBox(hWnd,
                                   L"Could not select file!",
                                   L"Error",
                                   NULL);
                        fail = TRUE;
                    }

                    /* Display the filename and its pathname */
					SetDlgItemText(hWnd, IDC_EDIT1, filename);

                    /* Create a handle of the file */
                    if (!(hFile = CreateFile(filename,
                                       FILE_WRITE_ATTRIBUTES,
                                       0,
                                       NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL,
                                       NULL)))
                    {
                        MessageBox(hWnd,
                                   L"Could not create filehandle!",
                                   L"Error",
                                   NULL);
                        fail = TRUE;
                    }

                    /* Get the specific filetime */
                    GetFT(hFile, hWnd);
					break;
				}
                /* Save button */
                case IDC_SAVE:
                    /* Save the user selected filetime */
                  	fail = !SetFT(hFile, hWnd);
                    break;
                /* Reload button */
                case IDC_RELOAD:
                    /* Get the filetime for reloading */
                    GetFT(hFile, hWnd);
                    break;
			}
			break;
		case WM_CLOSE:
			EndDialog(hWnd, 0);
			break;
	}
	return fail;
}

/*
 * Creation of the Dialogbox.
 */
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);
}
