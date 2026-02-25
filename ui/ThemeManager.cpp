#include <QStyle>
#include <QApplication>
#include <QStyleFactory>
#include <QMenu>

#include "ThemeManager.hpp"

ThemeManager *themeManager = new ThemeManager;

extern QString ReadFileText(const QString &path);

void ThemeManager::ApplyTheme(const QString &theme) {
    auto internal = [=] {
        if (this->system_style_name.isEmpty()) {
            this->system_style_name = qApp->style()->objectName();
        }
        if (this->current_theme == theme) {
            return;
        }

        bool ok;
        auto themeId = theme.toInt(&ok);

        if (ok) {
            // System & Built-in
            QString qss;

            if (themeId != 0) {
                QString path;
                std::map<QString, QString> replace;
                switch (themeId) {
                    case 1:
                        path = ":/themes/feiyangqingyun/qss/flatgray.css";
                        replace[":/qss/"] = ":/themes/feiyangqingyun/qss/";
                        break;
                    case 2:
                        path = ":/themes/feiyangqingyun/qss/lightblue.css";
                        replace[":/qss/"] = ":/themes/feiyangqingyun/qss/";
                        break;
                    case 3:
                        path = ":/themes/feiyangqingyun/qss/blacksoft.css";
                        replace[":/qss/"] = ":/themes/feiyangqingyun/qss/";
                        break;
                    case 4: {
                        // Windows 11 Dark - force dark palette with system style
                        auto system_style = QStyleFactory::create(this->system_style_name);
                        QPalette darkPalette;
                        darkPalette.setColor(QPalette::Window, QColor(32, 32, 32));
                        darkPalette.setColor(QPalette::WindowText, QColor(230, 230, 230));
                        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
                        darkPalette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
                        darkPalette.setColor(QPalette::ToolTipBase, QColor(50, 50, 50));
                        darkPalette.setColor(QPalette::ToolTipText, QColor(230, 230, 230));
                        darkPalette.setColor(QPalette::Text, QColor(230, 230, 230));
                        darkPalette.setColor(QPalette::Button, QColor(45, 45, 45));
                        darkPalette.setColor(QPalette::ButtonText, QColor(230, 230, 230));
                        darkPalette.setColor(QPalette::BrightText, QColor(255, 50, 50));
                        darkPalette.setColor(QPalette::Link, QColor(137, 180, 250));
                        darkPalette.setColor(QPalette::Highlight, QColor(29, 78, 216)); // Darker blue #1d4ed8
                        darkPalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255)); // White text
                        darkPalette.setColor(QPalette::PlaceholderText, QColor(140, 140, 140));
                        qApp->setPalette(darkPalette);
                        qApp->setStyle(system_style);
                        qApp->setStyleSheet("");
                        current_theme = theme;
                        auto nekoray_css2 = ReadFileText(":/neko/neko.css");
                        
                        QString darkSelectedFix = "QTableView { selection-color: #ffffff; selection-background-color: #1d4ed8; } QTreeView { selection-color: #ffffff; selection-background-color: #1d4ed8; }";
                        qApp->setStyleSheet(qApp->styleSheet().append("\n").append(nekoray_css2).append("\n").append(darkSelectedFix));
                        return;
                    }
                    default:
                        return;
                }
                qss = ReadFileText(path);
                for (auto const &[a, b]: replace) {
                    qss = qss.replace(a, b);
                }
            }

            auto system_style = QStyleFactory::create(this->system_style_name);

            if (themeId == 0) {
                // system theme
                qApp->setPalette(system_style->standardPalette());
                qApp->setStyle(system_style);
                qApp->setStyleSheet("");
            } else {
                if (themeId == 1 || themeId == 2 || themeId == 3) {
                    // feiyangqingyun theme
                    QString paletteColor = qss.mid(20, 7);
                    qApp->setPalette(QPalette(paletteColor));
                } else {
                    // other theme
                    qApp->setPalette(system_style->standardPalette());
                }
                qApp->setStyleSheet(qss);
            }
        } else {
            // QStyleFactory
            const auto &_style = QStyleFactory::create(theme);
            if (_style != nullptr) {
                qApp->setPalette(_style->standardPalette());
                qApp->setStyle(_style);
                qApp->setStyleSheet("");
            }
        }

        current_theme = theme;
    };
    internal();

    auto nekoray_css = ReadFileText(":/neko/neko.css");
    qApp->setStyleSheet(qApp->styleSheet().append("\n").append(nekoray_css));

    // Specific fix for blacksoft theme tabs
    if (theme == "blacksoft" || theme == "3") {
        QString blacksoftFix = 
            "QTabBar::tab:top { padding: 4px 12px; min-height: 22px; margin-top: 2px; } "
            "QTabBar::tab:selected { background-color: #2b2b2b; } "; // Darker than #444444 normal bg
        qApp->setStyleSheet(qApp->styleSheet().append("\n").append(blacksoftFix));
    }

    // Fix menu colors for light mode (palette() in CSS may not resolve correctly)
    bool isLight = qApp->palette().color(QPalette::Window).lightness() >= 128;
    if (isLight) {
        QString lightMenuFix =
            "QMenu { background-color: #f5f5f5; color: #1a1a1a; border: 1px solid #c0c0c0; border-radius: 4px; padding: 4px; }"
            "QMenu::item { color: #1a1a1a; padding: 6px 24px; border-radius: 4px; margin: 2px; }"
            "QMenu::item:selected { background-color: rgba(50, 100, 200, 0.15); color: #2563eb; }"
            "QComboBox QAbstractItemView { background-color: #f5f5f5; color: #1a1a1a; "
            "  selection-background-color: rgba(50, 100, 200, 0.2); selection-color: #1a1a1a; border: 1px solid #c0c0c0; }"
            "QComboBox { color: #1a1a1a; background-color: #f0f0f0; }";
        qApp->setStyleSheet(qApp->styleSheet().append("\n").append(lightMenuFix));
    }

    // Windows 11 style natively ignores QMenu stylesheets and renders dark popup menus if DWM is dark.
    // To ensure our CSS is respected, we dynamically push the Fusion style to all instantiated QMenus.
    QStyle *menuStyle = QStyleFactory::create("Fusion");
    if (menuStyle) {
        for (QWidget *widget : qApp->allWidgets()) {
            if (auto menu = qobject_cast<QMenu*>(widget)) {
                menu->setStyle(menuStyle);
            }
        }
    }
}
