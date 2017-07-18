#include "DAE2JMP/JMPExporter.h"

#include "DAE2JMP/JMPExporterMaterial.h"
#include "DAE2JMP/JMPExporterMesh.h"
#include "DAE2JMP/JMPExporterEntity.h"
#include "DAE2JMP/JMPExporterProperty.h"

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
        
        // Export properties
        exportProperties();
        
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
        for(auto &entity : this->mConfig.jmpData->getEntites())
        {
            // Open file to write
            std::string path = this->mConfig.outputFolder + entity.first + ".jmpEntity";
            
            this->mOutputStream.open(path, std::ios::binary | std::ios::trunc);
            
            exportEntity(entity.second);
            
            this->mOutputStream.close();
        }
        
    }
    
    void JMPExporter::exportProperties()
    {
        for(auto &property : this->mConfig.jmpData->getProperties())
        {
            // Open file to write
            std::string path = this->mConfig.outputFolder + property.first + ".jmpProperty";
            
            this->mOutputStream.open(path, std::ios::binary | std::ios::trunc);
            
            exportProperty(property.second);
            
            this->mOutputStream.close();
        }
    }
    
    bool JMPExporter::exportMaterial(const Material* mat)
    {
        std::cout << "Export material" << std::endl;
        
        JMPExporterMaterial exporterMaterial(this);
        return exporterMaterial.Export(mat);
    }
    
    bool JMPExporter::exportMesh(const Mesh* mesh)
    {
        std::cout << "Export mesh" << std::endl;
        
        JMPExporterMesh exporterMesh(this);
        return exporterMesh.Export(mesh);
    }
    
    bool JMPExporter::exportEntity(const Entity* entity)
    {
        std::cout << "Export entity" << std::endl;
        
        JMPExporterEntity exporterEntity(this);
        return exporterEntity.Export(entity);
    }
    
    bool JMPExporter::exportProperty(const Property* property)
    {
        std::cout << "Export property" << std::endl;
        
        JMPExporterProperty exporterProperty(this);
        return exporterProperty.Export(property);
    }
}
