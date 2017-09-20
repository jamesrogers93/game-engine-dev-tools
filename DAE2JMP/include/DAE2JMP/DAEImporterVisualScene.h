#ifndef _DAEIMPORTERVISUALSCENE_H
#define _DAEIMPORTERVISUALSCENE_H

#include <string>
#include <vector>

#include "DAE2JMP/DAEImporterBase.h"
#include <COLLADAFWVisualScene.h>

namespace DAE2JMP
{

    class DAEEntity
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
        
        std::vector<DAEEntity*> mChildren;
        
        DAEEntity(const Type &type) : mType(type)
        {}
        
    public:
        void addChild(DAEEntity *entity)
        {
            mChildren.push_back(entity);
        }
        
        void setName(const std::string &name) { this->mName = name; }
        const std::string& getName() { return this->mName; }
        
        unsigned int getNumChildren() { return this->mChildren.size(); }
        DAEEntity * getChild(const unsigned int &index) { return this->mChildren[index]; }
        
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
        
        float* getTransform() { return &this->mTransform[0]; }

        Type getType() { return mType; }
    };

    class DAENode : public DAEEntity
    {
    private:
        unsigned long long mGeometryId;
        unsigned long long mSkinControllerId;
        unsigned long long mMaterialId;
        //unsigned long long mSkeletonId;
        
        bool mHasGeometryId = false;
        bool mHasSkinControllerId = false;
        bool mHasMaterialId = false;
        //bool mHasSkeletonId = false;
        
    public:
        
        DAENode() : DAEEntity(NODE){}
        
        void setGeometryId(const unsigned long long geometryId)
        {
            this->mGeometryId = geometryId;
            this->mHasGeometryId = true;
        }
        
        void setSkinControllerId(const unsigned long long skinControllerId)
        {
            this->mSkinControllerId = skinControllerId;
            this->mHasSkinControllerId = true;
        }
        
        void setMaterialId(const unsigned long long materialId)
        {
            this->mMaterialId = materialId;
            this->mHasMaterialId = true;
        }
        
        const unsigned long long & getGeometryId()
        {
            return mGeometryId;
        }
        
        const unsigned long long & getSkinControllerId()
        {
            return this->mSkinControllerId;
        }
        
        const unsigned long long & getMaterialId()
        {
            return this->mMaterialId;
        }
        
        const bool &getHasGeometryId()
        {
            return this->mHasGeometryId;
        }
        
        const bool &getHasSkinControllerId()
        {
            return this->mHasSkinControllerId;
        }
        
        const bool &getHasMaterialId()
        {
            return this->mHasMaterialId;
        }
    };

    class DAEJoint : public DAEEntity
    {
    public:
        
        DAEJoint() : DAEEntity(JOINT)
        {}
        
    };

    struct DAEVisualScene
    {
        std::string name;
        
        std::vector<DAEEntity*> rootNodes;
        unsigned int rootNodeCount;
    };

    class DAEImporterVisualScene : public DAEImporterBase
    {
        
    public:
        DAEImporterVisualScene(DAEImporter *importer) : DAEImporterBase(importer)
        {}
        
        bool import(const COLLADAFW::VisualScene*);
        
    private:
        
        DAEVisualScene getVisualSceneData(const COLLADAFW::VisualScene*);
        
        DAEEntity* processEntityHierarchy(const COLLADAFW::Node *node);
        
    };
    
}

#endif /* _DAEIMPORTERVISUALSCENE_H */
