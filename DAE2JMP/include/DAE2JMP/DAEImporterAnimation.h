#ifndef _DAEIMPORTERANIMATION_H
#define _DAEIMPORTERANIMATION_H

#include <string>
#include <vector>

#include "DAE2JMP/DAEImporterBase.h"
#include <COLLADAFWAnimationCurve.h>

#include <glm/glm.hpp>

namespace DAE2JMP
{
    struct DAEAnimation
    {
        std::string name;
        
        std::vector<float> frames;
        std::vector<glm::mat4> transforms;
    };
    
    class DAEImporterAnimation : public DAEImporterBase
    {
        
    public:
        DAEImporterAnimation(DAEImporter *importer) : DAEImporterBase(importer)
        {}
        
        bool import(const COLLADAFW::AnimationCurve*);
        
    private:
        
        DAEAnimation getAnimationData(const COLLADAFW::AnimationCurve*);
        
    };
}

#endif /* _DAEIMPORTERANIMATION_H */
