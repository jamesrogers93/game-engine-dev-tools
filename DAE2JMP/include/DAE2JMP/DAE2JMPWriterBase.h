#ifndef _DAE2JMPWRITERBASE_H
#define _DAE2JMPWRITERBASE_H

class JMPWriter;

class WriterBase
{
public:
    WriterBase(JMPWriter *writer) : mJMPWriter(writer)
    {}
    
    virtual bool write() = 0;
    
protected:
    JMPWriter *mJMPWriter;
};

#endif /* _DAE2JMPWRITERBASE_H */
