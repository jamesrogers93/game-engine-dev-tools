#include "DAE2JMP/DAEImporterVisualScene.h"

#include "DAE2JMP/DAEImporter.h"

#include <COLLADAFWUniqueId.h>

bool DAEImporterVisualScene::import(const COLLADAFW::VisualScene* scene)
{
    if(scene == NULL) return false;
    
    VisualScene sceneData = getVisualSceneData(scene);
    
    return this->mDAEImporter->addLoadedVisualScene(scene->getObjectId(), sceneData);
}

VisualScene DAEImporterVisualScene::getVisualSceneData(const COLLADAFW::VisualScene* scene)
{
    VisualScene sceneData;
    
    return sceneData;
}
