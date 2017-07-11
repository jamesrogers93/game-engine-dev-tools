
// STD
#include <string>
#include <iostream>

#include "DAE2JMP/DAEImporter.h"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "Must provIde a DAE file path and a target file path" << std::endl;
        return 1;
    }
    else
    {
        // Prepare importer configuration
        DAEImporter::DAEImporterConfig config;
        config.inputFile = argv[1];
        config.outputFile = argv[2];
        config.loadGeometry = true;
        
        // Instantiate loader and read file
        DAEImporter importer(config);
        bool status =  importer.import();
        
        return status;
    }
}
