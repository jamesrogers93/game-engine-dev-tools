#ifndef _DAEIMPORTER_H
#define _DAEIMPORTER_H

#include <string>

#include "COLLADAFWIWriter.h"
#include "COLLADAFWUniqueId.h"
#include "COLLADAFWVisualScene.h"
#include "COLLADAFWLibraryNodes.h"
#include "COLLADAFWMaterial.h"
#include "COLLADAFWEffect.h"
#include "COLLADAFWImage.h"

#include "COLLADABUURI.h"
#include "Math/COLLADABUMathMatrix4.h"

#include "DAE2JMP/DAEImporterMesh.h"
#include "DAE2JMP/DAEImporterSkin.h"
#include "DAE2JMP/DAEImporterMaterial.h"
#include "DAE2JMP/DAEImporterEffect.h"
#include "DAE2JMP/DAEImporterSkinController.h"


//class Scene;

class DAEImporter : public COLLADAFW::IWriter
{
    
public:
    
    struct DAEImporterConfig
    {
        std::string inputFile, outputFile;
        bool loadGeometry = false;
    };
    
    DAEImporter(const DAEImporterConfig &config) : config(config){}
    
    const std::string getInputFile() {  return this->config.inputFile;  }
    const std::string getOutputFile() {  return this->config.outputFile;  }
    
    bool import();
    
    bool addLoadedMesh(const unsigned long long&, const Mesh&);
    bool addLoadedSkin(const unsigned long long&, const Skin&);
    bool addLoadedMaterial(const unsigned long long&, const Material&);
    bool addLoadedEffect(const unsigned long long&, const Effect&);
    bool addLoadedSkinController(const unsigned long long&, const SkinController&);
    
    Mesh* getLoadedMesh(const unsigned long long&);
    Skin* getLoadedSkin(const unsigned long long&);
    Material* getLoadedMaterial(const unsigned long long&);
    Effect* getLoadedEffect(const unsigned long long&);
    SkinController* getLoadedSkinController(const unsigned long long&);
    
    /** This method will be called if an error in the loading process occurred and the loader cannot
     continue to to load. The writer should undo all operations that have been performed.
     @param errorMessage A message containing informations about the error that occurred.
     */
    virtual void cancel(const COLLADABU::String& errorMessage);
    
    /** This is the method called. The writer hast to prepare to receive data.*/
    virtual void start();
    
    /** This method is called after the last write* method. No other methods will be called after this.*/
    virtual void finish();
    
    /** When this method is called, the writer must write the global document asset.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeGlobalAsset ( const COLLADAFW::FileInfo* asset );
    
    /** When this method is called, the writer must write the scene.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeScene ( const COLLADAFW::Scene* scene );
    
    /** When this method is called, the writer must write the entire visual scene.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeVisualScene ( const COLLADAFW::VisualScene* visualScene );
    
    /** When this method is called, the writer must handle all nodes contained in the
     library nodes.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeLibraryNodes ( const COLLADAFW::LibraryNodes* libraryNodes );
    
    /** When this method is called, the writer must write the geometry.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeGeometry ( const COLLADAFW::Geometry* geometry );
    
    /** When this method is called, the writer must write the material.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeMaterial( const COLLADAFW::Material* material );
    
    /** When this method is called, the writer must write the effect.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeEffect( const COLLADAFW::Effect* effect );
    
    /** When this method is called, the writer must write the camera.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeCamera( const COLLADAFW::Camera* camera );
    
    /** When this method is called, the writer must write the image.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeImage( const COLLADAFW::Image* image );
    
    /** When this method is called, the writer must write the light.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeLight( const COLLADAFW::Light* light );
    
    /** When this method is called, the writer must write the Animation.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeAnimation( const COLLADAFW::Animation* animation );
    
    /** When this method is called, the writer must write the AnimationList.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeAnimationList( const COLLADAFW::AnimationList* animationList );
    
    /** When this method is called, the writer must write the skin controller data.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData );
    
    /** When this method is called, the writer must write the controller.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeController( const COLLADAFW::Controller* controller );
    
    /** When this method is called, the writer must write the formulas. All the formulas of the entire
     COLLADA file are contained in @a formulas.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeFormulas( const COLLADAFW::Formulas* formulas );
    
    /** When this method is called, the writer must write the kinematics scene.
     @return The writer should return true, if writing succeeded, false otherwise.*/
    virtual bool writeKinematicsScene( const COLLADAFW::KinematicsScene* kinematicsScene );
    
private:
    DAEImporterConfig config;
    
    std::map<unsigned long long, Mesh> loadedMeshes;
    std::map<unsigned long long, Skin> loadedSkins;
    std::map<unsigned long long, Material> loadedMaterials;
    std::map<unsigned long long, Effect> loadedEffects;
    std::map<unsigned long long, SkinController> loadedSkinControllers;
    
    DAEImporter(const DAEImporter&);
    DAEImporter& operator=(const DAEImporter&);
};

#endif /* _DAEIMPORTER_H */
