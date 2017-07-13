#include "DAE2JMP/DAEUtil.h"

namespace DAE2JMP
{

    float getFloatValue(const COLLADAFW::FloatOrDoubleArray& array, const unsigned int &index)
    {
        if(array.getType() == COLLADAFW::FloatOrDoubleArray::DATA_TYPE_DOUBLE)
        {
            return (float)array.getDoubleValues()->getData()[index];
        }
        else
        {
            return array.getFloatValues()->getData()[index];
        }
    }

}