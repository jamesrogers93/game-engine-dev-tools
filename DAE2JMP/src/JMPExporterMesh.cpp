#include "DAE2JMP/JMPExporterMesh.h"

#include <ostream>

#include "DAE2JMP/JMPExporter.h"

#include "game-engine/Modules/Graphics/Mesh.h"

#include <glm/gtc/type_ptr.hpp>

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
        //std::vector<float> vertexData;
        //vertexData.reserve(vertices.size() * vertices[0].getNumElements());
        //for(unsigned int i = 0; i < vertices.size(); i++)
        //{
        //    std::vector<float> tempVertexData = vertices[i].getData();
        //    vertexData.insert(std::end(vertexData), std::begin(tempVertexData), std::end(tempVertexData));
        //}
        
        //*output << "-V " << vertices.size() << " ";
        //output->write(reinterpret_cast<char*>( &vertexData[0] ), vertexData.size() * sizeof(float));
        
        /**output << std::endl << std::endl;
        
        *output << std::endl << "FLOAT " << " " << std::endl;
        glm::vec4 f = glm::vec4(1.0f, 10.0f, 20.0f, 50.0f);
        output->write(reinterpret_cast<char*>( &f[0] ), sizeof(glm::vec4));
        *output << std::endl << "END FLOAT " << " " << std::endl;
        
        *output << std::endl << "INT " << " " << std::endl;
        glm::ivec4 i = glm::ivec4(1, 10, 20, 50);
        output->write(reinterpret_cast<char*>( &i[0] ), sizeof(glm::ivec4));
        *output << std::endl << "END INT " << " " << std::endl;
        
        *output << std::endl << std::endl;*/
        
        // TEST to write ivec to file as int instead of float
        *output << "-V " << vertices.size() << " ";
        for(unsigned int i = 0; i < vertices.size(); i++)
        {
            if(vertices[0].hasPosition())
            {
                glm::vec3 p = vertices[i].getPosition();
                output->write(reinterpret_cast<char*>( &p[0] ), sizeof(glm::vec3));
            }
            
            if(vertices[0].hasNormal())
            {
                glm::vec3 p = vertices[i].getNormal();
                output->write(reinterpret_cast<char*>( &p[0] ), sizeof(glm::vec3));
            }
            
            if(vertices[0].hasUV0())
            {
                glm::vec2 p = vertices[i].getUV0();
                output->write(reinterpret_cast<char*>( &p[0] ), sizeof(glm::vec2));
            }
            
            if(vertices[0].hasColour())
            {
                glm::vec4 p = vertices[i].getColour();
                output->write(reinterpret_cast<char*>( &p[0] ), sizeof(glm::vec4));
            }
            if(vertices[0].hasJointId())
            {
                glm::ivec4 p = vertices[i].getJointId();
                output->write(reinterpret_cast<char*>( &p[0] ), sizeof(glm::ivec4));
            }
            
            if(vertices[0].hasJointWeight())
            {
                glm::vec4 p = vertices[i].getJointWeight();
                output->write(reinterpret_cast<char*>( &p[0] ), sizeof(glm::vec4));
            }
        }
        
        
        // END TEST
        
       // std::ofstream test;
       // test.open("test.txt", std::ios::binary);
       // test.write(reinterpret_cast<char*>( &vertexData[0] ), vertexData.size() * sizeof(float));
       // test.close();
       /* for(unsigned int i = 0; i < vertices.size(); i++)
        {
            output->write( reinterpret_cast<char*>( &vertices[i] ), sizeof vertices[i] );
        }*/
        
        //std::ostream_iterator<float> output_iterator(*output, " ");
        //std::copy(vertexData.begin(), vertexData.end(), output_iterator);
        *output << std::endl;
        
        // Write index data
        *output << "-I " << indices.size() <<  " ";
        output->write(reinterpret_cast<char*>( &indices[0] ), indices.size() * sizeof(unsigned int));
        //std::ostream_iterator<unsigned int> output_iterator2(*output, " ");
        //std::copy(indices.begin(), indices.end(), output_iterator2);
        
        
        return true;
    }
}
