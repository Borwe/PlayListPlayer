#ifndef TYPESSHOW_H
#define TYPESSHOW_H

#include <QDialog>
#include <vector>

#include <data.h>

using VideoTypes=std::vector<Handlers::VideoType>;

namespace Ui {
class TypesShow;
}

class TypesShow : public QDialog
{
    Q_OBJECT

public:
    explicit TypesShow(QWidget *parent = 0);
    ~TypesShow();

private slots:
    void on_exit_view_clicked();

    void on_add_type_clicked();

private:
    Ui::TypesShow *ui;

    //hold the types
    VideoTypes types;
};

#endif // TYPESSHOW_H
