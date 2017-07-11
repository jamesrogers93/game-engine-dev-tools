#ifndef _DAEIMPORTERVISUALSCENE_H
#define _DAEIMPORTERVISUALSCENE_H

#include <string>
#include <vector>

#include "DAE2JMP/DAEImporterBase.h"
#include <COLLADAFWVisualScene.h>

class Entity
{
public:
    enum Type
    {
        NODE,
        JOINT
    };
protected:
    
    const Type mType;
    
    std::string mName;
    
    float mTransform[16];
    
    std::vector<Entity*> children;
    
    Entity(const Type &type) : mType(type)
    {}
    
public:
    void addChild(Entity *entity)
    {
        children.push_back(entity);
    }
    
    void setName(const std::string &name) { this->mName = name; }
    const std::string& getName() { return this->mName; }
    
    void setTransformIdentity()
    {
        this->mTransform[0] = 1.0;
        this->mTransform[1] = 0.0;
        this->mTransform[2] = 0.0;
        this->mTransform[3] = 0.0;
        
        this->mTransform[4] = 0.0;
        this->mTransform[5] = 1.0;
        this->mTransform[6] = 0.0;
        this->mTransform[7] = 0.0;
        
        this->mTransform[8] = 0.0;
        this->mTransform[9] = 0.0;
        this->mTransform[10] = 1.0;
        this->mTransform[11] = 0.0;
        
        this->mTransform[12] = 0.0;
        this->mTransform[13] = 0.0;
        this->mTransform[14] = 0.0;
        this->mTransform[15] = 1.0;
    }
    
    void setTransform(const float transform[16])
    {
        for(int i = 0; i < 16; i++)
        {
            this->mTransform[i] = transform[i];
        }
    }
    
public:
    Type getType() { return mType; }
};

class Node : public Entity
{
private:
    unsigned long long mInstanceGeometryId;
    unsigned long long mInstanceControllerId;
    
    bool mHasInstanceGeometryId;
    bool mHasInstanceControllerId;
    
public:
    
    Node() : Entity(NODE){}
    
    void setInstanceGeometryId(const unsigned long long instanceGeometryId)
    {
        this->mInstanceGeometryId = instanceGeometryId;
        this->mHasInstanceGeometryId = true;
    }
    
    void setInstanceControllerId(const unsigned long long instanceControllerId)
    {
        this->mInstanceControllerId = instanceControllerId;
        this->mHasInstanceControllerId = true;
    }
    
    const unsigned long long & getInstanceGeometryId()
    {
        return mInstanceGeometryId;
    }
    
    const unsigned long long & getInstanceControllerId()
    {
        return this->mInstanceControllerId;
    }
    
    const bool &getHasInstanceGeomeryId()
    {
        return this->mHasInstanceGeometryId;
    }
    
    const bool &getHasInstanceControllerId()
    {
        return this->mHasInstanceControllerId;
    }
};

class Joint : public Entity
{
public:
    
    Joint() : Entity(JOINT)
    {}
};

struct VisualScene
{
    std::string name;
    
    std::vector<Entity*> rootNodes;
    unsigned int rootNodeCount;
};

class DAEImporterVisualScene : public DAEImporterBase
{
    
public:
    DAEImporterVisualScene(DAEImporter *importer) : DAEImporterBase(importer)
    {}
    
    bool import(const COLLADAFW::VisualScene*);
    
private:
    
    VisualScene getVisualSceneData(const COLLADAFW::VisualScene*);
    
    Entity* processEntityHierarchy(const COLLADAFW::Node *node);
    
};


#endif /* _DAEIMPORTERVISUALSCENE_H */
