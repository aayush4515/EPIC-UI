#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
