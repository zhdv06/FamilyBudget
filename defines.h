#ifndef DEFINES_H
#define DEFINES_H

#include <QMetaType>

enum DatabaseStatus
{
    DS_Connected,
    DS_Disconnected
};

enum RequestType
{
    RT_Command,
    RT_Tables
};
Q_DECLARE_METATYPE(RequestType);

#endif // DEFINES_H
