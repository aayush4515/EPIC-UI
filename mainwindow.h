#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "corsika_input.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnClearCommands_clicked();                 // clears the commands from the slurm commands output area
    void on_btnCreateCommand_clicked();                 // creates a slurm command based on the input parameters

    void on_btnConfigureCorsika_clicked();

private:
    Ui::MainWindow *ui;
    CORSIKA_INPUT *corsikaWindow = nullptr;             // pointer to the corsika input window
};
#endif // MAINWINDOW_H
