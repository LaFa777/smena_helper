#ifndef ACTIVITYAGENT_H
#define ACTIVITYAGENT_H

#include <QDateTime>
#include <QDebug>
#include <QThread>

#include <uiohook.h>

/**
 * @brief Агент мониторинга активности пользователя
 *
 * Мониторит за активностью пользователя. Под активностью подразумевается:
 *  * движение мыши (тачпада)
 *  * нажатие на кнопку мыши (тачпада)
 *  * нажатие на клавишу клавиатуры (или комбинацию клавиш)
 */
class ActivityAgent : public QThread
{
    Q_OBJECT
public:
    static ActivityAgent * getInstance() {
        if(!instance)
            instance = new ActivityAgent();
        return instance;
    }
    QDateTime getTimeLastActivity();
    void touch(); // обновляет время последней активности

private:
    QDateTime timeLastActivity;

    // паттерн синглетон
    static ActivityAgent *instance;
    // Конструкторы и оператор присваивания недоступны клиентам
    ActivityAgent() {}
    ActivityAgent( const ActivityAgent& );
    ActivityAgent& operator=( ActivityAgent& );

signals:
    void activity(); // вызывается при любой активности

public slots:
    void run();
    void stop();
};

#endif // ACTIVITYAGENT_H
