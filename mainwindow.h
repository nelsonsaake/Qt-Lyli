#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "worker.h"
#include "fileinfo.h"
#include "folderinfo.h"
#include "writinganimation.h"
#include "countupanimation.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QTextBrowser>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum Mode{SimpleInterface, ExpertMode, ScanningMode, CancelMode=ScanningMode, FinishedMode};
    enum Page{OperationPage, FinishedPage};

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void showMessageOnStatus(QString message);
    void setup();
    void startLiveGif();
    void expertMode();
    void simpleInterface();
    void scanningMode();
    void returnToPreviousMode();
    void clearUi();
    void clearState();
    void hideStateModeWidgets();
    void changeMode(Mode mode);
    void gotoPage(Page page);
    void finishViewLiveAnimation();

private slots:
    void on_showNextUiUpdate();
    void on_browsePushButton_clicked();
    void on_cancelPushButton_clicked();
    void on_expertModePushButton_clicked();
    void on_simpleInterfacePushButton_clicked();
    void on_scanPushButton_clicked();
    void on_expertModeScanPushButton_clicked();
    void on_runAnotherScanButton_clicked();

public slots:
    void onFolderScanned(FolderInfo scannedFolder);
    void onFileRenamed(FileInfo renameFile);
    void onAllFinished();

signals:
    void scanFolder(QVector<FolderInfo> folders);
    void triggerTicker();
    void prepForWork();
    void cancelled();

private:
    Ui::MainWindow *ui;

    QListWidget *filesUi;
    QListWidget *foldersUi;
    QLineEdit *rootDirPathInput;
    QStatusBar *statusBar;
    QLabel *liveGifLabel;
    QPushButton *startButton;
    QPushButton *cancelButton;
    QPushButton *scanButton;
    QPushButton *expertModeButton;
    QPushButton *simpleInterfaceButton;
    QPushButton *expertModeScanButton;
    QStackedWidget *stackedWidget;
    QWidget *folderInputArea;
    QPushButton *browseButton;
    QLabel *scanCompletedLabel;
    QGraphicsView *graphicsView;

    QMovie *liveGif;

    QStringList tempScannedFolders;
    QStringList tempRenamedFiles;

    int foldersScannedCount;
    int renamedFilesCount;

    int listViewThreshold;

    // we store the previous mode only so we can return from cancel mode
    // so, previous mode can only be any mode except cancel mode
    // scanning mode == cancel mode, when you are scanning the only action allowed is cancel
    Mode previousMode;
    Mode currentMode;

    QVector<QWidget*> simpleInterfaceSpecificWidgets;
    QVector<QWidget*> expertModeSpecificWidgets;
    QVector<QWidget*> scanningModeSpecificWidgets;

    Worker cancelManager;
    Ticker ticker;
    WritingAnimation *writeScanCompleted;

    QGraphicsScene scene;
};
#endif // MAINWINDOW_H
