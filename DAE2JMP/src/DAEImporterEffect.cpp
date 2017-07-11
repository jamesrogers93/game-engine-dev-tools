#include "DAE2JMP/DAEImporterEffect.h"

#include "DAE2JMP/DAEImporter.h"

#include <COLLADAFWEffectCommon.h>

bool DAEImporterEffect::import(const COLLADAFW::Effect* eff)
{
    if(eff == NULL) return false;
    
    Effect effData = getEffectData(eff);
    
    return this->mDAEImporter->addLoadedEffect(eff->getObjectId(), effData);
}

Effect DAEImporterEffect::getEffectData(const COLLADAFW::Effect* eff)
{
    Effect effData;
    
    unsigned int numCommonEffects = eff->getCommonEffects().getCount();
    
    if(numCommonEffects == 0)
    {
        return effData;
    }
    
    const COLLADAFW::CommonEffectPointerArray comEffects = eff->getCommonEffects();

    // Only take the first common effect
    unsigned int i = 0;
    //for(unsigned int i = 0; i < numCommonEffects; i++)
    //{
        COLLADAFW::EffectCommon *comEff = comEffects[i];
        
        // Get Ambient
        COLLADAFW::ColorOrTexture ambient = comEff->getAmbient();
        if(ambient.getType() == COLLADAFW::ColorOrTexture::COLOR)
        {
            // Set colour data
            COLLADAFW::Color c = ambient.getColor();
            effData.ambient[0] = c.getRed();
            effData.ambient[1] = c.getGreen();
            effData.ambient[2] = c.getBlue();
            effData.ambient[3] = c.getAlpha();
            
            // Set flag
            effData.hasAmbient = true;
        }
        
        // Get Diffuse
        COLLADAFW::ColorOrTexture diffuse = comEff->getDiffuse();
        if(diffuse.getType() == COLLADAFW::ColorOrTexture::COLOR)
        {
            // Set colour data
            COLLADAFW::Color c = diffuse.getColor();
            effData.diffuse[0] = c.getRed();
            effData.diffuse[1] = c.getGreen();
            effData.diffuse[2] = c.getBlue();
            effData.diffuse[3] = c.getAlpha();
            
            // Set flag
            effData.hasDiffuse = true;
        }
        
        // Get Specular
        COLLADAFW::ColorOrTexture specular = comEff->getSpecular();
        if(specular.getType() == COLLADAFW::ColorOrTexture::COLOR)
        {
            // Set colour data
            COLLADAFW::Color c = diffuse.getColor();
            effData.specular[0] = c.getRed();
            effData.specular[1] = c.getGreen();
            effData.specular[2] = c.getBlue();
            effData.specular[3] = c.getAlpha();
            
            // Set flag
            effData.hasSpecular = true;
        }
        
        // Get shininess
        COLLADAFW::FloatOrParam shininess = comEff->getShininess();
        if(shininess.getType() == COLLADAFW::FloatOrParam::FLOAT)
        {
            // Set shininess data
            effData.shininess = shininess.getFloatValue();
            
            // Set flag
            effData.hasShininess = true;
        }
    
        effData.name = eff->getName();
   // }
    
    return effData;
}
