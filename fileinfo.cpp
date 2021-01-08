#include "fileinfo.h"



FileInfo::FileInfo(QString oldName, QString newName)
    :oldName(oldName), newName(newName){}

QVector<FileInfo> toFileInfoVector(const QFileInfoList &qFileInfoList)
{
    // We do alot of assumption here
    // in the context of this application, the we rename corrupt files
    // we do this for all the files in the list and return the vector

    // corrupt files have .lyli extensions after the original extension
    // we assume every item that is not a file has the .lyli extension
    // then, we proceed to remove that extension

    QString lyli = ".lyli"; // the type of files we are looking for
    QString lyliExtension = "*" + lyli;

    QVector<FileInfo> files;
    for(QFileInfo dirFile: qFileInfoList){
        if(!dirFile.isFile()) continue;
        QString oldName = dirFile.filePath();
        QString newName = dirFile.filePath().replace(lyli, "");
        files << FileInfo{oldName, newName};
    }

    return files;
}
