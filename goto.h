#ifndef GOTO_H
#define GOTO_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class GoTo;
}

class GoTo : public QDialog
{
    Q_OBJECT

public:
    explicit GoTo(QWidget *parent = 0, QPlainTextEdit *p=0);

public slots:
    void showGoTo();
    void moveCursor();

private:
    Ui::GoTo *ui;
    QPlainTextEdit *text;
};

#endif // GOTO_H
