#ifndef CORSIKA_INPUT_H
#define CORSIKA_INPUT_H

#include <QDialog>

namespace Ui {
class CORSIKA_INPUT;
}

class CORSIKA_INPUT : public QDialog
{
    Q_OBJECT

public:
    explicit CORSIKA_INPUT(QWidget *parent = nullptr);
    ~CORSIKA_INPUT();

private:
    Ui::CORSIKA_INPUT *ui;
};

#endif // CORSIKA_INPUT_H
