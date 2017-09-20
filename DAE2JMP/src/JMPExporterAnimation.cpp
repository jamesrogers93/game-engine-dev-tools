#include "DAE2JMP/JMPExporterAnimation.h"

#include "DAE2JMP/JMPExporter.h"

#include <game-engine/Modules/Animation/Animation.h>
#include <game-engine/Modules/Animation/JointAnimation.h>
#include <game-engine/Modules/Animation/KeyFrame.h>
#include <game-engine/Modules/Animation/JointTransform.h>

namespace DAE2JMP
{
    bool JMPExporterAnimation::Export(const Animation *animation)
    {
        std::ofstream *output = this->mJMPExporter->getOutputStream();
        
        std::map<std::string, JointAnimation> jointAnimations = animation->getJointAnimations();
        
        *output << animation->getLength() << std::endl;
        *output << jointAnimations.size() << std::endl;
        
        for(auto &joint : jointAnimations)
        {
            std::vector<KeyFrame> keyFrames = joint.second.getKeyFrames();
            unsigned int numKeyFrames = keyFrames.size();
            
            *output << joint.first << std::endl;
            *output << numKeyFrames << std::endl;
            *output << joint.second.getLength() << std::endl;
            
            for(unsigned int i = 0; i < numKeyFrames; i++)
            {
                *output << keyFrames[i].getTimeStamp() << std::endl;
                
                glm::vec4 position = keyFrames[i].getJointTransform().getPosition();
                glm::fquat rotation = keyFrames[i].getJointTransform().getRotation();
                
                *output << position.x << " " << position.y << " " << position.z << " " << position.w << std::endl;
                *output << rotation.x << " " << rotation.y << " " << rotation.z << " " << rotation.w << std::endl;
            }
        }
        
        return true;
    }
}
