#ifndef _DAEIMPORTERMATERIAL_H
#define _DAEIMPORTERMATERIAL_H

#include <string>

#include "DAE2JMP/DAEImporterBase.h"
#include <COLLADAFWMaterial.h>

struct Material
{
    std::string name;
    unsigned long long effectID;
};

class DAEImporterMaterial : public DAEImporterBase
{
    
public:
    DAEImporterMaterial(DAEImporter *importer) : DAEImporterBase(importer)
    {}
    
    bool import(const COLLADAFW::Material*);
    
private:
    
    Material getMaterialData(const COLLADAFW::Material*);
    
};


#endif /* _DAEIMPORTERMATERIAL_H */


