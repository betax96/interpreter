#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class ReplaceDialog;
}

class SearchReplaceDialog : public QDialog
{
    Q_OBJECT

public:
     explicit SearchReplaceDialog(QWidget *parent = 0, QPlainTextEdit *p = 0);

signals:
    void searchAvailableChange(bool);

public slots:
    bool search(bool noMsg=false);
    void switchSearchButton();
    void switchReplaceButton();
    void replace();
    void replaceAll();
    void showSearch();
    void showReplace();

private:
    Ui::ReplaceDialog *ui;
    QPlainTextEdit *text;
};

#endif // REPLACEDIALOG_H
