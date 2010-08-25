#include "XULWin/Components.h"
#include "XULWin/Algorithms.h"
#include "XULWin/Conversions.h"
#include "XULWin/Decorator.h"
#include "XULWin/Decorators.h"
#include "XULWin/Defaults.h"
#include "XULWin/Element.h"
#include "XULWin/Elements.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/Layout.h"
#include "XULWin/Unicode.h"
#include "XULWin/Window.h"
#include "XULWin/WinUtils.h"
#include "Poco/String.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>


namespace XULWin
{

    Component * CreateLabel(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new MarginDecorator(new Label(inComponent, inAttr));
    }


    Label::Label(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent,
                      inAttr,
                      TEXT("STATIC"),
                      0, // exStyle
                      SS_LEFT)
    {
    }


    std::string Label::getValue() const
    {
        return WinAPI::getWindowText(handle());
    }


    void Label::setValue(const std::string & inStringValue)
    {
        WinAPI::setWindowText(handle(), inStringValue);
    }


    CSSTextAlign Label::getCSSTextAlign() const
    {
        LONG styles = WinAPI::getWindowStyles(handle());
        if (styles & SS_LEFT)
        {
            return CSSTextAlign_Left;
        }
        else if (styles & SS_CENTER)
        {
            return CSSTextAlign_Center;
        }
        else if (styles & SS_RIGHT)
        {
            return CSSTextAlign_Right;
        }
        else
        {
            return CSSTextAlign_Left;
        }
    }


    void Label::setCSSTextAlign(CSSTextAlign inValue)
    {
        LONG styles = WinAPI::getWindowStyles(handle());
        styles &= ~SS_LEFT;
        styles &= ~SS_CENTER;
        styles &= ~SS_RIGHT;

        switch (inValue)
        {
            case CSSTextAlign_Left:
            {
                styles |= SS_LEFT;
                break;
            }
            case CSSTextAlign_Center:
            {
                styles |= SS_CENTER;
                break;
            }
            case CSSTextAlign_Right:
            {
                styles |= SS_RIGHT;
                break;
            }
            //case CSSTextAlign_Justify:
            //{
            //    styles |= 0;
            //    break;
            //}
        }
        WinAPI::setWindowStyle(handle(), styles);
    }


    bool Label::initAttributeControllers()
    {
        setAttributeController<StringValueController>(this);
        return Super::initAttributeControllers();
    }


    bool Label::initStyleControllers()
    {
        setStyleController<CSSTextAlignController>(this);
        return Super::initStyleControllers();
    }


