#include "trayicon.h"

TrayIcon::TrayIcon(QObject *parent) :
    QSystemTrayIcon(parent)
{
    this->initMenu();
    this->setState(SmenaState::DISCONNECT);

    this->monitoring = ActivityAgent::getInstance();
}

void TrayIcon::initMenu()
{
    contextMenu = new QMenu();
    contextMenu->addAction(
                tr("выход"),
                QApplication::instance(),
                SLOT(quit())
    );

    this->setContextMenu(contextMenu);
}

/**
 * @brief Устанавливает иконку в трее
 * @param state
 */
void TrayIcon::setState(SmenaState state)
{
    // если state не изменился, то не меняем иконку
    if (prevSmenaState == state)
        return;

    QIcon icon;
    switch (state) {
        case SmenaState::OPEN:
            icon = QIcon(":/bin/open.ico");
            break;
        case SmenaState::CLOSE:
            icon = QIcon(":/bin/close.ico");
            break;
        default:
            icon = QIcon(":/bin/no_connect.ico");
            break;
    }
    this->setIcon(icon);
}
