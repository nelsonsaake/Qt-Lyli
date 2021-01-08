#include "filerenamer.h"



FileRenamer::FileRenamer()
{

}

bool FileRenamer::hasNext()
{
    // bool == true, if workload is not empty

    return !files.isEmpty();
}

FileInfo FileRenamer::next()
{
    // take the first file from work load
    // delete it from work load
    // return it

    FileInfo file;
    if(hasNext()){
        file = files.first();
        files.removeFirst();
    }
    return file;
}

void FileRenamer::addNewFilesToWorkLoad(QVector<FileInfo> files)
{
    // add new files to current work load

    this->files << files;
}

bool FileRenamer::renameFile(FileInfo fileInfo)
{
    // checking if it cancelled,
    // that is to make the class cancel cooperative
    // so that when there is a cancel order it would act on it
    if(isCancelled()) return true;

    // rename file using fileInfo
    // oldName identifies the file
    // newName replace the oldName in the rename process
    QFile file;
    file.setFileName(fileInfo.oldName);
    if(file.exists()){
        if(file.rename(fileInfo.newName)){
            emit fileRenamed(fileInfo);
            return true;
        } else{
            qDebug() << "Error renaming file: rename failed";
            return false;
        }
    } else{
        qDebug() << "Error renaming file: file does not exist";
        return false;
    }
    return true;
}

void FileRenamer::renameFiles()
{
    // Rename all files in current work load

    QEventLoop eventLoop;
    while(hasNext()){
        for(int i=0; i<50 && hasNext(); i++){
            if(isCancelled()) return;
            renameFile(next());
        }

        // this function can go on for a while
        // so we manually allow the eventLoop to handle event -
        // after a while, that is, every 50 renames
        eventLoop.processEvents();
    }

    // if supply is finished, that means no work is coming
    // so send out an all finished signal
    if(isSupplierFinished) emit allFinished();
}

void FileRenamer::onPrepForWork()
{
    // make sure that state from any previous scan doesn't
    // affect this current scan
    // make sure this class instance is ready for scan

    // clear cancel state
    clearIsCancel();

    // reset supply state
    isSupplierFinished = false;
}

void FileRenamer::onRenameBatch(QVector<FileInfo> files)
{
    // add new files to workload
    addNewFilesToWorkLoad(files);

    // rename all files in workload
    renameFiles();
}

void FileRenamer::onSupplierFinished()
{
    // mark the fact that the supplier state is changed
    isSupplierFinished = true;

    // we trigger a rename with an empty list
    // just make sure we are not leaving any work undone
    // once it done renaming all files it would signal all finished
    onRenameBatch({});
}
