#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PRNGonGF.h"

class PRNGonGF : public QMainWindow
{
    Q_OBJECT

public:
    PRNGonGF(QWidget *parent = Q_NULLPTR);

private:
    Ui::PRNGonGFClass ui;
};
