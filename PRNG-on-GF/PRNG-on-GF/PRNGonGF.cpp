#include "PRNGonGF.h"
#include <string>

PRNGonGF::PRNGonGF(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.a_even_layout->addLayout(a_first);
    ui.c_even_layout->addLayout(c_first);
    ui.degree_edit->setText("1");
}

void remove_last_widget_from_layout(QBoxLayout* parent) {
    auto i = parent->count();
    auto wItem = parent->takeAt(i - 1);
    if (wItem->widget())
        wItem->widget()->setParent(NULL);
    delete wItem;
}

void remove_layout(QBoxLayout* layout) {

    QLayoutItem* item;
    QLayout* sublayout;
    QWidget* widget;

    while ((item = layout->takeAt(0))) {
        if ((sublayout = item->layout()) != 0) {}
        else if ((widget = item->widget()) != 0) { widget->hide(); delete widget; }
        else { delete item; }
    }

    delete layout;
}

void set_spinbox_max_value(QBoxLayout* parent, int max_degree) {

    QLayoutItem* item;
    QLayout* sublayout;
    QSpinBox* widget;

    auto count = parent->count();

    for (size_t i = 0; i < count; i++)
    {
        item = parent->itemAt(i);
        widget = dynamic_cast<QSpinBox*>(item->widget());
        widget->setMaximum(max_degree);
    }
 
}

QSpinBox* create_new_spinbox(int max_degree) {
    auto newSpinBox = new QSpinBox();
    newSpinBox->setMaximum(max_degree);
    newSpinBox->setMinimum(0);
    return newSpinBox;
}

void PRNGonGF::on_n_edit_returnPressed() {
    bool ok;
    int new_n = ui.n_edit->text().toInt(&ok);
    int max_degree = ui.degree_edit->text().toInt(&ok);
    bool use_even_odd = ui.even_checkbox->isChecked();
    if (ok == true) {
        if(new_n < 1)
            ui.statusBar->showMessage("N can't be less than 1");
        else {
            ui.statusBar->showMessage("New n: " + QString::number(new_n));
            int current_degree = a_first->count();
            for (size_t i = current_degree; i < new_n; i++)
            {
                a_first->addWidget(create_new_spinbox(max_degree));
                c_first->addWidget(create_new_spinbox(max_degree));

                if (use_even_odd) {
                    a_second->addWidget(create_new_spinbox(max_degree));
                    c_second->addWidget(create_new_spinbox(max_degree));
                }
            }
            for (size_t i = current_degree; i > new_n; i--) {
                remove_last_widget_from_layout(a_first);
                remove_last_widget_from_layout(c_first);
                if (use_even_odd) {
                    remove_last_widget_from_layout(a_second);
                    remove_last_widget_from_layout(c_second);
                }
            }
        }
    }
    else{
        ui.statusBar->showMessage("Degree can't be a letter");
    }
}


void PRNGonGF::on_degree_edit_returnPressed() {
    bool ok;
    int new_degree = ui.degree_edit->text().toInt(&ok);
    bool use_even_odd = ui.even_checkbox->isChecked();
    if (ok == true) {
        ui.statusBar->showMessage("New degree: " + QString::number(new_degree));
        set_spinbox_max_value(a_first, new_degree);
        set_spinbox_max_value(c_first, new_degree);
        if (use_even_odd) {
            set_spinbox_max_value(a_second, new_degree);
            set_spinbox_max_value(c_second, new_degree);
        }
    }
    else {
        ui.statusBar->showMessage("Degree can't be a letter");
    }
}

void PRNGonGF::on_even_checkbox_stateChanged() {
    bool use_even_odd = ui.even_checkbox->isChecked();
    if (!use_even_odd) {
       ui.statusBar->showMessage("Single mod is now used");

       ui.a_even_layout->removeItem(a_second);
       ui.c_even_layout->removeItem(c_second);
      
       remove_layout(a_second);
       remove_layout(c_second);
   
    }
    else {
        ui.statusBar->showMessage("Even/odd mod is now used");

        a_second = new QHBoxLayout();
        c_second = new QHBoxLayout();

        ui.a_even_layout->addLayout(a_second);
        ui.c_even_layout->addLayout(c_second);

        int current_degree = a_first->count();
        int old_degree = a_second->count();
        for (size_t i = old_degree; i < current_degree; i++)
        {
            auto newSpinBox3 = new QSpinBox();
            auto newSpinBox4 = new QSpinBox();
            a_second->addWidget(newSpinBox3);
            c_second->addWidget(newSpinBox4);
        }
    }
}
