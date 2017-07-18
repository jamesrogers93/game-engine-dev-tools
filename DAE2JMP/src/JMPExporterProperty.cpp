#include "DAE2JMP/JMPExporterProperty.h"

#include "DAE2JMP/JMPExporter.h"

#include "game-engine/Entity/Property.h"
#include "game-engine/Modules/Graphics/MeshProperty.h"
#include "game-engine/Modules/Graphics/AnimatableMeshProperty.h"

namespace DAE2JMP
{
    bool JMPExporterProperty::Export(const Property* property)
    {
        writeProperties(property);
        return true;
    }
    
    
    void JMPExporterProperty::writeProperties(const Property* property)
    {
        
        writePropertyBase(property);
        
        // Get entity type
        switch(property->getType())
        {
            case Property::MESH:
            {
                writeMeshProperty((MeshProperty*) property);
                break;
            }
            case Property::ANIMATABLE_MESH:
            {
                writeMeshProperty((MeshProperty*) property);
                writeAnimatableMeshProperty((AnimatableMeshProperty*) property);
                break;
            }
            default:
                break;
        }
    }
    
    void JMPExporterProperty::writePropertyBase(const Property *property)
    {
        std::ofstream *output = this->mJMPExporter->getOutputStream();
        
        // Write type
        *output << property->typeToString() << " ";
        
        // Write name
        *output << property->getName() << " ";
        
        *output << std::endl;
        
    }
    
    void JMPExporterProperty::writeMeshProperty(const MeshProperty *meshProperty)
    {
        std::ofstream *output = this->mJMPExporter->getOutputStream();
        
        // Write type
        if(meshProperty->hasMeshKey())
        {
            *output << "-G " << meshProperty->getMeshKey() << " ";
        }
        
        // Write name
        if(meshProperty->hasMaterialKey())
        {
            *output << "-M " << meshProperty->getMaterialKey() << " ";
        }
        
        // Write type
        if(meshProperty->hasShaderKey())
        {
            *output << "-S " << meshProperty->getShaderKey() << " ";
        }
        
        *output << std::endl;
    }
    
    void JMPExporterProperty::writeAnimatableMeshProperty(const AnimatableMeshProperty *meshProperty)
    {
        std::ofstream *output = this->mJMPExporter->getOutputStream();
        
        *output << "-J ";
        
        for(unsigned int i = 0; i < meshProperty->getJointKeys().size(); i++)
        {
            *output << meshProperty->getJointKeys()[i] << "$";
        }
        
        *output << std::endl;
    }
    
}
