#ifndef WINDOWSOPENFILEDIALOG_H_INCLUDED
#define WINDOWSOPENFILEDIALOG_H_INCLUDED


#include "XULWin/Windows.h"
#include <string>
#include <vector>


namespace XULWin
{

    namespace WinAPI
    {

        /**
         * Filter for image files (jpg, jpeg, bmp, png, gif, tif, tiff)
         */
        LPCWSTR GetImagesFilter();

        /**
         * Opens a file chooser dialog.
         *
         * Example filter:
         *   L"All supported\0"
         *   L"*.jpg;*.jpeg;*.bmp;*.png;*.gif;*.tif;*.tiff;*.3gp;*.avi;*.divx;*.flv;*.mov;*.mpeg;*.mpg;*.mp4;*.rm;*.wmv\0"
         *   L"JPG\0*.jpg;*.jpeg\0"
         *   L"BMP\0*.bmp\0"
         *   L"PNG\0*.png\0"
         *   L"GIF\0*.gif\0"
         *   L"TIFF\0*.tif;*.tiff\0"
         *   L"3GP\0*.3gp;*.3GP\0"
         *   L"AVI\0*.avi;*.AVI\0"
         *   L"DivX\0*.divx;*.DIVX\0"
         *   L"FLV\0*.flv;*.FLV\0"
         *   L"MOV\0*.mov;*.MOV\0"
         *   L"MPEG\0*.mpg;*.MPG;*.mpeg;*.MPEG;*.mp4;*.MP4\0"
         *   L"RM\0*.rm;*.RM\0"
         *   L"WMV\0*.wmv;*.WMV\0";
         */
        void ChooseFile(HWND mParent,
                        const std::string & inDialogTitle,
                        LPCWSTR inFilter,
                        bool inAllowMultiSelect,
                        std::vector<std::string> & outSelectedFiles);

    } // namespace WinAPI

} // namespace XULWin


#endif // WINDOWSOPENFILEDIALOG_H_INCLUDED
