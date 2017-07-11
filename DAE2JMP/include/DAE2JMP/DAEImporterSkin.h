#ifndef _DAEIMPORTERSKIN_H
#define _DAEIMPORTERSKIN_H

#include <map>
#include <vector>
#include <string>

#include "DAE2JMP/DAEImporterBase.h"
#include <COLLADAFWSkinControllerData.h>

//struct SkinVertex;
//typedef std::map<SkinVertex, int> SkinIndexMap;

struct SkinVertex
{
    std::vector<int> jointIndices;
    std::vector<unsigned int> weightIndices;
    
    SkinVertex(std::vector<int> &jointIndices, std::vector<unsigned int> &weightIndices) : jointIndices(jointIndices), weightIndices(weightIndices)
    {}
    
    bool operator<(const SkinVertex& rhs) const;
};

struct Skin
{
    std::string name;
    
    
    std::vector<std::string> jointNames;
    std::vector<int> jointIDs;
    std::vector<float> weights;
};

class DAEImporterSkin : public DAEImporterBase
{
private:
    static const float MAX_JOINT_INFLUENCE;
    
public:
    DAEImporterSkin(DAEImporter *importer) : DAEImporterBase(importer)
    {}
    
    bool import(const COLLADAFW::SkinControllerData*);
    
private:
    
    Skin getSkinData(const COLLADAFW::SkinControllerData*);
    //void addSkinVertex(SkinIndexMap&, Skin&, SkinVertex&);
    void addSkinVertex(Skin&, SkinVertex&);
    
};


#endif /* _DAE2JMPWRITERSKIN_H */


