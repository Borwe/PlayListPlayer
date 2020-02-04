#include "typesshow.h"
#include "ui_typesshow.h"
#include "singleinput.h"

#include <iostream>
#include <future>
#include <algorithm>
#include <QErrorMessage>
#include <QMessageBox>

TypesShow::TypesShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TypesShow)
{
    ui->setupUi(this);

    //Function for filling up the list of types to be displayed
    auto fillUpList=[](){
        return Handlers::VideoType::getAll();
    };

    auto fut=std::async(std::launch::async,fillUpList);

    this->types=fut.get();
    for(auto &type:types){
        ui->typesList->addItem(type.getType().c_str());
    }
}

TypesShow::~TypesShow()
{
    delete ui;
}

/**
 * @brief TypesShow::on_exit_view_clicked
 * Exit view
 */
void TypesShow::on_exit_view_clicked()
{
    this->close();
}

/**
 * @brief TypesShow::on_add_type_clicked
 * Show dialog that prompts user to give input or exit
 * Get input, then check if video type has anything similar
 * if not then add it, when user clicks add,
 * otherwise show error message
 */
void TypesShow::on_add_type_clicked()
{
    //display a dialog box for user to give input for data set
    SingleInput *in=new SingleInput(this,"Enter Types","Add Type","Enter type you would like to add");
    in->setModal(true);

    std::function<void(QWidget *parent,SingleInput *input)> func=[](QWidget *parent,SingleInput *input){

        std::string value=input->getValue();
        //check if such a type exists in the database
        if(Handlers::VideoType::getVideoTypeByType(value)!=nullptr){
            QErrorMessage *err=new QErrorMessage(parent);
            err->setModal(true);
            err->showMessage("The type you gave already exists, please try another");
            err->show();
        }else{
            //create a type object and then save it to db, and exit this
            Handlers::VideoType type(std::move(value));
            type.save();

            //show success message box
            QMessageBox *success=new QMessageBox(parent);
            success->setModal(true);
            success->setText("Saved to database");
            success->show();

            input->close();
            parent->close();
        }
    };


    in->setDoSomethingFunction(func,this);
    in->show();
}
