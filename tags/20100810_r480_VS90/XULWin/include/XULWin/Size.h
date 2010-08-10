#ifndef SIZE_H_INCLUDED
#define SIZE_H_INCLUDED


namespace XULWin
{

    template<class T>
    class GenericSize
    {
    public:
        GenericSize() :
            mWidth(0),
            mHeight(0)
        {
        }

        GenericSize(const T & inWidth, const T & inHeight) :
            mWidth(inWidth),
            mHeight(inHeight)
        {
        }


        const T & width() const
        {
            return mWidth;
        }


        const T & height() const
        {
            return mHeight;
        }

    private:
        T mWidth;
        T mHeight;
    };

    typedef GenericSize<int> Size;
    typedef GenericSize<float> SizeF;

} // namespace XULWin


#endif // SIZE_H_INCLUDED
