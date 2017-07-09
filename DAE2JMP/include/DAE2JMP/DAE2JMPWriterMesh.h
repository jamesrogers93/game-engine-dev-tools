#ifndef _DAE2JMPWRITERMESH_H
#define _DAE2JMPWRITERMESH_H

#include "DAE2JMP/DAE2JMPWriterBase.h"

#include <COLLADAFWMesh.h>

#include <vector>
#include <map>
#include <utility>

struct Vertex;

typedef std::vector<unsigned int> UIntList;
typedef std::vector<float> FloatList;
typedef std::map<Vertex, int> VertexIndexMap;
typedef UIntList VertexElements;

struct SourceData
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
    SourceData sources;
    VertexData vertices;
    VertexIndexData indices;
};

class WriterMesh : public WriterBase
{
private:
    //typedef std::pair<VertexIndexMap, VertexElements> VertexIndexData;
    
    static const std::string POSITION;
    static const std::string NORMAL;
    static const std::string UV0;
    static const std::string COLOUR;
    
    static const unsigned int POSITION_STRIDE;
    static const unsigned int NORMAL_STRIDE;
    
    const COLLADAFW::Mesh* mMesh;
    
public:
    WriterMesh(JMPWriter *writer, const COLLADAFW::Mesh* mesh) : WriterBase(writer), mMesh(mesh)
    {}
    
    bool write();
    
private:
    
    SourceData getSourceData(COLLADAFW::MeshPrimitive* meshPrimitive);
    std::pair<VertexIndexData, VertexData> getVertexData(COLLADAFW::MeshPrimitive* meshPrimitive, const SourceData &data);
    void addVertex(VertexIndexData &vertexIndexData, VertexData &vertexData, Vertex &vertex, const SourceData &data);
    
    void writeMeshHeader(std::ofstream &output, const SourceData &data);
    void writeVertices(std::ofstream &output, const std::pair<VertexIndexData, VertexData>& vertexData, const SourceData &sourceData);
    void writeIndices(std::ofstream &output, const std::pair<VertexIndexData, VertexData>& vertexData, const SourceData &sourceData);
};

#endif /* _DAE2JMPWRITERMESH_H */
