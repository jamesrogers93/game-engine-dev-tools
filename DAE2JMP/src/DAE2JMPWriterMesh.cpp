#include "DAE2JMP/DAE2JMPWriterMesh.h"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "DAE2JMP/JMPWriter.h"
#include "DAE2JMP/Scene.h"

#include <COLLADAFWMeshPrimitive.h>
#include <COLLADAFWArrayPrimitiveType.h>
#include <COLLADAFWMeshVertexData.h>

const std::string WriterMesh::POSITION = "position";
const std::string WriterMesh::NORMAL = "normal";
const std::string WriterMesh::UV0 = "uv0";
const std::string WriterMesh::COLOUR = "colour";

const unsigned int WriterMesh::POSITION_STRIDE = 3;
const unsigned int WriterMesh::NORMAL_STRIDE = 3;

bool Vertex::operator<( const Vertex& rhs ) const
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
}

bool WriterMesh::write()
{
    
    std::ofstream *output = this->mJMPWriter->getOutputStream();
    
    *output << "\nGEOMETRY" << " " << mMesh->getName() << std::endl;
    
    const COLLADAFW::MeshPrimitiveArray& meshPrimitives = mMesh->getMeshPrimitives();
    for(unsigned int i = 0; i < meshPrimitives.getCount(); i++ )
   {
        COLLADAFW::MeshPrimitive* meshPrimitive = meshPrimitives[0];
     
        // Get source and vertex data
        SourceData data = this->getSourceData(meshPrimitive);
        std::pair<VertexIndexData, VertexData> vertexData = this->getVertexData(meshPrimitive, data);
        
        Mesh m;
        m.sources = data;
        m.vertices = vertexData.second;
        m.indices = vertexData.first;
        
        Scene &scene = Scene::getInstance();
        
        scene.loadedMeshes[mMesh->getName()] = m;
        this->writeMeshHeader(*output, data);
    
        this->writeVertices(*output, vertexData, data);
        this->writeIndices(*output, vertexData, data);
    }
    
    return true;
}

SourceData WriterMesh::getSourceData(COLLADAFW::MeshPrimitive* meshPrimitive)
{
    SourceData data;
    
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

std::pair<VertexIndexData, VertexData> WriterMesh::getVertexData(COLLADAFW::MeshPrimitive* meshPrimitive, const SourceData &sourceData)
{
    
    std::pair<VertexIndexData, VertexData> vertexData;
    
    for(unsigned int i = 0; i < sourceData.positionIndicesCount; i++)
    {
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
        
        Vertex vertex(positionIndex, normalIndex, uv0Index, colourIndex);
        this->addVertex(vertexData.first, vertexData.second, vertex, sourceData);
    }
    
    return vertexData;
}

void WriterMesh::addVertex(VertexIndexData &vertexIndexData, VertexData &vertexData, Vertex &vertex, const SourceData &data)
{
    
    VertexIndexMap::const_iterator it = vertexIndexData.map.find(vertex);
    if(it == vertexIndexData.map.end())
    {
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
        vertexIndexData.elements.push_back(it->second);
    }
    
}

void WriterMesh::writeMeshHeader(std::ofstream &output, const SourceData &data)
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

void WriterMesh::writeVertices(std::ofstream &output, const std::pair<VertexIndexData, VertexData>& vertexData, const SourceData &sourceData)
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

void WriterMesh::writeIndices(std::ofstream &output, const std::pair<VertexIndexData, VertexData>& vertexData, const SourceData &sourceData)
{
    output << "I " << vertexData.first.elements.size() << " ";
    
    std::ostream_iterator<unsigned int> output_iterator2(output, " ");
    std::copy(vertexData.first.elements.begin(), vertexData.first.elements.end(), output_iterator2);
    
    output << std::endl;
    
}
