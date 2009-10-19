#ifndef ATTRIBUTECONTROLLER_H_INCLUDED
#define ATTRIBUTECONTROLLER_H_INCLUDED


#include "XULWin/Enums.h"
#include "XULWin/Point.h"
#include "XULWin/PathInstructions.h"
#include "XULWin/RGBColor.h"
#include <string>


namespace XULWin
{
    
    /**
     * Base class for attribute controllers.
     * Attribute controllers must implement the get and set methods.
     * These get and set methods are string based wrappers for the typed
     * C++ getters and setters provided by the subclasses.
     * Component subclasses should inherit the attribute controllers that
     * correspond with their own attributes.
     * For example the Button element has the 'label' attribute, therefore the
     * the NativeButton class should inherit the LabelController, and provide
     * implementations for the getLabel and setLabel methods.
     */
    class AttributeController
    {
    public:
        virtual void get(std::string & outValue) = 0;
        virtual void set(const std::string & inValue) = 0;
    };


    /**
     * Attribute: title
     * Type: string
     * Purpose: The text to appear in the title bar of the window.
     */
    class TitleController : public AttributeController
    {
    public:
        TitleController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual std::string getTitle() const = 0;

        virtual void setTitle(const std::string & inTitle) = 0;
    };


    /**
     * Attribute: width
     * Type: string (representing an integer)
     * Purpose: The preferred width of the element.
     */
    class WidthController : public AttributeController
    {
    public:
        WidthController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual int getWidth() const = 0;

        virtual void setWidth(int inWidth) = 0;
    };


    /**
     * Attribute: height
     * Type: string (representing an integer)
     * The preferred height of the element.
     */
    class HeightController : public AttributeController
    {
    public:
        HeightController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual int getHeight() const = 0;

        virtual void setHeight(int inHeight) = 0;
    };


    /**
     * Attribute: screenX
     * Type: integer
     * The horizontal position at which the window appears on the screen.
     */
    class ScreenXController : public AttributeController
    {
    public:
        ScreenXController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual int getScreenX() const = 0;

        virtual void setScreenX(int inX) = 0;
    };  


    /**
     * Attribute: screenY
     * Type: integer
     * The vertical position at which the window appears on the screen.
     */
    class ScreenYController : public AttributeController
    {
    public:
        ScreenYController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual int getScreenY() const = 0;

        virtual void setScreenY(int inY) = 0;
    };    


    /**
     * Attribute: flex
     * Type: string (representing an integer)
     * Indicates the flexibility of the element, which indicates how
     * an element's container distributes remaining empty space among its
     * children.
     */
    class FlexController : public AttributeController
    {
    public:
        FlexController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual int getFlex() const = 0;

        virtual void setFlex(int inFlex) = 0;
    };


    /**
     * Attribute: disabled
     * Type: boolean
     * Indicates whether the element is disabled or not.
     */
    class DisabledController : public AttributeController
    {
    public:
        DisabledController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual bool isDisabled() const = 0;

        virtual void setDisabled(bool inDisabled) = 0;
    };


    /**
     * Attribute: selected
     * Type: boolean
     * Indicates whether the element is selected or not.
     */
    class SelectedController : public AttributeController
    {
    public:
        SelectedController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual bool isSelected() const = 0;

        virtual void setSelected(bool inSelected) = 0;
    };


    /**
     * Attribute: hidden
     * Type: boolean 
     * If set to true, the element is not displayed.
     */
    class HiddenController : public AttributeController
    {
    public:
        HiddenController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual bool isHidden() const = 0;

        virtual void setHidden(bool inHidden) = 0;
    };


    /**
     * Attribute: label
     * Type: string 
     * The label that will appear on the element. If this is left out, no text
     * appears.
     */
    class LabelController : public AttributeController
    {
    public:
        LabelController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual std::string getLabel() const = 0;

        virtual void setLabel(const std::string & inLabel) = 0;
    };


    /**
     * Attribute: value
     * Type: string
     * The string attribute allows you to associate a data value with an
     * element.
     */
    class StringValueController : public AttributeController
    {
    public:
        StringValueController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual std::string getValue() const = 0;

        virtual void setValue(const std::string & inStringValue) = 0;
    };


    /**
     * Attribute: readonly
     * Type: boolean
     * If set to true, then the user cannot change the value of the element.
     * However, the value may still be modified by a script.
     */
    class ReadOnlyController : public AttributeController
    {
    public:
        ReadOnlyController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual bool isReadOnly() const = 0;

        virtual void setReadOnly(bool inReadOnly) = 0;
    };


