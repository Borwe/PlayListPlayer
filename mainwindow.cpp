#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <future>
#include <mutex>
#include <atomic>
#include <iostream>

//for showing types dialog
#include <typesshow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setup loadingbar
    this->progrssBar=new QProgressBar(this);
    progrssBar->setRange(0,0);
    progrssBar->setValue(-1);
    progrssBar->setVisible(false);
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
