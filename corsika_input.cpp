#include "corsika_input.h"
#include "ui_corsika_input.h"

#include <QMessageBox>
#include <QDialogButtonBox>

#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

// CORSIKA helpers

struct CorsikaParams {
    int    nrunner;
    int    nshow;
    int    prmpar;
    double erangeLow;
    double erangeHigh;
    double thetapLow;
    double thetapHigh;
    double phiLow;
    double phiHigh;
    int    seed1;
    int    seed2;
};

static std::string replaceAll(std::string text,
                               const std::string &token,
                               const std::string &value)
{
    size_t pos = 0;
    while ((pos = text.find(token, pos)) != std::string::npos) {
        text.replace(pos, token.size(), value);
        pos += value.size();
    }
    return text;
}

static void generateCorsikaFile(const CorsikaParams &p,
                                 const std::string   &templatePath,
                                 const std::string   &outputPath)
{
    std::ifstream in(templatePath);
    if (!in.is_open()) {
        // Create a default template if none exists
        std::ofstream tmpl("corsika_template.steer");
        if (!tmpl.is_open())
            throw std::runtime_error("Failed to create corsika_template.steer");

        tmpl << "# Default CORSIKA template\n";
        tmpl.close();

        in.open("corsika_template.steer");
        if (!in.is_open())
            throw std::runtime_error("Failed to reopen corsika_template.steer");
    }

    std::ostringstream buf;
    buf << in.rdbuf();
    std::string content = buf.str();

    content = replaceAll(content, "_RUNNR_",      std::to_string(p.nrunner));
    content = replaceAll(content, "_NSHOW_",      std::to_string(p.nshow));
    content = replaceAll(content, "_PRMPAR_",     std::to_string(p.prmpar));
    content = replaceAll(content, "_ERANGELOW_",  std::to_string(p.erangeLow));
    content = replaceAll(content, "_ERANGEHIGH_", std::to_string(p.erangeHigh));
    content = replaceAll(content, "_THETAPLOW_",  std::to_string(p.thetapLow));
    content = replaceAll(content, "_THETAPHIGH_", std::to_string(p.thetapHigh));
    content = replaceAll(content, "_PHIPLOW_",    std::to_string(p.phiLow));
    content = replaceAll(content, "_PHIPHIGH_",   std::to_string(p.phiHigh));
    content = replaceAll(content, "_SEED1_",      std::to_string(p.seed1));
    content = replaceAll(content, "_SEED2_",      std::to_string(p.seed2));

    std::ofstream out(outputPath);
    if (!out.is_open())
        throw std::runtime_error("Cannot write output: " + outputPath);

    out << content;
}

// ─── Dialog implementation ────────────────────────────────────────────────────

CORSIKA_INPUT::CORSIKA_INPUT(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CORSIKA_INPUT)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted,
            this, &CORSIKA_INPUT::onGenerate);
    connect(ui->buttonBox, &QDialogButtonBox::rejected,
            this, &QDialog::reject);
}

CORSIKA_INPUT::~CORSIKA_INPUT()
{
    delete ui;
}

void CORSIKA_INPUT::onGenerate()
{
    bool ok1, ok2, ok3, ok4, ok5, ok6, ok7, ok8, ok9, ok10, ok11;

    int    nrunner    = ui->lineRunnr     ->text().toInt   (&ok1);
    int    nshow      = ui->lineNshow     ->text().toInt   (&ok2);
    int    prmpar     = ui->linePrmpar    ->text().toInt   (&ok3);
    double erangeLow  = ui->lineErangeLow ->text().toDouble(&ok4);
    double erangeHigh = ui->lineErangeHigh->text().toDouble(&ok5);
    double thetapLow  = ui->lineThetapLow ->text().toDouble(&ok6);
    double thetapHigh = ui->lineThetapHigh->text().toDouble(&ok7);
    double phiLow     = ui->linePhiLow    ->text().toDouble(&ok8);
    double phiHigh    = ui->linePhiHigh   ->text().toDouble(&ok9);
    int    seed1      = ui->lineSeed1     ->text().toInt   (&ok10);
    int    seed2      = ui->lineSeed2     ->text().toInt   (&ok11);

    if (!ok1 || !ok2 || !ok3 || !ok10 || !ok11) {
        QMessageBox::warning(this, "Input error",
            "RUNNR, NSHOW, PRMPAR, SEED1, and SEED2 must be valid integers.");
        return;
    }

    if (!ok4 || !ok5 || !ok6 || !ok7 || !ok8 || !ok9) {
        QMessageBox::warning(this, "Input error",
            "ERANGELOW, ERANGEHIGH, THETAPLOW, THETAPHIGH, PHILOW, and PHIHIGH "
            "must be valid floating-point numbers.");
        return;
    }

    if (erangeLow > erangeHigh) {
        QMessageBox::warning(this, "Input error",
            "ERANGELOW must be less than or equal to ERANGEHIGH.");
        return;
    }

    if (thetapLow > thetapHigh) {
        QMessageBox::warning(this, "Input error",
            "THETAPLOW must be less than or equal to THETAPHIGH.");
        return;
    }

    if (phiLow > phiHigh) {
        QMessageBox::warning(this, "Input error",
            "PHILOW must be less than or equal to PHIHIGH.");
        return;
    }

    std::string outputPath = "corsika_inputs/corsika_run_" + std::to_string(nrunner) + ".steer";

    try {
        CorsikaParams params { nrunner, nshow, prmpar,
                               erangeLow, erangeHigh,
                               thetapLow, thetapHigh,
                               phiLow, phiHigh,
                               seed1, seed2 };
        generateCorsikaFile(params, "corsika_template.steer", outputPath);
        QMessageBox::information(this, "Done",
            QString("File written to:\n%1").arg(QString::fromStdString(outputPath)));
        accept();
    } catch (const std::exception &ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
}