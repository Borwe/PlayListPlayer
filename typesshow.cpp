#include "typesshow.h"
#include "ui_typesshow.h"
#include "singleinput.h"

#include <future>
#include <QDebug>
#include <algorithm>
#include <QErrorMessage>
#include <QMessageBox>

TypesShow::TypesShow(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::TypesShow),
    m_parent(parent)
{
    //display loading bar
    m_parent->beginLoading(QString("Loading Types View"));

    ui->setupUi(this);
    //make add_button have main focus
    ui->add_type->setFocus();
    //make remove_type button disabled by default
    ui->remove_type->setEnabled(false);

    //Function for filling up the list of types to be displayed
    auto fillUpList=[](){
        return Handlers::VideoType::getAll();
    };

    auto fut=std::async(std::launch::async,fillUpList);

    this->types=fut.get();
    for(auto &type:types){
        ui->typesList->addItem(type.getType().c_str());
    }

    //setup the connection to when an item is selected in the typesList
    //to enable the remove button
    connect(ui->typesList,&QListWidget::itemClicked,this,&TypesShow::typeSelected);
    //setup the remove button to remove the selected type from DB and list
    connect(ui->remove_type,&QPushButton::pressed,this,&TypesShow::on_remove_button_clicked);

    //end displaying loading bar
    m_parent->endLoading();
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

/**
 * @brief TypesShow::typeSelected
 * @param item
 * Notified when an item is selected on the list
 */
void TypesShow::typeSelected(QListWidgetItem *item){
    //set the item selected to item value
    this->selected_item=item;
    ui->remove_type->setEnabled(true);
}

/**
 * @brief removeItemFromQListWidget
 * @param list
 * @param item
 * Used for removing items from list widget if they exist and
 * repopulating list with default list just incase user deleted
 * everything
 */
void removeItemFromQListWidget(QListWidget *list,QPushButton *removeButton){
    //get row of item from list
    int row=list->currentRow();
    list->takeItem(row);

    removeButton->setEnabled(false);

    //clear focus
    list->clearFocus();

    //if the number of items is bellow 0, then now repopulate with the default list
    if(list->model()->rowCount()<=0){
        //we refill the list
        for(auto &type:Handlers::VideoType::getAll()){
            list->addItem(type.getType().c_str());
        }
    }
}

/**
 * @brief TypesShow::on_remove_type_clicked
 * When user has selected an item, then the delete button should be enabled
 * where by the user can now click the delete button and gracefully delete the selected item from the database
 * However should the user delete all items, a default set should be used to repopulate
 * everything
 */
void TypesShow::on_remove_button_clicked(){
    //show loading screen
    m_parent->beginLoading(QString("Removing item ").append(selected_item->text()));
    //get item
    if(selected_item->isSelected()){
        QString text=selected_item->text();
        //now get VideoType from DB matching and remove it
        Handlers::ShVideoType videoType=Handlers::VideoType::getVideoTypeByType(text.toStdString());
        //if null means no such type exists
        if(videoType==nullptr){
            //so just remove the item from the QListWidget
            removeItemFromQListWidget(ui->typesList,ui->remove_type);
        }else{
            //go ahead and remove it from the DB, then from the UI
            videoType->unSave();
            removeItemFromQListWidget(ui->typesList,ui->remove_type);
        }
    }else{
        qDebug()<<"Can't remove something not selected\n";
    }
    //end showing loading
    m_parent->endLoading();
}

