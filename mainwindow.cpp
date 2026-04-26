#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnClearCommands_clicked()
{
    ui->textDisplayCommands->clear();
}


void MainWindow::on_btnCreateCommand_clicked()
{

}


#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnClearCommands_clicked()
{
    ui->textDisplayCommands->clear();
}


void MainWindow::on_btnCreateCommand_clicked()
{
    QString program = ui->plainTextEdit->toPlainText();

    int PRMPAR = ui->boxPRMPAR->value();
    int nshow = ui->boxShow->value();
    QString erange = ui->lineRange->text();

    QString command = "sbatch ";

    command += program + " ";
    command += "--PRMPAR=" + QString::number(PRMPAR) + " ";
    command += "--Nshow=" + QString::number(nshow) + " ";
    command += "--ERange=" + erange + " ";

    ui->textDisplayCommands->append(command);
}

