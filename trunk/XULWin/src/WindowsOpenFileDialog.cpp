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
    
    unsigned int CALLBACK OpenFileDialogHook(HWND hWnd, UINT inMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (inMsg)
        {
	        case WM_INITDIALOG:
	        {
		        // You need to use a copy of the OPENFILENAME struct used to
		        // create this dialog. You can store a pointer to the
		        // OPENFILENAME struct in the ofn.lCustData so you can
		        // retrieve it here in the lParam. Once you have it, you
		        // need to hang on to it. Using window properties provides a
		        // good thread safe solution to using a global variable.
		        if(!SetProp(::GetParent(hWnd), _T("OFN"), (void *) lParam))
		        {
                    XULWin::ReportError("SetProp failed. Last error: " + XULWin::WinAPI::getLastError(::GetLastError()));
		        }
		        return 0;
	        }
	        case WM_NOTIFY:
	        {
		        // The OFNOTIFY struct is passed in the lParam of this message.
		        LPOFNOTIFY ofNotify = (LPOFNOTIFY) lParam;
		        switch (ofNotify->hdr.code)
		        {
			        case CDN_SELCHANGE:
			        {				        				        
                        XULWin::UInt32 bufferSize = CommDlg_OpenSave_GetSpec(::GetParent(hWnd), NULL, 0) + MAX_PATH;

				        // The OFN struct is stored in a property of dialog window
                        LPOPENFILENAME ofNotify = (LPOPENFILENAME)::GetProp(::GetParent(hWnd), TEXT("OFN"));

				        if (ofNotify->nMaxFile < static_cast<DWORD>(bufferSize))
				        {
                            static LPTSTR buffer(0);

					        // Free the previously allocated buffer.
					        if(buffer)
					        {
						        HeapFree(GetProcessHeap(), 0, buffer);
                                buffer = 0;
					        }

					        // Allocate a new buffer
					        buffer = (LPTSTR) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bufferSize);
					        if (buffer)
					        {
						        ofNotify->lpstrFile = buffer;
						        ofNotify->nMaxFile = bufferSize;
					        }
				        }
				        break;
			        }
		        }
		        break;
	        }
	        case WM_DESTROY:
	        {
		        // Also need to free the property with the OPENFILENAME struct.
		        ::RemoveProp(::GetParent(hWnd), TEXT("OFN"));
		        break;
            }
        }
        return 0;
    }

} // extern 'C'


namespace XULWin
{

    namespace WinAPI
    {

        const std::string & GetImagesFilter()
        {
            static const std::string cFilter = 
                "All supported\0"
                "*.jpg;*.jpeg;*.bmp;*.png;*.gif;*.tif;*.tiff\0"
                "JPG\0*.jpg;*.jpeg\0"
                "BMP\0*.bmp\0"
                "PNG\0*.png\0"
                "GIF\0*.gif\0"
                "TIFF\0*.tif;*.tiff\0";
            return cFilter;
        }


        void ChooseFile(HWND mParent,
                        const std::string & inDialogTitle,
                        const std::string & inFilter,
                        bool inAllowMultiSelect,
                        std::vector<std::string> & outSelectedFiles)
        {
	        OPENFILENAME openFileName;
            ::ZeroMemory(&openFileName, sizeof(openFileName));
	        openFileName.lStructSize = sizeof(openFileName);
	        openFileName.hwndOwner = mParent;
        	
            std::wstring utf16Filter = ToUTF16(inFilter);
            openFileName.lpstrFilter = utf16Filter.c_str();
	        openFileName.nFilterIndex = 0;
	        openFileName.lpstrFile = 0;
	        openFileName.nMaxFile = MAX_PATH;
	        openFileName.Flags = OFN_ENABLEHOOK | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
            if (inAllowMultiSelect)
            {
                openFileName.Flags |= OFN_ALLOWMULTISELECT;
            }
	        openFileName.lpfnHook = OpenFileDialogHook;
        	
            if(::GetOpenFileName(&openFileName))
	        {
		        // Split the string into filenames
		        for (WCHAR * offset = openFileName.lpstrFile + openFileName.nFileOffset;
			         *offset;
			         offset += (wcslen(offset) + 1))
		        {
			        if(openFileName.lpstrFile[wcslen(openFileName.lpstrFile) + 4] == L'\0')
			        {
				        outSelectedFiles.push_back(ToUTF8(openFileName.lpstrFile));
			        }
			        else
			        {
				        size_t dwLen = wcslen(openFileName.lpstrFile) + wcslen(offset) + 2;
				        WCHAR * buffer = new WCHAR[dwLen];
				        wcscpy_s(buffer, dwLen, openFileName.lpstrFile);
				        wcscat_s(buffer, dwLen, L"\\");
				        wcscat_s(buffer, dwLen, offset);
				        outSelectedFiles.push_back(ToUTF8(buffer));
				        delete buffer;
			        }
		        }
	        }
	        else
	        {
		        DWORD errorCode = CommDlgExtendedError();
                ReportError("Failed call to GetOpenFileName with error: " + WinAPI::getLastError(errorCode));
	        }
        }


    } // namespace WinAPI

} // namespace XULWin
