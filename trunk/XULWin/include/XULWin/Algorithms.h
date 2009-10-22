#ifndef ALGORITHMS_H_INCLUDED
#define ALGORITHMS_H_INCLUDED


namespace XULWin
{
    
    template<class DataType, class FwdIt, class GetValueFunction>
    DataType max_element_value(FwdIt begin, FwdIt end, DataType inDefault, GetValueFunction getValue)
    {
        if (begin == end)
        {
            return inDefault;
        }

        DataType result = getValue(*begin);
        ++begin;
        while (begin != end)
        {
            result = std::max<DataType>(result, getValue(*begin));
            ++begin;
        }
        return result;
    }
    
    template<class DataType, class FwdIt, class GetValueFunction>
    DataType sum_element_values(FwdIt begin, FwdIt end, DataType inDefault, GetValueFunction getValue)
    {
        if (begin == end)
        {
            return inDefault;
        }

        DataType result = getValue(*begin);
        ++begin;
        while (begin != end)
        {
            result += getValue(*begin);
            ++begin;
        }
        return result;
    }

} // namespace XULWin


#endif // ALGORITHMS_H_INCLUDED
