
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
        
        JMPData jmpData;
        
        // Prepare importer configuration
        DAEImporter::DAEImporterConfig imConfig;
        imConfig.inputFile = argv[1];
        imConfig.jmpData = &jmpData;
        
        
        
        // Instantiate loader and read file
        DAEImporter importer(imConfig);
        bool status =  importer.Import();
        
        if(status)
        {
            // Instantiate exporter
            JMPExporter::JMPExporterConfig exConfig;
            exConfig.outputFile = argv[2];
            exConfig.jmpData = &jmpData;
            
            JMPExporter exporter(exConfig);
            
            status = exporter.Export(importer);
        }
        
        return status;
    }
}
