#include "DAE2JMP/DAEImporterSkinController.h"

#include "DAE2JMP/DAEImporter.h"

#include <COLLADAFWUniqueId.h>

namespace DAE2JMP
{

    bool DAEImporterSkinController::import(const COLLADAFW::SkinController* skinController)
    {
        if(skinController == NULL) return false;
        
        DAESkinController skinContollerData = getSkinControllerData(skinController);
        
        return this->mDAEImporter->addLoadedSkinController(skinController->getObjectId(), skinContollerData);
    }

    DAESkinController DAEImporterSkinController::getSkinControllerData(const COLLADAFW::SkinController* skinController)
    {
        DAESkinController skinControllerData;
        
        // get mesh Id this controller points to
        skinControllerData.meshId = skinController->getSource().getObjectId();
        
        // Get skin data Id this contoller points to
        skinControllerData.skinId = skinController->getSkinControllerData().getObjectId();
        
        // Get array of joints that this contoller points to
        const COLLADAFW::UniqueIdArray *jointIds = &skinController->getJoints();
        
        // Get count of joints
        skinControllerData.numJointIds = jointIds->getCount();
        
        // Copy joint Ids
        skinControllerData.jointIds.reserve(skinControllerData.numJointIds);
        for(unsigned int i = 0; i < skinControllerData.numJointIds; i++)
        {
            skinControllerData.jointIds.push_back(jointIds->operator[](i).getObjectId());
        }
        
        return skinControllerData;
    }

}
