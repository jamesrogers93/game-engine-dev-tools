#ifndef _SCENE_H
#define _SCENE_H

// STD
#include <string>
#include <map>

#include "DAE2JMP/DAE2JMPWriterMesh.h"
#include "DAE2JMP/DAE2JMPWriterSkin.h"

class Scene
{
public:
    static Scene& getInstance()
    {
        static Scene instance;
        return instance;
    }
    
    std::map<std::string, Mesh> loadedMeshes;
    
private:
    Scene(){}
    
    Scene(Scene const&); // Don't implement
    void operator=(Scene const&); // Don't implement
};


#endif /* _SCENE_H */
