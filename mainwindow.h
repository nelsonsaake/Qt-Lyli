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

    // directly maps to the different mode of the application
    enum Mode{SimpleInterface, ExpertMode, ScanningMode, CancelMode=ScanningMode, FinishedMode};

    // directly maps to the different pages in the application
    enum Page{OperationPage, FinishedPage};

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // acquire handles from the form to member variables
    void getUiHandles();

    // initialize where neccessary
    void init();

    // do signals and slots connection and the rest
    void setup();


    // dispaly message on status bar
    void showMessageOnStatus(QString message);

    // Live Gif indicates the app is still alive
    // and not frozen
    void startLiveGif();

    // clear the items in the listViews displayed
    void clearUi();

    // clear reset haves initialized
    void clearState();

    // hide all widgets that are displayed in
    // any one mode
    void hideModeWidgets();

    // change the mode in the ui
    // change the mode in concerned member vars
    void changeMode(Mode mode);

    // change page
    void gotoPage(Page page);

    // reset values affected by previous scan
    void prepForScan();

    // all private slots are
    // handles to triggers caused from the ui
private slots:
    // shows ui update
    void on_showNextUiUpdate();

    // opens dialog to allow selection of
    // folder to be scanned
    void on_browsePushButton_clicked();

    // cancel scanning
    void on_cancelPushButton_clicked();

    // run scan in expert mode
    void on_expertModePushButton_clicked();

    // change mode to simple interface mode
    void on_simpleInterfacePushButton_clicked();

    // run scan in simple interface mode
    void on_scanPushButton_clicked();

    // change mode to expert mode
    void on_expertModeScanPushButton_clicked();

    // move from finished page to scan page
    void on_runAnotherScanButton_clicked();

protected slots:
    // change mode to expert mode
    void expertMode();

    // change mode to simple interface
    void simpleInterface();

    // change the mode to scanning mode
    // scanning mode == cancel mode
    void scanningMode();

    // change the mode to the last scan mode
    // scan modes == simple interface mode, expert mode
    void returnToPreviousScanMode();

public slots:
    // handles folderScanned signal
    // updates the folders scanned memeber list
    void onFolderScanned(FolderInfo scannedFolder);

    // handles fileRenamed signal
    // updates the files renamed member list
    void onFileRenamed(FileInfo renameFile);

    // receives notification when scan is completed
    void onAllFinished();

signals:
    // send a request to scan folder
    void scanFolder(QVector<FolderInfo> folders);

    // tells receivers to prepare for a new scan
    // that is, reset variables affected by previous scan
    void prepForWork();

    // tells receivers to stop whatever their doing
    void cancelled();

private:
    Ui::MainWindow *ui;

    // this are handles to ui widgets
    // they are acquired from the ui form during
    // construction
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

    // liveGif will play and manage the
    // gif that indicates that the app is running
    QMovie *liveGif;

    // where we cache folders and files before
    // they are display them as updates
    QStringList tempScannedFolders;
    QStringList tempRenamedFiles;

    // store the number of folders scanned
    int foldersScannedCount;

    // store the number of file renamed
    int renamedFilesCount;

    // listViewThreshold
    // if the listView Widget has too many items,
    // it slows down the ui
    // so once the list items hits a certain
    // manually set threshold we empty the list
    //
    // this is used by all listViews in the app
    int listViewThreshold;

    // we store the previous mode only so we can return from cancel mode
    // so, previous mode can only be any mode except cancel mode
    // scanning mode == cancel mode, when you are scanning the only action allowed is cancel
    Mode previousScanMode;
    Mode currentMode;

    // this handles allow grouping of certain widgets
    // so we can perform bulk operation on all of them
    // eg. hide, show, disable, enable ...
    //
    // each group represent a particular state of the
    // app
    QVector<QWidget*> simpleInterfaceSpecificWidgets;
    QVector<QWidget*> expertModeSpecificWidgets;
    QVector<QWidget*> scanningModeSpecificWidgets;

    // the cancelManager, manages the cancle state of
    // this object
    //
    // the object cannot inherit Worker and QMainWindow
    // since they both inherit QObject
    // hence composition
    Worker cancelManager;

    // ticker triggers ui updates
    //
    // updating the ui anytime we receive update
    // causes the ui to freeze sometimes
    //
    // hence the need to manually regualte the updates
    Ticker ticker;

    // this to kinda fancy display the results of
    // a scan
    //
    // it display the message as if it was now being
    // typed to the screen
    WritingAnimation *writeScanCompleted;
};
#endif // MAINWINDOW_H
