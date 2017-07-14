#ifndef _JMPEXPORTERMATERIAL_H
#define _JMPEXPORTERMATERIAL_H

#include <string>

#include "DAE2JMP/JMPExporterBase.h"

class Material;

namespace DAE2JMP
{
    
    class JMPExporterMaterial : public JMPExporterBase
    {
        
    public:
        JMPExporterMaterial(JMPExporter *exporter) : JMPExporterBase(exporter)
        {}
        
        bool Export(const Material*);
    };
    
}

#endif /* _JMPEXPORTERMATERIAL_H */
