#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QMenu>

/**
 * @brief Base class for layout based main windows.
 */
class QLayoutMainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief Constructor.
     * @param parent Parent widget.
     */
    explicit QLayoutMainWindow(QWidget* parent);

    /**
     * @brief Virtual destructor.
     */
    ~QLayoutMainWindow() override = default;

    // Preventing copying
    QLayoutMainWindow(const QLayoutMainWindow&) = delete;
    QLayoutMainWindow& operator=(const QLayoutMainWindow&) = delete;

    /**
     * @brief Method for setting layout storage filename.
     * @param filename Layout storage filename.
     */
    void setLayoutStorageFilename(const QString &filename);

    /**
     * @brief Method for getting layout storage filename.
     * @return Layout storage filename.
     */
    QString layoutStorageFilename() const;

    /**
     * @brief Method for saving current layout with dialog window
     * for layout name input.
     */
    void saveLayout();

    /**
     * @brief Method for saving current layout.
     * @param name Layout name.
     */
    void saveLayout(const QString& name);

    /**
     * @brief Method for deleting layout.
     * @param groupName Layout name.
     */
    void deleteLayout(const QString& groupName);

    /**
     * @brief Method for applying layout.
     * @param groupName Layout name.
     */
    void applyLayout(const QString& groupName);

    /**
     * @brief Method for getting list of available
     * layouts.
     * @return String list of available layouts.
     */
    QStringList availableLayouts() const;

    /**
     * @brief Method for getting current layout settings.
     * @return
     */
    QByteArray getCurrentLayout() const;

    /**
     * @brief Method for adding different widgets to dock.
     * @tparam T Widget that has to be added.
     * @param title Name of dock.
     * @param objectName Object name of dock. Has to be unique.
     * @return Created dock widget.
     */
    template<typename T>
    QDockWidget* addDockWidget(const QString& title, const QString& objectName)
    {
        // Creating dock widget
        auto dockWidget = new QDockWidget(title, this);
        dockWidget->setObjectName(objectName);

        // Creating widget for layout
        auto dockWidgetContent = new QWidget(dockWidget);

        // Creating layout
        auto verticalLayout = new QVBoxLayout(dockWidgetContent);

        // Setting up layout
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        // Creating  widget
        auto listWidget = new T(dockWidgetContent);

        // Adding widget to layout
        verticalLayout->addWidget(listWidget);

        // Adding dock widget content to dock widget.
        dockWidget->setWidget(dockWidgetContent);

        // If window menu specified, add view
        if (m_windowMenu)
        {
            auto a = m_windowMenu->addAction(title, [dockWidget](bool checked)
            {
                dockWidget->setVisible(checked);
            });

            a->setCheckable(true);

            connect(
                dockWidget,
                &QDockWidget::visibilityChanged,
                [a](bool visible)
                {
                    a->setChecked(visible);
                }
            );

        }

        return dockWidget;
    }

protected:

    /**
     * @brief Method for setting layout system save/load menu.
     * @param parentMenu
     */
    void initLayoutSystem(QMenu* parentMenu);

    /**
     * @brief Method for setting dock widget show/hide menu.
     * @param parentMenu
     */
    void initDockWidgetMenu(QMenu* parentMenu);

    /**
     * @brief Method that has to return window version. Different settings
     * for different versions will not work.
     * @return
     */
    virtual int getWindowVersion() const;

    /**
     * @brief Method for getting default layout.
     * @return Byte array representation of default layout.
     */
    virtual QByteArray getDefaultLayout() = 0;

    /**
     * @brief Method for updating available layouts from file.
     */
    void updateAvailableLayouts();

    /**
     * @brief Method for getting widget, bounded to dock widget.
     * @param dockWidget Pointer to dock widget.
     * @return Widget bounded to dock widget.
     */
    static QWidget* getWidgetFromDockWidget(QDockWidget* dockWidget);

private:

    /**
     * @brief Structure for holding layout data.
     */
    struct LayoutDataHolder
    {
        LayoutDataHolder() :
            menuEntry(),
            name()
        {}

        LayoutDataHolder(QMenu* menuEntry, QString name) :
            menuEntry(menuEntry),
            name(std::move(name))
        {}

        LayoutDataHolder(const LayoutDataHolder& holder) = default;
        LayoutDataHolder& operator=(const LayoutDataHolder& holder) = default;

        QMenu* menuEntry;
        QString name;
    };

    QList<LayoutDataHolder> m_layoutMenus;
    QMenu* m_layoutMenu;
    QMenu* m_windowMenu;
    QString m_layoutStorageFilename;
};