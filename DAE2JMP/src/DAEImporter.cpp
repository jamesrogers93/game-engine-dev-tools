#include "DAE2JMP/DAEImporter.h"

// OpenCOLLADA
#include "COLLADASaxFWLLoader.h"
#include <COLLADAFW.h>

#include "DAE2JMP/DAEImporterMesh.h"
#include "DAE2JMP/DAEImporterSkin.h"
#include "DAE2JMP/DAEImporterMaterial.h"
#include "DAE2JMP/DAEImporterEffect.h"
#include "DAE2JMP/DAEImporterSkinController.h"

bool DAEImporter::Import()
{
    
    bool status;

    COLLADASaxFWL::Loader loader;
    COLLADAFW::Root root(&loader, this);
    
    // Import scene
    status = root.loadDocument(this->config.inputFile);
    
    
    // Now convert imported data to JMP
    
    
    return status;
}

bool DAEImporter::addLoadedMesh(const unsigned long long& Id, const Mesh& mesh)
{
    if(this->loadedMeshes.find(Id) == this->loadedMeshes.end())
    {
        this->loadedMeshes[Id] = mesh;
        return true;
    }

    return false;
}

bool DAEImporter::addLoadedSkin(const unsigned long long& Id, const Skin& skin)
{
    if(this->loadedSkins.find(Id) == this->loadedSkins.end())
    {
        this->loadedSkins[Id] = skin;
        return true;
    }
    
    return false;
}

bool DAEImporter::addLoadedMaterial(const unsigned long long& Id, const Material& mat)
{
    if(this->loadedMaterials.find(Id) == this->loadedMaterials.end())
    {
        this->loadedMaterials[Id] = mat;
        return true;
    }
    
    return false;
}

bool DAEImporter::addLoadedEffect(const unsigned long long& Id, const Effect& eff)
{
    if(this->loadedEffects.find(Id) == this->loadedEffects.end())
    {
        this->loadedEffects[Id] = eff;
        return true;
    }

    return false;
}

bool DAEImporter::addLoadedSkinController(const unsigned long long& Id, const SkinController& sCont)
{
    if(this->loadedSkinControllers.find(Id) == this->loadedSkinControllers.end())
    {
        this->loadedSkinControllers[Id] = sCont;
        return true;
    }
    
    return false;
}

bool DAEImporter::addLoadedVisualScene(const unsigned long long& Id, const VisualScene& scene)
{
    if(this->loadedVisualScenes.find(Id) == this->loadedVisualScenes.end())
    {
        this->loadedVisualScenes[Id] = scene;
        return true;
    }
    
    return false;
}

Mesh* DAEImporter::getLoadedMesh(const unsigned long long& Id)
{
    if(this->loadedMeshes.find(Id) == this->loadedMeshes.end())
    {
        return NULL;
    }
    
    return &this->loadedMeshes[Id];
}

Skin* DAEImporter::getLoadedSkin(const unsigned long long& Id)
{
    if(this->loadedSkins.find(Id) == this->loadedSkins.end())
    {
        return NULL;
    }
    
    return &this->loadedSkins[Id];
}

Material* DAEImporter::getLoadedMaterial(const unsigned long long & Id)
{
    if(this->loadedMaterials.find(Id) == this->loadedMaterials.end())
    {
        return NULL;
    }
    
    return &this->loadedMaterials[Id];
}

Effect* DAEImporter::getLoadedEffect(const unsigned long long& Id)
{
    if(this->loadedEffects.find(Id) == this->loadedEffects.end())
    {
        return NULL;
    }
    
    return &this->loadedEffects[Id];
}

SkinController* DAEImporter::getLoadedSkinController(const unsigned long long& Id)
{
    if(this->loadedSkinControllers.find(Id) == this->loadedSkinControllers.end())
    {
        return NULL;
    }
    
    return &this->loadedSkinControllers[Id];
}

VisualScene* DAEImporter::getLoadedVisualScene(const unsigned long long& Id)
{
    if(this->loadedVisualScenes.find(Id) == this->loadedVisualScenes.end())
    {
        return NULL;
    }
    
    return &this->loadedVisualScenes[Id];
}

