#include "Dialog.h"

Dialog::Dialog(QWidget* parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

    parentWidget()->hide();
}

//closeEvent
void Dialog::closeEvent(QCloseEvent*)
{
    parentWidget()->show();
}