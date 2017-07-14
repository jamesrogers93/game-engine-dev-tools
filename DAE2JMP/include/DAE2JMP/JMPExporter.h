#ifndef _JMPEXPORTER_H
#define _JMPEXPORTER_H

// STD
#include <string>
#include <iostream>
#include <fstream>

#include "DAE2JMP/DAEImporter.h"
#include "DAE2JMP/JMPData.h"

class Material;
class Geometry;
class ENode;

namespace DAE2JMP
{

    class JMPExporter
    {
    public:
        struct JMPExporterConfig
        {
            std::string outputFolder;
            JMPData *jmpData;
        };
        
        JMPExporter(const JMPExporterConfig &config) : mConfig(config)
        {
            
        }
        
        
        bool Export();
        
        std::ofstream* getOutputStream() { return &this->mOutputStream; }
        
    private:
        JMPExporterConfig mConfig;
        
        std::ofstream mOutputStream;
        
        void exportMaterials();
        void exportGeometries();
        void exportEntites();
        
        bool exportMaterial(const Material*);
        bool exportGeometry(const Geometry*);
        bool exportEntity(const ENode*);
    };

}

#endif /* _JMPEXPORTER_H */
