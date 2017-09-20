#ifndef _DAE2JMPCONFIG_H
#define _DAE2JMPCONFIG_H

// STD
#include <string>

#include "DAE2JMP/JMPData.h"

namespace DAE2JMP
{
    
    struct DAE2JMPConfig
    {
        std::string inputFile;
        std::string outputFolder;
        JMPData *jmpData;
    };
}

#endif /* _DAE2JMPCONFIG_H */
