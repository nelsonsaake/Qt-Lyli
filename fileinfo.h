#ifndef FILEINFO_H
#define FILEINFO_H

#include <QString>



struct FileInfo
{
    QString oldName;
    QString newName;

    FileInfo(QString oldName="", QString newName="")
        :oldName(oldName), newName(newName){}

};

#endif // FILEINFO_H
