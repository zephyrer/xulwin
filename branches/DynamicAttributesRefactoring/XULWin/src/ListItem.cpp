#include "XULWin/ListItem.h"
#include "XULWin/Decorator.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/ListBox.h"
#include "XULWin/ListView.h"
#include "XULWin/ListCellElement.h"
#include "XULWin/ListCell.h"
#include "XULWin/Unicode.h"
#include "XULWin/WinUtils.h"


namespace XULWin
{

    ListItem::ListItem(Component * inParent, const AttributesMapping & inAttributesMapping) :
        PassiveComponent(inParent, inAttributesMapping),
        mSelected(false)
    {
    }
        
        
    bool ListItem::init()
    {
        // XUL hierarchy:
        // listbox/listitem/listcell
        //    ^---> listbox here has ListView object (not ListBox)
        if (ListView * listView = parent()->downcast<ListView>())
        {
            LVITEM lvItem;
            lvItem.mask = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
            lvItem.state = 0; 
            lvItem.stateMask = 0; 
            lvItem.iSubItem = 0;
            lvItem.lParam = (LPARAM)0;
            std::vector<ListCellElement*> listCells;
            el()->getElementsByType<ListCellElement>(listCells);
            
            if (!listCells.empty())
            {    
                static std::vector<std::wstring> labels;            
                labels.clear();

                for (size_t idx = 0; idx != listCells.size(); ++idx)
                {
                    labels.push_back(ToUTF16(listCells[idx]->component()->downcast<ListCell>()->getLabel()));
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
        return Super::init();
    }


    std::string ListItem::getLabel() const
    {
        return mLabel;
    }

    
    void ListItem::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }


    bool ListItem::isSelected() const
    {
        return mSelected;
    }


    void ListItem::setSelected(bool inSelected)
    {
        mSelected = inSelected;
    }


    bool ListItem::initAttributeControllers()
    {
        setAttributeController("label", static_cast<LabelController*>(this));
        setAttributeController("selected", static_cast<SelectedController*>(this));
        return Super::initAttributeControllers();
    }


    int ListItem::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        static int cMargin = 4;
        int result = 0; 
        if (ListBox * listBox = parent()->downcast<ListBox>())
        {
            int width = Windows::getTextSize(listBox->handle(), getLabel()).cx;
            int extraWidth = Windows::getSizeDifferenceBetweenWindowRectAndClientRect(listBox->handle()).cx;
            result = width + extraWidth + cMargin;
        }
        return result;
    }


    int ListItem::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        if (ListBox * listBox = parent()->downcast<ListBox>())
        {
            RECT rect;
            Windows::getListBoxItemRect(listBox->handle(), Windows::getListBoxIndexOf(listBox->handle(), getLabel()), rect);
            return rect.bottom - rect.top;
        }
        return 0;
    }
    
} // namespace XULWin
