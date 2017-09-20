#include "DAE2JMP/JMPData.h"

// STD
#include <map>
#include <iostream>

#include "DAE2JMP/DAEImporter.h"
#include "DAE2JMP/DAEImporterVisualScene.h"
#include "DAE2JMP/DAEImporterSkinController.h"
#include "DAE2JMP/DAEImporterMesh.h"
#include "DAE2JMP/DAEImporterSkin.h"
#include "DAE2JMP/DAEImporterAnimation.h"

// Game Engine
#include <game-engine/Entity/Entity.h>
#include <game-engine/Modules/Graphics/AnimatableMeshProperty.h>
#include <game-engine/Modules/Graphics/Mesh.h>
#include <game-engine/Modules/Graphics/Material.h>
#include <game-engine/Modules/Animation/JointEntity.h>
#include <game-engine/Modules/Animation/JointAnimation.h>
#include <game-engine/Modules/Animation/JointTransform.h>
#include <game-engine/Modules/Animation/Animation.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

namespace DAE2JMP
{

    void JMPData::convert(DAEImporter *importer)
    {
        //importer->get
        
        std::map<unsigned long long, DAEVisualScene> *visualScenes = importer->getLoadedVisualScenes();
        
        for(auto &visualScene : *visualScenes)
        {
            unsigned int numRootNodes = visualScene.second.rootNodeCount;
            
            for(int i = 0; i < numRootNodes; i++)
            {
                // Get the base root node from the visual scene
                DAEEntity *visualSceneNode = visualScene.second.rootNodes[i];
                
                processVisualScene(visualSceneNode, importer);
            }
        }
        
        std::map<std::string, JointAnimation> jointAnimations;
        float maxTimeStamp = 0.0;
        for(auto &animation : *importer->getLoadedAnimations())
        {
            std::vector<KeyFrame> keyFrames;
            
            size_t numFrames = animation.second.frames.size();
            for(unsigned int i = 0; i < numFrames; i++)
            {
                float timeStamp = animation.second.frames[i];
                glm::mat4 transform = animation.second.transforms[i];
                
                // Extract positon and quaternion rotation from transform
                glm::vec4 position = transform[3];
                glm::fquat rotation = glm::toQuat(transform);
                
                JointTransform jointTransform = JointTransform(position, rotation);
                KeyFrame keyFrame = KeyFrame(timeStamp, jointTransform);
                
                keyFrames.push_back(keyFrame);
            }
            
            //float length = std::max_element(animation.second.frames.begin(), animation.second.frames.end());
            float length = animation.second.frames[numFrames-1];
            JointAnimation jointAnimation = JointAnimation(length, keyFrames);
            
            if(length > maxTimeStamp)
            {
                maxTimeStamp = length;
            }
            
            if(jointAnimations.find(animation.second.name) == jointAnimations.end())
            {
                jointAnimations[animation.second.name] = jointAnimation;
            }
            else
            {
                std::cout << "Problem!" << std::endl;
            }
    
        }
        Animation *animation = new Animation(maxTimeStamp, jointAnimations);
        
        this->animations["unamed-animation"] = animation;
        
        // Test
        //AnimatableMeshProperty *test1 = (AnimatableMeshProperty*)this->properties["Alpha_Joints"];
        //AnimatableMeshProperty *test2 = (AnimatableMeshProperty*)this->properties["Alpha_Surface"];
        
        //int test = 4;
    }
    
    void JMPData::processVisualScene(DAEEntity *visualSceneNode, DAEImporter *importer)
    {
        if(visualSceneNode->getType() == DAEEntity::JOINT)
        {
            // 'Joint' node.
            // Process the joint heirarchy
            JointEntity *joint = processJoints((DAEJoint*)visualSceneNode, importer);
            joint->calculateInverseBindTransform();
            
            this->entites[joint->getName()] = joint;
        }
        else
        {
            // 'Node' node
            DAENode *daeNode = (DAENode*)visualSceneNode;
            
            if( daeNode->getHasGeometryId())
            {
                // If the node has a geometry, it is not an animated node?

                // Process MeshProperty
                processMeshProperty(daeNode, importer);
                
            }
            
            else if(daeNode->getHasSkinControllerId())
            {
                // IF the node has a skin controller, it is an animated node?
                
                // Process AnimatedMeshProperty
                processAnimatedMeshProperty(daeNode, importer);
            }
            else
            {
                // Must be some sort of container node.
                // Ignore it and process it's children
                for(unsigned int i = 0; i < visualSceneNode->getNumChildren(); i++)
                {
                    processVisualScene(visualSceneNode->getChild(i), importer);
                }
                
            }
        }
    }
    
