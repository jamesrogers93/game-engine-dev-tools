#include "DAE2JMP/JMPExporterMesh.h"

#include <ostream>

#include "DAE2JMP/JMPExporter.h"

#include "game-engine/Modules/Graphics/Mesh.h"

namespace DAE2JMP
{
    
    const std::string JMPExporterMesh::POSITION = "position";
    const std::string JMPExporterMesh::NORMAL = "normal";
    const std::string JMPExporterMesh::UV0 = "uv0";
    const std::string JMPExporterMesh::COLOUR = "colour";
    const std::string JMPExporterMesh::JOINT_ID = "joint_id";
    const std::string JMPExporterMesh::JOINT_WEIGHT = "joint_weight";
    
    bool JMPExporterMesh::Export(const Mesh* geo)
    {
     
        std::ofstream *output = this->mJMPExporter->getOutputStream();
        
        std::vector<Vertex> vertices = geo->getVertices();
        std::vector<unsigned int> indices = geo->getIndices();
        
        if(vertices.size() == 0)
            return false;
        
        *output << geo->getName() << std::endl;
        
        // Write header
        if(vertices[0].hasPosition())
        {
            *output << "-S " << POSITION << " " << vertices[0].POSITION_STRIDE << std::endl;
        }
        
        if(vertices[0].hasNormal())
        {
            *output << "-S " << NORMAL << " " << vertices[0].NORMAL_STRIDE << std::endl;
        }
        
        if(vertices[0].hasUV0())
        {
            *output << "-S " << UV0 << " " << vertices[0].UV0_STRIDE << std::endl;
        }
        
        if(vertices[0].hasColour())
        {
            *output << "-S " << COLOUR << " " << vertices[0].COLOUR_STRIDE << std::endl;
        }
        
        if(vertices[0].hasJointId())
        {
            *output << "-S " << JOINT_ID << " " << vertices[0].JOINT_STRIDE << std::endl;
        }
        
        if(vertices[0].hasJointWeight())
        {
            *output << "-S " << JOINT_WEIGHT << " " << vertices[0].JOINT_STRIDE << std::endl;
        }
        
        // Write vertex data
        std::vector<float> vertexData;
        vertexData.reserve(vertices.size() * vertices[0].getNumElements());
        for(unsigned int i = 0; i < vertices.size(); i++)
        {
            std::vector<float> tempVertexData = vertices[i].getData();
            vertexData.insert(std::end(vertexData), std::begin(tempVertexData), std::end(tempVertexData));
        }
        
        *output << "-V " << vertices.size() << " ";
        std::ostream_iterator<float> output_iterator(*output, " ");
        std::copy(vertexData.begin(), vertexData.end(), output_iterator);
        *output << std::endl;
        
        // Write index data
        *output << "-I " << indices.size() <<  " ";
        std::ostream_iterator<unsigned int> output_iterator2(*output, " ");
        std::copy(indices.begin(), indices.end(), output_iterator2);
        
        
        return true;
    }
}
