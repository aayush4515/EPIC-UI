#include "corsika_input.h"
#include "ui_corsika_input.h"

CORSIKA_INPUT::CORSIKA_INPUT(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CORSIKA_INPUT)
{
    ui->setupUi(this);
}

CORSIKA_INPUT::~CORSIKA_INPUT()
{
    delete ui;
}
