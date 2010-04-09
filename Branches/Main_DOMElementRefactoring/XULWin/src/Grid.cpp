#include "XULWin/Grid.h"
#include "XULWin/Algorithms.h"
#include "XULWin/ComponentManager.h"
#include "XULWin/ErrorReporter.h"
#include "XULWin/WinUtils.h"
#include <boost/bind.hpp>


namespace XULWin
{


    VirtualGrid::VirtualGrid(Component * inParent,
                             Poco::XML::Element * inDOMElement) :
        VirtualComponent(inParent, inDOMElement)
    {
    }


    int VirtualGrid::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const Columns * columns = findChildOfType<Columns>())
        {
            result = columns->calculateWidth(inSizeConstraint);
        }
        return result;

    }


    int VirtualGrid::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const Rows * rows = findChildOfType<Rows>())
        {
            result = rows->calculateHeight(inSizeConstraint);
        }
        return result;
    }


    void VirtualGrid::rebuildLayout()
    {
        //
        // Initialize helper variables
        //
        Columns * columns = findChildOfType<Columns>();
        if (!columns || columns->getChildCount() == 0)
        {
            return;
        }

        Rows * rows = findChildOfType<Rows>();
        if (!rows || rows->getChildCount() == 0)
        {
            return;
        }


        //
        // Get column size infos (min width and flex)
        //
        std::vector<SizeInfo> colWidths;
        for (size_t colIdx = 0; colIdx != columns->getChildCount(); ++colIdx)
        {
            if (Column * col = columns->getChild(colIdx)->downcast<Column>())
            {
                colWidths.push_back(
                    SizeInfo(FlexWrap(col->getFlex()),
                             MinSizeWrap(col->getWidth(Minimum)),
                             OptSizeWrap(col->getWidth(Preferred))));
            }
        }

        if (colWidths.empty())
        {
            ReportError("Grid has no columns!");
            return;
        }


        //
        // Get row size infos (min height and flex)
        //
        std::vector<SizeInfo> rowHeights;
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            if (Row * row = rows->getChild(rowIdx)->downcast<Row>())
            {
                rowHeights.push_back(
                    SizeInfo(FlexWrap(row->getFlex()),
                             MinSizeWrap(row->getHeight(Minimum)),
                             OptSizeWrap(row->getHeight(Preferred))));
            }
        }

        if (rowHeights.empty())
        {
            ReportError("Grid has no rows!");
            return;
        }


        //
        // Get bounding rect for all cells
        //
        GenericGrid<Rect> outerRects(rows->getChildCount(), columns->getChildCount());
        Rect clientRect(clientRect());
        GridLayoutManager::GetOuterRects(clientRect, colWidths, rowHeights, outerRects);


        //
        // Get size info for each cell
        //
        GenericGrid<CellInfo> widgetInfos(rows->getChildCount(), columns->getChildCount(), CellInfo(0, 0, Start, Start));
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            if (Row * row = rows->getChild(rowIdx)->downcast<Row>())
            {
                int rowHeight = row->getHeight();
                for (size_t colIdx = 0; colIdx != columns->getChildCount(); ++colIdx)
                {
                    if (Column * column = columns->getChild(colIdx)->downcast<Column>())
                    {
                        if (colIdx < row->getChildCount())
                        {
                            Component * child = row->getChild(colIdx);
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
        GenericGrid<Rect> innerRects(rows->getChildCount(), columns->getChildCount());
        GridLayoutManager::GetInnerRects(outerRects, widgetInfos, innerRects);


        //
        // Apply inner rect to each widget inside a cell
        //
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            for (size_t colIdx = 0; colIdx != columns->getChildCount(); ++colIdx)
            {
                if (rowIdx < rows->getChildCount())
                {
                    if (Row * row = rows->getChild(rowIdx)->downcast<Row>())
                    {
                        if (colIdx < row->getChildCount())
                        {
                            Component * child = columns->getChild(colIdx);
                            const Rect & r = innerRects.get(rowIdx, colIdx);
                            child->move(r.x(), r.y(), r.width(), r.height());
                        }
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
               Poco::XML::Element * inDOMElement) :
        NativeControl(inParent, inDOMElement, TEXT("STATIC"), WS_EX_CONTROLPARENT, WS_TABSTOP)
    {
    }


    int Grid::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const Columns * columns = findChildOfType<Columns>())
        {
            result = columns->calculateWidth(inSizeConstraint);
        }
        return result;

    }


    int Grid::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;
        if (const Rows * rows = findChildOfType<Rows>())
        {
            result = rows->calculateHeight(inSizeConstraint);
        }
        return result;
    }


    void Grid::rebuildLayout()
    {
        //
        // Initialize helper variables
        //
        Columns * columns = findChildOfType<Columns>();
        if (!columns || columns->getChildCount() == 0)
        {
            return;
        }

        Rows * rows = findChildOfType<Rows>();
        if (!rows || rows->getChildCount() == 0)
        {
            return;
        }


        //
        // Get column size infos (min width and flex)
        //
        std::vector<SizeInfo> colWidths;
        for (size_t colIdx = 0; colIdx != columns->getChildCount(); ++colIdx)
        {
            if (Column * col = columns->getChild(colIdx)->downcast<Column>())
            {
                colWidths.push_back(
                    SizeInfo(FlexWrap(col->getFlex()),
                             MinSizeWrap(col->getWidth(Minimum)),
                             OptSizeWrap(col->getWidth(Preferred))));
            }
        }

        if (colWidths.empty())
        {
            ReportError("Grid has no columns!");
            return;
        }


        //
        // Get row size infos (min height and flex)
        //
        std::vector<SizeInfo> rowHeights;
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            if (Row * row = rows->getChild(rowIdx)->downcast<Row>())
            {
                rowHeights.push_back(
                    SizeInfo(FlexWrap(row->getFlex()),
                             MinSizeWrap(row->getHeight(Minimum)),
                             OptSizeWrap(row->getHeight(Preferred))));
            }
        }

        if (rowHeights.empty())
        {
            ReportError("Grid has no rows!");
            return;
        }


        //
        // Get bounding rect for all cells
        //
        GenericGrid<Rect> outerRects(rows->getChildCount(), columns->getChildCount());
        Rect clientRect(clientRect());
        GridLayoutManager::GetOuterRects(clientRect, colWidths, rowHeights, outerRects);


        //
        // Get size info for each cell
        //
        GenericGrid<CellInfo> widgetInfos(rows->getChildCount(), columns->getChildCount(), CellInfo(0, 0, Start, Start));
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            if (Row * row = rows->getChild(rowIdx)->downcast<Row>())
            {
                int rowHeight = row->getHeight();
                for (size_t colIdx = 0; colIdx != columns->getChildCount(); ++colIdx)
                {
                    if (Column * column = columns->getChild(colIdx)->downcast<Column>())
                    {
                        if (colIdx < row->getChildCount())
                        {
                            Component * child = row->getChild(colIdx);
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
        GenericGrid<Rect> innerRects(rows->getChildCount(), columns->getChildCount());
        GridLayoutManager::GetInnerRects(outerRects, widgetInfos, innerRects);


        //
        // Apply inner rect to each widget inside a cell
        //
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            for (size_t colIdx = 0; colIdx != columns->getChildCount(); ++colIdx)
            {
                if (rowIdx < rows->getChildCount())
                {
                    if (Row * row = rows->getChild(rowIdx)->downcast<Row>())
                    {
                        if (colIdx < row->getChildCount())
                        {
                            Component * child = row->getChild(colIdx);
                            const Rect & r = innerRects.get(rowIdx, colIdx);
                            child->move(r.x(), r.y(), r.width(), r.height());
                        }
                    }
                }
            }
        }

        //
        // Rebuild child layoutss
        //
        rebuildChildLayouts();
    }

    

    Rows::Rows(Component * inParent, Poco::XML::Element * inDOMElement) :
        VirtualComponent(inParent, inDOMElement)
    {
    }


    int Rows::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;

        std::vector<Row *> rows;

        //
        // 1. Obtain the list of rows from the grid element
        //
        GetImmediateChildComponents<Row>(parentComponent(), rows);

        //
        // 2. Get the max row width
        //
        result = max_element_value(rows.begin(),
                                   rows.end(),
                                   0,
                                   boost::bind(&Component::calculateWidth, _1, inSizeConstraint));
        return result;
    }


    int Rows::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;

        std::vector<Row *> rows;

        //
        // 1. Obtain the list of rows from the grid element
        //
        GetImmediateChildComponents<Row>(parentComponent(), rows);

        //
        // 2. Get the sum of row heights
        //
        result = sum_element_values(rows.begin(),
                                    rows.end(),
                                    0,
                                    boost::bind(&Component::calculateHeight, _1, inSizeConstraint));
        return result;
    }


    Columns::Columns(Component * inParent, Poco::XML::Element * inElement) :
        VirtualComponent(inParent, inElement)
    {
    }


    int Columns::calculateWidth(SizeConstraint inSizeConstraint) const
    {
        int result = 0;

        std::vector<Column *> columns;

        //
        // 1. Obtain the list of columns from the grid element
        //
        GetImmediateChildComponents<Column>(parentComponent(), columns);

        //
        // 2. Get the max column width
        //
        result = sum_element_values(columns.begin(),
                                    columns.end(),
                                    0,
                                    boost::bind(&Component::calculateWidth, _1, inSizeConstraint));
        return result;
    }


    int Columns::calculateHeight(SizeConstraint inSizeConstraint) const
    {
        int result = 0;

        std::vector<Column *> columns;

        //
        // 1. Obtain the list of columns from the grid element
        //
        GetImmediateChildComponents<Column>(parentComponent(), columns);

        //
        // 2. Get the sum of column heights
        //
        result = max_element_value(columns.begin(),
                                   columns.end(),
                                   0,
                                   boost::bind(&Component::calculateHeight, _1, inSizeConstraint));
        return result;
    }


    Row::Row(Component * inParent, Poco::XML::Element * inElement) :
        VirtualComponent(inParent, inElement)
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


    Column::Column(Component * inParent, Poco::XML::Element * inElement) :
        VirtualComponent(inParent, inElement)
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
        Component * rows(0);
        int ownIndex = -1;
        Component * grid = parentComponent()->parentComponent();
        for (size_t idx = 0; idx != grid->getChildCount(); ++idx)
        {
            Component * child = grid->getChild(idx);
            if (child->tagName() == Rows::TagName())
            {
                rows = child;
            }
            else if (child->tagName() == Columns::TagName())
            {
                for (size_t ownI = 0; ownI != child->getChildCount(); ++ownI)
                {
                    if (child->getChild(ownI)->commandId() == commandId())
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
            ReportError("Could not find 'rows' element in GridElement.");
            return 0;
        }
        if (ownIndex == -1)
        {
            ReportError("ColumnElement was unable to find itself in its parent container.");
            return 0;
        }

        int res = 0;
        for (size_t rowIdx = 0; rowIdx != rows->getChildCount(); ++rowIdx)
        {
            Component * row = rows->getChild(rowIdx);
            if (ownIndex < row->getChildCount())
            {
                int w = row->getChild(ownIndex)->getWidth(inSizeConstraint);
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

} // namespace XULWin