    void JMPData::processMeshProperty(DAENode *daeNode, DAEImporter *importer)
    {
        std::string materialKey;
        if(daeNode->getHasMaterialId())
        {
            unsigned long long matId = daeNode->getMaterialId();
            materialKey = processMaterial(matId, importer);
        }
        
        unsigned long long meshId = daeNode->getHasGeometryId();
    
        if(importer->getLoadedMeshes()->find(meshId) != importer->getLoadedMeshes()->end())
        {
            // Found
            DAEMesh *daeMesh = &(*importer->getLoadedMeshes())[meshId];
            
            // Create a mesh  and add it to the meshes map
            Mesh *mesh = processMesh(daeMesh);
            this->meshes[daeMesh->name] = mesh;
            
            // Create a MeshProperty and add it to the properties map
            MeshProperty *meshProperty;
            meshProperty = new MeshProperty(daeNode->getName());
            meshProperty->setMeshKey(daeMesh->name);
            meshProperty->setMaterialKey(materialKey);
            this->properties[daeNode->getName()] = meshProperty;
        }
        else
        {
            // Not found
            std::cout << "Problem!" << std::endl;
        }
    }
    
    void JMPData::processAnimatedMeshProperty(DAENode *daeNode, DAEImporter *importer)
    {
        std::string materialKey;
        if(daeNode->getHasMaterialId())
        {
            unsigned long long matId = daeNode->getMaterialId();
            materialKey = processMaterial(matId, importer);
        }
        // Get instance controller id
        unsigned long long contId = daeNode->getSkinControllerId();
        
        if(importer->getLoadedSkinControllers()->find(contId) != importer->getLoadedSkinControllers()->end())
        {
            // Found
            DAESkinController *c = &importer->getLoadedSkinControllers()->operator[](contId);
            
            // Get the mesh ID in the skin controller
            unsigned long long meshId = c->meshId;
            
            // Get the skin data ID in the skin controller
            unsigned long long skinId = c->skinId;
            
            // Get the joints ID
            std::vector<unsigned long long> jointIds = c->jointIds;
            
            // Look up mesh and skin id
            // We need to merge these two to create a geometry that has the mesh data and skin data
            if(importer->getLoadedMeshes()->find(meshId) != importer->getLoadedMeshes()->end() &&
               importer->getLoadedSkins()->find(skinId) != importer->getLoadedSkins()->end())
            {
                // Found
                DAEMesh *daeMesh = &importer->getLoadedMeshes()->operator[](meshId);
                DAESkin *daeSkin = &importer->getLoadedSkins()->operator[](skinId);
                
                // Create a mesh from skin and mesh and add it to meshes
                Mesh *mesh = processMesh(daeMesh, daeSkin);
                this->meshes[daeMesh->name] = mesh;
                
                // Create the AnimatableMeshProperty and add it to the properties map
                AnimatableMeshProperty *animMeshProperty;
                animMeshProperty = new AnimatableMeshProperty(daeNode->getName());
                animMeshProperty->setMeshKey(daeMesh->name);
                animMeshProperty->setMaterialKey(materialKey);
                animMeshProperty->setJointKeys(processJointNames(c, importer));
                this->properties[daeNode->getName()] = animMeshProperty;
                
            }
            else
            {
                // Not found
                std::cout << "Problem!" << std::endl;
            }
            
        }
        else
        {
            // Not found
            std::cout << "Problem!" << std::endl;
            
        }

    }
    
    Mesh* JMPData::processMesh(const DAEMesh* mesh, const DAESkin* skin)
    {
        VertexIndexMap vertexOrderMap;
        
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        unsigned int nextIndex = 0;
        
        
        // Loop over all of the position indices
        for(unsigned int i = 0; i < mesh->positionIndicesCount; i++)
        {
            // Create a vertex with the source data indices
            VertexIndex vertex;
            
            vertex.setPosition(mesh->positionIndices[i]);

            if(mesh->hasNormals)
            {
                vertex.setNormal(mesh->normalIndices[i]);
            }
            
            if(mesh->hasUV0s)
            {
                vertex.setUV0(mesh->uv0Indices[i]);
            }
            
            if(mesh->hasColours)
            {
                vertex.setColour(mesh->colourIndices[i]);
            }
            
            // We use the position indices for the joint data because each joint data belongs to the position
            vertex.setJointId(mesh->positionIndices[i]);
            vertex.setJointWeight(mesh->positionIndices[i]);
            
            // Add vertex to the vertexData
            this->addVertex(vertexOrderMap, vertex, mesh, skin, vertices, indices, nextIndex);
        }

        return new Mesh(mesh->name, vertices, indices);
    }
    
