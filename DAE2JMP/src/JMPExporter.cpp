#include "DAE2JMP/JMPExporter.h"

#include "DAE2JMP/JMPExporterMaterial.h"
#include "DAE2JMP/JMPExporterGeometry.h"

namespace DAE2JMP
{

    bool JMPExporter::Export()
    {
        // Export materials
        exportMaterials();
        
        // Export Geometries
        exportMeshes();
        
        // Export entity hierarchy
        exportEntites();
        
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
    
    void JMPExporter::exportMeshes()
    {
        for(auto &mesh : this->mConfig.jmpData->getMeshes())
        {
            // Open file to write
            std::string path = this->mConfig.outputFolder + mesh.first + ".jmpMesh";
            
            this->mOutputStream.open(path, std::ios::binary | std::ios::trunc);
            
            exportMesh(mesh.second);
            
            this->mOutputStream.close();
        }
    }
    
    void JMPExporter::exportEntites()
    {
        std::string path = this->mConfig.inputFile;
        std::string name;
        
        size_t sep = path.find_last_of("\\/");
        if (sep != std::string::npos)
            path = path.substr(sep + 1, path.size() - sep - 1);
        
        size_t dot = path.find_last_of(".");
        if (dot != std::string::npos)
        {
            name = path.substr(0, dot);
        }
        else
        {
            name = path;
        }
        
    }
    
    bool JMPExporter::exportMaterial(const Material* mat)
    {
        
        JMPExporterMaterial exporterMaterial(this);
        
        return exporterMaterial.Export(mat);
    }
    
    bool JMPExporter::exportMesh(const Mesh* mesh)
    {
        JMPExporterMesh exporterMesh(this);
        
        return exporterMesh.Export(mesh);
    }
    
    bool JMPExporter::exportEntity(const Entity*)
    {
        return true;
    }

}
