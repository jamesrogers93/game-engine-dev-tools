#include "DAE2JMP/JMPExporterEntity.h"

#include "DAE2JMP/JMPExporter.h"

#include "game-engine/Entity/Entity.h"
#include "game-engine/Modules/Animation/JointEntity.h"

namespace DAE2JMP
{
    bool JMPExporterEntity::Export(const Entity* entity)
    {
        writeEntites(entity);
        return true;
    }
    
    
    void JMPExporterEntity::writeEntites(const Entity* entity)
    {
        
        writeEntityBase(entity);
        
        // Get entity type
        switch(entity->getType())
        {
            case Entity::JOINT:
            {
                writeJointEntity((JointEntity*) entity);
                break;
            }
            default:
                break;
        }
        
        // Write children
        for(unsigned int i = 0; i < entity->getChildren().size(); i++)
        {
            writeEntites(entity->getChildren()[i]);
        }
    }
    
    void JMPExporterEntity::writeEntityBase(const Entity *entity)
    {
        std::ofstream *output = this->mJMPExporter->getOutputStream();

        // Write type
        *output << entity->typeToString() << " ";
        
        // Write name
        *output << entity->getName() << " ";
        
        // Write parent name, if any
        if(entity->hasParent())
        {
             *output << entity->getParent()->getName();
        }
        else
        {
            *output << "root";
        }
        
        *output << std::endl;
        
    }
    
    void JMPExporterEntity::writeJointEntity(const JointEntity *joint)
    {
        std::ofstream *output = this->mJMPExporter->getOutputStream();
        
        //Write the bind pose
        for(unsigned int i = 0; i < 4; i++)
        {
            for(unsigned int j = 0; j < 4; j++)
            {
                *output << joint->getLocalBindTransform()[i][j] << " ";
            }
        }
        
        // Write the inverseBindPose
        for(unsigned int i = 0; i < 4; i++)
        {
            for(unsigned int j = 0; j < 4; j++)
            {
                *output << joint->getInverseBindTransform()[i][j] << " ";
            }
        }
        
        *output << std::endl;
    }
    
}
