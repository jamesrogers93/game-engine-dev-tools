#ifndef _JMPEXPORTERPROPERTY_H
#define _JMPEXPORTERPROPERTY_H

#include <string>

#include "DAE2JMP/JMPExporterBase.h"

class Property;
class MeshProperty;
class AnimatableMeshProperty;

namespace DAE2JMP
{
    
    class JMPExporterProperty : public JMPExporterBase
    {
        
    public:
        JMPExporterProperty(JMPExporter *exporter) : JMPExporterBase(exporter)
        {}
        
        bool Export(const Property*);
        
    private:
        
        void writeProperties(const Property*);
        void writePropertyBase(const Property*);
        void writeMeshProperty(const MeshProperty*);
        void writeAnimatableMeshProperty(const AnimatableMeshProperty*);
    };
    
}

#endif /* _JMPEXPORTERPROPERTY_H */
