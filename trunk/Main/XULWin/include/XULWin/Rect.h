#ifndef RECT_H_INCLUDED
#define RECT_H_INCLUDED


#include "XULWin/Point.h"
#include "XULWin/Size.h"


namespace XULWin
{

    template<class T>
    class GenericRect
    {
    public:        
        GenericRect()
        {
        }

        GenericRect(const GenericPoint<T> & inLocation, const GenericSize<T> & inSize) :
            mLocation(inLocation),
            mSize(inSize)
        {
        }

        GenericRect(T inX, T inY, T inWidth, T inHeight) :
            mLocation(inX, inY),
            mSize(inWidth, inHeight)
        {
        }
        
        bool operator==(const GenericRect<T> & inOtherRect)
        {
            return mLocation == inOtherRect.mLocation && mSize == inOtherRect.mSize;
        }
                  
        bool operator!=(const GenericRect<T> & inOtherRect)
        {
            return !(inOtherRect == *this);
        }

        T x() const { return mLocation.x(); }

        T y() const { return mLocation.y(); }

        T width() const { return mSize.width(); }

        T height() const { return mSize.height(); }

        const GenericPoint<T> & location() const { return mLocation; }

        const GenericSize<T> & size() const { return mSize; }

    private:
        GenericPoint<T> mLocation;
        GenericSize<T> mSize;
    };

    typedef GenericRect<int> Rect;
    typedef GenericRect<float> RectF;

} // namespace XULWin


#endif // RECT_H_INCLUDED
