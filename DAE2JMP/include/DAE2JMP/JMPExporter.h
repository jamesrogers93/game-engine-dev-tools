#ifndef _JMPEXPORTER_H
#define _JMPEXPORTER_H

// STD
#include <string>
#include <iostream>
#include <fstream>

#include "DAE2JMP/DAEImporter.h"
#include "DAE2JMP/DAE2JMPConfig.h"

class Material;
class Geometry;
class Entity;

namespace DAE2JMP
{

    class JMPExporter
    {
    public:
        
        JMPExporter(const DAE2JMPConfig &config) : mConfig(config)
        {
            
        }
        
        
        bool Export();
        
        std::ofstream* getOutputStream() { return &this->mOutputStream; }
        
    private:
        DAE2JMPConfig mConfig;
        
        std::ofstream mOutputStream;
        
        void exportMaterials();
        void exportMeshes();
        void exportEntites();
        
        bool exportMaterial(const Material*);
        bool exportMesh(const Mesh*);
        bool exportEntity(const Entity*);
    };

}

#endif /* _JMPEXPORTER_H */
