#include "PRNGonGF.h"

PRNGonGF::PRNGonGF(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.a_even_layout->addLayout(a_first);
    ui.c_even_layout->addLayout(c_first);
    ui.a_label_layout->addWidget(a_label_first);
    ui.c_label_layout->addWidget(c_label_first);
    ui.degree_edit->setText("1");
}

void PRNGonGF::remove_last_widget_from_layout(QBoxLayout* parent) {
    auto i = parent->count();
    auto wItem = parent->takeAt(i - 1);
    if (wItem->widget())
        wItem->widget()->setParent(NULL);
    delete wItem;
}

void PRNGonGF::remove_layout(QBoxLayout* layout) {

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

void PRNGonGF::set_spinbox_max_value(QBoxLayout* parent, int max_degree) {

    QLayoutItem* item;
    QLayout* sublayout;
    QSpinBox* widget;

    auto count = parent->count();

    for (size_t i = 0; i < count; i++)
    {
        item = parent->itemAt(i);
        widget = dynamic_cast<QSpinBox*>(item->widget());
        widget->setMaximum(std::pow(2, max_degree) - 1);
    }
 
}

QSpinBox* PRNGonGF::create_new_spinbox(int max_degree) {
    auto newSpinBox = new QSpinBox();
    newSpinBox->setMaximum(std::pow(2, max_degree) - 1);
    newSpinBox->setMaximumWidth(50);
    newSpinBox->setMinimumWidth(50);
    newSpinBox->setMinimum(0);
    return newSpinBox;
}

void PRNGonGF::on_n_edit_returnPressed() {
    bool ok;
    int max_degree = ui.degree_edit->text().toInt();
    int new_n = ui.n_edit->text().toInt(&ok);
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
    int max_degree = ui.degree_edit->text().toInt();
    if (use_even_odd) {
        ui.statusBar->showMessage("Even/odd mod is now used");

        a_second = new QHBoxLayout();
        c_second = new QHBoxLayout();

        a_label_second = new QLabel("A odd");
        c_label_second = new QLabel("C odd");

        a_label_first->setText("A even");
        c_label_first->setText("C even");

        ui.a_even_layout->addLayout(a_second);
        ui.c_even_layout->addLayout(c_second);

        ui.a_label_layout->addWidget(a_label_second);
        ui.c_label_layout->addWidget(c_label_second);

        int current_degree = a_first->count();
        int old_degree = a_second->count();
        for (size_t i = old_degree; i < current_degree; i++)
        {
            a_second->addWidget(create_new_spinbox(max_degree));
            c_second->addWidget(create_new_spinbox(max_degree));
        }
    }
    else {
    ui.statusBar->showMessage("Single mod is now used");

    a_label_first->setText("A");
    c_label_first->setText("C");

    ui.a_even_layout->removeItem(a_second);
    ui.c_even_layout->removeItem(c_second);

    remove_layout(a_second);
    remove_layout(c_second);

    remove_last_widget_from_layout(ui.a_label_layout);
    remove_last_widget_from_layout(ui.c_label_layout);

 
    }
  
}

void PRNGonGF::on_build_prng_clicked() {

    bool ok1;
    bool ok2;
    int n = ui.degree_edit->text().toInt(&ok1);
    int N = ui.n_edit->text().toInt(&ok2);
    bool use_even_odd = ui.even_checkbox->isChecked();
    int sum_type = ui.field_box->currentIndex();  // 0 - 2^N, 1 - G

    if (ok1 && ok2) {

        try{
            int p = 2;

            auto primitives = prim_poly::find_prim(n);
            auto field = GaloisField(p, n, primitives.front());

            std::vector<GFElement> A;
            std::vector<GFElement> C;

            int a_count = a_first->count();

            for (size_t i = 0; i < a_count; i++)
            {
                auto current_spin_A_even = dynamic_cast<QSpinBox*>(a_first->itemAt(i)->widget());
                auto current_spin_C_even = dynamic_cast<QSpinBox*>(c_first->itemAt(i)->widget());
                bool ok1;
                bool ok2;
                int value_a = current_spin_A_even->text().toInt(&ok1);
                int value_c = current_spin_C_even->text().toInt(&ok2);
                if (ok1 && ok2) {
                    A.push_back(field.create_polynomial(prim_poly::detail::to_binary(value_a, n)));
                    C.push_back(field.create_polynomial(prim_poly::detail::to_binary(value_c, n)));
                }
                else {
                    throw std::exception("Wrong A/C parameters");
                }
            }

            auto generator = Generator(field, N, A, C);

            if (use_even_odd) {
                std::vector<GFElement> A_odd;
                std::vector<GFElement> C_odd;
                int a_count = a_first->count();

                for (size_t i = 0; i < a_count; i++)
                {
                    auto current_spin_A_odd = dynamic_cast<QSpinBox*>(c_second->itemAt(i)->widget());
                    auto current_spin_C_odd = dynamic_cast<QSpinBox*>(c_second->itemAt(i)->widget());
                    bool ok1;
                    bool ok2;
                    int value_a = current_spin_A_odd->text().toInt(&ok1);
                    int value_c = current_spin_C_odd->text().toInt(&ok2);
                    if (ok1 && ok2) {
                        A_odd.push_back(field.create_polynomial(prim_poly::detail::to_binary(value_a, n)));
                        C_odd.push_back(field.create_polynomial(prim_poly::detail::to_binary(value_c, n)));
                    }
                    else {
                        throw std::exception("Wrong A/C parameters");
                    }
                }
                generator.set_odd_A(A_odd);
                generator.set_odd_C(C_odd);

                

            }

            // 0 - 2^N, 1 - G

            if (sum_type) {
                generator.set_even_iteration_add(Generator::field_add);
                generator.set_odd_iteration_add(Generator::field_add);
            }

            std::ostringstream result_string;
            generator.print_all_cycles(result_string);
            
            Dialog* mDialog = new Dialog(this, result_string.str().c_str());
            mDialog->show();
        }
        catch(std::exception& exc){
            ui.statusBar->showMessage(exc.what());
        }

        
        


    }
   
}