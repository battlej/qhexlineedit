#ifndef QHEXLINEEDIT_H
#define QHEXLINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include <QDebug>
#include <QMenu>
#include <QKeyEvent>
#include <QClipboard>
#include <QApplication>

class QHexLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	explicit QHexLineEdit(QWidget *parent = 0);
	void keyPressEvent(QKeyEvent * event);
	void keyReleaseEvent(QKeyEvent * event);
	void contextMenuEvent(QContextMenuEvent * event);
	void focusInEvent(QFocusEvent * e);

	QString textInlucdedSpace();
	QString text();
	unsigned int bytes();

private:
	QString preValue;
	bool checked;
	QMenu *menu;
	void displayUpdate();

public slots:
signals:

public slots:
	void copyWithoutSpace();
};

#endif // QHEXLINEEDIT_H
