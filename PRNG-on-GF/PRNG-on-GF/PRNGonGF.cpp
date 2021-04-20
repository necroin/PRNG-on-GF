#include "PRNGonGF.h"
#include <string>

PRNGonGF::PRNGonGF(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.a_even_layout->addLayout(a_first);
    ui.c_even_layout->addLayout(c_first);
}

void PRNGonGF::on_degree_edit_returnPressed() {
    bool ok;
    int new_degree = ui.degree_edit->text().toInt(&ok);
    bool use_even_odd = ui.even_checkbox->isChecked();
    if (ok == true) {
        if(new_degree < 1)
            ui.statusBar->showMessage("Degree can't be less than 1");
        else {
            ui.statusBar->showMessage("New degree: " + QString::number(new_degree));
            int current_degree = ui.a_even_layout->count();
            for (size_t i = current_degree; i < new_degree + 1; i++)
            {
                auto newSpinBox1 = new QSpinBox();
                auto newSpinBox2 = new QSpinBox();
                a_first->addWidget(newSpinBox1);
                c_first->addWidget(newSpinBox2);

                if (use_even_odd) {
                    auto newSpinBox3 = new QSpinBox();
                    auto newSpinBox4 = new QSpinBox();
                    a_second->addWidget(newSpinBox3);
                    c_second->addWidget(newSpinBox4);
                }
            }
        }
    }
    else{
        ui.statusBar->showMessage("Degree can't be a letter");
    }
}

void PRNGonGF::on_n_edit_returnPressed() {
    bool ok;
    int new_n = ui.n_edit->text().toInt(&ok);
    if (ok == true) {
        ui.statusBar->showMessage("New N: " + QString::number(new_n));
    }
    else {
        ui.statusBar->showMessage("Degree can't be a letter");
    }
}

void PRNGonGF::on_even_checkbox_stateChanged() {
    bool use_even_odd = ui.even_checkbox->isChecked();
    if (!use_even_odd) {
        ui.statusBar->showMessage("Now single mod used");
       ui.a_even_layout->removeItem(a_second);
       ui.c_even_layout->removeItem(c_second);
      
       QLayoutItem* item;
       QLayout* sublayout;
       QWidget* widget;
       while ((item = a_second->takeAt(0))) {
           if ((sublayout = item->layout()) != 0) { }
           else if ((widget = item->widget()) != 0) { widget->hide(); delete widget; }
           else { delete item; }
       }

       delete a_second;

       while ((item = c_second->takeAt(0))) {
           if ((sublayout = item->layout()) != 0) { }
           else if ((widget = item->widget()) != 0) { widget->hide(); delete widget; }
           else { delete item; }
       }

       delete c_second;
   
    }
    else {
        a_second = new QHBoxLayout();
        c_second = new QHBoxLayout();
        ui.statusBar->showMessage("Now even/odd mod used");
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
