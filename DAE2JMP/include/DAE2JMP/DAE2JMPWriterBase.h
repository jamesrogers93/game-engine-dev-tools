#ifndef _DAE2JMPWRITERBASE_H
#define _DAE2JMPWRITERBASE_H

class JMPWriter;

class WriterBase
{
public:
    WriterBase(JMPWriter *writer) : mJMPWriter(writer)
    {}
    
protected:
    JMPWriter *mJMPWriter;
};

#endif /* _DAE2JMPWRITERBASE_H */