    int Label::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        std::string text = WinAPI::getWindowText(handle());
        int width = WinAPI::getTextSize(handle(), text).cx;
        return width;
    }


    int Label::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return WinAPI::getTextSize(handle(), WinAPI::getWindowText(handle())).cy;
    }


    Component * CreateButton(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new MarginDecorator(new Button(inComponent, inAttr));
    }


    Button::Button(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent,
                      inAttr,
                      TEXT("BUTTON"),
                      0, // exStyle
                      WS_TABSTOP | BS_PUSHBUTTON)
    {
    }


    int Button::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::buttonHeight();
    }


    int Button::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        std::string text = WinAPI::getWindowText(handle());
        int minWidth = WinAPI::getTextSize(handle(), text).cx;
        minWidth += Defaults::textPadding();
        return std::max<int>(minWidth, Defaults::buttonWidth());
    }


    Component * CreateCheckBox(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new MarginDecorator(new CheckBox(inComponent, inAttr));
    }


    CheckBox::CheckBox(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent,
                      inAttr,
                      TEXT("BUTTON"),
                      0,
                      WS_TABSTOP | BS_AUTOCHECKBOX)
    {
    }


    int CheckBox::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::controlHeight();
    }


    int CheckBox::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::checkBoxMinimumWidth() + WinAPI::getTextSize(handle(), WinAPI::getWindowText(handle())).cx;
    }


    bool CheckBox::isChecked() const
    {
        return WinAPI::isCheckBoxChecked(handle());
    }


    void CheckBox::setChecked(bool inChecked)
    {
        WinAPI::setCheckBoxChecked(handle(), inChecked);
    }


    bool CheckBox::initAttributeControllers()
    {
        setAttributeController<CheckedController>(this);
        return Super::initAttributeControllers();
    }


    Description::Description(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent,
                      inAttr,
                      TEXT("STATIC"),
                      0, // exStyle
                      SS_LEFT)
    {
    }


    std::string Description::getValue() const
    {
        return WinAPI::getWindowText(handle());
    }


    void Description::setValue(const std::string & inStringValue)
    {
        WinAPI::setWindowText(handle(), inStringValue);
    }


    bool Description::initAttributeControllers()
    {
        setAttributeController<StringValueController>(this);
        return Super::initAttributeControllers();
    }


    int Description::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::textPadding();
    }


    int Description::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return WinAPI::getMultilineTextHeight(handle());
    }
    
    Component * CreateTextBox(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new MarginDecorator(new TextBox(inComponent, inAttr));
    }


    TextBox::TextBox(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent,
                      inAttr,
                      TEXT("EDIT"),
                      WS_EX_CLIENTEDGE, // exStyle
                      WS_TABSTOP | GetFlags(inAttr)),
        mRows(1)
    {
    }


    DWORD TextBox::GetFlags(const AttributesMapping & inAttr)
    {
        DWORD flags = 0;
        AttributesMapping::const_iterator it = inAttr.find("type");
        if (it != inAttr.end() && it->second == "password")
        {
            flags |= ES_PASSWORD;
        }
        it = inAttr.find("multiline");
        if (it != inAttr.end() && it->second == "true")
        {
            flags |= WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;
        }
        else
        {
            flags |= ES_AUTOHSCROLL;
        }
        return flags;
    }


    std::string TextBox::getValue() const
    {
        return WinAPI::getWindowText(handle());
    }


    void TextBox::setValue(const std::string & inStringValue)
    {
        WinAPI::setWindowText(handle(), inStringValue);
    }


    bool TextBox::isReadOnly() const
    {
        return WinAPI::isTextBoxReadOnly(handle());
    }


    void TextBox::setReadOnly(bool inReadOnly)
    {
        WinAPI::setTextBoxReadOnly(handle(), inReadOnly);
    }


    int TextBox::getRows() const
    {
        return mRows;
    }


    void TextBox::setRows(int inRows)
    {
        mRows = inRows;
    }


    bool TextBox::initAttributeControllers()
    {
        setAttributeController<StringValueController>(this);
        setAttributeController<ReadOnlyController>(this);
        setAttributeController<RowsController>(this);
        return Super::initAttributeControllers();
    }


    int TextBox::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::textBoxWidth();;
    }


    int TextBox::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::controlHeight() * getRows();
    }


    LRESULT TextBox::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        if (inMessage == WM_KEYDOWN)
        {
            if (wParam == VK_TAB)
            {
                //
                // Tabbing doesn't work on multiline textboxes.
                // This code fixes that.
                //
                long style = ::GetWindowLong(handle(), GWL_STYLE);
                if ((style & ES_MULTILINE) && (style & WS_TABSTOP))
                {
                    BOOL shift = 0x8000 & GetKeyState(VK_SHIFT);
                    ::SetFocus(::GetNextDlgTabItem(GetParent(handle()), handle(), shift));
                    return 0;
                }
            }
            else
            {
                //
                // CTRL-a select all
                //
                const int cKeyboard_a = 65;
                if (HIWORD(::GetKeyState(VK_CONTROL)) && wParam == cKeyboard_a)
                {
                    ::SendMessage(handle(), EM_SETSEL, 0, -1);
                }
            }
        }
        return Super::handleMessage(inMessage, wParam, lParam);
    }
    
    Component * CreateScrollbar(Component * inComponent, const AttributesMapping & inAttr)
    {
        return new Decorator(new Scrollbar(inComponent, inAttr));
    }


    DWORD Scrollbar::GetFlags(const AttributesMapping & inAttributesMapping)
    {
        DWORD flags = 0;
        AttributesMapping::const_iterator it = inAttributesMapping.find("orient");
        if (it != inAttributesMapping.end())
        {
            const std::string & orient = it->second;
            if (orient == "horizontal")
            {
                flags |= SBS_HORZ | SBS_RIGHTALIGN;
            }
            else if (orient == "vertical")
            {
                flags |= SBS_VERT | SBS_BOTTOMALIGN;
            }
            else
            {
                ReportError("Invalid orient found for scrollbar!");
            }
        }
        return flags;
    }


    Scrollbar::Scrollbar(Component * inParent, const AttributesMapping & inAttributesMapping) :
        NativeControl(inParent, inAttributesMapping,
                      TEXT("SCROLLBAR"),
                      0, // exStyle
                      WS_TABSTOP | GetFlags(inAttributesMapping)),
        mEventListener(0),
        mIncrement(0)
    {
        mExpansive = true;
        WinAPI::setScrollInfo(handle(), 100, 10, 0);
    }


    int Scrollbar::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::scrollbarWidth();
    }


    int Scrollbar::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::scrollbarWidth();
    }


    LRESULT Scrollbar::handleMessage(UINT inMessage, WPARAM wParam, LPARAM lParam)
    {
        if (WM_VSCROLL == inMessage || WM_HSCROLL == inMessage)
        {
            int totalHeight = 0;
            int pageHeight = 0;
            int currentPosition = 0;
            WinAPI::getScrollInfo(handle(), totalHeight, pageHeight, currentPosition);
            switch (LOWORD(wParam))
            {
                case SB_LINEUP: // user clicked the top arrow
                {
                    currentPosition -= 1;
                    break;
                }
                case SB_LINEDOWN: // user clicked the bottom arrow
                {
                    currentPosition += 1;
                    break;
                }
                case SB_PAGEUP: // user clicked the scroll bar shaft above the scroll box
                {
                    currentPosition -= pageHeight;
                    break;
                }
                case SB_PAGEDOWN: // user clicked the scroll bar shaft below the scroll box
                {
                    currentPosition += pageHeight;
                    break;
                }
                case SB_THUMBTRACK: // user dragged the scroll box
                {
                    currentPosition = HIWORD(wParam);
                    break;
                }
                default:
                {
                    break;
                }
            }
            if (currentPosition < 0)
            {
                currentPosition = 0;
            }
            if (currentPosition > totalHeight)
            {
                currentPosition = totalHeight;
            }

            setAttribute("curpos", Int2String(currentPosition));
            return 0;
        }
        else if (WM_MOUSEWHEEL == inMessage)
        {
            short numDelta = HIWORD(wParam);
            short numPages = numDelta / WHEEL_DELTA;
            int totalHeight = 0;
            int pageHeight = 0;
            int currentPosition = 0;
            WinAPI::getScrollInfo(handle(), totalHeight, pageHeight, currentPosition);
            currentPosition = currentPosition - numPages*pageHeight;
            if (currentPosition < 0)
            {
                currentPosition = 0;
            }
            if (currentPosition > totalHeight)
            {
                currentPosition = totalHeight;
            }
            setAttribute("curpos", Int2String(currentPosition));
            return 0;
        }
        return NativeControl::handleMessage(inMessage, wParam, lParam);
    }


    int Scrollbar::getCurrentPosition() const
    {
        return WinAPI::getScrollPos(handle());
    }


    void Scrollbar::setCurrentPosition(int inCurrentPosition)
    {
        int totalHeight = 0;
        int pageHeight = 0;
        int oldCurPos = 0;
        WinAPI::getScrollInfo(handle(), totalHeight, pageHeight, oldCurPos);

        // The order in which curpos, maxpos and pageincrement
        // will be set (alphabetically by attribute name) can cause
        // impossible scrollbar states (e.g. currentpos or pageincrement
        // greater than maxpos). And we want to avoid that.
        // Our workaround is to detect such states here, and change invalid
        // values to valid ones.
        if (pageHeight == 0)
        {
            pageHeight = 1;
        }
        if (totalHeight < pageHeight)
        {
            totalHeight = pageHeight + 1;
        }
        if (totalHeight < inCurrentPosition)
        {
            totalHeight = inCurrentPosition + 1;
        }
        WinAPI::setScrollInfo(handle(), totalHeight, pageHeight, inCurrentPosition);
        if ((oldCurPos != inCurrentPosition) && eventHandler())
        {
            eventHandler()->curposChanged(this, oldCurPos, inCurrentPosition);
        }
    }


    int Scrollbar::getMaxPosition() const
    {
        int totalHeight = 0;
        int pageHeight = 0;
        int curPos = 0;
        WinAPI::getScrollInfo(handle(), totalHeight, pageHeight, curPos);
        return totalHeight;
    }


    void Scrollbar::setMaxPosition(int inMaxPosition)
    {
        int dummy = 0;
        int pageHeight = 0;
        int curPos = 0;
        WinAPI::getScrollInfo(handle(), dummy, pageHeight, curPos);

        // The order in which setCurPos, setMaxPos and setPageIncrement
        // will be set (alphabetically by attribute name) can cause
        // impossible scrollbar states (e.g. currentpos or pageincrement
        // greater than maxpos). And we want to avoid that.
        // Our workaround is to detect such states here, and change invalid
        // values to valid ones.
        if (pageHeight == 0)
        {
            pageHeight = 1;
        }
        if (inMaxPosition <= pageHeight)
        {
            pageHeight = inMaxPosition - 1;
        }
        WinAPI::setScrollInfo(handle(), inMaxPosition, pageHeight, curPos);
    }


    void Scrollbar::setIncrement(int inIncrement)
    {
        mIncrement = inIncrement;
    }


    int Scrollbar::getIncrement() const
    {
        return mIncrement;
    }


    void Scrollbar::setPageIncrement(int inPageIncrement)
    {
        int totalHeight = 0;
        int dummy = 0;
        int curPos = 0;
        WinAPI::getScrollInfo(handle(), totalHeight, dummy, curPos);

        // The order in which setCurPos, setMaxPos and setPageIncrement
        // will be set (alphabetically by attribute name) can cause
        // impossible scrollbar states (e.g. currentpos or pageincrement
        // greater than maxpos). And we want to avoid that.
        // Our workaround is to detect such states here, and change invalid
        // values to valid ones.
        if (totalHeight == 0)
        {
            totalHeight = 1;
        }
        if (curPos > totalHeight)
        {
            totalHeight = curPos + 1;
        }
        if (inPageIncrement >= totalHeight)
        {
            totalHeight = inPageIncrement + 1;
        }
        WinAPI::setScrollInfo(handle(), totalHeight, inPageIncrement, curPos);
    }


    int Scrollbar::getPageIncrement() const
    {
        int totalHeight = 0;
        int pageHeight = 0;
        int curPos = 0;
        WinAPI::getScrollInfo(handle(), totalHeight, pageHeight, curPos);
        return pageHeight;
    }


    bool Scrollbar::initAttributeControllers()
    {
        setAttributeController<ScrollbarCurrentPositionController>(this);
        setAttributeController<ScrollbarMaxPositionController>(this);
        setAttributeController<ScrollbarIncrementController>(this);
        setAttributeController<ScrollbarPageIncrementController>(this);
        return Super::initAttributeControllers();
    }


    VirtualBox::VirtualBox(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr),
        mBoxLayouter(this)
    {
    }


    Orient VirtualBox::getOrient() const
    {
        return mOrient.or(Horizontal);
    }


    Align VirtualBox::getAlign() const
    {
        return mAlign.or(Stretch);
    }


    bool VirtualBox::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    void VirtualBox::rebuildLayout()
    {
        mBoxLayouter.rebuildLayout();
    }


    Component * CreateBox(Component * inParent, const AttributesMapping & inAttr)
    {
        return new Decorator(new Box(inParent, inAttr));
    }

    Component * CreateHBox(Component * inParent, const AttributesMapping & inAttr)
    {
        Box * box(new Box(inParent, inAttr));
        box->setOrient(Horizontal);
        return new Decorator(box);
    }


    Component * CreateVBox(XULWin::Component * inParent, const AttributesMapping & inAttr)
    {
        Box * box(new Box(inParent, inAttr));
        box->setOrient(Vertical);
        return new Decorator(box);
    }


    Box::Box(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr),
        mBoxLayouter(this)
    {        
    }


    Orient Box::getOrient() const
    {
        return mOrient.or(Vertical);
    }
    
    
    void Box::setOrient(Orient inOrient)
    {
        mOrient = inOrient;
    }


    Align Box::getAlign() const
    {
        return mAlign.or(Stretch);
    }


    void Box::rebuildLayout()
    {
        mBoxLayouter.rebuildLayout();
    }


    int Box::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return mBoxLayouter.calculateWidth(inSizeConstraint);
    }


    int Box::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return mBoxLayouter.calculateHeight(inSizeConstraint);
    }


    Rect Box::clientRect() const
    {
        return Super::clientRect();
    }


    const Component * Box::getChild(size_t idx) const
    {
        return mElement->children()[idx]->component();
    }


    Component * Box::getChild(size_t idx)
    {
        return mElement->children()[idx]->component();
    }


    void Box::rebuildChildLayouts()
    {
        return Super::rebuildChildLayouts();
    }


    Separator::Separator(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent,
                      inAttr,
                      TEXT("STATIC"),
                      0, // exStyle
                      SS_GRAYFRAME)
    {
        mExpansive = true;
    }


    int Separator::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    int Separator::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 1;
    }


    Spacer::Spacer(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr)
    {
    }


    int Spacer::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return 0;
    }


    int Spacer::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return 0;
    }


    MenuButton::MenuButton(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent,
                      inAttr,
                      TEXT("BUTTON"),
                      0, // exStyle
                      WS_TABSTOP | BS_PUSHBUTTON)
    {
    }


    int MenuButton::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return WinAPI::getTextSize(handle(), WinAPI::getWindowText(handle())).cx + Defaults::textPadding()*2;
    }


    int MenuButton::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::buttonHeight();
    }


    VirtualGrid::VirtualGrid(Component * inParent,
                             const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr)
    {
    }


    int VirtualGrid::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const Columns * cols = findChildOfType<Columns>())
        {
            result = cols->getWidth(inSizeConstraint);
        }
        return result;

    }


    int VirtualGrid::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const Rows * rows = findChildOfType<Rows>())
        {
            result = rows->getHeight(inSizeConstraint);
        }
        return result;
    }


    void VirtualGrid::rebuildLayout()
    {
        //
        // Initialize helper variables
        //
        int numCols = 0;
        int numRows = 0;
        ElementPtr columns;
        ElementPtr rows;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (child->tagName() == XMLRows::TagName())
            {
                rows = child;
                numRows = rows->children().size();
            }
            else if (child->tagName() == XMLColumns::TagName())
            {
                columns = child;
                numCols = columns->children().size();
            }
            else
            {
                ReportError("XMLGrid contains incompatible child element: '" + child->tagName() + "'");
            }
        }
        if (!rows || !columns)
        {
            ReportError("XMLGrid has no rows or no columns!");
            return;
        }

        if (rows->children().empty())
        {
            ReportError("XMLGrid has no rows!");
            return;
        }

        if (columns->children().empty())
        {
            ReportError("XMLGrid has no columns!");
            return;
        }


        //
        // Get column size infos (min width and flex)
        //
        std::vector<SizeInfo> colWidths;
        for (size_t colIdx = 0; colIdx != columns->children().size(); ++colIdx)
        {
            if (Column * col = columns->children()[colIdx]->component()->downcast<Column>())
            {
                colWidths.push_back(
                    SizeInfo(FlexWrap(String2Int(col->el()->getAttribute("flex"), 0)),
                             MinSizeWrap(col->getWidth(Minimum)),
                             OptSizeWrap(col->getWidth(Preferred))));
            }
        }

        if (colWidths.empty())
        {
            ReportError("XMLGrid has no columns!");
            return;
        }


        //
        // Get row size infos (min height and flex)
        //
        std::vector<SizeInfo> rowHeights;
        for (size_t rowIdx = 0; rowIdx != rows->children().size(); ++rowIdx)
        {
            if (Row * row = rows->children()[rowIdx]->component()->downcast<Row>())
            {
                rowHeights.push_back(
                    SizeInfo(FlexWrap(String2Int(row->el()->getAttribute("flex"), 0)),
                             MinSizeWrap(row->getHeight(Minimum)),
                             OptSizeWrap(row->getHeight(Preferred))));
            }
        }

        if (rowHeights.empty())
        {
            ReportError("XMLGrid has no rows!");
            return;
        }


        //
        // Get bounding rect for all cells
        //
        GenericGrid<Rect> outerRects(numRows, numCols);
        GridLayoutManager::GetOuterRects(clientRect(), colWidths, rowHeights, outerRects);


        //
        // Get size info for each cell
        //
        GenericGrid<CellInfo> widgetInfos(numRows, numCols, CellInfo(0, 0, Start, Start));
        for (size_t rowIdx = 0; rowIdx != numRows; ++rowIdx)
        {
            if (Row * row = rows->children()[rowIdx]->component()->downcast<Row>())
            {
                for (size_t colIdx = 0; colIdx != numCols; ++colIdx)
                {
                    if (Column * column = columns->children()[colIdx]->component()->downcast<Column>())
                    {
                        if (colIdx < row->getChildCount())
                        {
                            Component * child = row->el()->children()[colIdx]->component();
                            widgetInfos.set(rowIdx, colIdx,
                                            CellInfo(child->getWidth(),
                                                     child->getHeight(),
                                                     String2Align(row->el()->getAttribute("align"), Stretch),
                                                     String2Align(column->el()->getAttribute("align"), Stretch)));
                        }
                    }
                }
            }
        }


        //
        // Get inner rect for each cell
        //
        GenericGrid<Rect> innerRects(numRows, numCols);
        GridLayoutManager::GetInnerRects(outerRects, widgetInfos, innerRects);


        //
        // Apply inner rect to each widget inside a cell
        //
        for (size_t rowIdx = 0; rowIdx != numRows; ++rowIdx)
        {
            for (size_t colIdx = 0; colIdx != numCols; ++colIdx)
            {
                if (rowIdx < rows->children().size())
                {
                    ElementPtr rowEl = rows->children()[rowIdx];
                    if (colIdx < rowEl->children().size())
                    {
                        Component * child = rowEl->children()[colIdx]->component();
                        const Rect & r = innerRects.get(rowIdx, colIdx);
                        child->move(r.x(), r.y(), r.width(), r.height());
                    }
                }
            }
        }

        //
        // Rebuild child layoutss
        //
        rebuildChildLayouts();
    }


    Grid::Grid(Component * inParent,
               const AttributesMapping & inAttr) :
        NativeControl(inParent, inAttr, TEXT("STATIC"), WS_EX_CONTROLPARENT, WS_TABSTOP)
    {
    }


    int Grid::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const Columns * cols = findChildOfType<Columns>())
        {
            result = cols->getWidth(inSizeConstraint);
        }
        return result;

    }


    int Grid::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const Rows * rows = findChildOfType<Rows>())
        {
            result = rows->getHeight(inSizeConstraint);
        }
        return result;
    }


    void Grid::rebuildLayout()
    {
        //
        // Initialize helper variables
        //
        int numCols = 0;
        int numRows = 0;
        ElementPtr columns;
        ElementPtr rows;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr child = el()->children()[idx];
            if (child->tagName() == XMLRows::TagName())
            {
                rows = child;
                numRows = rows->children().size();
            }
            else if (child->tagName() == XMLColumns::TagName())
            {
                columns = child;
                numCols = columns->children().size();
            }
            else
            {
                ReportError("XMLGrid contains incompatible child element: '" + child->tagName() + "'");
            }
        }
        if (!rows || !columns)
        {
            ReportError("XMLGrid has no rows or no columns!");
            return;
        }

        if (rows->children().empty())
        {
            ReportError("XMLGrid has no rows!");
            return;
        }

        if (columns->children().empty())
        {
            ReportError("XMLGrid has no columns!");
            return;
        }


        //
        // Get column size infos (min width and flex)
        //
        std::vector<SizeInfo> colWidths;
        for (size_t colIdx = 0; colIdx != columns->children().size(); ++colIdx)
        {
            if (Column * col = columns->children()[colIdx]->component()->downcast<Column>())
            {
                colWidths.push_back(
                    SizeInfo(FlexWrap(col->getFlex()),
                             MinSizeWrap(col->getWidth(Minimum)),
                             OptSizeWrap(col->getWidth(Preferred))));
            }
        }

        if (colWidths.empty())
        {
            ReportError("XMLGrid has no columns!");
            return;
        }


        //
        // Get row size infos (min height and flex)
        //
        std::vector<SizeInfo> rowHeights;
        for (size_t rowIdx = 0; rowIdx != rows->children().size(); ++rowIdx)
        {
            if (Row * row = rows->children()[rowIdx]->component()->downcast<Row>())
            {
                rowHeights.push_back(
                    SizeInfo(FlexWrap(row->getFlex()),
                             MinSizeWrap(row->getHeight(Minimum)),
                             OptSizeWrap(row->getHeight(Preferred))));
            }
        }

        if (rowHeights.empty())
        {
            ReportError("XMLGrid has no rows!");
            return;
        }


        //
        // Get bounding rect for all cells
        //
        GenericGrid<Rect> outerRects(numRows, numCols);
        Rect clientRect(clientRect());
        GridLayoutManager::GetOuterRects(clientRect, colWidths, rowHeights, outerRects);


        //
        // Get size info for each cell
        //
        GenericGrid<CellInfo> widgetInfos(numRows, numCols, CellInfo(0, 0, Start, Start));
        for (size_t rowIdx = 0; rowIdx != numRows; ++rowIdx)
        {
            if (Row * row = rows->children()[rowIdx]->component()->downcast<Row>())
            {
                for (size_t colIdx = 0; colIdx != numCols; ++colIdx)
                {
                    if (Column * column = columns->children()[colIdx]->component()->downcast<Column>())
                    {
                        if (colIdx < row->getChildCount())
                        {
                            Component * child = row->el()->children()[colIdx]->component();
                            widgetInfos.set(rowIdx, colIdx,
                                            CellInfo(child->getWidth(),
                                                     child->getHeight(),
                                                     row->getAlign(),
                                                     column->getAlign()));
                        }
                    }
                }
            }
        }


        //
        // Get inner rect for each cell
        //
        GenericGrid<Rect> innerRects(numRows, numCols);
        GridLayoutManager::GetInnerRects(outerRects, widgetInfos, innerRects);


        //
        // Apply inner rect to each widget inside a cell
        //
        for (size_t rowIdx = 0; rowIdx != numRows; ++rowIdx)
        {
            for (size_t colIdx = 0; colIdx != numCols; ++colIdx)
            {
                if (rowIdx < rows->children().size())
                {
                    ElementPtr rowEl = rows->children()[rowIdx];
                    if (colIdx < rowEl->children().size())
                    {
                        Component * child = rowEl->children()[colIdx]->component();
                        const Rect & r = innerRects.get(rowIdx, colIdx);
                        child->move(r.x(), r.y(), r.width(), r.height());
                    }
                }
            }
        }

        //
        // Rebuild child layoutss
        //
        rebuildChildLayouts();
    }


    Rows::Rows(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr)
    {
    }


    int Rows::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;

        std::vector<XMLRow *> rows;

        //
        // 1. Obtain the list of rows from the grid element
        //
        if (Component * grid = parent())
        {
            if (XMLGrid::TagName() == grid->el()->tagName())
            {
                grid->el()->getElementsByType<XMLRow>(rows);
            }
        }

        //
        // 2. Get the max row width
        //
        result = max_element_value(rows.begin(),
                                   rows.end(),
                                   0,
                                   boost::bind(&Component::getWidth,
                                               boost::bind(&Element::component, _1),
                                               inSizeConstraint));
        return result;
    }


    int Rows::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;

        std::vector<XMLRow *> rows;

        //
        // 1. Obtain the list of rows from the grid element
        //
        if (Component * grid = parent())
        {
            if (XMLGrid::TagName() == grid->el()->tagName())
            {
                grid->el()->getElementsByType<XMLRow>(rows);
            }
        }

        //
        // 2. Get the sum of row heights
        //
        result = sum_element_values(rows.begin(),
                                    rows.end(),
                                    0,
                                    boost::bind(&Component::getHeight,
                                                boost::bind(&Element::component, _1),
                                                inSizeConstraint));
        return result;
    }


    Columns::Columns(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr)
    {
    }


    int Columns::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;

        std::vector<XMLColumn *> columns;

        //
        // 1. Obtain the list of columns from the grid element
        //
        if (Component * grid = parent())
        {
            if (XMLGrid::TagName() == grid->el()->tagName())
            {
                grid->el()->getElementsByType<XMLColumn>(columns);
            }
        }

        //
        // 2. Get the max column width
        //
        result = sum_element_values(columns.begin(),
                                    columns.end(),
                                    0,
                                    boost::bind(&Component::getWidth,
                                                boost::bind(&Element::component, _1),
                                                inSizeConstraint));
        return result;
    }


    int Columns::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;

        std::vector<XMLColumn *> columns;

        //
        // 1. Obtain the list of columns from the grid element
        //
        if (Component * grid = parent())
        {
            if (XMLGrid::TagName() == grid->el()->tagName())
            {
                grid->el()->getElementsByType<XMLColumn>(columns);
            }
        }

        //
        // 2. Get the sum of column heights
        //
        result = max_element_value(columns.begin(),
                                   columns.end(),
                                   0,
                                   boost::bind(&Component::getHeight,
                                               boost::bind(&Element::component, _1),
                                               inSizeConstraint));
        return result;
    }


    Row::Row(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr)
    {
    }


    int Row::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return calculateSumChildWidths(inSizeConstraint);
    }


    int Row::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return calculateMaxChildHeight(inSizeConstraint);
    }


    Column::Column(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr)
    {
    }


    Align Column::getAlign() const
    {
        return mAlign.or(Stretch);
    }


    int Column::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        // This is a special case: we need to get all rows first and calculate
        // the width of each row's corresponding column
        ElementPtr rows;
        int ownIndex = -1;
        Element * grid = el()->parent()->parent();
        for (size_t idx = 0; idx != grid->children().size(); ++idx)
        {
            ElementPtr child = grid->children()[idx];
            if (child->tagName() == XMLRows::TagName())
            {
                rows = child;
            }
            else if (child->tagName() == XMLColumns::TagName())
            {
                for (size_t ownI = 0; ownI != child->children().size(); ++ownI)
                {
                    if (child->children()[ownI]->component()->componentId() == componentId())
                    {
                        ownIndex = ownI;
                    }
                }
            }
            if (rows && ownIndex != -1)
            {
                break;
            }
        }
        if (!rows)
        {
            ReportError("Could not find 'rows' element in XMLGrid.");
            return 0;
        }
        if (ownIndex == -1)
        {
            ReportError("XMLColumn was unable to find itself in its parent container.");
            return 0;
        }

        int res = 0;
        for (size_t rowIdx = 0; rowIdx != rows->children().size(); ++rowIdx)
        {
            ElementPtr row = rows->children()[rowIdx];
            if (ownIndex < row->children().size())
            {
                int w = row->children()[ownIndex]->component()->getWidth(inSizeConstraint);
                if (w > res)
                {
                    res = w;
                }
            }
        }
        return res;
    }


    int Column::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return calculateSumChildHeights(inSizeConstraint);
    }


    RadioGroup::RadioGroup(Component * inParent, const AttributesMapping & inAttr) :
        VirtualBox(inParent, inAttr)
    {
    }


    Radio::Radio(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent,
                      inAttr,
                      TEXT("BUTTON"),
                      0, // exStyle
                      WS_TABSTOP | BS_RADIOBUTTON)
    {
    }


    int Radio::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::radioButtonMinimumWidth() + WinAPI::getTextSize(handle(), WinAPI::getWindowText(handle())).cx;
    }


    int Radio::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::controlHeight();
    }


    ProgressMeter::ProgressMeter(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent,
                      inAttr,
                      PROGRESS_CLASS,
                      0, // exStyle
                      PBS_SMOOTH)
    {
        WinAPI::initializeProgressMeter(mHandle, 100);
    }


    int ProgressMeter::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return Defaults::progressMeterWidth();
    }


    int ProgressMeter::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::progressMeterHeight();
    }


    int ProgressMeter::getValue() const
    {
        return WinAPI::getProgressMeterProgress(handle());
    }


    void ProgressMeter::setValue(int inValue)
    {
        WinAPI::setProgressMeterProgress(handle(), inValue);
    }


    bool ProgressMeter::initAttributeControllers()
    {
        setAttributeController<IntValueController>(this);
        return Super::initAttributeControllers();
    }


    Deck::Deck(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr),
        mSelectedIndex(0)
    {
    }


    int Deck::getSelectedIndex() const
    {
        return mSelectedIndex;
    }


    void Deck::setSelectedIndex(int inSelectedIndex)
    {
        mSelectedIndex = inSelectedIndex;
        rebuildLayout();
        invalidateRect();
    }


    void Deck::rebuildLayout()
    {
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            ElementPtr element = el()->children()[idx];
            bool visible = idx == mSelectedIndex;
            element->component()->setHidden(!visible);
            if (visible)
            {
                Rect rect = clientRect();
                Component * n = element->component();
                n->move(rect.x(), rect.y(), rect.width(), rect.height());
            }
        }
        rebuildChildLayouts();
    }


    int Deck::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return calculateMaxChildWidth(inSizeConstraint);
    }


    int Deck::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return calculateMaxChildHeight(inSizeConstraint);
    }


    bool Deck::initAttributeControllers()
    {
        setAttributeController<SelectedIndexController>(this);
        return Super::initAttributeControllers();
    }


    Tabs::Tabs(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
    {
    }


    Tab::Tab(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
    {
    }


    TabPanels::Instances TabPanels::sInstances;


    TabPanels::TabPanels(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr),
        mParentHandle(0),
        mTabBarHandle(0),
        mSelectedIndex(0),
        mChildCount(0)
    {
        NativeComponent * nativeParent = NativeControl::FindNativeParent(inParent);
        if (!nativeParent)
        {
            ReportError("TabPanels constructor failed because no native parent was found.");
            return;
        }

        mParentHandle = nativeParent->handle();

        mTabBarHandle = ::CreateWindowEx
                        (
                            0,
                            WC_TABCONTROL,
                            0,
                            WS_TABSTOP | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
                            0, 0, 0, 0,
                            mParentHandle,
                            (HMENU)mComponentId.value(),
                            NativeComponent::GetModuleHandle(),
                            0
                        );
        ::SendMessage(mTabBarHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
        mOrigProc = (WNDPROC)(LONG_PTR)::SetWindowLongPtr(mParentHandle, GWL_WNDPROC, (LONG)(LONG_PTR)&TabPanels::MessageHandler);
        sInstances.insert(std::make_pair(mParentHandle, this));
    }


    TabPanels::~TabPanels()
    {
        Instances::iterator it = sInstances.find(mParentHandle);
        if (it != sInstances.end())
        {
            sInstances.erase(it);
        }
        ::SetWindowLongPtr(mParentHandle, GWL_WNDPROC, (LONG)(LONG_PTR)mOrigProc);
        ::DestroyWindow(mTabBarHandle);
    }


    void TabPanels::addTabPanel(TabPanel * inPanel)
    {
        if (Tab * tab = getCorrespondingTab(mChildCount))
        {
            WinAPI::appendTabPanel(mTabBarHandle, tab->el()->getAttribute("label"));
            mChildCount++;
        }
        update();
    }


    Tab * TabPanels::getCorrespondingTab(size_t inIndex)
    {
        for (size_t idx = 0; idx != el()->parent()->children().size(); ++idx)
        {
            if (el()->parent()->children()[idx]->tagName() == XMLTabs::TagName())
            {
                if (XMLTabs * tabs = el()->parent()->children()[idx]->downcast<XMLTabs>())
                {
                    return tabs->children()[inIndex]->component()->downcast<Tab>();
                }
            }
        }
        return 0;
    }


    void TabPanels::rebuildLayout()
    {
        Rect rect = clientRect();
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            Component * element = getChild(idx);
            element->move(rect.x(),
                          rect.y() + Defaults::tabHeight(),
                          rect.width(),
                          rect.height() - Defaults::tabHeight());
        }
        ::MoveWindow(mTabBarHandle, rect.x(), rect.y(), rect.width(), Defaults::tabHeight(), FALSE);
        rebuildChildLayouts();
    }


    int TabPanels::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return calculateMaxChildWidth(inSizeConstraint);
    }


    int TabPanels::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::tabHeight() + calculateMaxChildHeight(inSizeConstraint);
    }


    void TabPanels::update()
    {
        int selectedIndex = TabCtrl_GetCurSel(mTabBarHandle);
        for (size_t idx = 0; idx != mChildCount; ++idx)
        {
            getChild(idx)->setHidden(idx != selectedIndex);
        }
    }


    LRESULT TabPanels::MessageHandler(HWND inHandle, UINT inMessage, WPARAM wParam, LPARAM lParam)
    {

        Instances::iterator it = sInstances.find(inHandle);
        if (it == sInstances.end())
        {
            return ::DefWindowProc(inHandle, inMessage, wParam, lParam);
        }

        TabPanels * pThis = it->second;

        switch (inMessage)
        {
            case WM_NOTIFY:
            {
                if (((LPNMHDR)lParam)->code == TCN_SELCHANGE)
                {
                    pThis->update();

                    // HACK!
                    // When changing tabs the min-max sizes of the window can
                    // change and we need a way to make sure the window
                    // enlarges if needed. Our workaround consists of moving
                    // the window to the same location but one pixel higher,
                    // and then move it back to its original size.
                    // This seemingly zero-op has two side effects:
                    // 1. The minmax sizes will be enforced again.
                    // 2. The content of the tabpanel is refreshed correctly.
                    Window * wnd = pThis->findParentWindow();
                    if (wnd)
                    {
                        RECT rw;
                        ::GetWindowRect(wnd->handle(), &rw);
                        int w = rw.right - rw.left;
                        int h = rw.bottom - rw.top;
                        ::MoveWindow(wnd->handle(), rw.left, rw.top, w, h+1, FALSE);
                        ::MoveWindow(wnd->handle(), rw.left, rw.top, w, h, FALSE);
                    }
                    return TRUE;
                }
            }
        }

        if (pThis->mOrigProc)
        {
            return ::CallWindowProc(pThis->mOrigProc, inHandle, inMessage, wParam, lParam);
        }
        else
        {
            return ::DefWindowProc(inHandle, inMessage, wParam, lParam);
        }
    }


    TabPanel::TabPanel(Component * inParent, const AttributesMapping & inAttr) :
        VirtualBox(inParent, inAttr)
    {
    }


    bool TabPanel::init()
    {
        if (TabPanels * parent = el()->parent()->component()->downcast<TabPanels>())
        {
            parent->addTabPanel(this);
        }
        return Super::init();
    }


    GroupBox::GroupBox(Component * inParent, const AttributesMapping & inAttr) :
        VirtualBox(inParent, inAttr),
        mGroupBoxHandle(0),
        mMarginLeft(2),
        mMarginTop(16),
        mMarginRight(2),
        mMarginBottom(2)
    {

        // HACK!!!
        // Group boxes don't render properly if the parent window has the
        // WS_CLIPCHILDREN style defined.
        // There should be some more decent way to fix this. But for now
        // I just remove the flag from the parent. This may result in more
        // flickering during manual resize of the XMLWindow.
        WinAPI::removeWindowStyle(NativeControl::FindNativeParent(inParent)->handle(), WS_CLIPCHILDREN);


        mGroupBoxHandle = CreateWindowEx(0,
                                         TEXT("BUTTON"),
                                         0,
                                         WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
                                         0, 0, 0, 0,
                                         NativeControl::FindNativeParent(inParent)->handle(),
                                         (HMENU)mComponentId.value(),
                                         NativeComponent::GetModuleHandle(),
                                         0);
        ::SendMessage(mGroupBoxHandle, WM_SETFONT, (WPARAM)::GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
    }


    GroupBox::~GroupBox()
    {
        ::DestroyWindow(mGroupBoxHandle);
    }


    Orient GroupBox::getOrient() const
    {
        return mOrient.or(Vertical);
    }


    void GroupBox::setCaption(const std::string & inLabel)
    {
        WinAPI::setWindowText(mGroupBoxHandle, inLabel);
    }


    int GroupBox::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int textWidth = Defaults::textPadding() + WinAPI::getTextSize(mGroupBoxHandle, WinAPI::getWindowText(mGroupBoxHandle)).cx;
        int contentWidth = mBoxLayouter.calculateWidth(inSizeConstraint);
        return mMarginLeft + std::max<int>(textWidth, contentWidth) + mMarginRight;
    }


    int GroupBox::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return mMarginTop + mBoxLayouter.calculateHeight(inSizeConstraint) + mMarginBottom;
    }


    void GroupBox::rebuildLayout()
    {
        Rect clientRect(Super::clientRect());
        ::MoveWindow(mGroupBoxHandle,
                     clientRect.x(),
                     clientRect.y(),
                     clientRect.width(),
                     clientRect.height(),
                     FALSE);
        mBoxLayouter.rebuildLayout();
    }


    Rect GroupBox::clientRect() const
    {
        Rect clientRect(Super::clientRect());
        Rect rect(clientRect.x() + mMarginLeft,
                  clientRect.y() + mMarginTop,
                  clientRect.width() - mMarginRight - mMarginLeft,
                  clientRect.height() - mMarginTop - mMarginBottom);
        return rect;
    }


    // The 'caption' is optionally the first child element of the groupbox.
    // This element is responsible for setting the groupbox title and it has
    // no other purpose than that.
    // I think this is a design error in Mozilla XUL. The groupbox title should
    // have been an attribute, not a child element.
    // However, we want to be compatible with Mozilla XUL so we have to support it.

    // In order to keep the layout managers from taking the caption element
    // into account we never return it and return the element after it instead.
    // This requires getChildCount to be overridden as well to make it return
    // the number of children minus one.
    const Component * GroupBox::getChild(size_t idx) const
    {
        if (Super::getChildCount() > 0)
        {
            if (Super::getChild(0)->el()->tagName() == XMLCaption::TagName())
            {
                assert(idx + 1 < Super::getChildCount());
                if (idx + 1 < Super::getChildCount())
                {
                    return Super::getChild(idx + 1);
                }
            }
        }
        return Super::getChild(idx);
    }


    Component * GroupBox::getChild(size_t idx)
    {
        return const_cast<Component *>(static_cast<const GroupBox *>(this)->getChild(idx));
    }


    // Returns number of children minus the caption element.
    // See documention of GroupBox::getChild method for more information.
    size_t GroupBox::getChildCount() const
    {
        if (Super::getChildCount() > 0)
        {
            if (Super::getChild(0)->el()->tagName() == XMLCaption::TagName())
            {
                return Super::getChildCount() - 1;
            }
        }
        return Super::getChildCount();
    }


    Caption::Caption(Component * inParent, const AttributesMapping & inAttr) :
        VirtualComponent(inParent, inAttr)
    {
    }


    int Caption::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        if (NativeComponent * comp = NativeControl::FindNativeParent(mParent))
        {
            return WinAPI::getTextSize(comp->handle(), mElement->getAttribute("label")).cx;
        }
        return 0;
    }


    int Caption::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        if (NativeComponent * comp = NativeControl::FindNativeParent(mParent))
        {
            return WinAPI::getTextSize(comp->handle(), mElement->getAttribute("label")).cy;
        }
        return 0;
    }


    bool Caption::init()
    {
        if (GroupBox * groupBox = mParent->downcast<GroupBox>())
        {
            groupBox->setCaption(mElement->getAttribute("label"));
        }
        return Super::init();
    }


    Tree::Tree(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent, inAttr, WC_TREEVIEW, 0, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS)
    {
    }


    int Tree::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = Defaults::controlWidth();
        if (const TreeChildren * children = findChildOfType<TreeChildren>())
        {
            result = Defaults::treeIndent() + children->getWidth(inSizeConstraint);
        }
        return result;
    }


    int Tree::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const TreeChildren * children = findChildOfType<TreeChildren>())
        {
            result = children->getHeight(inSizeConstraint);
        }
        return result;
    }


    void Tree::addInfo(const TreeItemInfo & inInfo)
    {
        addInfo(TVI_ROOT, TVI_FIRST, inInfo);
    }


    HTREEITEM Tree::addInfo(HTREEITEM inRoot, HTREEITEM inPrev, const TreeItemInfo & inInfo)
    {
        std::wstring label = ToUTF16(inInfo.data());

        TVITEM tvi;
        tvi.mask = TVIF_TEXT;
        tvi.pszText = const_cast<TCHAR *>(label.c_str());
        tvi.cchTextMax = label.size();

        TVINSERTSTRUCT tvins;
        tvins.item = tvi;
        tvins.hInsertAfter = inPrev;
        tvins.hParent = inRoot;
        inPrev = (HTREEITEM)SendMessage(handle(), TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

        HTREEITEM prevChild = TVI_FIRST;
        TreeItemInfo::const_iterator it = inInfo.begin(), end = inInfo.end();
        for (; it != end; ++it)
        {
            const TreeItemInfo * itemInfo = *it;
            prevChild = addInfo(inPrev, prevChild, *itemInfo);
        }

        return inPrev;
    }


    bool Tree::init()
    {
        if (TreeChildren * children = findChildOfType<TreeChildren>())
        {
            if (Component * firstChild = children->el()->children()[0]->component())
            {
                if (TreeItem * item = firstChild->downcast<TreeItem>())
                {
                    addInfo(item->itemInfo());
                }
            }
        }
        return Super::init();
    }


    TreeChildren::TreeChildren(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
    {
    }


    int TreeChildren::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            const Component * child = getChild(idx);
            if (const TreeItem * item = child->downcast<TreeItem>())
            {
                result += item->getHeight(inSizeConstraint);
            }
        }
        return result;
    }


    int TreeChildren::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            const Component * child = getChild(idx);
            if (const TreeItem * item = child->downcast<TreeItem>())
            {
                int minWidth = item->getWidth(inSizeConstraint);
                if (result < minWidth)
                {
                    result = minWidth;
                }
            }
        }
        return result;
    }


    TreeItem::TreeItem(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
    {
    }


    int TreeItem::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const TreeRow * row = findChildOfType<TreeRow>())
        {
            int minHeight = row->getHeight(inSizeConstraint);
            if (result < minHeight)
            {
                result = minHeight;
            }
        }
        if (const TreeChildren * treeChildren = findChildOfType<TreeChildren>())
        {
            result += treeChildren->getHeight(inSizeConstraint);
        }
        return result;
    }


    int TreeItem::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const TreeRow * row = findChildOfType<TreeRow>())
        {
            int minWidth = row->getWidth(inSizeConstraint);
            if (result < minWidth)
            {
                result = minWidth;
            }
        }
        if (const TreeChildren * treeChildren = findChildOfType<TreeChildren>())
        {
            int minWidth = Defaults::treeIndent() + treeChildren->getWidth(inSizeConstraint);
            if (result < minWidth)
            {
                result = minWidth;
            }
        }
        return result;
    }


    bool TreeItem::isOpened() const
    {
        //if (NativeComponent * comp = NativeControl::FindNativeParent(const_cast<TreeItem*>(this)))
        //{
        //    TreeView_GetItemState(comp->handle(), hti, mask);
        //}
        return false; // TODO: implement
    }


    bool TreeItem::init()
    {
        if (TreeRow * row = findChildOfType<TreeRow>())
        {
            if (TreeCell * cell = row->findChildOfType<TreeCell>())
            {
                std::string label = cell->getLabel();
                mItemInfo.setData(label);
            }
        }
        if (TreeChildren * treeChildren = findChildOfType<TreeChildren>())
        {
            for (size_t idx = 0; idx != treeChildren->getChildCount(); ++idx)
            {
                Component * child = treeChildren->getChild(idx);
                if (TreeItem * item = child->downcast<TreeItem>())
                {
                    mItemInfo.addChild(&item->itemInfo());
                }
            }
        }
        return Super::init();
    }


    TreeCols::TreeCols(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
    {
    }


    TreeCol::TreeCol(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
    {
    }


    TreeRow::TreeRow(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
    {
    }


    int TreeRow::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return calculateMaxChildWidth(inSizeConstraint);
    }


    int TreeRow::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        for (size_t idx = 0; idx != getChildCount(); ++idx)
        {
            const Component * child = getChild(idx);
            if (const TreeCell * cell = child->downcast<TreeCell>())
            {
                result += cell->getHeight(inSizeConstraint);
            }
        }
        return result;
    }


    TreeCell::TreeCell(Component * inParent, const AttributesMapping & inAttr) :
        PhonyComponent(inParent, inAttr)
    {
    }


    bool TreeCell::initAttributeControllers()
    {
        setAttributeController<LabelController>(this);
        return Super::initAttributeControllers();
    }


    std::string TreeCell::getLabel() const
    {
        return mLabel;
    }


    void TreeCell::setLabel(const std::string & inLabel)
    {
        mLabel = inLabel;
    }


    int TreeCell::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (NativeComponent * comp = NativeControl::FindNativeParent(const_cast<TreeCell *>(this)))
        {
            result = WinAPI::getTextSize(comp->handle(), getLabel()).cx + Defaults::textPadding();
        }
        return result;
    }


    int TreeCell::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return Defaults::treeItemHeight();
    }


    Statusbar::Statusbar(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent, inAttr, STATUSCLASSNAME, 0, SBARS_SIZEGRIP),
        mBoxLayouter(this)
    {
    }


    bool Statusbar::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    int Statusbar::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return calculateSumChildWidths(inSizeConstraint);
    }


    int Statusbar::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int ownHeight = WinAPI::Window_GetHeight(handle());
        int maxChildHeight = calculateMaxChildHeight(inSizeConstraint);
        return std::max<int>(ownHeight, maxChildHeight);
    }


    Orient Statusbar::getOrient() const
    {
        return Horizontal;
    }


    Align Statusbar::getAlign() const
    {
        return Center;
    }


    Rect Statusbar::clientRect() const
    {
        Rect clientRect(Super::clientRect());
        // Substract from width one square to make place for the resize gripper widget
        return Rect(clientRect.x(), clientRect.y(), clientRect.width() - WinAPI::Window_GetHeight(handle()), clientRect.height());
    }


    void Statusbar::rebuildLayout()
    {
        mBoxLayouter.rebuildLayout();
    }


    void Statusbar::rebuildChildLayouts()
    {
        Super::rebuildChildLayouts();
    }


    StatusbarPanel::StatusbarPanel(Component * inParent, const AttributesMapping & inAttr) :
        NativeControl(inParent, inAttr, TEXT("STATIC"), 0, 0)
    {
    }


    bool StatusbarPanel::initAttributeControllers()
    {
        return Super::initAttributeControllers();
    }


    int StatusbarPanel::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        return WinAPI::getTextSize(handle(), getLabel()).cx;
    }


    int StatusbarPanel::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        return WinAPI::getTextSize(handle(), getLabel()).cy;
    }


} // namespace XULWin
