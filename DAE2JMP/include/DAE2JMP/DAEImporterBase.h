#ifndef _DAEIMPORTERBASE_H
#define _DAEIMPORTERBASE_H

namespace DAE2JMP
{
    class DAEImporter;

    class DAEImporterBase
    {
    public:
        DAEImporterBase(DAEImporter *importer) : mDAEImporter(importer)
        {}
        
        //virtual bool write() = 0;
        
    protected:
        DAEImporter *mDAEImporter;
    };
}

#endif /* _DAEIMPORTERBASE_H */
