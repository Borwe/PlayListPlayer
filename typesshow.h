#ifndef TYPESSHOW_H
#define TYPESSHOW_H

#include <QDialog>
#include <QListWidgetItem>
#include <vector>

#include <mainwindow.h>
#include <data.h>

using VideoTypes=std::vector<Handlers::VideoType>;

namespace Ui {
class TypesShow;
}

class TypesShow : public QDialog
{
    Q_OBJECT

public:
    explicit TypesShow(MainWindow *parent = 0);
    ~TypesShow();

private slots:
    void on_exit_view_clicked();

    void on_add_type_clicked();

    void on_remove_button_clicked();

    void typeSelected(QListWidgetItem *item);
private:
    Ui::TypesShow *ui;

    //hold the types
    VideoTypes types;

    //To hold the parent window
    MainWindow * m_parent;

    //To hold the type info once it is selected
    QListWidgetItem *selected_item;
};

#endif // TYPESSHOW_H
