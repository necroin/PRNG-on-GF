#pragma once

#include <QtWidgets/QMainWindow>
#include <QCloseEvent>
#include "ui_Dialog.h"
#include <string>
#include <QString>

class Dialog : public QMainWindow
{
    Q_OBJECT

public:
    Dialog(QWidget* parent = Q_NULLPTR, QString result = "none");

protected:

    void closeEvent(QCloseEvent*);

private:
    Ui::DialogClass ui;


};

