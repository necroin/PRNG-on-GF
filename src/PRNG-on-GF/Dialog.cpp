#include "Dialog.h"

Dialog::Dialog(QWidget* parent, QString result) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    ui.result_label->setText(result);
    parentWidget()->hide();
}

//closeEvent
void Dialog::closeEvent(QCloseEvent*)
{
    parentWidget()->show();
}