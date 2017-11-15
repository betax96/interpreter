#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "configfile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

public slots:
    void newFile();
    bool openFile(QString filePath = "");
    bool save();
    bool saveAs();
    void selectFont();
    void statusBar(bool);
    void resultsBox(bool);
    void cursorPos();
    void evaluate();
    void copy();
    void cut();
    void paste();
    void remove();
    void selectAll();
    void undoAvailable(bool);
    void redoAvailable(bool);
    void undo();
    void redo();
    void switchFindNextAction(bool);
    void switchMenuDocIsEmpty();
    void closeEvent(QCloseEvent *event);
    void loadCfg(QString configFile);
    void dropEventOpen(QString mimeText);

private:
    Ui::MainWindow *ui;
    QString fileName;
    QString filePath;
    QFont font;
    QLabel *statusBarLabel;
    bool isSaved;
    ConfigFile *cfg;
    QString recentDir;
};

#endif // MAINWINDOW_H
