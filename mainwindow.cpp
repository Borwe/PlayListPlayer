#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <future>
#include <mutex>
#include <atomic>
#include <QFileDialog>
#include <iostream>
#include <handlers.h>

//for showing types dialog
#include <typesshow.h>
#include <QErrorMessage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setup loadingbar
    this->progrssBar=new QProgressBar(this);
    progrssBar->setRange(0,0);
    progrssBar->setValue(-1);
    progrssBar->setVisible(true);
    //attatch it to statusbar
    ui->statusbar->addWidget(progrssBar);

    do_initial_Loading();
}

template<typename R,typename T>
void MainWindow::startLoadingAction(std::string &loadingLabel,std::function<R(T)> &func){
    //set the title of the progress bar and show it
    progrssBar->setToolTip(loadingLabel.c_str());
    progrssBar->setVisible(true);

    std::lock_guard<std::mutex>(this->loading_mutex);
    loading=true;

    //then now start the function in another thread
    std::async(std::launch::async,func);
}

template<typename R>
void MainWindow::startLoadingAction(std::string &loadingLabel,std::function<R ()> func){

}

void MainWindow::stopLoadingAction(){

    progrssBar->setVisible(false);

    std::lock_guard<std::mutex>(this->loading_mutex);
    loading=false;
}

void startGettingFiles(){
    qDebug()<<"GETTING FILES\n";
}

void MainWindow::do_initial_Loading(){
    std::string text("shit");
    startLoadingAction<void>(text,startGettingFiles);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionExit_triggered()
{
    qDebug() <<"Exiting application\n";
    //futureLoads.~future();
    std::exit(0);
}

/**
 * @brief MainWindow::on_actionAdd_Types_triggered
 */
void MainWindow::on_actionAdd_Types_triggered()
{
    TypesShow *typs=new TypesShow(this);
    typs->setModal(true);
    typs->show();
}

void MainWindow::on_actionShow_MultiMedia_Types_triggered()
{
    on_actionAdd_Types_triggered();
}

void MainWindow::on_selected_file(const QString &name){
    //turn name of the string to normal std::string
    std::string dirLocation(name.toStdString());

    auto files=Handlers::getFilesInDir(dirLocation);

    //if folder empty, prompt user to select another folder
    if(files->empty()){
        QErrorMessage *error=new QErrorMessage(this);
        error->setModal(true);
        error->showMessage("Please select a folder with something in it\nThe one selected is <b>Empty</b>");
        error->show();
    }else{

        //if folder contains no files with selected type,prompt user to add relevant types
        files=Handlers::getFilesOfMultimedia(files);
        if(files->empty()){

            QErrorMessage *error=new QErrorMessage(this);
            error->setModal(true);
            error->showMessage("No valid multimedia type detected <b>Please add more</b>");
            error->show();
        }
    }

}

void MainWindow::on_actionCreate_Open_New_PlayList_triggered()
{
    //open folder for choosing files
    QFileDialog *fileSearch=new QFileDialog(this);
    fileSearch->setDirectory("./");
    //so that we only use directories
    fileSearch->setFileMode(QFileDialog::FileMode::DirectoryOnly);

    connect(fileSearch,SIGNAL(fileSelected(const QString &)),this,SLOT(on_selected_file(const QString &)));

    fileSearch->setModal(true);
    fileSearch->show();

}
