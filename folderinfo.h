#ifndef FOLDERNAME_H
#define FOLDERNAME_H

#include <QString>



struct FolderInfo
{
    QString path;

    FolderInfo(QString path="")
        :path(path){}
};

#endif // FOLDERNAME_H
