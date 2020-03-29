#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <future>
#include <mutex>
#include <QFileDialog>
#include <iostream>
#include <handlers.h>
#include <thread>

//for showing types dialog
#include <typesshow.h>
#include <QErrorMessage>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->firstLoaded.store(0);
    ui->setupUi(this);


    //setup loadingbar
    this->loadingBar=new QProgressBar(this);
    loadingBar->setRange(0,0);
    loadingBar->setValue(-1);
    loadingBar->setVisible(false);
    loadingBar->setFixedHeight(20);
    //setup loadingbar label
    loadingLabel=new QLabel("Complete",this);

    //attatch it to statusbar
    ui->statusbar->addWidget(loadingLabel);
    ui->statusbar->addWidget(loadingBar);

    //for handling loading bar and loading texts via signal
    connect(this,&MainWindow::beginLoad,this,&MainWindow::beginLoading);
    connect(this,SIGNAL(endLoad()),this,SLOT(endLoading()));
}

void MainWindow::showEvent(QShowEvent *event){
    QWidget::showEvent(event);
    ++this->firstLoaded;
    if(this->firstLoaded==1){
        do_initial_Loading();
    }
}

void MainWindow::beginLoading(QString title){
    std::unique_lock<std::mutex> uq(this->loading_mutex);

    //check if loading is true
    if(this->loading==true){
        //increment count
    }
    this->loadingBar->setVisible(true);
    this->loading=true;
    this->loadingLabel->setText(title);
    qDebug()<<"Starting loading\n";
}

void MainWindow::endLoading(){
    std::unique_lock<std::mutex> uq(this->loading_mutex);
    this->loadingBar->setVisible(false);
    this->loading=false;
    this->loadingLabel->setText("Completed");
    qDebug()<<"Done loading\n";
}


void startGettingFiles(){
    qDebug()<<"GETTING FILES\n";
}

void MainWindow::do_initial_Loading(){
    std::function<void()> starting_events=[this](){
        auto title=QString("Startup setup");
        emit this->beginLoad(title);

        //just a test, supposed to implement something actually, later on
        std::this_thread::sleep_for(std::chrono::seconds(5));

        emit this->endLoad();
    };

    std::thread *thread=new std::thread(starting_events);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionExit_triggered()
{
    qDebug() <<"Exiting application\n";
    this->close();
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
