#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PRNGonGF.h"

class PRNGonGF : public QMainWindow
{
    Q_OBJECT

private slots:
    void on_degree_edit_returnPressed();

public:
    PRNGonGF(QWidget *parent = Q_NULLPTR);

private:
    Ui::PRNGonGFClass ui;
};
