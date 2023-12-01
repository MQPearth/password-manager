#pragma once

#include <QWidget>
#include <QString>

class message_util
{
public:
	static void show(QWidget* parent, const QString& text);
};

