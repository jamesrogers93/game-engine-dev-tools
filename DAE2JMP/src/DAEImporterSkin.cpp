#include "DAE2JMP/DAEImporterSkin.h"

#include <vector>
#include <utility>

#include "DAE2JMP/DAEImporter.h"
#include "DAE2JMP/DAEUtil.h"

bool SkinVertex::operator<( const SkinVertex& rhs ) const
{
    if(jointIndices.size() < rhs.jointIndices.size())
        return true;
    
    if(jointIndices.size() > rhs.jointIndices.size())
        return false;
    
    for(int i = 0; i < jointIndices.size(); i++)
    {
        if ( jointIndices[i] < rhs.jointIndices[i] )
            return true;
    
        if ( jointIndices[i] > rhs.jointIndices[i] )
            return false;
    
        if ( weightIndices[i] < rhs.weightIndices[i] )
            return true;
    
        if ( weightIndices[i] > rhs.weightIndices[i] )
            return false;
    }
    
    return false;
}

const float DAEImporterSkin::MAX_JOINT_INFLUENCE = 4;

bool DAEImporterSkin::import(const COLLADAFW::SkinControllerData* skin)
{
    
    Skin skinData = getSkinData(skin);
    
    return this->mDAEImporter->addLoadedSkin(skin->getObjectId(), skinData);
}

Skin DAEImporterSkin::getSkinData(const COLLADAFW::SkinControllerData* skinControllerData)
{
    Skin skinData;  // Contains the accumalated skinVertex data
    
    size_t vertexCount = skinControllerData->getVertexCount ();
    // The list with the numbers of influences per vertex.
    const COLLADAFW::UIntValuesArray& jointsPerVertex = skinControllerData->getJointsPerVertex ();
    // The joint indices
    const COLLADAFW::IntValuesArray& jointIndices = skinControllerData->getJointIndices ();
    // The list of indices to the weight list.
    const COLLADAFW::UIntValuesArray& weightIndices = skinControllerData->getWeightIndices ();
    // The weight list.
    const COLLADAFW::FloatOrDoubleArray& weights = skinControllerData->getWeights ();
    
    unsigned int index = 0;
    for(unsigned int i = 0; i < vertexCount; i++)
    {
        unsigned int numVertexInfluences = jointsPerVertex[i];
        
        // Add all skin data to a temporary vector
        std::vector<std::pair<int, float> > jointWeight;
        
        for(unsigned int j = 0; j < numVertexInfluences; j++)
        {
            unsigned int jointIndex = jointIndices[index + j];
            unsigned int weightIndex = weightIndices[index + j];
            float weight = getFloatValue(weights, weightIndex);
            jointWeight.push_back(std::make_pair(jointIndex, weight));
        }
        
        if(numVertexInfluences > MAX_JOINT_INFLUENCE)
        {
            // Remove least significant weights and normalise
            
            // Sort into descending order
            std::sort(jointWeight.begin(), jointWeight.end(), [] (const std::pair<int,float> &left, const std::pair<int,float> &right)
                      {
                          return left.second > right.second;
                      });
            
            // Keep first MAX_JOINT_INFLUENCE elements
            jointWeight.resize(MAX_JOINT_INFLUENCE);
            
            // Normalise weights
            float sum = 0.0;
            for (auto& n : jointWeight)
            {
                sum += n.second;
            }
            for (auto& n : jointWeight)
            {
                n.second /= sum;
            }
            
        }
        else if(numVertexInfluences < MAX_JOINT_INFLUENCE)
        {
            // Add all data to skinData
            for(int i = 0; i < numVertexInfluences; i++)
            {
                skinData.jointIDs.push_back(jointWeight[i].first);
                skinData.weights.push_back(jointWeight[i].second);
            }
            
            // Pad with 0s till size of MAX_JOINT_INFLUENCE
            for(unsigned int i = numVertexInfluences; i < MAX_JOINT_INFLUENCE; i++)
            {
                skinData.jointIDs.push_back(0);
                skinData.weights.push_back(0.0);
            }
        }
        else
        {
            // Size is the same, just add all data on to end
            for(unsigned int i = 0; i < MAX_JOINT_INFLUENCE; i++)
            {
                skinData.jointIDs.push_back(jointWeight[i].first);
                skinData.weights.push_back(jointWeight[i].second);
            }
        }
        
        
        index += numVertexInfluences;
        
    }
    
    skinData.name = skinControllerData->getName();
    
    return skinData;
}

/*void DAEImporterSkin::addSkinVertex(Skin& skinData, SkinVertex& skinVertex)
//void DAEImporterSkin::addSkinVertex(SkinIndexMap& map, Skin& skinData, SkinVertex& skinVertex)
{
    //SkinIndexMap::const_iterator it = map.find(skinVertex);
    
    //if(it == map.end())
    //{
        // Not found
       // map[skinVertex] = 0;
        
        // Add all skin data to a temporary vector
        std::vector<std::pair<int, float> > jointWeight;
        
        // Get joints and weights
        if(mSkin->getWeights().getType() == COLLADAFW::FloatOrDoubleArray::DATA_TYPE_DOUBLE)
        {
            const double *weightsArray = mSkin->getWeights().getDoubleValues()->getData();
            
            for(unsigned int i = 0; i < skinVertex.jointIndices.size(); i++)
            {
                jointWeight.push_back(std::make_pair(skinVertex.jointIndices[i], (float)weightsArray[i]));
            }
        }
        else
        {
            const float *weightsArray = mSkin->getWeights().getFloatValues()->getData();
            
            for(unsigned int i = 0; i < skinVertex.jointIndices.size(); i++)
            {
                unsigned int jointIndex = skinVertex.jointIndices[i];
                unsigned int weightIndex = skinVertex.weightIndices[i];
                jointWeight.push_back(std::make_pair(jointIndex, weightsArray[weightIndex]));
            }
        }
        
        if(skinVertex.jointIndices.size() > 4)
        {
            // Remove least significant weights and normalise
            
            // Sort into descending order
            std::sort(jointWeight.begin(), jointWeight.end(), [] (const std::pair<int,float> &left, const std::pair<int,float> &right)
                      {
                          return left.second > right.second;
                      });
            
            // Keep first 4 elements
            jointWeight.resize(4);
            
            // Normalise weights
            float sum = 0.0;
            for (auto& n : jointWeight)
            {
                sum += n.second;
            }
            for (auto& n : jointWeight)
            {
                n.second /= sum;
            }
            
        }
        else if(skinVertex.jointIndices.size() < 4)
        {
            // Add all data to skinData
            for(int i = 0; i < skinVertex.jointIndices.size(); i++)
            {
                skinData.joints.push_back(jointWeight[i].first);
                skinData.weights.push_back(jointWeight[i].second);
            }
            
            // Pad with 0s till size of 4
            for(unsigned int i = skinVertex.jointIndices.size(); i < 4; i++)
            {
                skinData.joints.push_back(0);
                skinData.weights.push_back(0.0);
            }
        }
        else
        {
            // Size is the same, just add all data on to end
            for(unsigned int i = 0; i < 4; i++)
            {
                skinData.joints.push_back(jointWeight[i].first);
                skinData.weights.push_back(jointWeight[i].second);
            }
        }
    //}
    //else
    //{
        // Found
        // Dont need to store index
        // Do nothing
    //}
}*/
