#include "qhexlineedit.h"

QHexLineEdit::QHexLineEdit(QWidget *parent) :
	QLineEdit(parent)
{
	menu= new QMenu();
	menu->addAction(tr("Cut"), this, SLOT(cut()), QKeySequence::Cut);
	menu->addAction(tr("Copy"), this, SLOT(copyWithoutSpace()), QKeySequence::Copy);
	menu->addAction(tr("Paste"), this, SLOT(paste()), QKeySequence::Paste);
	menu->addSeparator();
	menu->addAction(tr("Select All"), this, SLOT(selectAll()), QKeySequence::SelectAll);
	menu->addSeparator();
	menu->addAction(tr("Copy (Included space)"), this, SLOT(copy()));

}

void QHexLineEdit::keyPressEvent(QKeyEvent *event)
{
	checked = false;
	QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
	if(keyEvent->modifiers()==Qt::ControlModifier && (keyEvent->key()=='c' || keyEvent->key()=='C'))
	{
		copyWithoutSpace();
		return;
	}

	if(keyEvent->modifiers()!=Qt::ControlModifier && keyEvent->modifiers()!=Qt::AltModifier && keyEvent->modifiers()!=Qt::ShiftModifier)
	{
		if(keyEvent->key()>=0x20 && keyEvent->key()<=0x7E)
		{
			// block works except 0-9a-fA-F
			if(!((keyEvent->key()>=0x30 && keyEvent->key()<=0x39) || (keyEvent->key()>=0x61 && keyEvent->key()<=0x66) || (keyEvent->key()>=0x41 && keyEvent->key()<=0x46)))
			{
				return;
			}
		} else {
			// compare works
			if(!this->hasSelectedText()) {
				int cp = this->cursorPosition();
				if(keyEvent->key()==Qt::Key_Backspace) // backspace
				{
					if(this->text().at(cp-1)==0x20)
					{
						this->setCursorPosition(cp-1);
					}
				}
				if(keyEvent->key()==Qt::Key_Delete) // delete
				{
					if(this->text().at(cp)==0x20)
					{
						this->setCursorPosition(cp+1);
					}
				}
			}
		}
	}
	preValue = this->text();
	QLineEdit::keyPressEvent(event);
}


void QHexLineEdit::keyReleaseEvent(QKeyEvent *event)
{
	if(preValue!=this->text()) {
		displayUpdate();
	}
	QLineEdit::keyReleaseEvent(event);
}

void QHexLineEdit::displayUpdate()
{
	QString text = this->text();
	QString removeText;
	unsigned int size = text.size();
	int icheck = 0;
	bool last=false;
	int cp = this->cursorPosition();
	for(unsigned int i=0;i<size;i++) {
		if ((text.at(i) >= '0' && text.at(i) <= '9') || (text.at(i) >= 'A' && text.at(i) <= 'F') || (text.at(i) >= 'a' && text.at(i) <= 'f'))
		{
			icheck++;
			removeText.append((text.at(i)).toUpper());
			last=false;
			if(icheck==2) {
				removeText.append(0x20);
				icheck=0;
				last=true;
				cp++;
			}
			checked = true;
		} else {
			cp--;
		}
	}
	if(last) {
		removeText.remove(-1, 1);
	}
	this->setText(removeText);
	this->setCursorPosition(cp);
}


void QHexLineEdit::copyWithoutSpace()
{
	QString cstr;
	if(this->hasSelectedText()) {
		cstr = this->selectedText();

	} else {
		cstr = this->text().replace(" ", "");
	}
	cstr = cstr.trimmed().replace(" ", "");
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(cstr);
}

void QHexLineEdit::contextMenuEvent(QContextMenuEvent * event)
{
	preValue = this->text();
	menu->exec(QCursor::pos());
	if(preValue!=this->text()) {
		displayUpdate();
	}
}

void QHexLineEdit::focusInEvent(QFocusEvent * e)
{
	if(!checked) {
		displayUpdate();
		checked = true;
	}
	QLineEdit::focusInEvent(e);
}

QString QHexLineEdit::textInlucdedSpace()
{
	return QLineEdit::text();
}

QString QHexLineEdit::text()
{
	return QLineEdit::text().trimmed().replace(" ", "");
}

unsigned int QHexLineEdit::bytes()
{
	unsigned int size = text().length();
	if(size%2==1) {
		size++;
	}
	return size;
}
