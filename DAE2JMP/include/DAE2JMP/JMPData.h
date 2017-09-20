#ifndef _JMPDATA_H
#define _JMPDATA_H

#include <map>
#include <string>

#include <game-engine/Modules/Graphics/Mesh.h>
#include <game-engine/Modules/Graphics/Vertex.h>

class Material;
class Mesh;
class Entity;
class JointEntity;
class Property;
class Animation;

namespace DAE2JMP
{
    class DAEImporter;
    class DAEEntity;
    class DAENode;
    class DAEJoint;
    struct DAEMesh;
    struct DAESkin;
    struct DAEMaterial;
    struct DAESkinController;
    
    typedef std::map<VertexIndex, int> VertexIndexMap;

    class JMPData
    {
    private:
        std::map<std::string, Mesh*> meshes;
        std::map<std::string, Material*> materials;
        std::map<std::string, Entity*> entites;
        std::map<std::string, Property*> properties;
        std::map<std::string, Animation*> animations;
        
    public:
        JMPData(){}
        
        void convert(DAEImporter *);
        
        std::map<std::string, Mesh*>& getMeshes() { return this->meshes; }
        std::map<std::string, Material*>& getMaterials() { return this->materials; }
        std::map<std::string, Entity*>& getEntites() { return this->entites; }
        std::map<std::string, Property*>& getProperties() { return this->properties; }
        std::map<std::string, Animation*>& getAnimations() { return this->animations; }
        
    private:
        
        Mesh* processMesh(const DAEMesh*, const DAESkin*);
        Mesh* processMesh(const DAEMesh*);
        
        void addVertex(VertexIndexMap &vertexOrderMap, VertexIndex &vertex, const DAEMesh* mesh, const DAESkin* skin, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, unsigned int &nextIndex);
        
        void addVertex(VertexIndexMap &vertexOrderMap, VertexIndex &vertex, const DAEMesh* mesh, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, unsigned int &nextIndex);
        
        void processMeshProperty(DAENode*, DAEImporter*);
        void processAnimatedMeshProperty(DAENode*, DAEImporter*);
        void processVisualScene(DAEEntity*, DAEImporter*);
        std::string processMaterial(const unsigned long long &, DAEImporter*);
        JointEntity* processJoints(DAEJoint*, DAEImporter *);
        std::vector<std::string> processJointNames(DAESkinController*, DAEImporter*);
        
        
    };

}

#endif /* _JMPDATA_H */
