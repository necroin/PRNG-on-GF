#pragma once

#include <QtWidgets/QMainWindow>
#include <QCloseEvent>
#include "ui_Dialog.h"

class Dialog : public QMainWindow
{
    Q_OBJECT

public:
    Dialog(QWidget* parent = Q_NULLPTR);

protected:

    void closeEvent(QCloseEvent*);

private:
    Ui::DialogClass ui;


};

