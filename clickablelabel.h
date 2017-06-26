#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QWidget>

class ClickableLabel : public QLabel
{
    Q_OBJECT
signals:
    void Clicked();

public:
    ClickableLabel(QWidget * parent);

protected:
    void mouseReleaseEvent(QMouseEvent* );
};

#endif // CLICKABLELABEL_H
