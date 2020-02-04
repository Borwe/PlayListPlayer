#ifndef SINGLEINPUT_H
#define SINGLEINPUT_H

#include <QDialog>
#include <functional>
#include <memory>

namespace Ui {
class SingleInput;
}

class SingleInput : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief SingleInput
     * @param parent
     * @param title
     * @param button_title
     * @param toolTip
     */
    SingleInput(QWidget *parent = 0,
                         std::string title=std::string("Inputs"),
                         std::string button_title="Add",
                         std::string toolTip="Type text Here");
    ~SingleInput();

    //setters
    void setDoSomethingFunction(std::function<void(QWidget *,SingleInput *)> &func,QWidget *parent){
        this->parent=parent;
        this->func=func;
    }

    //getters
    std::string getValue() const{
        return value;
    }

private slots:
    void on_addButton_clicked();

private:
    Ui::SingleInput *ui;

    std::string button_title;
    std::string toolTip;
    std::string title;

    //To hold the value to be used
    std::string value;

    //To hold function that does something
    std::function<void(QWidget *,SingleInput *input)> func;

    //hold the parent
    QWidget *parent;
};

#endif // SINGLEINPUT_H
