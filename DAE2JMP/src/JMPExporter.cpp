#include "DAE2JMP/JMPExporter.h"

#include "DAE2JMP/JMPExporterMaterial.h"

namespace DAE2JMP
{

    bool JMPExporter::Export()
    {
        //this->mOutputStream.open(this->mConfig.outputFile, std::ios::binary | std::ios::trunc);
        
        // Export materials
        exportMaterials();
        
        // Export Geometries
        exportGeometries();
        
        // Export entity hierarchy
        //for(auto &entity : )
        
        
        return true;
    }
    
    void JMPExporter::exportMaterials()
    {
        for(auto& mat : this->mConfig.jmpData->getMaterials())
        {
            // Open file to write
            std::string path = this->mConfig.outputFolder + mat.first + ".jmpMat";
            
            this->mOutputStream.open(path, std::ios::binary | std::ios::trunc);
            
            exportMaterial(mat.second);
            
            this->mOutputStream.close();
        }
    }
    
    void JMPExporter::exportGeometries()
    {
        for(auto &geo : this->mConfig.jmpData->getGeometries())
        {
            // Open file to write
            std::string path = this->mConfig.outputFolder + geo.first + ".jmpMesh";
            
            this->mOutputStream.open(path, std::ios::binary | std::ios::trunc);
            
            exportGeometry(geo.second);
            
            this->mOutputStream.close();
        }
    }
    
    void JMPExporter::exportEntites()
    {
        
    }
    
    bool JMPExporter::exportMaterial(const Material* mat)
    {
        
        JMPExporterMaterial exporterMaterial(this);
        
        return exporterMaterial.Export(mat);
    }
    
    bool JMPExporter::exportGeometry(const Geometry*)
    {
        return true;
    }
    
    bool JMPExporter::exportEntity(const ENode*)
    {
        return true;
    }

}
