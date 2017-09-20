#ifndef _JMPEXPORTERANIMATION_H
#define _JMPEXPORTERANIMATION_H

#include <string>

#include "DAE2JMP/JMPExporterBase.h"

class Animation;

namespace DAE2JMP
{
    
    class JMPExporterAnimation : public JMPExporterBase
    {
        
    public:
        JMPExporterAnimation(JMPExporter *exporter) : JMPExporterBase(exporter)
        {}
        
        bool Export(const Animation*);
    };
    
}

#endif /* _JMPEXPORTERANIMATION_H */
