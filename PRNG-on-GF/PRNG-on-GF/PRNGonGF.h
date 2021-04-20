#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpinBox>
#include "ui_PRNGonGF.h"

class PRNGonGF : public QMainWindow
{
    Q_OBJECT

private slots:
    void on_degree_edit_returnPressed();
    void on_n_edit_returnPressed();
    void on_even_checkbox_stateChanged();

public:
    PRNGonGF(QWidget *parent = Q_NULLPTR);

private:
    Ui::PRNGonGFClass ui;
    QHBoxLayout* a_first = new QHBoxLayout();
    QHBoxLayout* c_first = new QHBoxLayout();
    QHBoxLayout* a_second = nullptr;
    QHBoxLayout* c_second = nullptr;
};
