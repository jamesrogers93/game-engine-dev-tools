#ifndef _DAEIMPORTERMESH_H
#define _DAEIMPORTERMESH_H

#include "DAE2JMP/DAEImporterBase.h"

#include <COLLADAFWMesh.h>

#include <string>
#include <vector>
#include <map>
#include <utility>

/*struct Vertex;

typedef std::vector<unsigned int> UIntList;
typedef std::vector<float> FloatList;
typedef std::map<Vertex, int> VertexIndexMap;
typedef UIntList VertexElements;

struct VertexSourceData
{
    // Index data
    COLLADAFW::UIntValuesArray* positionIndices;
    COLLADAFW::UIntValuesArray* normalIndices;
    COLLADAFW::UIntValuesArray* uv0Indices;
    COLLADAFW::UIntValuesArray* colourIndices;
    
    // Data present
    bool hasPositions = false;
    bool hasNormals = false;
    bool hasUV0s = false;
    bool hasColours = false;
    
    // Count of data
    size_t positionIndicesCount = 0;
    size_t normalIndicesCount = 0;
    size_t uv0IndicesCount = 0;
    size_t colourIndicesCount = 0;
    
    // Stride of data
    size_t positionStride = 0;
    size_t normalStride = 0;
    size_t uv0Stride = 0;
    size_t colourStride = 0;
};

struct Vertex
{
    unsigned int positionIndex;
    unsigned int normalIndex;
    unsigned int uv0Index;
    unsigned int colourIndex;
    
    Vertex(const unsigned int &positionIndex, const unsigned int &normalIndex, const unsigned int &uv0Index, const unsigned int &colourIndex) : positionIndex(positionIndex), normalIndex(normalIndex), uv0Index(uv0Index), colourIndex(colourIndex)
    {}
    
    bool operator<(const Vertex& rhs) const;
};

struct VertexIndexData
{
    VertexIndexMap map;
    VertexElements elements;
    unsigned int nextVertexIndex;
};

struct VertexData
{
    FloatList positions;
    FloatList normals;
    FloatList uv0s;
    FloatList colours;
};

struct Mesh
{
    VertexSourceData sources;
    VertexData vertices;
    VertexIndexData indices;
};*/

namespace DAE2JMP
{
    struct DAEMesh
    {
        std::string name;
        
        // Source data
        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> uv0s;
        std::vector<float> colours;
        
        // Index data
        std::vector<unsigned int> positionIndices;
        std::vector<unsigned int> normalIndices;
        std::vector<unsigned int> uv0Indices;
        std::vector<unsigned int> colourIndices;
        
        // Data present flags
        bool hasPositions = false;
        bool hasNormals = false;
        bool hasUV0s = false;
        bool hasColours = false;
        
        // Count of data
        size_t positionsCount = 0;
        size_t normalsCount = 0;
        size_t uv0sCount = 0;
        size_t coloursCount = 0;
        size_t positionIndicesCount = 0;
        size_t normalIndicesCount = 0;
        size_t uv0IndicesCount = 0;
        size_t colourIndicesCount = 0;
        
        // Stride of data
        size_t positionStride = 0;
        size_t normalStride = 0;
        size_t uv0Stride = 0;
        size_t colourStride = 0;
    };

    class DAEImporterMesh : public DAEImporterBase
    {
    private:
        //typedef std::pair<VertexIndexMap, VertexElements> VertexIndexData;
        
        static const std::string POSITION;
        static const std::string NORMAL;
        static const std::string UV0;
        static const std::string COLOUR;
        
        static const unsigned int POSITION_STRIDE;
        static const unsigned int NORMAL_STRIDE;
        
        //const COLLADAFW::Mesh* mMesh;
        
    public:
        //ImporterMesh(JMPImporter *importer, const COLLADAFW::Mesh* mesh) : ImporterBase(importer), mMesh(mesh)
        //{}
        DAEImporterMesh(DAEImporter *importer) : DAEImporterBase(importer)
        {}
        
        bool import(const COLLADAFW::Mesh* mesh);
        
        //bool write();
        
    private:
        
        DAEMesh getMeshData(const COLLADAFW::Mesh* mesh);
        
        /*VertexSourceData getSourceData(COLLADAFW::MeshPrimitive* meshPrimitive);
        std::pair<VertexIndexData, VertexData> getVertexData(COLLADAFW::MeshPrimitive* meshPrimitive, const VertexSourceData &data);
        void addVertex(VertexIndexData &vertexIndexData, VertexData &vertexData, Vertex &vertex, const VertexSourceData &data);
        
        void writeMeshHeader(std::ofstream &output, const VertexSourceData &data);
        void writeVertices(std::ofstream &output, const std::pair<VertexIndexData, VertexData>& vertexData, const VertexSourceData &sourceData);
        void writeIndices(std::ofstream &output, const std::pair<VertexIndexData, VertexData>& vertexData, const VertexSourceData &sourceData);*/
    };

}

#endif /* _DAEIMPORTERMESH_H */
