#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->plainTextEdit);
    ui->actionSave->setIcon(QIcon(":/qt-project.org/styles/commonstyle/images/standardbutton-save-128.png"));
    ui->actionSave_As->setIcon(QIcon(":/qt-project.org/styles/commonstyle/images/harddrive-128.png"));
    ui->actionOpen->setIcon(QIcon(":/qt-project.org/styles/commonstyle/images/standardbutton-open-128.png"));
    ui->actionNew->setIcon(QIcon(":/qt-project.org/styles/commonstyle/images/file-128.png"));
    connect(ui->actionNew,QAction::triggered,this,MainWindow::newFile);
    connect(ui->actionOpen,QAction::triggered,this,MainWindow::openFile);
    connect(ui->actionSave,QAction::triggered,this,MainWindow::saveFile);
    connect(ui->actionSave_As,QAction::triggered,this,MainWindow::saveFileAs);
    connect(ui->actionExit,QAction::triggered,this,MainWindow::close);
    connect(ui->actionCopy,QAction::triggered,ui->plainTextEdit,QPlainTextEdit::copy);
    connect(ui->actionCut,QAction::triggered,ui->plainTextEdit,QPlainTextEdit::cut);
    connect(ui->actionPaste,QAction::triggered,ui->plainTextEdit,QPlainTextEdit::paste);
    connect(ui->actionUndo,QAction::triggered,ui->plainTextEdit,QPlainTextEdit::undo);
    connect(ui->actionRedo,QAction::triggered,ui->plainTextEdit,QPlainTextEdit::redo);
    connect(ui->actionSelect_All,QAction::triggered,ui->plainTextEdit,QPlainTextEdit::selectAll);
    connect(ui->plainTextEdit,QPlainTextEdit::textChanged,this,MainWindow::textChanged);

    connect(ui->plainTextEdit,QPlainTextEdit::copyAvailable,ui->actionCopy,QAction::setEnabled);
    connect(ui->plainTextEdit,QPlainTextEdit::copyAvailable,ui->actionCut,QAction::setEnabled);
    connect(ui->plainTextEdit,QPlainTextEdit::redoAvailable,ui->actionRedo,QAction::setEnabled);
    connect(ui->plainTextEdit,QPlainTextEdit::undoAvailable,ui->actionUndo,QAction::setEnabled);
    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    newFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    ui->plainTextEdit->clear();
    m_filename.clear();
    m_saved = false;
    ui->statusbar->showMessage("New File");
}

void MainWindow::openFile()
{
    QString tempFilename = QFileDialog::getOpenFileName(this,"Open File",QString(),"Text File (*.txt);;All File (*.*)");
    if(tempFilename.isEmpty()) return;
    QFile file(tempFilename);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this,"Error",file.errorString());
        return;
    }
    m_filename = tempFilename;
    QTextStream stream (&file);
    ui->plainTextEdit->setPlainText(stream.readAll());
    file.close();
    ui->statusbar->showMessage("File: " + m_filename);
    m_saved = true;
}

void MainWindow::saveFile()
{
    if(m_filename.isEmpty()){
        saveFileAs();
        return;
    }
    QFile file(m_filename);
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this,"Error",file.errorString());
        return;
    }
    QTextStream stream (&file);
    stream << ui->plainTextEdit->toPlainText();
    file.close();
    ui->statusbar->showMessage("File: " + m_filename);
    m_saved = true;
}

void MainWindow::saveFileAs()
{
    QString tempFilename = QFileDialog::getSaveFileName(this,"Save File",QString(),"Text File (*.txt);;All File (*.*)");
    if(tempFilename.isEmpty()) return;
    m_filename = tempFilename;
    saveFile();
}

void MainWindow::textChanged()
{
    m_saved = false;
}
