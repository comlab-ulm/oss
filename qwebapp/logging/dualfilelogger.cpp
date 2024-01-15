/**
  @file
  @author Stefan Frings
*/

#include "dualfilelogger.h"

using namespace stefanfrings;

DualFileLogger::DualFileLogger( const int refreshInterval, QObject* parent)
    :Logger(parent)
{
     firstLogger=new FileLogger( refreshInterval, this);
     secondLogger=new FileLogger( refreshInterval, this);
}

void DualFileLogger::log(const QtMsgType type, const QString& message, const QString &file, const QString &function, const int line)
{
    firstLogger->log(type,message,file,function,line);
    secondLogger->log(type,message,file,function,line);
}

void DualFileLogger::clear(const bool buffer, const bool variables)
{
    firstLogger->clear(buffer,variables);
    secondLogger->clear(buffer,variables);
}
