#include "message.h"
#include <QMessageBox>


void message_util::show(QWidget* parent, const QString& text) {
	QMessageBox* msgBox = new QMessageBox(parent);
	msgBox->setText(text);
	msgBox->exec();
}