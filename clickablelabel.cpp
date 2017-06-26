#include "clickablelabel.h"

#include <QWidget>

ClickableLabel::ClickableLabel(QWidget * parent): QLabel(parent)
{

}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *)
{
    emit Clicked();
}
