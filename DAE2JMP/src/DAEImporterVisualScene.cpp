#include "DAE2JMP/DAEImporterVisualScene.h"

#include "DAE2JMP/DAEImporter.h"

#include <COLLADAFWNode.h>
#include <COLLADAFWUniqueId.h>
#include <COLLADAFWInstanceGeometry.h>
#include <COLLADAFWInstanceController.h>
#include <COLLADAFWTransformation.h>
#include <COLLADAFWMatrix.h>
#include <COLLADABUMathMatrix4.h>

bool DAEImporterVisualScene::import(const COLLADAFW::VisualScene* scene)
{
    if(scene == NULL) return false;
    
    VisualScene sceneData = getVisualSceneData(scene);
    
    return this->mDAEImporter->addLoadedVisualScene(scene->getObjectId(), sceneData);
}

VisualScene DAEImporterVisualScene::getVisualSceneData(const COLLADAFW::VisualScene* scene)
{
    VisualScene sceneData;
    
    const COLLADAFW::NodePointerArray *rootNodes = &scene->getRootNodes();
    
    sceneData.rootNodeCount = rootNodes->getCount();
    
    sceneData.rootNodes.reserve(sceneData.rootNodeCount);
    
    for(unsigned int i = 0; i < sceneData.rootNodeCount; i++)
    {
        const COLLADAFW::Node *node = rootNodes->operator[](i);
        Entity *entity = processEntityHierarchy(node);
        
        sceneData.rootNodes.push_back(entity);
    }
    

    
    return sceneData;
}

Entity* DAEImporterVisualScene::processEntityHierarchy(const COLLADAFW::Node *node)
{
    Entity *entity;
    
    // Get node type
    if(node->getType() == COLLADAFW::Node::NODE)
    {
        // Its a NODE, may have geomerty and/or controller instances
        entity = new Node();
        Node* nodeData = (Node*) entity;
        
        // If this node has an instance geometry, set it.
        const COLLADAFW::InstanceGeometryPointerArray* instanceGeometries = &node->getInstanceGeometries();
        if(instanceGeometries->getCount() > 0)
        {
            const COLLADAFW::InstanceGeometry* instanceGeometry = instanceGeometries->operator[](0);
            
            nodeData->setInstanceGeometryId(instanceGeometry->getUniqueId());
        }
        
        // If this node has an instance controller, set it.
        const COLLADAFW::InstanceControllerPointerArray* instanceControllers = &node->getInstanceControllers();
        if(instanceControllers->getCount() > 0)
        {
            const COLLADAFW::InstanceController* instanceController = instanceControllers->operator[](0);
            
            nodeData->setInstanceControllerId(instanceController->getUniqueId());
        }
        
    }
    else
    {
        // Must be type JOINT
        entity = new Joint();
        //Joint *jointData = (Joint*) entity;
        
        // Do nothing
        
    }
    
    // Get transformation data
    
    const COLLADAFW::TransformationPointerArray* transformations = &node->getTransformations();
    
    if(transformations->getCount() == 0)
    {
        entity->setTransformIdentity();
    }
    else
    {
        COLLADAFW::Transformation::TransformationType type = transformations->operator[](0)->getTransformationType();
    
        if(type == COLLADAFW::Transformation::MATRIX)
        {
            
            COLLADAFW::Matrix* matrix = (COLLADAFW::Matrix*)transformations->operator[](0);
            
            COLLADABU::Math::Matrix4 matrix4 = matrix->getMatrix();
            
            float transformation[16];
            transformation[0] = matrix4[0][0];
            transformation[1] = matrix4[1][0];
            transformation[2] = matrix4[2][0];
            transformation[3] = matrix4[3][0];
            
            transformation[4] = matrix4[0][1];
            transformation[5] = matrix4[1][1];
            transformation[6] = matrix4[2][1];
            transformation[7] = matrix4[3][1];
            
            transformation[8] = matrix4[0][2];
            transformation[9] = matrix4[1][2];
            transformation[10] = matrix4[2][2];
            transformation[11] = matrix4[3][2];
            
            transformation[12] = matrix4[0][3];
            transformation[13] = matrix4[1][3];
            transformation[14] = matrix4[2][3];
            transformation[15] = matrix4[3][3];
            
            entity->setTransform(transformation);
            
            
        }
        else
        {
            std::cout << "Unhandled transformation type" << std::endl;
        }
    }
    
    // Get name
    entity->setName(node->getName());
    
    // Now recursively process the child node, if any.
    const COLLADAFW::NodePointerArray *children = &node->getChildNodes();
    
    unsigned int numChildren = children->getCount();
    
    for(unsigned int i = 0; i < numChildren; i++)
    {
        const COLLADAFW::Node *childNode = children->operator[](i);
        Entity *childEntity = processEntityHierarchy(childNode);
        
        entity->addChild(childEntity);
    }
    
    return entity;
}
