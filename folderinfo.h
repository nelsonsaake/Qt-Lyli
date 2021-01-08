#ifndef FOLDERNAME_H
#define FOLDERNAME_H

#include <QFileInfo>
#include <QString>


/**
 * FolderInfo
 *
 * FolderInfo holds all the neccessary info required
 * by the folderScanner to go through a folder and
 * find corrupted files
 */
struct FolderInfo
{
    // Stores folder path
    QString path;

    FolderInfo(QString path="");
};

// a covenience function to convent qFileInfo to folderInfo
FolderInfo toFolderInfo(const QFileInfo &qFileInfo);

// a covenience function to convent QFileInfoList to QVector of FolderInfo
QVector<FolderInfo> toFolderInfoVector(const QFileInfoList &qFileInfoList);

#endif // FOLDERNAME_H
