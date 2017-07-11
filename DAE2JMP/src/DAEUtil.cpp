#include "DAE2JMP/DAEUtil.h"

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
