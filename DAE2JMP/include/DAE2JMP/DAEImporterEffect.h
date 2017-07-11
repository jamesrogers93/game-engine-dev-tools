#ifndef _DAEIMPORTEREFFECT_H
#define _DAEIMPORTEREFFECT_H

#include <string>

#include "DAE2JMP/DAEImporterBase.h"
#include <COLLADAFWEffect.h>

struct Effect
{
    std::string name;
    
    float ambient[4] = {0.0};
    float diffuse[4] = {0.0};
    float specular[4] = {0.0};
    float shininess = 0.0;
    
    bool hasAmbient = false;
    bool hasDiffuse = false;
    bool hasSpecular = false;
    bool hasShininess = false;
};

class DAEImporterEffect : public DAEImporterBase
{
    
public:
    DAEImporterEffect(DAEImporter *importer) : DAEImporterBase(importer)
    {}
    
    bool import(const COLLADAFW::Effect*);
    
private:
    
    Effect getEffectData(const COLLADAFW::Effect*);
    
};


#endif /* _DAEIMPORTEREFFECT_H */
