#ifndef _JMPEXPORTERBASE_H
#define _JMPEXPORTERBASE_H

namespace DAE2JMP
{
    class JMPExporter;
    
    class JMPExporterBase
    {
    public:
        JMPExporterBase(JMPExporter *exporter) : mJMPExporter(exporter)
        {}
        
    protected:
        JMPExporter *mJMPExporter;
    };
}

#endif /* _JMPEXPORTERBASE_H */
