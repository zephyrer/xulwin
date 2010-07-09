#ifndef WINDOWSOPENFILEDIALOG_H_INCLUDED
#define WINDOWSOPENFILEDIALOG_H_INCLUDED


#include "XULWin/Windows.h"
#include <string>
#include <vector>


namespace XULWin
{

    namespace WinAPI
    {

        const std::string & GetImagesFilter();

        /**
         * Opens a file chooser dialog.
         *
         * Example filter:
         *   "All supported\0"
         *   "*.jpg;*.jpeg;*.bmp;*.png;*.gif;*.tif;*.tiff;*.3gp;*.avi;*.divx;*.flv;*.mov;*.mpeg;*.mpg;*.mp4;*.rm;*.wmv\0"
         *   "JPG\0*.jpg;*.jpeg\0"
         *   "BMP\0*.bmp\0"
         *   "PNG\0*.png\0"
         *   "GIF\0*.gif\0"
         *   "TIFF\0*.tif;*.tiff\0"
         *   "3GP\0*.3gp;*.3GP\0"
         *   "AVI\0*.avi;*.AVI\0"
         *   "DivX\0*.divx;*.DIVX\0"
         *   "FLV\0*.flv;*.FLV\0"
         *   "MOV\0*.mov;*.MOV\0"
         *   "MPEG\0*.mpg;*.MPG;*.mpeg;*.MPEG;*.mp4;*.MP4\0"
         *   "RM\0*.rm;*.RM\0"
         *   "WMV\0*.wmv;*.WMV\0";
         */
        void ChooseFile(HWND mParent,
                        const std::string & inDialogTitle,
                        const std::string & inFilter,
                        bool inAllowMultiSelect,
                        std::vector<std::string> & outSelectedFiles);

    } // namespace WinAPI

} // namespace XULWin


#endif // WINDOWSOPENFILEDIALOG_H_INCLUDED