    Mesh* JMPData::processMesh(const DAEMesh* mesh)
    {
        
        VertexIndexMap vertexOrderMap;
        
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        unsigned int nextIndex = 0;
        
        // Loop over all of the position indices
        for(unsigned int i = 0; i < mesh->positionIndicesCount; i++)
        {
            // Create a vertex with the source data indices
            VertexIndex vertex;
            
            vertex.setPosition(mesh->positionIndices[i]);
            
            if(mesh->hasNormals)
            {
                vertex.setNormal(mesh->normalIndices[i]);
            }
            
            if(mesh->hasUV0s)
            {
                vertex.setUV0(mesh->uv0Indices[i]);
            }
            
            if(mesh->hasColours)
            {
                vertex.setColour(mesh->colourIndices[i]);
            }
            
            // Add vertex to the vertexData
            this->addVertex(vertexOrderMap, vertex, mesh, vertices, indices, nextIndex);
        }
        
        return new Mesh(mesh->name, vertices, indices);
    }

    void JMPData::addVertex(VertexIndexMap &vertexOrderMap, VertexIndex &vertexTemp, const DAEMesh* mesh, const DAESkin* skin, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, unsigned int &nextIndex)
    {
        
        
        // Check if this vertex already exists
        VertexIndexMap::const_iterator it = vertexOrderMap.find(vertexTemp);
        if(it == vertexOrderMap.end())
        {
            // Does not exist.
            // Add source data provided by indices to the respective source array.
            // Then add new element and append to the end of the elements list.
            indices.push_back(vertexOrderMap[vertexTemp] = nextIndex++);
            
            Vertex vertex;
            
            if(vertexTemp.hasPosition())
            {
                unsigned int index = Vertex::POSITION_STRIDE * vertexTemp.getPosition();
                
                float x = mesh->positions[index];
                float y = mesh->positions[index+1];
                float z = mesh->positions[index+2];
                
                vertex.setPosition(glm::vec3(x, y, z));
            }
            
            if(vertexTemp.hasNormal())
            {
                unsigned int index = Vertex::NORMAL_STRIDE * vertexTemp.getNormal();
                
                float x = mesh->normals[index];
                float y = mesh->normals[index+1];
                float z = mesh->normals[index+2];
                
                vertex.setNormal(glm::vec3(x, y, z));
            }
            
            if(vertexTemp.hasUV0())
            {
                unsigned int index = Vertex::UV0_STRIDE * vertexTemp.getUV0();
                
                float u = mesh->uv0s[index];
                float v = mesh->uv0s[index+1];
                
                vertex.setUV0(glm::vec2(u, v));
            }
            
            if(vertexTemp.hasColour())
            {
                unsigned int index = Vertex::COLOUR_STRIDE * vertexTemp.getColour();
                
                float r = mesh->colours[index];
                float g = mesh->colours[index+1];
                float b = mesh->colours[index+2];
                float a = mesh->colours[index+3];
                
                vertex.setColour(glm::vec4(r, g, b, a));
            }
            
            if(vertexTemp.hasJointId())
            {
                unsigned int index = Vertex::JOINT_STRIDE * vertexTemp.getJointId();
                
                float x = skin->jointIds[index];
                float y = skin->jointIds[index+1];
                float z = skin->jointIds[index+2];
                float w = skin->jointIds[index+3];
                
                vertex.setJointId(glm::ivec4(x, y, z, w));
            }
            
            if(vertexTemp.hasJointWeight())
            {
                unsigned int index = Vertex::JOINT_STRIDE * vertexTemp.getJointWeight();
                
                float x = skin->weights[index];
                float y = skin->weights[index+1];
                float z = skin->weights[index+2];
                float w = skin->weights[index+3];
                
                vertex.setJointWeight(glm::vec4(x, y, z, w));
            }
            
            vertices.push_back(vertex);
        }
        else
        {
            // Vertex exists.
            // Add the element of the found vertex to the elements array.
            indices.push_back(it->second);
        }
    }
    
