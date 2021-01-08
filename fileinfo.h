#ifndef FILEINFO_H
#define FILEINFO_H

#include <QFileInfo>
#include <QString>



struct FileInfo
{
    // this mainly used my the file renamer to rename files
    // oldName identifies the file
    // newName to replace the old one
    QString oldName;
    QString newName;

    FileInfo(QString oldName="", QString newName="");
};

// a convenience function to convert FileInfoList to QVector<FileInfo>
QVector<FileInfo> toFileInfoVector(const QFileInfoList &qFileInfoList);

#endif // FILEINFO_H
