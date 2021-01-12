#include "folderinfo.h"



FolderInfo::FolderInfo(QString path)
    :path(path){}


FolderInfo toFolderInfo(const QFileInfo &qFileInfo)
{
    // convert QFileInfo to FolderInfo

    return FolderInfo{qFileInfo.path()};
}

QVector<FolderInfo> toFolderInfoVector(const QFileInfoList &qFileInfoList)
{
    // convert a QFileInfoList AKA QList<QFileInfo> into a QVector of FolderInfo

    QVector<FolderInfo> folders;
    for(QFileInfo fileInfo: qFileInfoList){
        folders << FolderInfo{fileInfo.filePath()};
    }
    return folders;
}
