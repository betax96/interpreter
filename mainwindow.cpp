#include "mainwindow.h"
#include "ui_mainwindow.h"

extern void setTextEditError(QPlainTextEdit* edit1);
extern int parse_string(const char* in);
extern int errorLineNumber();


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isSaved = true;
    int y = ui->plainTextEdit->textCursor().blockNumber();
    int x = ui->plainTextEdit->textCursor().positionInBlock();
    statusBarLabel = new QLabel("Linea "+QString::number(y+1)+", columna "+QString::number(x+1));
    SearchReplaceDialog *rd = new SearchReplaceDialog(this,ui->plainTextEdit);
    GoTo *gd = new GoTo(this,ui->plainTextEdit);

    fileName = "sin titulo";
    this->setWindowTitle("Interpreter: "+fileName);

    loadCfg("config.cfg");

    ui->statusBar->addPermanentWidget(statusBarLabel);
    ui->actionDeshacer->setDisabled(true);
    ui->actionRehacer->setDisabled(true);
    ui->actionBuscar_siguiente->setDisabled(true);
    ui->actionIr_a->setDisabled(true);
    ui->actionBuscar->setDisabled(true);
    ui->actionReemplazar->setDisabled(true);

    connect(ui->actionIr_a,SIGNAL(triggered(bool)),gd,SLOT(showGoTo()));
    connect(ui->actionNuevo,SIGNAL(triggered(bool)),this,SLOT(newFile()));
    connect(ui->actionAbrir,SIGNAL(triggered(bool)),this,SLOT(openFile()));
    connect(ui->actionSalir,SIGNAL(triggered(bool)),this, SLOT(close()));
    connect(ui->actionGuardar,SIGNAL(triggered(bool)),this,SLOT(save()));
    connect(ui->actionGuardar_como,SIGNAL(triggered(bool)),this,SLOT(saveAs()));
    connect(ui->actionFuente,SIGNAL(triggered(bool)),this,SLOT(selectFont()));
    connect(ui->plainTextEdit,SIGNAL(cursorPositionChanged()),this,SLOT(cursorPos()));
    connect(ui->actionEvaluar,SIGNAL(triggered(bool)),this,SLOT(evaluate()));
    connect(ui->actionBarra_de_estado,SIGNAL(triggered(bool)),this,SLOT(statusBar(bool)));
    connect(ui->actionResultados,SIGNAL(triggered(bool)),this,SLOT(resultsBox(bool)));
    connect(ui->actionBuscar,SIGNAL(triggered(bool)),rd,SLOT(showSearch()));
    connect(ui->actionBuscar_siguiente,SIGNAL(triggered(bool)),rd,SLOT(search()));
    connect(ui->actionReemplazar,SIGNAL(triggered(bool)),rd,SLOT(showReplace()));
    connect(ui->actionCopiar,SIGNAL(triggered(bool)),this,SLOT(copy()));
    connect(ui->actionPegar,SIGNAL(triggered(bool)),this,SLOT(paste()));
    connect(ui->actionCortar,SIGNAL(triggered(bool)),this,SLOT(cut()));
    connect(ui->actionEliminar,SIGNAL(triggered(bool)),this,SLOT(remove()));
    connect(ui->actionSeleccionar_todo,SIGNAL(triggered(bool)),this,SLOT(selectAll()));
    connect(ui->plainTextEdit,SIGNAL(undoAvailable(bool)),this,SLOT(undoAvailable(bool)));
    connect(ui->plainTextEdit,SIGNAL(redoAvailable(bool)),this,SLOT(redoAvailable(bool)));
    connect(ui->actionDeshacer,SIGNAL(triggered(bool)),this,SLOT(undo()));
    connect(ui->actionRehacer,SIGNAL(triggered(bool)),this,SLOT(redo()));
    connect(rd,SIGNAL(searchAvailableChange(bool)),this,SLOT(switchFindNextAction(bool)));
    connect(ui->plainTextEdit->document(),SIGNAL(contentsChanged()),this,SLOT(switchMenuDocIsEmpty()));
    connect(ui->plainTextEdit,SIGNAL(dropEventSignal(QString)),this,SLOT(dropEventOpen(QString)));
}

