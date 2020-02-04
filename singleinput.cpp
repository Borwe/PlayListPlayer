#include "singleinput.h"
#include "ui_singleinput.h"

#include <QErrorMessage>

SingleInput::SingleInput(QWidget *parent,std::string  title,std::string  button_title,std::string  toolTip) :
    QDialog(parent),
    title(title),
    button_title(button_title),
    toolTip(toolTip),
    ui(new Ui::SingleInput)
{
    ui->setupUi(this);

    //setup the button title
    ui->addButton->setText(button_title.c_str());

    //setup tooltip
    ui->lineInput->setToolTip(toolTip.c_str());
}

SingleInput::~SingleInput()
{
    delete ui;
}

void SingleInput::on_addButton_clicked()
{
    //check that user has given a value in input
    //ifnot show an error
    std::string input=ui->lineInput->text().toStdString();
    if(input.empty() || input.size()<=0){
        //display error dialog
        QErrorMessage *err=new QErrorMessage(this);
        err->setModal(true);
        err->showMessage("Sorry no input given");
    }else{
        //set the value to this input
        this->value=input;
        func(this->parent,this);
    }

}
