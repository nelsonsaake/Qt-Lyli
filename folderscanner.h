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
    FolderScanner() {}

protected:
    bool hasNext(){
        return !folders.isEmpty();
    }

    FolderInfo next(){
        FolderInfo folder;
        if(hasNext()){
            folder=folders.first();
            folders.removeFirst();
        }
        return folder;
    }

    void newWorkLoad(QVector<FolderInfo> folders){
        this->folders << folders;
    }

    void scanFolder(FolderInfo folder){

        QString path = folder.path;
        QDir dir(path);
        QList<QFileInfo> dirFiles;
        QList<QFileInfo> dirSubFolders;

        QString lyli = ".lyli"; // the type of files we are looking for
        QString lyliExtension = "*" + lyli;
        QStringList nameFilter{lyliExtension};

        if(!dir.exists()) {
            qDebug() << "Error extracting targets from directory: " << path << " provided doesnot exist";
            return;
        }

        dirFiles = dir.entryInfoList(nameFilter, QDir::Files);
        dirSubFolders = dir.entryInfoList({}, QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

        QVector<FileInfo> files;
        QVector<FolderInfo> folders;

        for(QFileInfo dirFile: dirFiles){
            QString oldName = dirFile.filePath();
            QString newName = dirFile.filePath().replace(lyli, "");
            files << FileInfo{oldName, newName};
        }
        for(QFileInfo dirSubFolder: dirSubFolders){
            folders << FolderInfo{dirSubFolder.filePath()};
        }

        if(!files.isEmpty()) this->files << files;
        if(!folders.isEmpty()) newWorkLoad(folders);
        emit folderScanned(folder);
    }

    void wrapup(){

        // if we exit by cancelling
        // we don't want to send any futher signals
        // that might indicate that everything is going on as usually
        if(isCancelled()) return;

        // let anyone concerned know work is finished
        emit workFinished();

        // pass on results from work done
        emit renameBatch(files);
    }

    void scan(){

        // we work and break periodically so the eventloop
        // can handle signals coming in and other events

        QEventLoop eventLoop;
        while(hasNext()){
            for(int i=0; i<50 && hasNext(); i++){
                if(isCancelled()) return;
                scanFolder(next());
            }
            eventLoop.processEvents();
        }

        // if we done for good
        if(!hasNext()) wrapup();
    }

public slots:    
    void onScanBatch(QVector<FolderInfo> folders){
        newWorkLoad(folders);
        scan();
    }

    void onPrepForWork(){
        this->folders.clear();
        clearIsCancel();
    }

    void onCancelled(){
        folders.clear();
        setIsCancelled();
    }

signals:
    void renameBatch(QVector<FileInfo> files);
    void folderScanned(FolderInfo folder);
    void workFinished();

private:
    QVector<FolderInfo> folders;
    QVector<FileInfo> files;
};

#endif // TARGETBUILDER_H
