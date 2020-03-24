#include "EconomicEngineDebugGUI.h"
#include <iostream>

EconomicEngineDebugGUI::EconomicEngineDebugGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.btn_add, &QPushButton::pressed,this, &EconomicEngineDebugGUI::on_btn_add_clicked);
}

void EconomicEngineDebugGUI::on_btn_add_clicked()
{
	ui.lb_x->setText("oui");

	QMessageBox msgBox;
	msgBox.setText("The document has been modified.");
	msgBox.setInformativeText("Do you want to save your changes?");
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();
}
