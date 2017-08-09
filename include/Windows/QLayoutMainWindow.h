//
// Created by megaxela on 8/9/17.
//

#pragma once

#include <QMainWindow>

namespace Windows
{

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
        virtual ~QLayoutMainWindow();

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

    protected:

        void initLayoutSystem(QMenu* parentMenu);

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

    private:

        // Preventing copying
        QLayoutMainWindow(const QLayoutMainWindow&) = delete;
        QLayoutMainWindow& operator=(const QLayoutMainWindow&) = delete;

        /**
         * @brief Structure for holding layout data.
         */
        struct LayoutDataHolder
        {
            LayoutDataHolder() :
                    menuEntry(),
                    name()
            {}

            LayoutDataHolder(QMenu* menuEntry, const QString& name) :
                    menuEntry(menuEntry),
                    name(name)
            {}

            LayoutDataHolder(const LayoutDataHolder& holder) :
                    menuEntry(holder.menuEntry),
                    name(holder.name)
            {

            }

            LayoutDataHolder& operator=(const LayoutDataHolder& holder)
            {
                menuEntry = holder.menuEntry;
                name = holder.name;
                return (*this);
            }

            QMenu* menuEntry;
            QString name;
        };

        QList<LayoutDataHolder> m_layoutMenus;
        QMenu* m_parentMenu;
        QString m_layoutStorageFilename;
    };
}