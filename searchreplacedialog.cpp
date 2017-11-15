#include "searchreplacedialog.h"
#include "ui_searchreplacedialog.h"
#include <QMessageBox>

SearchReplaceDialog::SearchReplaceDialog(QWidget *parent, QPlainTextEdit *p) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    text = p;
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->radioButton_2->setChecked(true);
    ui->lineEdit->setFocus();
    ui->pushButton->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(search()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(replace()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(replaceAll()));
    connect(ui->lineEdit,SIGNAL(textEdited(QString)),this,SLOT(switchSearchButton()));
    connect(ui->lineEdit_2,SIGNAL(textEdited(QString)),this,SLOT(switchReplaceButton()));
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(close()));
}



bool SearchReplaceDialog::search(bool noMsg){
    bool success = false;
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit->text().isNull()){
        return success;
    }
    bool toUp = ui->radioButton->isChecked();
    bool caseSensitive = ui->checkBox->isChecked();
    QTextCursor c;
    if(!toUp){
        if(caseSensitive){
            c = text->document()->find(ui->lineEdit->text(),text->textCursor(),QTextDocument::FindCaseSensitively);
        }else{
            c = text->document()->find(ui->lineEdit->text(),text->textCursor());
        }
    }else{
        if(caseSensitive){
            c = text->document()->find(ui->lineEdit->text(),text->textCursor(),QTextDocument::FindCaseSensitively|QTextDocument::FindBackward);
        }else{
            c = text->document()->find(ui->lineEdit->text(),text->textCursor(),QTextDocument::FindBackward);
        }
    }
    if(!c.isNull()){
        text->setTextCursor(c);
        parentWidget()->activateWindow();
        success = true;
    }else{
        if(!noMsg){
            QMessageBox msg(QMessageBox::Information,"Interpreter","No se encontro \""+ui->lineEdit->text()+"\"",QMessageBox::Ok,this);
            msg.exec();
        }
    }
    return success;
}

void SearchReplaceDialog::replace(){
    bool caseSensitive = ui->checkBox->isChecked();
    bool q;
    if(caseSensitive){
        q = !text->textCursor().selectedText().compare(ui->lineEdit->text(),Qt::CaseSensitive);
    }else{
        q = !text->textCursor().selectedText().compare(ui->lineEdit->text(),Qt::CaseInsensitive);
    }
    if(q){
        text->textCursor().insertText(ui->lineEdit_2->text());
    }else{
        search();
    }
}

void SearchReplaceDialog::replaceAll(){
    bool success = false;
    while(bool v = search(true)){
        success = success||v;
        text->textCursor().insertText(ui->lineEdit_2->text());
    }
    if(!success){
        QMessageBox msg(QMessageBox::Information,"Interpreter","No se encontro \""+ui->lineEdit->text()+"\"",QMessageBox::Ok,this);
        msg.exec();
    }
}

void SearchReplaceDialog::switchSearchButton(){
    bool sw = ui->lineEdit->text().isEmpty()||ui->lineEdit->text().isNull();
    ui->pushButton->setDisabled(sw);
    emit searchAvailableChange(!sw);
}

void SearchReplaceDialog::switchReplaceButton(){
    bool sw = ui->lineEdit_2->text().isEmpty()||ui->lineEdit_2->text().isNull();
    ui->pushButton_2->setDisabled(sw);
    ui->pushButton_3->setDisabled(sw);

}

void SearchReplaceDialog::showSearch(){
    ui->lineEdit_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_2->hide();
    ui->label_2->hide();
    setWindowTitle("Buscar");
    show();
}

void SearchReplaceDialog::showReplace(){
    ui->lineEdit_2->show();
    ui->pushButton_3->show();
    ui->pushButton_2->show();
    ui->label_2->show();
    setWindowTitle("Reemplazar");
    show();
}
