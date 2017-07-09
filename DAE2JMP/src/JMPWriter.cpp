#include "DAE2JMP/JMPWriter.h"

// OpenCOLLADA
#include "COLLADASaxFWLLoader.h"
#include <COLLADAFW.h>

#include "DAE2JMP/DAE2JMPWriterMesh.h"
#include "DAE2JMP/DAE2JMPWriterSkin.h"

bool JMPWriter::write()
{
    
    COLLADASaxFWL::Loader loader;
    COLLADAFW::Root root(&loader, this);
    
    // Load scene
    if( !root.loadDocument(this->config.inputFile))
    {
        return false;
    }
    
    return true;
}


void JMPWriter::cancel(const COLLADABU::String& errorMessage)
{
    std::cout << "Cancel" << std::endl;
    std::cout << "Failed to read file: " + this->config.inputFile << std::endl;
    std::cout << errorMessage << std::endl;
}

void JMPWriter::start()
{
    std::cout << "Started reading file: " + this->config.inputFile << std::endl;
}

void JMPWriter::finish()
{
    std::cout << "Finished reading file: " + this->config.inputFile << std::endl;
}

bool JMPWriter::writeGlobalAsset ( const COLLADAFW::FileInfo* asset )
{
    std::cout << "write global asset" << std::endl;
    return true;
}

bool JMPWriter::writeScene ( const COLLADAFW::Scene* scene )
{
    std::cout << "Wrtie scene" << std::endl;
    return true;
}

bool JMPWriter::writeVisualScene ( const COLLADAFW::VisualScene* visualScene )
{
    std::cout << "Write visual scene" << std::endl;
    return true;
}

bool JMPWriter::writeLibraryNodes ( const COLLADAFW::LibraryNodes* libraryNodes )
{
    std::cout << "Write library nodes" << std::endl;
    return true;
}

bool JMPWriter::writeGeometry ( const COLLADAFW::Geometry* geometry )
{
    std::cout << "Write geometry" << std::endl;

    if(geometry->getType() != COLLADAFW::Geometry::GEO_TYPE_MESH)
    {
        return true;
    }
    
    WriterMesh writerMesh( this, (COLLADAFW::Mesh*)geometry );
    return writerMesh.write();
}

bool JMPWriter::writeMaterial( const COLLADAFW::Material* material )
{
    std::cout << "Write material" << std::endl;
    return true;
}

bool JMPWriter::writeEffect( const COLLADAFW::Effect* effect )
{
    std::cout << "Write effect" << std::endl;
    return true;
}

bool JMPWriter::writeCamera( const COLLADAFW::Camera* camera )
{
    std::cout << "Write camera" << std::endl;
    return true;
}

bool JMPWriter::writeImage( const COLLADAFW::Image* image )
{
    std::cout << "Write image" << std::endl;
    return true;
}

bool JMPWriter::writeLight( const COLLADAFW::Light* light )
{
    std::cout << "Write light" << std::endl;
    return true;
}

bool JMPWriter::writeAnimation( const COLLADAFW::Animation* animation )
{
    std::cout << "Write animation" << std::endl;
    return true;
}

bool JMPWriter::writeAnimationList( const COLLADAFW::AnimationList* animationList )
{
    std::cout << "Write animation list" << std::endl;
    return true;
}

bool JMPWriter::writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData )
{
    std::cout << "Write skin contoller data" << std::endl;
    
    WriterSkin writerSkin( this, skinControllerData );
    return writerSkin.write();
}

bool JMPWriter::writeController( const COLLADAFW::Controller* controller )
{
    std::cout << "Write controller" << std::endl;
    return true;
}

bool JMPWriter::writeFormulas( const COLLADAFW::Formulas* formulas )
{
    std::cout << "Write formulas" << std::endl;
    return true;
}

bool JMPWriter::writeKinematicsScene( const COLLADAFW::KinematicsScene* kinematicsScene )
{
    std::cout << "Write kinematics scene" << std::endl;
    return true;
}





