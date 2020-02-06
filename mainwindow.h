#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QVector>
#include <mutex>
#include <functional>
#include <QProgressBar>

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

    void on_actionAdd_Types_triggered();

    void on_actionShow_MultiMedia_Types_triggered();

    void on_actionCreate_Open_New_PlayList_triggered();

    void on_selected_file(const QString &name);

private:
    Ui::MainWindow *ui;

    /**
     * For holding the vectors of files currently read
     */
    QVector<QFile> filesToRead;

    /**
     * @brief do_initial_Loading
     * For doing the loading of files in directory, and storing thing in a vector
     */
    void do_initial_Loading();
    bool loading=false;
    //lock for loading
    std::mutex loading_mutex;
    //used for showing loadings
    QProgressBar *progrssBar;

    template<typename R,typename T>
    void startLoadingAction(std::string &loadingLabel,std::function<R(T)> &func);
    template<typename R>
    void startLoadingAction(std::string &loadingLabel,std::function<R ()> func);
    void stopLoadingAction();
};
#endif // MAINWINDOW_H
