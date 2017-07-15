
// STD
#include <string>
#include <iostream>

#include "DAE2JMP/JMPData.h"
#include "DAE2JMP/DAEImporter.h"
#include "DAE2JMP/JMPExporter.h"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "Must provIde a DAE file path and a target file path" << std::endl;
        return 1;
    }
    else
    {
        
        DAE2JMP::JMPData jmpData;
        
        // Prepare importer configuration
        DAE2JMP::DAE2JMPConfig config;
        config.inputFile = argv[1];
        config.outputFolder = argv[2];
        config.jmpData = &jmpData;
        
        
        
        // Instantiate loader and read file
        DAE2JMP::DAEImporter importer(config);
        bool status =  importer.Import();
        
        if(status)
        {
            // Instantiate exporter
            DAE2JMP::JMPExporter exporter(config);
            
            status = exporter.Export();
        }
        
        return status;
    }
}
