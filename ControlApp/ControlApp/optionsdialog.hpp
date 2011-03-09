#ifndef OPTIONSDIALOG_HPP
#define OPTIONSDIALOG_HPP

#include <QDialog>

#include "options.hpp"

namespace Ui {
    class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog (const Options& opts, QWidget *parent = 0);
    ~OptionsDialog();

    Options getOptions ();

protected slots:
    void remoteModeToggled (bool val);

private:
    Ui::OptionsDialog *ui;
};

#endif // OPTIONSDIALOG_HPP
