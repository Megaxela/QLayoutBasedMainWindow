#pragma once

#include <QDialog>

namespace Ui
{
    class SaveLayoutDialog;
}

/**
 * @brief Dialog for asking user for name of layout.
 */
class LayoutSavingDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param parent Parent widget.
     */
    explicit LayoutSavingDialog(QWidget* parent);

    // Preventing copying
    LayoutSavingDialog(const LayoutSavingDialog&) = delete;
    LayoutSavingDialog& operator=(const LayoutSavingDialog&) = delete;

    /**
     * @brief Method for getting entered layout name.
     * If returned value is empty, user is canceled
     * enter.
     * @return
     */
    QString layoutName() const;

private slots:
    /**
     * @brief Slot, that's calling on OK button pressed.
     */
    void onOkButtonPressed();

    /**
     * @brief Slot, that's calling on Cancel button pressed.
     */
    void onCancelButtonPressed();

private:

    /**
     * @brief Method for connecting all widgets to slots.
     */
    void performConnections();

    Ui::SaveLayoutDialog* m_ui;
    QString m_layoutName;
};

