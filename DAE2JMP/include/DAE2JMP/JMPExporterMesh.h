#ifndef _JMPEXPORTERGEOMETRY_H
#define _JMPEXPORTERGEOMETRY_H

#include <string>

#include "DAE2JMP/JMPExporterBase.h"

class Mesh;

namespace DAE2JMP
{
    
    class JMPExporterMesh : public JMPExporterBase
    {
    private:
        static const std::string POSITION;
        static const std::string NORMAL;
        static const std::string UV0;
        static const std::string COLOUR;
        static const std::string JOINT_ID;
        static const std::string JOINT_WEIGHT;
        
    public:
        JMPExporterMesh(JMPExporter *exporter) : JMPExporterBase(exporter)
        {}
        
        bool Export(const Mesh*);
    };
    
}

#endif /* _JMPEXPORTERGEOMETRY_H */
