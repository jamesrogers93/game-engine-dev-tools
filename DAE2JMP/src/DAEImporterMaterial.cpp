#include "DAE2JMP/DAEImporterMaterial.h"

#include "DAE2JMP/DAEImporter.h"

#include <COLLADAFWUniqueId.h>

bool DAEImporterMaterial::import(const COLLADAFW::Material* mat)
{
    if(mat == NULL) return false;
    
    Material matData = getMaterialData(mat);
    
    return this->mDAEImporter->addLoadedMaterial(mat->getObjectId(), matData);
}

Material DAEImporterMaterial::getMaterialData(const COLLADAFW::Material* mat)
{
    Material matData;
    
    matData.effectId = mat->getInstantiatedEffect().getObjectId();
    
    matData.name = mat->getName();
    
    return matData;
}
