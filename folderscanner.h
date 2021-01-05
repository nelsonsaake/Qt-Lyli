#ifndef TARGETBUILDER_H
#define TARGETBUILDER_H

#include "worker.h"
#include "fileinfo.h"
#include "folderinfo.h"
#include "ticker.h"

#include <QDir>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QObject>
#include <QEventLoop>



class FolderScanner : public Worker
{
    // scanner goes through a folder
    // returns in the manner of emiting signals
    // returns subfolders and files

    Q_OBJECT

public:
    FolderScanner();

protected:
    bool hasNext();

    FolderInfo next();

    void newWorkLoad(QVector<FolderInfo> folders);

    void scanFolder(FolderInfo folder);

    void wrapup();

    void scan();

public slots:    
    void onScanBatch(QVector<FolderInfo> folders);

    void onPrepForWork();

    void onCancelled();

signals:
    void renameBatch(QVector<FileInfo> files);
    void folderScanned(FolderInfo folder);
    void workFinished();

private:
    QVector<FolderInfo> folders;
    QVector<FileInfo> files;
};

#endif // TARGETBUILDER_H
