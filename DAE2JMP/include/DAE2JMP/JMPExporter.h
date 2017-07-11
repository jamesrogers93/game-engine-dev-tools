#ifndef _JMPEXPORTER_H
#define _JMPEXPORTER_H

// STD
#include <string>
#include <iostream>
#include <fstream>

#include "DAE2JMP/DAEImporter.h"
#include "DAE2JMP/JMPData.h"

class JMPExporter
{
public:
    struct JMPExporterConfig
    {
        std::string outputFile;
        JMPData *jmpData;
    };
    
    JMPExporter(const JMPExporterConfig &config) : mConfig(config)
    {
        
    }
    
    
    bool Export(const DAEImporter &importer);
    
private:
    JMPExporterConfig mConfig;
    
    std::ofstream mOutputStream;
};

#endif /* _JMPEXPORTER_H */
