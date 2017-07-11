#ifndef _DAEIMPORTERVISUALSCENE_H
#define _DAEIMPORTERVISUALSCENE_H

#include <string>
#include <vector>

#include "DAE2JMP/DAEImporterBase.h"
#include <COLLADAFWVisualScene.h>

struct VisualScene
{
    
    std::vector<VisualScene> children;
    
    unsigned long long instanceGeometryId;
    unsigned long long instanceControllerId;
};

class DAEImporterVisualScene : public DAEImporterBase
{
    
public:
    DAEImporterVisualScene(DAEImporter *importer) : DAEImporterBase(importer)
    {}
    
    bool import(const COLLADAFW::VisualScene*);
    
private:
    
    VisualScene getVisualSceneData(const COLLADAFW::VisualScene*);
    
};


#endif /* _DAEIMPORTERVISUALSCENE_H */
