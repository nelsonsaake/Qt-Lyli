#ifndef FILERENAMER_H
#define FILERENAMER_H

#include "worker.h"
#include "fileinfo.h"

#include <QFile>
#include <QDebug>
#include <QEventLoop>



class FileRenamer : public Worker
{

    Q_OBJECT

public:
    FileRenamer(){}

protected:
    bool hasNext(){
        return !files.isEmpty();
    }

    FileInfo next(){
        FileInfo file;
        if(hasNext()){
            file = files.first();
            files.removeFirst();
        }
        return file;
    }

    void addNewFilesToWorkLoad(QVector<FileInfo> files){
        this->files << files;
    }

    bool renameFile(FileInfo fileInfo){

        if(isCancelled()) return true;

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

    void renameFiles(){

        QEventLoop eventLoop;
        while(hasNext()){
            for(int i=0; i<50 && hasNext(); i++){
                if(isCancelled()) return;
                renameFile(next());
            }
            eventLoop.processEvents();
        }

        if(isSupplierFinished) emit allFinished();
    }

public slots:
    void onRenameBatch(QVector<FileInfo> files){
        addNewFilesToWorkLoad(files);
        renameFiles();
    }

    void onSupplierFinished(){
        isSupplierFinished = true;
        onRenameBatch({});
    }

signals:
    void fileRenamed(FileInfo file);
    void allFinished();

private:
    QVector<FileInfo> files;
    bool isSupplierFinished;
};

#endif // FILERENAMER_H
