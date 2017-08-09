//
// Created by megaxela on 8/9/17.
//

#include <QtCore/QSettings>
#include "include/Windows/QLayoutMainWindow.h"
#include <QMenu>
#include <iostream>
#include <include/Dialogs/LayoutSavingDialog.h>

Windows::QLayoutMainWindow::QLayoutMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_layoutMenus(),
    m_parentMenu(nullptr),
    m_layoutStorageFilename("layout.ini")
{

}

Windows::QLayoutMainWindow::~QLayoutMainWindow()
{

}

void Windows::QLayoutMainWindow::updateAvailableLayouts()
{
    // Clearing old layouts
    for (auto& oldMenus : m_layoutMenus)
    {
        oldMenus.menuEntry->deleteLater();
    }

    m_layoutMenus.clear();

    // Loading new layouts
    QSettings layouts(m_layoutStorageFilename, QSettings::IniFormat);

    auto groups = layouts.childGroups();

    if (m_parentMenu == nullptr)
    {
        std::cerr << "Can't update available layouts without parent menu layout" << std::endl;
        return;
    }

    for (auto groupName : groups)
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
        auto newMenu = new QMenu(layouts.value("name").toString(), m_parentMenu);

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

        m_parentMenu->addMenu(newMenu);

        m_layoutMenus.push_back({newMenu, groupName});

        layouts.endGroup();
    }
}

void Windows::QLayoutMainWindow::setLayoutStorageFilename(const QString& filename)
{
    m_layoutStorageFilename = filename;
}

QString Windows::QLayoutMainWindow::layoutStorageFilename() const
{
    return m_layoutStorageFilename;
}

void Windows::QLayoutMainWindow::deleteLayout(const QString& groupName)
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

void Windows::QLayoutMainWindow::applyLayout(const QString& groupName)
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

QStringList Windows::QLayoutMainWindow::availableLayouts() const
{
    QStringList list;
    
    for (auto element : m_layoutMenus)
    {
        list.append(element.name);
    }
    
    return list;
}

int Windows::QLayoutMainWindow::getWindowVersion() const
{
    return 0;
}

QByteArray Windows::QLayoutMainWindow::getCurrentLayout() const
{
    return saveState(getWindowVersion());
}

void Windows::QLayoutMainWindow::initLayoutSystem(QMenu *parentMenu)
{
    m_parentMenu = parentMenu;

    // Adding Save and Load buttons
    auto saveAction = new QAction(tr("Save"), m_parentMenu);

    connect(saveAction,
            &QAction::triggered,
            this,
            [this](){ saveLayout(); });

    auto loadDefaultAction = new QAction(tr("Default"), m_parentMenu);

    connect(loadDefaultAction,
            &QAction::triggered,
            [this](){ applyLayout(QString()); });

    m_parentMenu->addAction(saveAction);
    m_parentMenu->addSeparator();
    m_parentMenu->addAction(loadDefaultAction);

    updateAvailableLayouts();
}

void Windows::QLayoutMainWindow::saveLayout()
{
    // Display layout saving dialog
    Dialogs::LayoutSavingDialog dialog(this);
    dialog.exec();

    auto layoutName = dialog.layoutName();

    // If name is empty, saving was canceled
    if (layoutName.isEmpty())
    {
        return;
    }

    saveLayout(layoutName);
}

void Windows::QLayoutMainWindow::saveLayout(const QString& name)
{
    QSettings layouts(m_layoutStorageFilename, QSettings::IniFormat);

    layouts.beginGroup("layout-" + name);

    layouts.setValue("name", name);
    layouts.setValue("dock_location", getCurrentLayout());

    layouts.endGroup();

    updateAvailableLayouts();
}