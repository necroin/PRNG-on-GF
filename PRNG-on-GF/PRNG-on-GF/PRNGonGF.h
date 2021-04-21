#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpinBox>
#include <string>
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

    void remove_last_widget_from_layout(QBoxLayout* parent);
    void remove_layout(QBoxLayout* layout);
    void set_spinbox_max_value(QBoxLayout* parent, int max_degree);
    QSpinBox* create_new_spinbox(int max_degree);
};
