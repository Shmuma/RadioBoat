#include "optionsdialog.hpp"
#include "ui_optionsdialog.h"

OptionsDialog::OptionsDialog(const Options& opts, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    if (opts.localMode ())
        ui->localModeRadioButton->toggle ();
    else
        ui->remoteModeRadioButton->toggle ();
    ui->hostEdit->setText (opts.host ());
    ui->portEdit->setText (QString::number (opts.port ()));
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}


void OptionsDialog::remoteModeToggled (bool val)
{
    ui->remoteOptionsFrame->setEnabled (val);
}



Options OptionsDialog::getOptions ()
{
    Options res;

    res.setLocalMode (ui->localModeRadioButton->isChecked ());
    res.setHost (ui->hostEdit->text ());
    res.setPort (ui->portEdit->text ().toInt ());
    return res;
}
