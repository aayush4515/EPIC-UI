#include "corsika_input.h"
#include "ui_corsika_input.h"

#include <QMessageBox>
#include <QDialogButtonBox>

#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <stdexcept>

// CORSIKA helpers

struct CorsikaParams {
    int    nshow;
    int    prmpar;
    double erangeLow;
    int    nrunner;
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
    if (!in.is_open()){
        // create a new file called corsika_template.inp
        std::ofstream tmpl("corsika_template.inp");
        if (!tmpl.is_open()) {
            throw std::runtime_error("Failed to create corsika_template.inp");
        }

        // Optional: write default template contents
        tmpl << "# Default CORSIKA template\n";
        tmpl.close();

        // reopen the newly created template file
        in.open("corsika_template.inp");
        if (!in.is_open()) {
            throw std::runtime_error("Failed to reopen corsika_template.inp");
        }
    }

    std::ostringstream buf;
    buf << in.rdbuf();
    std::string content = buf.str();

    // TODO: This shouldnt be random.
    std::mt19937 rng(static_cast<unsigned>(p.nrunner) * 2654435761u);
    int seed1 = static_cast<int>(rng() % 900000000) + 1;
    int seed2 = static_cast<int>(rng() % 900000000) + 1;

    content = replaceAll(content, "_RUNNR_",     std::to_string(p.nrunner));
    content = replaceAll(content, "_NSHOW_",     std::to_string(p.nshow));
    content = replaceAll(content, "_PRMPAR_",    std::to_string(p.prmpar));
    content = replaceAll(content, "_ERANGELOW_", std::to_string(p.erangeLow));
    content = replaceAll(content, "_SEED1_",     std::to_string(seed1));
    content = replaceAll(content, "_SEED2_",     std::to_string(seed2));

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

    // QDialogButtonBox accepted() = OK clicked
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
    bool ok1, ok2, ok3, ok4;
    int    nrunner = ui->lineRunnr ->text().toInt(&ok1);
    int    nshow   = ui->lineNshow ->text().toInt(&ok2);
    int    prmpar  = ui->linePrmpar->text().toInt(&ok3);
    double erange  = ui->lineErange->text().toDouble(&ok4);

    if (!ok1 || !ok2 || !ok3 || !ok4) {
        QMessageBox::warning(this, "Input error",
            "All fields must be valid numbers.\n"
            "RUNNR / NShow / PRMPAR → integers\n"
            "ERange → floating-point");
        return;
    }

    std::string outputPath = "corsika_inputs/corsika_run_" + std::to_string(nrunner) + ".inp";

    try {
        CorsikaParams params { nshow, prmpar, erange, nrunner };
        generateCorsikaFile(params, "corsika_template.inp", outputPath);
        QMessageBox::information(this, "Done",
            QString("File written to:\n%1").arg(QString::fromStdString(outputPath)));
        accept();  // close the dialog on success
    } catch (const std::exception &ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
}