#include "XULWin/WindowsOpenFileDialog.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"
#include "XULWin/Types.h"
#include <assert.h>
#include <shlobj.h>
#include <tchar.h>


extern "C"
{
    
    unsigned int CALLBACK DialogHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        //static HWND hwndParentDialog;
        LPOFNOTIFY lpofn;
        int cbLength;
        static LPTSTR lpsz;
        //static int LastLen;

        switch (uMsg)
        {
            case WM_INITDIALOG:
            {
                // You need to use a copy of the OPENFILENAME struct used to
                // create this dialog. You can store a pointer to the
                // OPENFILENAME struct in the ofn.lCustData so you can
                // retrieve it here in the lParam. Once you have it, you
                // need to hang on to it. Using window properties provides a
                // good thread safe solution to using a global variable.
                if(!SetProp(GetParent(hwnd), _T("OFN"), (void *) lParam))
                {
                    MessageBox(NULL, _T("SET PRop Failed"), _T("ERROR"), MB_OK);
                }
                return (0);
            }
            case WM_COMMAND:
            {
                OutputDebugString(_T("command\n"));
                break;
            }
            case WM_NOTIFY:
            {
                // The OFNOTIFY struct is passed in the lParam of this
                // message.
                lpofn = (LPOFNOTIFY) lParam;

                switch (lpofn->hdr.code)
                {
                    case CDN_SELCHANGE:
                    {
                        LPOPENFILENAME lpofn;
                        cbLength = CommDlg_OpenSave_GetSpec(GetParent(hwnd), NULL, 0);

                        cbLength += _MAX_PATH;

                        // The OFN struct is stored in a property of dialog window
                        lpofn = (LPOPENFILENAME) GetProp(GetParent(hwnd), _T("OFN"));

                        if (int(lpofn->nMaxFile) < cbLength)
                        {
                            // Free the previously allocated buffer.
                            if(lpsz)
                            {
                                HeapFree(GetProcessHeap(), 0, lpsz);
                            }
                            // Allocate a new buffer
                            lpsz = (LPTSTR) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbLength*sizeof(_T('\0')));
                            if (lpsz)
                            {
                                lpofn->lpstrFile = lpsz;
                                lpofn->nMaxFile  = cbLength;
                            }
                        }
                        break;
                    }
                }
                break;
            }
            case WM_DESTROY:
            {
                // Also need to free the property with the OPENFILENAME
                // struct.
                RemoveProp(GetParent(hwnd), _T("OFN"));
                break;
             }
        }
        return (0);
    }

} // extern "C"



namespace XULWin
{

    namespace WinAPI
    {

        LPCWSTR GetImagesFilter()
        {
            return L"All supported\0"
                   L"*.jpg;*.jpeg;*.bmp;*.png;*.gif;*.tif;*.tiff\0"
                   L"JPG\0*.jpg;*.jpeg\0"
                   L"BMP\0*.bmp\0"
                   L"PNG\0*.png\0"
                   L"GIF\0*.gif\0"
                   L"TIFF\0*.tif;*.tiff\0";
        }


        void ChooseFile(HWND inParentWindow,
                        const std::string & inDialogTitle,
                        LPCWSTR inFilter,
                        bool inAllowMultiSelect,
                        std::vector<std::string> & outSelectedFiles)
        {
            // Allocate a new buffer
            OPENFILENAME openFileName;
            ZeroMemory(&openFileName, sizeof(openFileName));
            openFileName.lStructSize = sizeof(openFileName);
            openFileName.hwndOwner = inParentWindow;
            openFileName.lpstrFilter = inFilter;
            openFileName.nFilterIndex = 0;
            openFileName.lpstrFile = 0;
            openFileName.nMaxFile = MAX_PATH;
            openFileName.Flags = OFN_ENABLEHOOK | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
            if (inAllowMultiSelect)
            {
                openFileName.Flags |= OFN_ALLOWMULTISELECT;
            }
            openFileName.lpfnHook = DialogHook;
            
            if(GetOpenFileName(&openFileName))
            {
                // Split the string into filenames
                for (WCHAR * offset = openFileName.lpstrFile + openFileName.nFileOffset;
                     *offset;
                     offset += (wcslen(offset) + 1))
                {
                    if(openFileName.lpstrFile[ wcslen(openFileName.lpstrFile) + 4 ] == L'\0')
                    {
                        outSelectedFiles.push_back(ToUTF8(openFileName.lpstrFile));
                    }
                    else
                    {
                        size_t bufferLength = wcslen(openFileName.lpstrFile) + wcslen(offset) + 2;
                        WCHAR * buffer = new WCHAR[bufferLength];
                        wcscpy_s(buffer, bufferLength, openFileName.lpstrFile);
                        wcscat_s(buffer, bufferLength, L"\\");
                        wcscat_s(buffer, bufferLength, offset);
                        outSelectedFiles.push_back(ToUTF8(buffer));
                        delete buffer;
                    }
                }
            }
            else
            {
                ReportError("::GetOpenFileName failed with error: " + WinAPI::System_GetLastError(CommDlgExtendedError()));
            }

        }

    } // namespace WinAPI

} // namespace XULWin
