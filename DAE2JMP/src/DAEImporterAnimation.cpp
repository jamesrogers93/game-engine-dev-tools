#include "DAE2JMP/DAEImporterAnimation.h"
#include "DAE2JMP/DAEUtil.h"
#include "DAE2JMP/DAEImporter.h"

#include <COLLADAFWUniqueId.h>

namespace DAE2JMP
{

    bool DAEImporterAnimation::import(const COLLADAFW::AnimationCurve *animationCurve)
    {
        DAEAnimation animationData = getAnimationData(animationCurve);
        
        return this->mDAEImporter->addLoadedAnimation(animationCurve->getObjectId(), animationData);
    }
    
    DAEAnimation DAEImporterAnimation::getAnimationData(const COLLADAFW::AnimationCurve *animationCurve)
    {
        DAEAnimation animationData;
        
        // Get the name
        animationData.name = animationCurve->getName();
        
        // Get the frames
        std::vector<float> frames;
        for(unsigned int i = 0; i < animationCurve->getInputValues().getValuesCount(); i++)
        {
            frames.push_back(getFloatValue(animationCurve->getInputValues(), i));
        }
        animationData.frames = frames;
        
        // Get the transforms
        std::vector<glm::mat4> transforms;
        for(unsigned int i = 0; i < animationCurve->getOutputValues().getValuesCount(); i+=16)
        {
            glm::mat4 transform;
            unsigned int index = i;
            for(unsigned int j = 0; j < 4; j++)
            {
                for(unsigned int k = 0; k < 4; k++)
                {
                    transform[k][j] = getFloatValue(animationCurve->getOutputValues(), index++);
                }
            }
            transforms.push_back(transform);
        }
        animationData.transforms = transforms;
        
        return animationData;
    }
}
