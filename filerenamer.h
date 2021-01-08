#ifndef FILERENAMER_H
#define FILERENAMER_H

#include "worker.h"
#include "fileinfo.h"

#include <QFile>
#include <QDebug>
#include <QEventLoop>


/**
 * FileRenamer
 *
 * FileRenamer as the name suggests renames a bunch of files
 */
class FileRenamer : public Worker
{

    Q_OBJECT

public:
    FileRenamer();

protected:
    // checks is the more files available to be renamed
    bool hasNext();

    // returns the next file in the list to be renamed
    FileInfo next();

    // add new files to work load
    void addNewFilesToWorkLoad(QVector<FileInfo> files);

public:
    // rename a single file
    bool renameFile(FileInfo fileInfo);

protected:
    // rename all files in the list (work load)
    void renameFiles();

public slots:
    // clear state from old scans
    void onPrepForWork();

    // this triggers a rename of all the files in work load
    // including the new batch
    void onRenameBatch(QVector<FileInfo> files);

    // handles supply finish signal
    void onSupplierFinished();

signals:
    // signal file renamed
    void fileRenamed(FileInfo file);

    // signal all work finished
    void allFinished();

private:
    // workload or list of files to be renamed
    QVector<FileInfo> files;

    // state of the files supplier
    //
    // we can querry it, but if it is on another thread,
    // we would have to use signals and slots
    // I'm trying to not use it if possible
    bool isSupplierFinished;
};

#endif // FILERENAMER_H
