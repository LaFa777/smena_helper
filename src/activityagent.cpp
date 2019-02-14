#include "activityagent.h"

ActivityAgent* ActivityAgent::instance = 0;

// гасим все логгирование библиотеки
bool no_logger_uiohook(unsigned int level, const char *format, ...) {}

/**
 * @brief Запускает event loop для прослушивания событий
 */
QDateTime ActivityAgent::getTimeLastActivity()
{
    return this->timeLastActivity;
}

void ActivityAgent::touch()
{
    this->timeLastActivity = QDateTime::currentDateTime();
}

void ActivityAgent::run()
{
    // Set the logger callback for library output.
    hook_set_logger_proc(&no_logger_uiohook);

    // Set the event callback for uiohook events.
    hook_set_dispatch_proc([](uiohook_event * const event){
        ActivityAgent *monitoring = ActivityAgent::getInstance();
        // обновляем время последней активности
        monitoring->touch();
        emit monitoring->activity();
    });

    // Start the hook and block.
    // NOTE If EVENT_HOOK_ENABLED was delivered, the status will always succeed.
    int status = hook_run();

    // TODO: возвращать статус в сигнале
}

/**
 * @brief Останавливает цикл прослушивания event'ов
 */
void ActivityAgent::stop()
{
    int status = hook_stop();
    switch (status) {
            // System level errors.
            case UIOHOOK_ERROR_OUT_OF_MEMORY:
                    qDebug() << tr("Failed to allocate memory. (%1)").arg(status);
                    break;

            // NOTE This is the only platform specific error that occurs on hook_stop().
            case UIOHOOK_ERROR_X_RECORD_GET_CONTEXT:
                    qDebug() << tr("Failed to get XRecord context. (%1)").arg(status);
                    break;

            // Default error.
            case UIOHOOK_FAILURE:
            default:
                    qDebug() << tr("An unknown hook error occurred. (%1)").arg(status);
                    break;
    }
}
