#include <QtCore/QSettings>
#include <Windows/QLayoutMainWindow.h>
#include <Dialogs/LayoutSavingDialog.h>


QLayoutMainWindow::QLayoutMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_layoutMenus(),
    m_layoutMenu(nullptr),
    m_windowMenu(nullptr),
    m_layoutStorageFilename("layout.ini"),
    m_stateSavingEnabled(true)
{

}

QLayoutMainWindow::~QLayoutMainWindow()
{
    if (m_stateSavingEnabled)
    {
        saveCurrentLayoutState();
    }
}

void QLayoutMainWindow::setStateCloseEnabled(bool enabled)
{
    m_stateSavingEnabled = enabled;
}

void QLayoutMainWindow::updateAvailableLayouts()
{
    // Clearing old layouts
    for (auto&& oldMenus : m_layoutMenus)
    {
        oldMenus.menuEntry->deleteLater();
    }

    m_layoutMenus.clear();

    // Loading new layouts
    QSettings layouts(m_layoutStorageFilename, QSettings::IniFormat);

    auto groups = layouts.childGroups();

    if (m_layoutMenu == nullptr)
    {
        return;
    }

    for (auto&& groupName : groups)
    {
        layouts.beginGroup(groupName);

        if (!layouts.contains("name"))
        {
            continue;
        }

        if (!layouts.contains("dock_location"))
        {
            continue;
        }

        // Creating actions for menu
        auto newMenu = new QMenu(layouts.value("name").toString(), m_layoutMenu);

        auto applyAction = new QAction(tr("Apply"), newMenu);
        newMenu->addAction(applyAction);

        connect(applyAction,
                &QAction::triggered,
                [this, groupName]()
                {
                    applyLayout(groupName);
                });

        auto deleteAction = new QAction(tr("Delete"), newMenu);
        newMenu->addAction(deleteAction);

        connect(deleteAction,
                &QAction::triggered,
                [this, groupName]()
                {
                    deleteLayout(groupName);
                });

        m_layoutMenu->addMenu(newMenu);

        m_layoutMenus.push_back({newMenu, groupName});

        layouts.endGroup();
    }
}

void QLayoutMainWindow::setLayoutStorageFilename(const QString& filename)
{
    m_layoutStorageFilename = filename;
}

QString QLayoutMainWindow::layoutStorageFilename() const
{
    return m_layoutStorageFilename;
}

void QLayoutMainWindow::deleteLayout(const QString& groupName)
{
    QSettings layouts(m_layoutStorageFilename, QSettings::IniFormat);

    if (!layouts.childGroups().contains(groupName))
    {
        return;
    }

    layouts.beginGroup(groupName);

    layouts.remove("");

    layouts.endGroup();

    updateAvailableLayouts();
}

void QLayoutMainWindow::applyLayout(const QString& groupName)
{
    if (groupName.isEmpty())
    {
        restoreState(getDefaultLayout(), getWindowVersion());
        return;
    }

    QSettings layouts(m_layoutStorageFilename, QSettings::IniFormat);

    if (!layouts.childGroups().contains(groupName))
    {
        return;
    }

    layouts.beginGroup(groupName);

    restoreState(layouts.value("dock_location").toByteArray(), getWindowVersion());

    layouts.endGroup();
}

QStringList QLayoutMainWindow::availableLayouts() const
{
    QStringList list;
    
    for (auto&& element : m_layoutMenus)
    {
        list.append(element.name);
    }
    
    return list;
}

int QLayoutMainWindow::getWindowVersion() const
{
    return 0;
}

QByteArray QLayoutMainWindow::getCurrentLayout() const
{
    return saveState(getWindowVersion());
}

void QLayoutMainWindow::initLayoutSystem(QMenu *parentMenu)
{
    // Removing central widget
    setCentralWidget(nullptr);

    m_layoutMenu = parentMenu;

    // Adding Save and Load buttons
    auto saveAction = new QAction(tr("Save"), m_layoutMenu);

    connect(saveAction,
            &QAction::triggered,
            this,
            [this](){ saveLayout(); });

    auto loadDefaultAction = new QAction(tr("Default"), m_layoutMenu);

    connect(loadDefaultAction,
            &QAction::triggered,
            [this](){ applyLayout(QString()); });

    m_layoutMenu->addAction(saveAction);
    m_layoutMenu->addSeparator();
    m_layoutMenu->addAction(loadDefaultAction);

    updateAvailableLayouts();
}

void QLayoutMainWindow::initDockWidgetMenu(QMenu* parentMenu)
{
    m_windowMenu = parentMenu;
}

void QLayoutMainWindow::saveLayout()
{
    // Display layout saving dialog
    LayoutSavingDialog dialog(this);
    dialog.exec();

    auto layoutName = dialog.layoutName();

    // If name is empty, saving was canceled
    if (layoutName.isEmpty())
    {
        return;
    }

    saveLayout(layoutName);
}

void QLayoutMainWindow::saveLayout(const QString& name)
{
    QSettings layouts(m_layoutStorageFilename, QSettings::IniFormat);

    layouts.beginGroup("layout-" + name);

    layouts.setValue("name", name);
    layouts.setValue("dock_location", getCurrentLayout());

    layouts.endGroup();

    updateAvailableLayouts();
}

QWidget *QLayoutMainWindow::getWidgetFromDockWidget(QDockWidget *dockWidget)
{
    return dynamic_cast<QVBoxLayout*>(
        dockWidget->widget()
            ->children()[0]
    )->itemAt(0)->widget();
}

void QLayoutMainWindow::saveCurrentLayoutState()
{
    QSettings layouts(m_layoutStorageFilename, QSettings::IniFormat);

    layouts.beginGroup("current-layout");

    layouts.setValue("dock_location", getCurrentLayout());

    layouts.endGroup();
}

void QLayoutMainWindow::loadCurrentLayoutState()
{
    QSettings layouts(m_layoutStorageFilename, QSettings::IniFormat);

    if (!layouts.childGroups().contains("current-layout"))
    {
        return;
    }

    layouts.beginGroup("current-layout");

    restoreState(layouts.value("dock_location").toByteArray(), getWindowVersion());

    layouts.endGroup();
}
