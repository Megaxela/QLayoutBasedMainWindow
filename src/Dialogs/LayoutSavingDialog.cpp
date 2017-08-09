//
// Created by megaxela on 8/9/17.
//

#include "Dialogs/LayoutSavingDialog.h"
#include <QtWidgets/QDialogButtonBox>
#include <ui_layout_saving_dialog.h>

Dialogs::LayoutSavingDialog::LayoutSavingDialog(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::SaveLayoutDialog),
        m_layoutName()
{
    m_ui->setupUi(this);
    performConnections();
}

QString Dialogs::LayoutSavingDialog::layoutName() const
{
    return m_layoutName;
}

void Dialogs::LayoutSavingDialog::onOkButtonPressed()
{
    m_layoutName = m_ui->layoutNameLineEdit->text();
    close();
}

void Dialogs::LayoutSavingDialog::onCancelButtonPressed()
{
    close();
}

void Dialogs::LayoutSavingDialog::performConnections()
{
    connect(m_ui->buttonBox,
            &QDialogButtonBox::accepted,
            this,
            &Dialogs::LayoutSavingDialog::onOkButtonPressed);

    connect(m_ui->buttonBox,
            &QDialogButtonBox::rejected,
            this,
            &Dialogs::LayoutSavingDialog::onCancelButtonPressed);
}