    void JMPData::addVertex(VertexIndexMap &vertexOrderMap, VertexIndex &vertexTemp, const DAEMesh* mesh, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, unsigned int &nextIndex)
    {
        
        
        // Check if this vertex already exists
        VertexIndexMap::const_iterator it = vertexOrderMap.find(vertexTemp);
        if(it == vertexOrderMap.end())
        {
            // Does not exist.
            // Add source data provided by indices to the respective source array.
            // Then add new element and append to the end of the elements list.
            indices.push_back(vertexOrderMap[vertexTemp] = indices.size());
            
            Vertex vertex;
            
            if(vertexTemp.hasPosition())
            {
                unsigned int index = Vertex::POSITION_STRIDE * vertexTemp.getPosition();
                
                float x = mesh->positions[index];
                float y = mesh->positions[index+1];
                float z = mesh->positions[index+2];
                
                vertex.setPosition(glm::vec3(x, y, z));
            }
            
            if(vertexTemp.hasNormal())
            {
                unsigned int index = Vertex::NORMAL_STRIDE * vertexTemp.getNormal();
                
                float x = mesh->normals[index];
                float y = mesh->normals[index+1];
                float z = mesh->normals[index+2];
                
                vertex.setNormal(glm::vec3(x, y, z));
            }
            
            if(vertexTemp.hasUV0())
            {
                unsigned int index = Vertex::UV0_STRIDE * vertexTemp.getUV0();
                
                float u = mesh->uv0s[index];
                float v = mesh->uv0s[index+1];
                
                vertex.setUV0(glm::vec2(u, v));
            }
            
            if(vertexTemp.hasColour())
            {
                unsigned int index = Vertex::COLOUR_STRIDE * vertexTemp.getColour();
                
                float r = mesh->colours[index];
                float g = mesh->colours[index+1];
                float b = mesh->colours[index+2];
                float a = mesh->colours[index+3];
                
                vertex.setColour(glm::vec4(r, g, b, a));
            }
            
            vertices.push_back(vertex);
        }
        else
        {
            // Vertex exists.
            // Add the element of the found vertex to the elements array.
            indices.push_back(it->second);
        }
    }
    
    std::string JMPData::processMaterial(const unsigned long long &matId, DAEImporter* importer)
    {
        
        std::string name;
        
        // Get material from importer
        if(importer->getLoadedMaterials()->find(matId) == importer->getLoadedMaterials()->end())
        {
            // Not found
            return name;
        }
        
        DAEMaterial *material = &(*importer->getLoadedMaterials())[matId];
        
        // Now find effect pointed to by material
        if(importer->getLoadedEffects()->find(material->effectId) == importer->getLoadedEffects()->end())
        {
            return name;
        }
        
        DAEEffect *eff = &(*importer->getLoadedEffects())[material->effectId];
        
        glm::vec4 diffuse;
        if(eff->hasDiffuse)
        {
            diffuse = glm::vec4(eff->diffuse[0], eff->diffuse[1], eff->diffuse[2], eff->diffuse[3]);
        }
        
        glm::vec4 specular;
        if(eff->hasSpecular)
        {
            specular = glm::vec4(eff->specular[0], eff->specular[1], eff->specular[2], eff->specular[3]);
        }
        
        float shininess;
        if(eff->hasShininess)
        {
            shininess = eff->shininess;
        }
        
        // Now store material
        this->materials[material->name] = new Material(material->name, diffuse, specular, shininess);
        
        // Return its name
        return material->name;
    }
    
    
    JointEntity* JMPData::processJoints(DAEJoint* daeJoint, DAEImporter *importer)
    {
        std::string name = daeJoint->getName();
            
        JointEntity *joint = new JointEntity(name);
            
        float* t = daeJoint->getTransform();
            
        glm::mat4 T = glm::mat4(t[0],t[1],t[2],t[3],
                                t[4],t[5],t[6],t[7],
                                t[8],t[9],t[10],t[11],
                                t[12],t[13],t[14],t[15]);
        
        joint->setLocalBindTransform(T);
        //joint->transformOW(T);
        //joint->setInverseBindPose(glm::inverse(T));
        
        // Process Children
        for(unsigned int i = 0; i < daeJoint->getNumChildren(); i++)
        {
            DAEJoint *daeJointChild = (DAEJoint*)daeJoint->getChild(i);
            
            JointEntity *jointChild = processJoints(daeJointChild, importer);
            
            joint->addChild(jointChild);
        }
        
        return joint;

    }
    
    std::vector<std::string> JMPData::processJointNames(DAESkinController* skin, DAEImporter* importer)
    {
        std::vector<std::string> jointNames;
        
        for(unsigned int i = 0; i < skin->numJointIds; i++)
        {
            if(importer->getLoadedJointNames()->find(skin->jointIds[i]) != importer->getLoadedJointNames()->end())
            {
                // Found
                std::string name = (*importer->getLoadedJointNames())[skin->jointIds[i]];
                jointNames.push_back(name);
            }
            else
            {
                // Not found
                std::cout << "Problem!" << std::endl;
            }
        }
        
        return jointNames;
    }
}