void MainWindow::evaluate()
{
    ui->plainTextEdit_2->clear();
    setTextEditError(ui->plainTextEdit_2);
    parse_string(ui->plainTextEdit->toPlainText().toLocal8Bit().constData());
    bool haveError = !ui->plainTextEdit_2->document()->isEmpty();
    if(haveError){
        QTextCursor cursor(ui->plainTextEdit->document()->findBlockByLineNumber(errorLineNumber()-1));
        ui->plainTextEdit->setTextCursor(cursor);
    }else{
        ui->plainTextEdit_2->setPlainText("No se han encontrado errores.");
    }


}

void MainWindow::newFile(){
    bool newFile = true;
    if(!isSaved){
        QMessageBox msg(QMessageBox::Question,tr("Interpreter"),"¿Desea guardar los cambios hechos a \""+fileName+"\"?\n",
                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,this);

        msg.setButtonText(QMessageBox::Cancel,"Cancelar");
        msg.setButtonText(QMessageBox::Yes,"Si");

        int clickedButton = msg.exec();
        if(clickedButton == QMessageBox::Yes){
            if(!save())
                newFile = false;
        }else if(clickedButton == QMessageBox::Cancel){
            newFile = false;
        }
    }
    if(newFile){
        filePath = "";
        fileName = "sin titulo";
        this->setWindowTitle("Interpreter: "+fileName);
        ui->plainTextEdit->setPlainText("");
        isSaved = true;
    }
}

bool MainWindow::openFile(QString filePath){

    bool openFile = true;
    if(!isSaved){
        QMessageBox msg(QMessageBox::Question,tr("Interpreter"),"¿Desea guardar los cambios hechos a \""+fileName+"\"?\n",
                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,this);

        msg.setButtonText(QMessageBox::Cancel,"Cancelar");
        msg.setButtonText(QMessageBox::Yes,"Si");

        int clickedButton = msg.exec();
        if(clickedButton == QMessageBox::Yes){
            if(!save())
                openFile = false;
        }else if(clickedButton == QMessageBox::Cancel){
            openFile = false;
        }
    }
    if(!openFile){
        return false;
    }

    if(filePath.isEmpty()||filePath.isNull()){
        filePath = QFileDialog::getOpenFileName(this, tr("Abrir archivo"),recentDir,tr("Text Files (*.txt)"));
    }
    if(filePath.isEmpty()||filePath.isNull()){
        return false;
    }

    QFile f(filePath);

    if(!f.open(QIODevice::ReadOnly)){
        qFatal("No se pudo abrir el fichero...");
        return false;
    }
    QFileInfo fi(f.fileName());
    QString fileName = fi.baseName();

    this->fileName = fileName;
    this->filePath = filePath;

    recentDir = fi.absoluteDir().absolutePath();
    cfg->createOrModify("RecentDir",fi.absoluteDir().absolutePath());
    cfg->save();

    QTextStream in(&f);
    QString inTxt = "";
    while(!in.atEnd()){
        inTxt=inTxt+in.readLine();
        if(!in.atEnd()){
             inTxt=inTxt+"\n";
        }
    }
    ui->plainTextEdit->document()->setPlainText(inTxt);
    f.close();
    this->setWindowTitle("Interpreter: "+fileName);
    isSaved = true;
    return true;
}

bool MainWindow::save(){
    if(filePath.isEmpty()||filePath.isNull()){
        return saveAs();
    }

    QFile f(filePath);

    if(!f.open(QIODevice::WriteOnly|QIODevice::Text)){
        qFatal("No se pudo guardar el fichero...");
        return false;
    }
    QTextStream out(&f);
    QString outTxt = ui->plainTextEdit->document()->toPlainText();
    out<<outTxt;
    f.close();
    return isSaved = true;
}

bool MainWindow::saveAs(){

    QString filePath = QFileDialog::getSaveFileName(this,tr("Guardar como"),QDir::currentPath(),tr("Text Files (*.txt)"));

    if(filePath.isEmpty()||filePath.isNull()){
        return false;
    }

    QFile f(filePath);

    if(!f.open(QIODevice::WriteOnly|QIODevice::Text)){
        qFatal("No se pudo abrir el fichero...");
        return false;
    }

    QFileInfo fi(f.fileName());
    QString fileName = fi.baseName();
    this->fileName=fileName;
    this->filePath=filePath;

    QString outTxt = ui->plainTextEdit->document()->toPlainText();
    QTextStream out(&f);
    out<<outTxt;
    f.close();

    this->setWindowTitle("Interpreter: "+fileName);
    return isSaved = true;

}

