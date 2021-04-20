#include "PRNGonGF.h"

PRNGonGF::PRNGonGF(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void PRNGonGF::on_degree_edit_returnPressed() {
    ui.statusBar->showMessage(ui.degree_edit->text());
}