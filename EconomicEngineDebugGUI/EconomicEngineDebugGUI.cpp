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

}
