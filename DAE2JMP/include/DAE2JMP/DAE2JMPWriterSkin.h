#ifndef _DAE2JMPWRITERSKIN_H
#define _DAE2JMPWRITERSKIN_H

#include "DAE2JMP/DAE2JMPWriterBase.h"

#include <COLLADAFWSkinControllerData.h>

class WriterSkin : public WriterBase
{
private:
    
    const COLLADAFW::SkinControllerData* mSkin;
    
public:
    WriterSkin(JMPWriter *writer, const COLLADAFW::SkinControllerData* skin) : WriterBase(writer), mSkin(skin)
    {}
    
    bool write();
};


#endif /* _DAE2JMPWRITERSKIN_H */


