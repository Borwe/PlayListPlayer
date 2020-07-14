#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QVector>
#include <mutex>
#include <functional>
#include <QProgressBar>
#include <QLabel>
#include <future>
#include <atomic>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_actionExit_triggered();

    void on_actionShow_MultiMedia_Types_triggered();

    void on_actionCreate_Open_New_PlayList_triggered();

    void on_selected_file(const QString &name);

    /**
     * @brief do_initial_Loading
     * For doing the loading of files in directory, and storing thing in a vector
     */
    void do_initial_Loading();

public slots:
    /**
     * @brief beginLoading
     * @param title
     * Used to begin enabling the loading bar and change loading text
     */
    void beginLoading(QString title);

    /**
     * @brief endLoading
     * Used to stop displaying loading bar and complete loading text
     */
    void endLoading();

signals:
    void beginLoad(QString title);
    void endLoad();

private:
    Ui::MainWindow *ui;

    void showEvent(QShowEvent *event);

    //lock for loading
    std::mutex loading_mutex;
    //used for showing loadings
    QProgressBar *loadingBar;
    bool loading=false;
    QLabel *loadingLabel;

    //variable to check if first load or not
    std::atomic_int firstLoaded;
};
#endif // MAINWINDOW_H
