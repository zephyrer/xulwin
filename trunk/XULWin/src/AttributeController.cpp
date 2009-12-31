#include "XULWin/AttributeController.h"
#include "XULWin/Conversions.h"
#include "XULWin/Defaults.h"


namespace XULWin
{

    void TitleController::get(std::string & outValue)
    {
        outValue = getTitle();
    }


    void TitleController::set(const std::string & inValue)
    {
        setTitle(inValue);
    }


    void WidthController::get(std::string & outValue)
    {
        outValue = Int2String(getWidth());
    }


    void WidthController::set(const std::string & inValue)
    {
        setWidth(String2Int(inValue));
    }


    void HeightController::get(std::string & outValue)
    {
        outValue = Int2String(getHeight());
    }


    void HeightController::set(const std::string & inValue)
    {
        setHeight(String2Int(inValue));
    }


    void ScreenXController::get(std::string & outValue)
    {
        outValue = Int2String(getScreenX());
    }


    void ScreenXController::set(const std::string & inValue)
    {
        setScreenX(String2Int(inValue));
    }

    
    void ScreenYController::get(std::string & outValue)
    {
        outValue = Int2String(getScreenY());
    }


    void ScreenYController::set(const std::string & inValue)
    {
        setScreenY(String2Int(inValue));
    }


    void FlexController::get(std::string & outValue)
    {
        outValue = Int2String(getFlex());
    }


    void FlexController::set(const std::string & inValue)
    {
        setFlex(String2Int(inValue));
    }


    void DisabledController::get(std::string & outValue)
    {
        outValue = Bool2String(isDisabled());
    }


    void DisabledController::set(const std::string & inValue)
    {
        setDisabled(String2Bool(inValue, Defaults::Attributes::disabled()));
    }



    void SelectedController::get(std::string & outValue)
    {
        outValue = Bool2String(isSelected());
    }


    void SelectedController::set(const std::string & inValue)
    {
        setSelected(String2Bool(inValue, true));
    }


    void HiddenController::get(std::string & outValue)
    {
        outValue = Bool2String(isHidden());
    }


    void HiddenController::set(const std::string & inValue)
    {
        setHidden(String2Bool(inValue, Defaults::Attributes::disabled()));
    }


    void LabelController::get(std::string & outValue)
    {
        outValue = getLabel();
    }


    void LabelController::set(const std::string & inValue)
    {
        setLabel(inValue);
    }


    void StringValueController::get(std::string & outValue)
    {
        outValue = getValue();
    }


    void StringValueController::set(const std::string & inValue)
    {
        setValue(inValue);
    }


    void ReadOnlyController::get(std::string & outValue)
    {
        outValue = Bool2String(isReadOnly());
    }


    void ReadOnlyController::set(const std::string & inValue)
    {
        setReadOnly(String2Bool(inValue, Defaults::Attributes::readonly()));
    }
    

    void RowsController::get(std::string & outValue)
    {
        outValue = Int2String(getRows());
    }


    void RowsController::set(const std::string & inValue)
    {
        setRows(String2Int(inValue));
    }


    void CheckedController::get(std::string & outValue)
    {
        outValue = Bool2String(isChecked());
    }


    void CheckedController::set(const std::string & inValue)
    {
        setChecked(String2Bool(inValue, Defaults::Attributes::checked()));
    }


    void OrientController::get(std::string & outValue)
    {
        outValue = Orient2String(getOrient());
    }


    void OrientController::set(const std::string & inValue)
    {
        setOrient(String2Orient(inValue, Defaults::Attributes::orient()));
    }


    void AlignController::get(std::string & outValue)
    {
        outValue = Align2String(getAlign());
    }


    void AlignController::set(const std::string & inValue)
    {
        setAlign(String2Align(inValue, Defaults::Attributes::align()));
    }


    void IntValueController::get(std::string & outValue)
    {
        outValue = Int2String(getValue());
    }


    void IntValueController::set(const std::string & inValue)
    {
        setValue(String2Int(inValue));
    }


    void SelectedIndexController::get(std::string & outSelectedIndex)
    {
        outSelectedIndex = Int2String(getSelectedIndex());
    }


    void SelectedIndexController::set(const std::string & inSelectedIndex)
    {
        setSelectedIndex(String2Int(inSelectedIndex));
    }


    void ScrollbarCurrentPositionController::get(std::string & outCurrentPosition)
    {
        outCurrentPosition = Int2String(getCurrentPosition());
    }


    void ScrollbarCurrentPositionController::set(const std::string & inCurrentPosition)
    {
        setCurrentPosition(String2Int(inCurrentPosition));
    }


    void ScrollbarMaxPositionController::get(std::string & outMaxPosition)
    {
        outMaxPosition = Int2String(getMaxPosition());
    }


    void ScrollbarMaxPositionController::set(const std::string & inMaxPosition)
    {
        setMaxPosition(String2Int(inMaxPosition));
    }


    void ScrollbarIncrementController::get(std::string & outIncrement)
    {
        outIncrement = Int2String(getIncrement());
    }


    void ScrollbarIncrementController::set(const std::string & inIncrement)
    {
        setIncrement(String2Int(inIncrement));
    }


    void ScrollbarPageIncrementController::get(std::string & outPageIncrement)
    {
        outPageIncrement = Int2String(getPageIncrement());
    }


    void ScrollbarPageIncrementController::set(const std::string & inPageIncrement)
    {
        setPageIncrement(String2Int(inPageIncrement));
    }


    void SrcController::get(std::string & outSrc)
    {
        outSrc = getSrc();
    }


    void SrcController::set(const std::string & inSrc)
    {
        setSrc(inSrc);
    }


    void KeepAspectRatioController::get(std::string & outKeepAspectRatio)
    {
        outKeepAspectRatio = Bool2String(getKeepAspectRatio());
    }


    void KeepAspectRatioController::set(const std::string & inKeepAspectRatio)
    {
        setKeepAspectRatio(String2Bool(inKeepAspectRatio, true));
    }


    void SVG_Polygon_PointsController::get(std::string & outValue)
    {
        outValue = Points2String(getPoints());
    }


    void SVG_Polygon_PointsController::set(const std::string & inPoints)
    {
        setPoints(String2Points(inPoints));
    }


    void SVG_Path_InstructionsController::get(std::string & outValue)
    {
        outValue = SVGPathInstructions2String(getPathInstructions());
    }


    void SVG_Path_InstructionsController::set(const std::string & inPathInstructions)
    {
        setPathInstructions(String2SVGPathInstructions(inPathInstructions));
    }


    void SVG_FillController::get(std::string & outValue)
    {
        outValue = RGBColor2String(getSVGFill());
    }


    void SVG_FillController::set(const std::string & inValue)
    {
        RGBColor result;
        if (String2RGBColor(inValue, result))
        {
            setSVGFill(result);
        }
    }


    void SVG_StrokeController::get(std::string & outValue)
    {
        outValue = RGBColor2String(getSVGStroke());
    }


    void SVG_StrokeController::set(const std::string & inValue)
    {
        RGBColor result;
        if (String2RGBColor(inValue, result))
        {
            setSVGStroke(result);
        }
    }


    void SVG_StrokeWidthController::get(std::string & outValue)
    {
        outValue = Int2String(getSVGStrokeWidth());
    }


    void SVG_StrokeWidthController::set(const std::string & inValue)
    {
        setSVGStrokeWidth(String2Int(inValue));
    }

} // namespace XULWin
