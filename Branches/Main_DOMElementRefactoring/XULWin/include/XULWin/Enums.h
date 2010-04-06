#ifndef ENUMS_H_INCLUDED
#define ENUMS_H_INCLUDED


namespace XULWin
{
    
    enum WindowPos
    {
        WindowPos_Default,
        WindowPos_CenterInScreen
    };


    enum DialogResult
    {
        DialogResult_Ok,
        DialogResult_Cancel
    };


    enum SizeConstraint
    {
        Minimum,
        Preferred,
        Maximum
    };


    enum Orient
    {
        Horizontal,
        Vertical
    };


    enum Align
    {
        Start,
        Center,
        End,
        Stretch
    };


    enum CSSTextAlign
    {
        CSSTextAlign_Left,
        CSSTextAlign_Center,
        CSSTextAlign_Right,
        //CSSTextAlign_Justify // not supported atm
    };


    enum CSSOverflow
    {
        CSSOverflow_Visible,
        CSSOverflow_Hidden,
        CSSOverflow_Scroll,
        CSSOverflow_Auto
    };


} // namespace XULWin


#endif // ENUMS_H_INCLUDED
