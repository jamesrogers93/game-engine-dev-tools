#ifndef _DAEIMPORTERMATERIAL_H
#define _DAEIMPORTERMATERIAL_H

#include <string>

#include "DAE2JMP/DAEImporterBase.h"
#include <COLLADAFWMaterial.h>

namespace DAE2JMP
{
    struct DAEMaterial
    {
        std::string name;
        unsigned long long effectId;
    };

    class DAEImporterMaterial : public DAEImporterBase
    {
        
    public:
        DAEImporterMaterial(DAEImporter *importer) : DAEImporterBase(importer)
        {}
        
        bool import(const COLLADAFW::Material*);
        
    private:
        
        DAEMaterial getMaterialData(const COLLADAFW::Material*);
        
    };

}

#endif /* _DAEIMPORTERMATERIAL_H */


