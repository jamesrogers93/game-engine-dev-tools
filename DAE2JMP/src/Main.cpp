
// STD
#include <string>
#include <iostream>

#include "DAE2JMP/JMPWriter.h"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "Must provide a DAE file path and a target file path" << std::endl;
        return 1;
    }
    else
    {
        // Prepare loader configuration
        JMPWriter::JMPWriterConfig config;
        config.inputFile = argv[1];
        config.outputFile = argv[2];
        config.loadGeometry = true;
        
        // Instantiate loader and read file
        JMPWriter writer(config);
        return writer.write();
    }
}
