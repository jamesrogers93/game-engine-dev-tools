#include "DAE2JMP/JMPExporter.h"

bool JMPExporter::Export(const DAEImporter &importer)
{
    
    // Open file to write
    this->mOutputStream.open(this->mConfig.outputFile, std::ios::binary | std::ios::trunc);
    
    
    
    return true;
}
