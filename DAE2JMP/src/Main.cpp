
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
        DAE2JMP::DAEImporter::DAEImporterConfig imConfig;
        imConfig.inputFile = argv[1];
        imConfig.jmpData = &jmpData;
        
        
        
        // Instantiate loader and read file
        DAE2JMP::DAEImporter importer(imConfig);
        bool status =  importer.Import();
        
        if(status)
        {
            // Instantiate exporter
            DAE2JMP::JMPExporter::JMPExporterConfig exConfig;
            exConfig.outputFolder = argv[2];
            exConfig.jmpData = &jmpData;
            
            DAE2JMP::JMPExporter exporter(exConfig);
            
            status = exporter.Export();
        }
        
        return status;
    }
}
