#include "trayicon.h"

TrayIcon::TrayIcon(QObject *parent) :
    QSystemTrayIcon(parent)
{
    this->initMenu();
    this->prevSmenaState = SmenaState::OPEN;
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
    if (this->prevSmenaState == state)
        return;

    this->prevSmenaState = state;

    QIcon icon;
    switch (state) {
        case SmenaState::OPEN:
            icon = QIcon(":/bin/open.ico");
            this->setToolTip("Беги скорей");
            break;
        case SmenaState::CLOSE:
            icon = QIcon(":/bin/close.ico");
            this->setToolTip("Пока что занято =(");
            break;
        default:
            icon = QIcon(":/bin/no_connect.ico");
            this->setToolTip("Нет соединения с сервером");
            break;
    }
    this->setIcon(icon);
}

void TrayIcon::needUpdateAlert()
{
    this->showMessage("Доступно обновление", "Пожалуйста обновитесь");
}
