#include <Dialogs/LayoutSavingDialog.h>
#include <QDialogButtonBox>
#include <ui_layout_saving_dialog.h>

LayoutSavingDialog::LayoutSavingDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::SaveLayoutDialog),
        m_layoutName()
{
    m_ui->setupUi(this);

    setModal(true);

    performConnections();
}

QString LayoutSavingDialog::layoutName() const
{
    return m_layoutName;
}

void LayoutSavingDialog::onOkButtonPressed()
{
    m_layoutName = m_ui->layoutNameLineEdit->text();
    close();
}

void LayoutSavingDialog::onCancelButtonPressed()
{
    close();
}

void LayoutSavingDialog::performConnections()
{
    connect(m_ui->buttonBox,
            &QDialogButtonBox::accepted,
            this,
            &LayoutSavingDialog::onOkButtonPressed);

    connect(m_ui->buttonBox,
            &QDialogButtonBox::rejected,
            this,
            &LayoutSavingDialog::onCancelButtonPressed);
}