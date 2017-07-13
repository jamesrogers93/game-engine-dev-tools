#include "DAE2JMP/DAEImporterMesh.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "DAE2JMP/DAEImporter.h"
#include "DAE2JMP/DAEUtil.h"

#include <COLLADAFWMeshPrimitive.h>
#include <COLLADAFWArrayPrimitiveType.h>
#include <COLLADAFWMeshVertexData.h>

namespace DAE2JMP
{

    const std::string DAEImporterMesh::POSITION = "position";
    const std::string DAEImporterMesh::NORMAL = "normal";
    const std::string DAEImporterMesh::UV0 = "uv0";
    const std::string DAEImporterMesh::COLOUR = "colour";

    const unsigned int DAEImporterMesh::POSITION_STRIDE = 3;
    const unsigned int DAEImporterMesh::NORMAL_STRIDE = 3;

    /*bool Vertex::operator<( const Vertex& rhs ) const
    {
        if ( positionIndex < rhs.positionIndex )
            return true;
        
        if ( positionIndex > rhs.positionIndex )
            return false;
        
        if ( normalIndex < rhs.normalIndex )
            return true;
        
        if ( normalIndex > rhs.normalIndex )
            return false;
        
        if ( uv0Index < rhs.uv0Index )
            return true;
        
        if ( uv0Index > rhs.uv0Index )
            return false;
        
        if ( colourIndex < rhs.colourIndex )
            return true;
        
        if ( colourIndex > rhs.colourIndex )
            return false;
        
        return false;
    }*/

    bool DAEImporterMesh::import(const COLLADAFW::Mesh* mesh)
    {
        if(mesh == NULL) return false;
        
        
        DAEMesh meshData = getMeshData(mesh);
        
        return this->mDAEImporter->addLoadedMesh(mesh->getObjectId(), meshData);
    }

    DAEMesh DAEImporterMesh::getMeshData(const COLLADAFW::Mesh* mesh)
    {
        DAEMesh meshData;
        
        COLLADAFW::MeshPrimitive *meshPrimitive;
        if(mesh->getMeshPrimitives()[0] == NULL)
        {
            return meshData;
        }
        meshPrimitive = mesh->getMeshPrimitives()[0];
        
        // Get source data
        
        //
        // Positions
        //
        
        // Get source data
        const COLLADAFW::FloatOrDoubleArray positions = mesh->getPositions();
        const COLLADAFW::UIntValuesArray *positionIndices = &meshPrimitive->getPositionIndices();
        
        // Get data counts
        meshData.positionsCount = positions.getValuesCount();
        meshData.positionIndicesCount = positionIndices->getCount();
        
        // Copy data to vectors
        for(unsigned int i = 0; i < meshData.positionsCount; i++)
        {
            meshData.positions.push_back(getFloatValue(positions, i));
        }
        
        for(unsigned int i = 0; i < meshData.positionIndicesCount; i++)
        {
            meshData.positionIndices.push_back(positionIndices->getData()[i]);
        }
        
        // Set flag
        meshData.hasPositions = (meshData.positionIndicesCount != 0);
        
        // Set Stride
        meshData.positionStride = POSITION_STRIDE;
        
        //
        // Normals
        //
        
        // Get source data
        const COLLADAFW::FloatOrDoubleArray normals = mesh->getNormals();
        const COLLADAFW::UIntValuesArray *normalIndices = &meshPrimitive->getNormalIndices();
        
        // Get data counts
        meshData.normalsCount = normals.getValuesCount();
        meshData.normalIndicesCount = normalIndices->getCount();
        
        // Copy data to vectors
        for(unsigned int i = 0; i < meshData.normalsCount; i++)
        {
            meshData.normals.push_back(getFloatValue(normals, i));
        }
        
        for(unsigned int i = 0; i < meshData.normalIndicesCount; i++)
        {
            meshData.normalIndices.push_back(normalIndices->getData()[i]);
        }
        
        // Set flag
        meshData.hasNormals = (meshData.normalIndicesCount != 0);
        
        // Set Stride
        meshData.normalStride = NORMAL_STRIDE;
        
        //
        // UV0s
        //
        
        // Get source data
        const COLLADAFW::FloatOrDoubleArray uv0s = mesh->getUVCoords();
        const COLLADAFW::IndexListArray &uv0IndicesArray = meshPrimitive->getUVCoordIndicesArray();
        
        if ( !uv0IndicesArray.empty() )
        {
            const COLLADAFW::UIntValuesArray *uv0Indices = &uv0IndicesArray[0]->getIndices();
            
            // Get data counts
            meshData.uv0sCount = uv0s.getValuesCount();
            meshData.uv0IndicesCount = uv0Indices->getCount();
            
            // Copy data to vectors
            for(unsigned int i = 0; i < meshData.uv0sCount; i++)
            {
                meshData.uv0s.push_back(getFloatValue(uv0s, i));
            }
            
            for(unsigned int i = 0; i < meshData.uv0IndicesCount; i++)
            {
                meshData.uv0Indices.push_back(uv0Indices->getData()[i]);
            }
            
            // Set flag
            meshData.hasUV0s = (meshData.uv0IndicesCount != 0);
            
            // Set Stride
            meshData.uv0Stride = mesh->getUVCoords().getStride(0);
            
        }
        
        //
        // Colours
        //
        
        // Get source data
        const COLLADAFW::FloatOrDoubleArray colours = mesh->getColors();
        const COLLADAFW::IndexListArray &colourIndicesArray = meshPrimitive->getColorIndicesArray();
        
        if ( !colourIndicesArray.empty() )
        {
            const COLLADAFW::UIntValuesArray *colourIndices = &colourIndicesArray[0]->getIndices();
            
            // Get data counts
            meshData.coloursCount = colours.getValuesCount();
            meshData.colourIndicesCount = colourIndices->getCount();
            
            // Copy data to vectors
            for(unsigned int i = 0; i < meshData.coloursCount; i++)
            {
                meshData.colours.push_back(getFloatValue(colours, i));
            }
            
            for(unsigned int i = 0; i < meshData.colourIndicesCount; i++)
            {
                meshData.colourIndices.push_back(colourIndices->getData()[i]);
            }
            
            // Set flag
            meshData.hasColours = (meshData.colourIndicesCount != 0);
            
            // Set Stride
            meshData.colourStride = mesh->getColors().getStride(0);
            
        }
        
        meshData.name = mesh->getName();
        
        return meshData;
    }


