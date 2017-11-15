#include "goto.h"
#include "ui_goto.h"

GoTo::GoTo(QWidget *parent, QPlainTextEdit *p) :
    QDialog(parent),
    ui(new Ui::GoTo)
{
    ui->setupUi(this);
    text = p;
    setWindowFlags(windowFlags()& ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Ir a");
    ui->spinBox->setMinimum(1);
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(moveCursor()));
}

void GoTo::showGoTo(){
    ui->spinBox->setMaximum(text->document()->blockCount());
    ui->spinBox->setValue(text->textCursor().blockNumber()+1);
    show();
}

void GoTo::moveCursor(){
    int thisBlock = text->textCursor().blockNumber()+1;
    QTextCursor cur = text->textCursor();
    if(ui->spinBox->value()>thisBlock){
        cur.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,ui->spinBox->value()-thisBlock);
    }else if(ui->spinBox->value()<thisBlock){
        cur.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor,thisBlock-ui->spinBox->value());
    }
    text->setTextCursor(cur);
}