void MainWindow::selectFont(){
    bool ok;
    QFontDialog fd;

    QFont font = QFontDialog::getFont(&ok, this->font, this,"Fuente");

    if(ok){
        this->font=font;
        ui->plainTextEdit->setFont(font);
        cfg->createOrModify("Font",font.toString());
        cfg->save();
    }
}

void MainWindow::statusBar(bool statusBarVisible){
    ui->statusBar->setVisible(statusBarVisible);
    if(statusBarVisible){
        cfg->createOrModify("StatusBarVisible","1");
    }else{
        cfg->createOrModify("StatusBarVisible","0");
    }
    cfg->save();
}

void MainWindow::resultsBox(bool resultsBoxVisible)
{
    ui->plainTextEdit_2->setVisible(resultsBoxVisible);
}

void MainWindow::cursorPos(){
    qint32 x = ui->plainTextEdit->textCursor().blockNumber();
    qint32 y = ui->plainTextEdit->textCursor().positionInBlock();
    statusBarLabel->setText("Linea "+QString::number(x+1)+", columna "+QString::number(y+1));
}

void MainWindow::copy(){
    ui->plainTextEdit->copy();
}

void MainWindow::paste(){
    ui->plainTextEdit->paste();
}

void MainWindow::remove(){
    ui->plainTextEdit->textCursor().removeSelectedText();
}

void MainWindow::cut(){
    ui->plainTextEdit->cut();
}

void MainWindow::selectAll(){
    ui->plainTextEdit->selectAll();

}

void MainWindow::undoAvailable(bool available){
    ui->actionDeshacer->setDisabled(!available);
}

void MainWindow::redoAvailable(bool available){
    ui->actionRehacer->setDisabled(!available);
}

void MainWindow::undo(){
    ui->plainTextEdit->undo();
}

void MainWindow::redo(){
    ui->plainTextEdit->redo();
}

void MainWindow::switchFindNextAction(bool searchAvailable){
    ui->actionBuscar_siguiente->setDisabled(!searchAvailable);
}

void MainWindow::switchMenuDocIsEmpty(){ 
    bool isEmpty = ui->plainTextEdit->document()->isEmpty();
    if(isEmpty){
        isSaved = true;
    }else{
        isSaved = false;
    }
    ui->actionIr_a->setDisabled(isEmpty);
    ui->actionBuscar->setDisabled(isEmpty);
    ui->actionReemplazar->setDisabled(isEmpty);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!isSaved){
        QMessageBox msg(QMessageBox::Question,tr("Interpreter"),"¿Desea guardar los cambios hechos a \""+fileName+"\"?\n",
                                               QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,this);

        msg.setButtonText(QMessageBox::Cancel,"Cancelar");
        msg.setButtonText(QMessageBox::Yes,"Si");

        int clickedButton = msg.exec();
        if(clickedButton == QMessageBox::Yes){
            if(save()){
                event->accept();
            }else{
                event->ignore();
            }
        }else if(clickedButton == QMessageBox::Cancel){
            event->ignore();
        }else if(clickedButton == QMessageBox::No){
            event->accept();
        }
    }
}

void MainWindow::dropEventOpen(QString mimeText){
    openFile(mimeText);
}


void MainWindow::loadCfg(QString configFile){
    cfg = new ConfigFile(configFile);
    cfg->open();

    CfgData *cData = cfg->findByName("Font");

    if(cData==0){
        font = QFont("Courier",10);
    }else{
        font.fromString(cData->value());
    }
    ui->plainTextEdit->setFont(font);

    cData = cfg->findByName("StatusBarVisible");
    if(cData==0||cData->value()=="0"){
        ui->statusBar->setVisible(false);
        ui->actionBarra_de_estado->setChecked(false);
    }else if(cData->value()=="1"){
        ui->statusBar->setVisible(true);
        ui->actionBarra_de_estado->setChecked(true);
    }

    cData = cfg->findByName("RecentDir");
    if(cData==0){
        recentDir = QDir::currentPath();
    }else{
        recentDir = cData->value();
    }
}