    /*bool DAEImporterMesh::write()
    {
        
        //std::ofstream *output = this->mJMPImporter->getOutputStream();
        
        //*output << "\nGEOMETRY" << " " << mMesh->getName() << std::endl;
        
        const COLLADAFW::MeshPrimitiveArray& meshPrimitives = mMesh->getMeshPrimitives();
        //for(unsigned int i = 0; i < meshPrimitives.getCount(); i++ )
        //{
            COLLADAFW::MeshPrimitive* meshPrimitive = meshPrimitives[0];
        
            // Get source and vertex data
            VertexSourceData data = this->getSourceData(meshPrimitive);
            std::pair<VertexIndexData, VertexData> vertexData = this->getVertexData(meshPrimitive, data);
            
            Mesh m;
            m.sources = data;
            m.vertices = vertexData.second;
            m.indices = vertexData.first;
            
            Scene &scene = Scene::getInstance();
            
            scene.loadedMeshes[mMesh->getName()] = m;
            //this->writeMeshHeader(*output, data);
        
            //this->writeVertices(*output, vertexData, data);
            //this->writeIndices(*output, vertexData, data);
        //}
        
        return true;
    }

    VertexSourceData DAEImporterMesh::getSourceData(COLLADAFW::MeshPrimitive* meshPrimitive)
    {
        VertexSourceData data;
        
        // Positions
        data.positionIndices = &meshPrimitive->getPositionIndices();
        data.positionIndicesCount = data.positionIndices->getCount();
        data.hasPositions = (data.positionIndicesCount != 0);
        data.positionStride = POSITION_STRIDE;
        
        // Normals
        data.normalIndices = &meshPrimitive->getNormalIndices();
        data.normalIndicesCount = data.normalIndices->getCount();
        data.hasNormals = (data.normalIndicesCount != 0);
        data.normalStride = NORMAL_STRIDE;
        
        // uv0
        const COLLADAFW::IndexListArray& uv0IndicesList = meshPrimitive->getUVCoordIndicesArray();
        if ( !uv0IndicesList.empty() )
        {
            data.uv0Indices = &uv0IndicesList[0]->getIndices();
            data.uv0IndicesCount = data.uv0Indices->getCount();
            data.hasUV0s = (data.uv0IndicesCount != 0);
            if(data.hasUV0s)
            {
                data.uv0Stride = mMesh->getUVCoords().getStride(0);
            }
        }
        else
        {
            data.hasUV0s = false;
        }
        
        // Colours
        const COLLADAFW::IndexListArray& colourIndicesList = meshPrimitive->getColorIndicesArray();
        if ( !colourIndicesList.empty() )
        {
            data.colourIndices = &colourIndicesList[0]->getIndices();
            data.colourIndicesCount = data.colourIndices->getCount();
            data.hasColours = (data.colourIndicesCount != 0);
            if(data.hasColours)
            {
                data.colourStride = mMesh->getColors().getStride(0);
            }
        }
        else
        {
            data.hasColours = false;
        }
        
        return data;
    }

    std::pair<VertexIndexData, VertexData> DAEImporterMesh::getVertexData(COLLADAFW::MeshPrimitive* meshPrimitive, const VertexSourceData &sourceData)
    {
        
        std::pair<VertexIndexData, VertexData> vertexData;
        
        // Loop over all of the position indices
        for(unsigned int i = 0; i < sourceData.positionIndicesCount; i++)
        {
            // Get the index for the source data
            unsigned int positionIndex = (*sourceData.positionIndices)[i];
            
            unsigned int normalIndex = 0;
            if(sourceData.hasNormals)
            {
                normalIndex = (*sourceData.normalIndices)[i];
            }
            
            unsigned int uv0Index = 0;
            if(sourceData.hasUV0s)
            {
                uv0Index = (*sourceData.uv0Indices)[i];
            }
            
            unsigned int colourIndex = 0;
            if(sourceData.hasColours)
            {
                colourIndex = (*sourceData.colourIndices)[i];
            }
            
            // Create a vertex with the source data indices
            Vertex vertex(positionIndex, normalIndex, uv0Index, colourIndex);
            
            // Add vertex to the vertexData
            this->addVertex(vertexData.first, vertexData.second, vertex, sourceData);
        }
        
        return vertexData;
    }

    void DAEImporterMesh::addVertex(VertexIndexData &vertexIndexData, VertexData &vertexData, Vertex &vertex, const VertexSourceData &data)
    {
        // Check if this vertex already exists
        VertexIndexMap::const_iterator it = vertexIndexData.map.find(vertex);
        if(it == vertexIndexData.map.end())
        {
            // Does not exist.
            // Add source data provIded by indices to the respective source array.
            // Then add new element and append to the end of the elements list.
            
            vertexIndexData.elements.push_back(vertexIndexData.map[vertex] = vertexIndexData.nextVertexIndex++);
            
            if(mMesh->getPositions().getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE)
            {
                const double *positionsArray = mMesh->getPositions().getDoubleValues()->getData();
                positionsArray+= data.positionStride * vertex.positionIndex;
                
                for(unsigned int i = 0; i < data.positionStride; i++)
                {
                    vertexData.positions.push_back((float)positionsArray[i]);
                }
            }
            else
            {
                const float *positionsArray = mMesh->getPositions().getFloatValues()->getData();
                positionsArray+= data.positionStride * vertex.positionIndex;
                
                for(unsigned int i = 0; i < data.positionStride; i++)
                {
                    vertexData.positions.push_back(positionsArray[i]);
                }
            }
            
            if(data.hasNormals)
            {
                if(mMesh->getNormals().getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE)
                {
                    const double *normalsArray = mMesh->getNormals().getDoubleValues()->getData();
                    normalsArray+= data.normalStride * vertex.normalIndex;
                    
                    for(unsigned int i = 0; i < data.normalStride; i++)
                    {
                        vertexData.normals.push_back((float)normalsArray[i]);
                    }
                }
                else
                {
                    const float *normalsArray = mMesh->getNormals().getFloatValues()->getData();
                    normalsArray+= data.normalStride * vertex.normalIndex;
                    
                    for(unsigned int i = 0; i < data.normalStride; i++)
                    {
                        vertexData.normals.push_back(normalsArray[i]);
                    }
                }
            }
            
            if(data.hasUV0s)
            {
                if(mMesh->getUVCoords().getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE)
                {
                    const double *uv0Array = mMesh->getUVCoords().getDoubleValues()->getData();
                    uv0Array+= data.uv0Stride * vertex.uv0Index;
                    
                    for(unsigned int i = 0; i < data.uv0Stride; i++)
                    {
                        vertexData.uv0s.push_back((float)uv0Array[i]);
                    }
                }
                else
                {
                    const float *uv0Array = mMesh->getUVCoords().getFloatValues()->getData();
                    uv0Array+= data.uv0Stride * vertex.uv0Index;
                    
                    for(unsigned int i = 0; i < data.uv0Stride; i++)
                    {
                        vertexData.uv0s.push_back(uv0Array[i]);
                    }
                }
            }
            
            if(data.hasColours)
            {
                if(mMesh->getColors().getType() == COLLADAFW::MeshVertexData::DATA_TYPE_DOUBLE)
                {
                    const double *colourArray = mMesh->getColors().getDoubleValues()->getData();
                    colourArray+= data.colourStride * vertex.colourIndex;
                    
                    for(unsigned int i = 0; i < data.colourStride; i++)
                    {
                        vertexData.colours.push_back((float)colourArray[i]);
                    }
                }
                else
                {
                    const float *colourArray = mMesh->getColors().getFloatValues()->getData();
                    colourArray+= data.colourStride * vertex.colourIndex;
                    
                    for(unsigned int i = 0; i < data.colourStride; i++)
                    {
                        vertexData.colours.push_back(colourArray[i]);
                    }
                }
            }
        }
        else
        {
            // Vertex exists.
            // Add the element of the found vertex to the elements array.
            
            vertexIndexData.elements.push_back(it->second);
        }
        
    }

     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
    void DAEImporterMesh::writeMeshHeader(std::ofstream &output, const VertexSourceData &data)
    {
        // Positions
        if(data.hasPositions)
        {
            output << "S " << POSITION << " " << data.positionStride << std::endl;
        }
        
        // Normals
        if(data.hasNormals)
        {
            output << "S " << NORMAL << " " << data.normalStride << std::endl;
        }
        
        // uv0s
        if(data.hasUV0s)
        {
            output << "S " << UV0 << " " << data.uv0Stride << std::endl;
        }
        
        // Colours
        if(data.hasColours)
        {
            output << "S " << COLOUR << " " << data.colourStride << std::endl;
        }
    }

    void DAEImporterMesh::writeVertices(std::ofstream &output, const std::pair<VertexIndexData, VertexData>& vertexData, const VertexSourceData &sourceData)
    {
        output << "V " << vertexData.first.nextVertexIndex;
        
        
        for(unsigned int i = 0; i < vertexData.first.nextVertexIndex; i++)
        {
            
            unsigned int positionIndex = i * sourceData.positionStride;
            for(unsigned int j = 0; j < sourceData.positionStride; j++)
            {
                output << " " << vertexData.second.positions[positionIndex+j];
            }
            
            if(sourceData.hasNormals)
            {
                unsigned int normalIndex = i * sourceData.normalStride;
                for(unsigned int j = 0; j < sourceData.normalStride; j++)
                {
                    output << " " << vertexData.second.normals[normalIndex+j];
                    
                }
            }
            
            if(sourceData.hasUV0s)
            {
                unsigned int uv0Index = i * sourceData.uv0Stride;
                for(unsigned int j = 0; j < sourceData.uv0Stride; j++)
                {
                    output << " " << vertexData.second.uv0s[uv0Index+j];
                    
                }
            }
            
            if(sourceData.hasColours)
            {
                unsigned int colourIndex = i * sourceData.colourStride;
                for(unsigned int j = 0; j < sourceData.colourStride; j++)
                {
                    output << " " << vertexData.second.colours[colourIndex+j];
                    
                }
            }
        }
        
        output << std::endl;
    }

    void DAEImporterMesh::writeIndices(std::ofstream &output, const std::pair<VertexIndexData, VertexData>& vertexData, const VertexSourceData &sourceData)
    {
        output << "I " << vertexData.first.elements.size() << " ";
        
        std::ostream_iterator<unsigned int> output_iterator2(output, " ");
        std::copy(vertexData.first.elements.begin(), vertexData.first.elements.end(), output_iterator2);
        
        output << std::endl;
        
    }*/

}
