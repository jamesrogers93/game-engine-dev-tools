#ifndef _JMPDATA_H
#define _JMPDATA_H

#include <map>
#include <string>

//#include <game-engine/Core/Entity/ENode.h>
#include <game-engine/Core/Modules/Graphics/Geometry.h>
#include <game-engine/Core/Modules/Graphics/Vertex.h>

class Material;
class Geometry;
class ENode;
class AnimatableEntity;

namespace DAE2JMP
{
    class DAEImporter;
    class DAEEntity;
    struct DAEMesh;
    struct DAESkin;
    struct DAEMaterial;
    struct DAESkinController;
    
    typedef std::map<VertexIndex, int> VertexIndexMap;

    class JMPData
    {
    private:
        std::map<std::string, Geometry*> geometries;
        std::map<std::string, Material*> materials;
        std::map<std::string, ENode*> entities;
        
    public:
        JMPData(){}
        
        void convert(DAEImporter *);
        
    private:
        ENode* processVisualScene(DAEEntity *, DAEImporter *);
        
        void processSkinController(AnimatableEntity *, const unsigned long long &, DAEImporter *);
        
        Geometry* processGeometry(const DAEMesh*, const DAESkin*);
        
        void addVertex(VertexIndexMap &vertexOrderMap, VertexIndex &vertex, const DAEMesh* mesh, const DAESkin* skin, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
        
        std::string processMaterial(const unsigned long long &, DAEImporter*);
        
        std::vector<std::string> processJointNames(DAESkinController*, DAEImporter*);
        
    };

}

#endif /* _JMPDATA_H */
