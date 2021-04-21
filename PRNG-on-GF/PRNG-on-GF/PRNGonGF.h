#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpinBox>
#include <string>
#include "ui_PRNGonGF.h"
#include "Dialog.h"
#include "Galois Field/Generator/Generator.h"
#include <sstream>
#include <QString>
#include <QScrollArea>

class PRNGonGF : public QMainWindow
{
    Q_OBJECT

private slots:
    void on_degree_edit_returnPressed();
    void on_n_edit_returnPressed();
    void on_even_checkbox_stateChanged();
    void on_build_prng_clicked();
    void on_n_edit_textChanged();
    void on_degree_edit_textChanged();
    void on_n_save_clicked();
    void on_degree_save_clicked();

public:
    PRNGonGF(QWidget *parent = Q_NULLPTR);

private:
    Ui::PRNGonGFClass ui;
    QHBoxLayout* a_first = nullptr;
    QHBoxLayout* c_first = nullptr;
    QHBoxLayout* a_second = nullptr;
    QHBoxLayout* c_second = nullptr;

    QLabel* a_label_first = new QLabel("A");
    QLabel* c_label_first = new QLabel("C");
    QLabel* a_label_second = nullptr;
    QLabel* c_label_second = nullptr;

    void remove_last_widget_from_layout(QBoxLayout* parent);
    void remove_layout(QBoxLayout* layout);
    void set_spinbox_max_value(QBoxLayout* parent, int max_degree);
    QSpinBox* create_new_spinbox(int max_degree);
};
