#include "XULWin/ListItemImpl.h"
#include "XULWin/Decorator.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/ListBoxImpl.h"
#include "XULWin/ListViewImpl.h"
#include "XULWin/ListCell.h"
#include "XULWin/ListCellImpl.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    ListItemImpl::ListItemImpl(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping),
        mSelected(false)
    {
    }
        
        
    bool ListItemImpl::initImpl()
    {
        // XUL hierarchy:
        // listbox/listitem/listcell
        //    ^---> listbox here has ListViewImpl object (not ListBoxImpl)
        if (ListViewImpl * listView = parent()->downcast<ListViewImpl>())
        {
            LVITEM lvItem;
            lvItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
            lvItem.state = 0; 
            lvItem.stateMask = 0; 
            lvItem.iSubItem = 0;
            lvItem.lParam = (LPARAM)0;
            std::vector<ListCell*> listCells;
            el()->getElementsByType<ListCell>(listCells);
            
            if (!listCells.empty())
            {    
                static std::vector<std::wstring> labels;            
                labels.clear();

                for (size_t idx = 0; idx != listCells.size(); ++idx)
                {
                    labels.push_back(ToUTF16(listCells[idx]->impl()->downcast<ListCellImpl>()->getLabel()));
                }
                lvItem.lParam = (LPARAM)labels[0].c_str();
                lvItem.iItem = getIndex();
                lvItem.pszText = LPSTR_TEXTCALLBACK;
                if (-1 == ListView_InsertItem(listView->handle(), &lvItem))
                {
                    ReportError("Inserting item into list view failed. Reason: " + Windows::getLastError(::GetLastError()));
                }
            }
        }
        return Super::initImpl();
    }


    std::string ListItemImpl::getLabel() const
    {
        return mLabel;
    }

    
    void ListItemImpl::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }


    bool ListItemImpl::isSelected() const
    {
        return mSelected;
    }


    void ListItemImpl::setSelected(bool inSelected)
    {
        mSelected = inSelected;
    }


    bool ListItemImpl::initAttributeControllers()
    {
        setAttributeController("label", static_cast<LabelController*>(this));
        setAttributeController("selected", static_cast<SelectedController*>(this));
        return Super::initAttributeControllers();
    }


    int ListItemImpl::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        static int cMargin = 4;
        int result = 0; 
        if (ListBoxImpl * listBox = parent()->downcast<ListBoxImpl>())
        {
            int width = Windows::getTextSize(listBox->handle(), getLabel()).cx;
            int extraWidth = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(listBox->handle()).cx;
            result = width + extraWidth + cMargin;
        }
        return result;
    }


    int ListItemImpl::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        if (ListBoxImpl * listBox = parent()->downcast<ListBoxImpl>())
        {
            RECT rect;
            Windows::getListBoxItemRect(listBox->handle(), Windows::getListBoxIndexOf(listBox->handle(), getLabel()), rect);
            return rect.bottom - rect.top;
        }
        return 0;
    }
    
} // namespace XULWin