void DAEImporter::cancel(const COLLADABU::String& errorMessage)
{
    std::cout << "Cancel" << std::endl;
    std::cout << "Failed to read file: " + this->config.inputFile << std::endl;
    std::cout << errorMessage << std::endl;
}

void DAEImporter::start()
{
    std::cout << "Started reading file: " + this->config.inputFile << std::endl;
}

void DAEImporter::finish()
{
    std::cout << "Finished reading file: " + this->config.inputFile << std::endl;
}

bool DAEImporter::writeGlobalAsset ( const COLLADAFW::FileInfo* asset )
{
    std::cout << "write global asset" << std::endl;
    return true;
}

bool DAEImporter::writeScene ( const COLLADAFW::Scene* scene )
{
    std::cout << "Write scene" << std::endl;
    
    return true;
}

bool DAEImporter::writeVisualScene ( const COLLADAFW::VisualScene* visualScene )
{
    std::cout << "Write visual scene" << std::endl;
    
    DAEImporterVisualScene importerVisualScene(this);
    return importerVisualScene.import(visualScene);
}

bool DAEImporter::writeLibraryNodes ( const COLLADAFW::LibraryNodes* libraryNodes )
{
    std::cout << "Write library nodes" << std::endl;
    return true;
}

bool DAEImporter::writeGeometry ( const COLLADAFW::Geometry* geometry )
{
    std::cout << "Write geometry" << std::endl;

    if(geometry->getType() != COLLADAFW::Geometry::GEO_TYPE_MESH)
    {
        return true;
    }
    
    //ImporterMesh importerMesh( this, (COLLADAFW::Mesh*)geometry );
    //return importerMesh.write();
    
    DAEImporterMesh importerMesh(this);
    return importerMesh.import((COLLADAFW::Mesh*)geometry);
}

bool DAEImporter::writeMaterial( const COLLADAFW::Material* material )
{
    std::cout << "Write material" << std::endl;
    
    DAEImporterMaterial importerMaterial(this);
    return importerMaterial.import(material);
}

bool DAEImporter::writeEffect( const COLLADAFW::Effect* effect )
{
    std::cout << "Write effect" << std::endl;
    
    DAEImporterEffect importerEffect(this);
    return importerEffect.import(effect);
}

bool DAEImporter::writeCamera( const COLLADAFW::Camera* camera )
{
    std::cout << "Write camera" << std::endl;
    return true;
}

bool DAEImporter::writeImage( const COLLADAFW::Image* image )
{
    std::cout << "Write image" << std::endl;
    return true;
}

bool DAEImporter::writeLight( const COLLADAFW::Light* light )
{
    std::cout << "Write light" << std::endl;
    return true;
}

bool DAEImporter::writeAnimation( const COLLADAFW::Animation* animation )
{
    std::cout << "Write animation" << std::endl;
    return true;
}

bool DAEImporter::writeAnimationList( const COLLADAFW::AnimationList* animationList )
{
    std::cout << "Write animation list" << std::endl;
    return true;
}

bool DAEImporter::writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData )
{
    std::cout << "Write skin contoller data" << std::endl;
    
    DAEImporterSkin importerSkin(this);
    return importerSkin.import(skinControllerData);
}

bool DAEImporter::writeController( const COLLADAFW::Controller* controller )
{
    std::cout << "Write controller" << std::endl;
    
    
    if(controller->getControllerType() != COLLADAFW::Controller::CONTROLLER_TYPE_SKIN)
    {
        return true;
    }
    
    COLLADAFW::SkinController *skinController = (COLLADAFW::SkinController*)controller;
    
    DAEImporterSkinController importerSkinController(this);
    return importerSkinController.import(skinController);
}

bool DAEImporter::writeFormulas( const COLLADAFW::Formulas* formulas )
{
    std::cout << "Write formulas" << std::endl;
    return true;
}

bool DAEImporter::writeKinematicsScene( const COLLADAFW::KinematicsScene* kinematicsScene )
{
    std::cout << "Write kinematics scene" << std::endl;
    return true;
}





