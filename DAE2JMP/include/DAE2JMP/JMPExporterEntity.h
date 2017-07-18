#ifndef _JMPEXPORTERENTITY_H
#define _JMPEXPORTERENTITY_H

#include <string>

#include "DAE2JMP/JMPExporterBase.h"

class Entity;
class JointEntity;

namespace DAE2JMP
{
    
    class JMPExporterEntity : public JMPExporterBase
    {
        
    public:
        JMPExporterEntity(JMPExporter *exporter) : JMPExporterBase(exporter)
        {}
        
        bool Export(const Entity*);
        
    private:
        
        void writeEntites(const Entity*);
        void writeEntityBase(const Entity*);
        void writeJointEntity(const JointEntity*);
    };
    
}

#endif /* _JMPEXPORTERENTITY_H */
