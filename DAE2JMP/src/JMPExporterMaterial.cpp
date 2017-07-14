#include "DAE2JMP/JMPExporterMaterial.h"

#include "DAE2JMP/JMPExporter.h"

#include "game-engine/Core/Modules/Graphics/Material.h"

#include "glm/gtx/string_cast.hpp"

namespace DAE2JMP
{
    bool JMPExporterMaterial::Export(const Material* mat)
    {
        
        std::ofstream *output = this->mJMPExporter->getOutputStream();
        
        // Write diffuse
        if(mat->hasDiffuseTexture())
        {
            *output << "diff T " << mat->getDiffuseTexture() << std::endl;
        }
        else if (mat->hasDiffuseSolid())
        {
            glm::vec4 diff = mat->getDiffuseSolid();
            *output << "diff S " << diff.x << " " << diff.y << " " << diff.z << " " << diff.w << std::endl;
        }
        
        // Write specular
        if(mat->hasSpecularTexture())
        {
            *output << "spec T " << mat->getSpecularTexture() << std::endl;
        }
        else if (mat->hasSpecularSolid())
        {
            glm::vec4 spec = mat->getSpecularSolid();
            *output << "spec S " << spec.x << " " << spec.y << " " << spec.z << " " << spec.w << std::endl;
        }
        
        // Write shininess
        *output << "shin " << mat->getShininess() << std::endl;
        
        return true;
    }
    
}
