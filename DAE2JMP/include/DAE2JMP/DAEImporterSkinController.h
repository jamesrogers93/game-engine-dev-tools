#ifndef _DAEIMPORTERSKINCONTROLLER_H
#define _DAEIMPORTERSKINCONTROLLER_H

#include <string>
#include <vector>

#include "DAE2JMP/DAEImporterBase.h"
#include <COLLADAFWSkinController.h>

struct SkinController
{
    unsigned long long meshID;
    unsigned long long skinID;
    
    std::vector<unsigned long long> jointIds;
    unsigned int numJointIDs;
};

class DAEImporterSkinController : public DAEImporterBase
{
    
public:
    DAEImporterSkinController(DAEImporter *importer) : DAEImporterBase(importer)
    {}
    
    bool import(const COLLADAFW::SkinController*);
    
private:
    
    SkinController getSkinControllerData(const COLLADAFW::SkinController*);
    
};


#endif /* _DAEIMPORTERSKINCONTROLLER_H */
