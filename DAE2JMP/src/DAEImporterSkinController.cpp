#include "DAE2JMP/DAEImporterSkinController.h"

#include "DAE2JMP/DAEImporter.h"

#include <COLLADAFWUniqueId.h>

bool DAEImporterSkinController::import(const COLLADAFW::SkinController* skinController)
{
    if(skinController == NULL) return false;
    
    SkinController skinContollerData = getSkinControllerData(skinController);
    
    return this->mDAEImporter->addLoadedSkinController(skinController->getObjectId(), skinContollerData);
}

SkinController DAEImporterSkinController::getSkinControllerData(const COLLADAFW::SkinController* skinController)
{
    SkinController skinControllerData;
    
    // get mesh id this controller points to
    skinControllerData.meshID = skinController->getSource().getObjectId();
    
    // Get skin data id this contoller points to
    skinControllerData.skinID = skinController->getSkinControllerData().getObjectId();
    
    // Get array of joints that this contoller points to
    const COLLADAFW::UniqueIdArray *jointIDs = &skinController->getJoints();
    
    // Get count of joints
    skinControllerData.numJointIDs = jointIDs->getCount();
    
    // Copy joint ids
    skinControllerData.jointIds.reserve(skinControllerData.numJointIDs);
    for(unsigned int i = 0; i < skinControllerData.numJointIDs; i++)
    {
        skinControllerData.jointIds.push_back(jointIDs->operator[](i).getObjectId());
    }
    
    return skinControllerData;
}
