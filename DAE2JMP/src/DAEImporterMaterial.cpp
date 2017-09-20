#include "DAE2JMP/DAEImporterMaterial.h"

#include "DAE2JMP/DAEImporter.h"

#include <COLLADAFWUniqueId.h>

namespace DAE2JMP
{

    bool DAEImporterMaterial::import(const COLLADAFW::Material* mat)
    {
        if(mat == NULL) return false;
        
        DAEMaterial matData = getMaterialData(mat);
        
        return this->mDAEImporter->addLoadedMaterial(mat->getObjectId(), matData);
    }

    DAEMaterial DAEImporterMaterial::getMaterialData(const COLLADAFW::Material* mat)
    {
        DAEMaterial matData;
        
        matData.effectId = mat->getInstantiatedEffect().getObjectId();
        
        matData.name = mat->getName();
        
        return matData;
    }

}
