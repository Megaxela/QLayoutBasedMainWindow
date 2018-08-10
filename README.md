# Qt Layout based MainWindow
It's base for dock based layout without main widget.

## Build
It's CMake based library, so here is steps to build it as static library.

0. Clone repo: `git clone https://github.com/Megaxela/QLayoutBasedMainWindow`
0. Go into repo: `cd QLayoutBasedMainWindow`
0. Create build folder: `mkdir build`
0. Go into build folder: `cd build`
0. Generate build file for your compiler: `cmake ..`
0. Build library: `cmake --build .` 

## Example
**Creating proper MainWindow**

```cpp
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QLayoutMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent=nullptr) : 
        QLayoutMainWindow(parent)
    {
        // Argument is not required (it can be `nullptr`), but it
        // allows user to save/load and change
        // layout in menu bar.
        initLayoutSystem(m_ui->menuLayout);
        
        // This call is not required. It specifies
        // `QMenu` object, that will handle `QAction`s 
        // for `QDockWidget` manipulation.
        initDockWidgetMenu(m_ui->menuWindow);
    
        addDockWidget<QListWidget>(
            "Some list widget" // It will be used as dock name
            "SomeListWidget"   // This value has to be unique, it used for save/load.
        );
        
        addDockWidget<QTreeWidget>(
            "Some tree widget" // It will be used as dock name
            "SomeTreeWidget"   // This value has to be unique, it used for save/load.
        );
        
        // If you have to easily get created 
        // widget, instead of dock widget, you 
        // can use this statement.
        auto createdWidget = dynamic_cast<QListWidget*>(
            getWidgetFromDockWidget(
                addDockWidget<QListWidget>(
                    "Another dock widget"
                    "AnotherDockWidget"
                )
            )
        );
        
        // This function is required to load
        // layouts to layout system from save file.
        // By default its 'layout.ini' file. You
        // can change it with setLayoutStorageFilename method.
        updateAvailableLayouts();
    }
    
protected:
    QByteArray getDefaultLayout()
    {
        // Value here can be determined by 
        // calling saveState(/* version here */).toBase64();
        return "...";
    }
    
private:
    Ui::MainWindow* m_ui;
}
```

## LICENSE

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

Library is licensed under the [MIT License](https://opensource.org/licenses/MIT) 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.