    /**
     * Attribute: rows
     * Type: integer
     * The number of rows to display in the element. If the element contains
     * more than this number of rows, a scrollbar will appear which the user
     * can use to scroll to the other rows.
     */
    class RowsController : public AttributeController
    {
    public:
        RowsController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual int getRows() const = 0;

        virtual void setRows(int inRows) = 0;
    };


    class CheckedController : public AttributeController
    {
    public:
        CheckedController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual bool isChecked() const = 0;

        virtual void setChecked(bool inChecked) = 0;
    };


    class OrientController : public AttributeController
    {
    public:
        OrientController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual Orient getOrient() const = 0;

        virtual void setOrient(Orient inOrient) = 0;
    };


    class AlignController : public AttributeController
    {
    public:
        AlignController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual Align getAlign() const = 0;

        virtual void setAlign(Align inAlign) = 0;
    };


    class IntValueController : public AttributeController
    {
    public:
        IntValueController(){}

        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual int getValue() const = 0;

        virtual void setValue(int inValue) = 0;
    };


    class SelectedIndexController : public AttributeController
    {
    public:
        SelectedIndexController(){}

        virtual void get(std::string & outSelectedIndex);

        virtual void set(const std::string & inSelectedIndex);

        virtual int getSelectedIndex() const = 0;

        virtual void setSelectedIndex(int inSelectedIndex) = 0;
    };


    class ScrollbarCurrentPositionController : public AttributeController
    {
    public:
        ScrollbarCurrentPositionController(){}

        virtual void get(std::string & outCurpos);

        virtual void set(const std::string & inCurpos);

        virtual int getCurrentPosition() const = 0;

        virtual void setCurrentPosition(int inCurrentPosition) = 0;
    };


    class ScrollbarMaxPositionController : public AttributeController
    {
    public:
        ScrollbarMaxPositionController(){}

        virtual void get(std::string & outMaxPosition);

        virtual void set(const std::string & inMaxPosition);

        virtual int getMaxPosition() const = 0;

        virtual void setMaxPosition(int inMaxPosition) = 0;
    };


    class ScrollbarIncrementController : public AttributeController
    {
    public:
        ScrollbarIncrementController(){}

        virtual void get(std::string & outIncrement);

        virtual void set(const std::string & inIncrement);

        virtual int getIncrement() const = 0;

        virtual void setIncrement(int inIncrement) = 0;
    };


    class ScrollbarPageIncrementController : public AttributeController
    {
    public:
        ScrollbarPageIncrementController(){}

        virtual void get(std::string & outPageIncrement);

        virtual void set(const std::string & inPageIncrement);

        virtual int getPageIncrement() const = 0;

        virtual void setPageIncrement(int inPageIncrement) = 0;
    };


    class SrcController : public AttributeController
    {
    public:
        SrcController(){}

        virtual void get(std::string & outPageIncrement);

        virtual void set(const std::string & inPageIncrement);

        virtual std::string getSrc() const = 0;

        virtual void setSrc(const std::string & inSrc) = 0;
    };


    /**
     * This is a non-standard extension that applies to Image elements only.
     * The attribute "keepaspectratio" has a boolean value indicating that the
     * resized width and height should be congruent with the natural width and
     * height.
     */
    class KeepAspectRatioController : public AttributeController
    {
    public:
        KeepAspectRatioController(){}

        virtual void get(std::string & outPageIncrement);

        virtual void set(const std::string & inPageIncrement);

        virtual bool getKeepAspectRatio() const = 0;

        virtual void setKeepAspectRatio(bool inKeepAspectRatio) = 0;
    };


    class PointsController : public AttributeController
    {
    public:
        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual const Points & getPoints() const = 0;

        virtual void setPoints(const Points & inPoints) = 0;
    };


    class PathInstructionsController : public AttributeController
    {
    public:
        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual const SVG::PathInstructions & getPathInstructions() const = 0;

        virtual void setPathInstructions(const SVG::PathInstructions & inPathInstructions) = 0;
    };


    class FillController : public AttributeController
    {
    public:
        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual void setFill(const RGBColor & inColor) = 0;

        virtual const RGBColor & getFill() const = 0;
    };


    class StrokeController : public AttributeController
    {
    public:
        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual void setStroke(const RGBColor & inColor) = 0;

        virtual const RGBColor & getStroke() const = 0;
    };


    class StrokeWidthController : public AttributeController
    {
    public:
        virtual void get(std::string & outValue);

        virtual void set(const std::string & inValue);

        virtual void setStrokeWidth(int inStrokeWidth) = 0;

        virtual int getStrokeWidth() const = 0;
    };

} // namespace XULWin


#endif // ATTRIBUTECONTROLLER_H_INCLUDED
