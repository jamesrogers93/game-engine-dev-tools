#ifndef _JMPDATA_H
#define _JMPDATA_H

#include <map>
#include <string>

#include <game-engine/Modules/Graphics/Mesh.h>
#include <game-engine/Modules/Graphics/Vertex.h>

class Material;
class Mesh;
class JointEntity;
//class AnimatableMeshProperty;
class Property;

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
        std::map<std::string, JointEntity*> joints;
        std::map<std::string, Property*> properties;
        
    public:
        JMPData(){}
        
        void convert(DAEImporter *);
        
        std::map<std::string, Mesh*>& getMeshes() { return this->meshes; }
        std::map<std::string, Material*>& getMaterials() { return this->materials; }
        std::map<std::string, JointEntity*>& getJointEntites() { return this->joints; }
        void processVisualScene(DAEEntity*, DAEImporter*);
        std::map<std::string, Property*>& getProperties() { return this->properties; }
        
    private:

        void processMeshProperty(DAENode*, DAEImporter*);
        void processAnimatedMeshProperty(DAENode*, DAEImporter*);
        
        Mesh* processMesh(const DAEMesh*, const DAESkin*);
        Mesh* processMesh(const DAEMesh*);
        
        void addVertex(VertexIndexMap &vertexOrderMap, VertexIndex &vertex, const DAEMesh* mesh, const DAESkin* skin, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
        
        void addVertex(VertexIndexMap &vertexOrderMap, VertexIndex &vertex, const DAEMesh* mesh, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
        
        std::string processMaterial(const unsigned long long &, DAEImporter*);
        
        JointEntity* processJoints(DAEJoint*, DAEImporter *);
        std::vector<std::string> processJointNames(DAESkinController*, DAEImporter*);
        
    };

}

#endif /* _JMPDATA_H */
