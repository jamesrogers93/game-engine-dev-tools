#include "DAE2JMP/JMPData.h"

// STD
#include <map>
#include <iostream>

#include "DAE2JMP/DAEImporter.h"
#include "DAE2JMP/DAEImporterVisualScene.h"
#include "DAE2JMP/DAEImporterSkinController.h"
#include "DAE2JMP/DAEImporterMesh.h"
#include "DAE2JMP/DAEImporterSkin.h"

// Game Engine
#include <game-engine/Core/Entity/ENode.h>
#include <game-engine/Animatable.h>
#include <game-engine/JointEntity.h>
#include <game-engine/Core/Modules/Graphics/Geometry.h>
#include <game-engine/Core/Modules/Graphics/Material.h>

#include <glm/glm.hpp>

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
                
                ENode *node = processVisualScene(visualSceneNode, importer);
                
                this->entities[node->getName()] = node;
                
                
            }
        }
    }
    
    ENode* JMPData::processVisualScene(DAEEntity *daeEntity, DAEImporter *importer)
    {
        ENode *eNode;
        
        std::string name = daeEntity->getName();
        
        if(daeEntity->getType() == DAEEntity::JOINT)
        {
            DAEJoint *daeJoint = (DAEJoint*)daeEntity;
            
            eNode = new JointEntity(name);
            JointEntity *jointNode = (JointEntity*)eNode;
            
            float* t = daeJoint->getTransform();
            
            glm::mat4 T = glm::mat4(t[0],t[1],t[2],t[3],
                                    t[4],t[5],t[6],t[7],
                                    t[8],t[9],t[10],t[11],
                                    t[12],t[13],t[14],t[15]);
            
            jointNode->setInverseBindPose(glm::inverse(T));
            
        }
        
        
        else if(daeEntity->getType() == DAEEntity::NODE)
        {
            DAENode *daeNode = (DAENode*)daeEntity;
        
            // Process Material
            std::string materialKey;
            if(daeNode->getHasMaterialId())
            {
                unsigned long long matId = daeNode->getMaterialId();
                materialKey = processMaterial(matId, importer);
            }
            
            if( daeNode->getHasGeometryId())
            {
                // If the node has a geometry, it is not an animated node?
                // TODO
                // Load in geometry and referenece to it
            }
            
            else if(daeNode->getHasSkinControllerId())
            {
                // If the node has a controller, it is an animated node?
                eNode = new AnimatableEntity(name);
                AnimatableEntity *animNode = (AnimatableEntity*)eNode;
                
                // Get instance controller id
                unsigned long long contId = daeNode->getSkinControllerId();
                
                // Process skin controller
                // 
                processSkinController(animNode, contId, importer);
                
                animNode->setMaterialKey(materialKey);
            }
        }
        
        // Process Children
        for(unsigned int i = 0; i < daeEntity->getNumChildren(); i++)
        {
            DAEEntity *child = daeEntity->getChild(i);
            
            ENode *child1 = processVisualScene(child, importer);
            
            eNode->addChild(child1);
        }
        
        return eNode;
    }
    
    void JMPData::processSkinController(AnimatableEntity *animNode, const unsigned long long &id, DAEImporter *importer)
    {
        if(importer->getLoadedSkinControllers()->find(id) != importer->getLoadedSkinControllers()->end())
        {
            // Found
            DAESkinController *c = &importer->getLoadedSkinControllers()->operator[](id);
            
            // Get the mesh ID
            unsigned long long meshId = c->meshId;
            
            // Get the skin data ID
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
                
                // Create a geometry from skin and mesh and add it to geometries
                Geometry *geometry = processGeometry(daeMesh, daeSkin);
                this->geometries[daeMesh->name] = geometry;
                
                // Set the geometry key in the node
                animNode->setGeometryKey(daeMesh->name);
                
                // Get the joint Names
                //animNode->setJointKeys(daeSkin->jointNames);
                animNode->setJointKeys(processJointNames(c, importer));
                
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
    
    Geometry* JMPData::processGeometry(const DAEMesh* mesh, const DAESkin* skin)
    {
        
        VertexIndexMap vertexOrderMap;
        
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        
        
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
            this->addVertex(vertexOrderMap, vertex, mesh, skin, vertices, indices);
        }

        return new Geometry(vertices, indices);
    }

    void JMPData::addVertex(VertexIndexMap &vertexOrderMap, VertexIndex &vertexTemp, const DAEMesh* mesh, const DAESkin* skin, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices)
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
        this->materials[material->name] = new Material(diffuse, specular, shininess);
        
        // Return its name
        return material->name;
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
