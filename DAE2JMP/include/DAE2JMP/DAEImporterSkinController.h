#ifndef _DAEIMPORTERSKINCONTROLLER_H
#define _DAEIMPORTERSKINCONTROLLER_H

#include <string>
#include <vector>

#include "DAE2JMP/DAEImporterBase.h"
#include <COLLADAFWSkinController.h>

namespace DAE2JMP
{
    struct DAESkinController
    {
        unsigned long long meshId;
        unsigned long long skinId;
        
        std::vector<unsigned long long> jointIds;
        unsigned int numJointIds;
    };

    class DAEImporterSkinController : public DAEImporterBase
    {
        
    public:
        DAEImporterSkinController(DAEImporter *importer) : DAEImporterBase(importer)
        {}
        
        bool import(const COLLADAFW::SkinController*);
        
    private:
        
        DAESkinController getSkinControllerData(const COLLADAFW::SkinController*);
        
    };

}

#endif /* _DAEIMPORTERSKINCONTROLLER_H */
