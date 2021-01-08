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


/**
 * FolderScanner
 *
 * FolderScanner is goes through folders and their subfolders
 * Looking for files ending with .lyli, once it is done,
 * It would return the list by emiting a signal.
 *
 * Returns the list of files by emiting signal so that it can
 * share the list over different threads
 *
*/
class FolderScanner : public Worker
{

    Q_OBJECT

public:
    FolderScanner();

    // return the list sub folders as vector FolderInfo
    QVector<FolderInfo> getSubFolders(const QDir &dir);

    // return the list of files in folder ending with
    // .lyli as vector of FileInfo
    QVector<FileInfo> getCorruptedFiles(const QDir &dir);

protected:
    // bool = true, if workload is not empty
    bool hasNext();

    // return the next folder in workload
    FolderInfo next();

    // add folders to current workload
    void newWorkLoad(QVector<FolderInfo> folders);

    // add files found in the scan to list of files
    // the list of files is part of the state of this object
    void newResults(QVector<FileInfo> corruptedFiles);

    // scans a single folder, it doesn't even scan it subfolders
    void scanFolder(FolderInfo folder);

    // emits files found durring the scan,
    // emit work finished signal
    void wrapup();

    // scan all folders in the current workload
    void scan();

    // throw away the list of all folders in the current workload
    void dumpCurrentWorkLoad();

    // shows an error when the dir doesn't exist
    void handleFolderDoesNotExist(QDir dir);

public slots:    
    // add folders to workload
    // schedules a full scan
    void onScanBatch(QVector<FolderInfo> folders);

    // clear state from previous scan
    // make sure instance is ready for scan
    void onPrepForWork();

    // handle cancelling scan operation
    void onCancelled();

signals:
    // return corrupted files found from scan as a signal
    void corruptedFilesFound(QVector<FileInfo> corruptedFiles);

    // signal folder has been scanned
    void folderScanned(FolderInfo folder);

    // signal work is finished
    void workFinished();

private:
    // current workload AKA folders to be scanned
    QVector<FolderInfo> folders;

    // corrupted files found durring the scan
    QVector<FileInfo> corruptedFiles;
};

#endif // TARGETBUILDER_H
