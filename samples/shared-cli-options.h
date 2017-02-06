#ifndef FACTORKEY_SAMPLES_SHARED_CLI_OPTIONS_H
#define FACTORKEY_SAMPLES_SHARED_CLI_OPTIONS_H

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QMap>
#include <functional>

namespace samples
{
    namespace cli
    {
        const QString ENTRY_ID_OPTION    = QLatin1String("id");
        const QString STORAGE_FILE_OPTION= QLatin1String("db");

        bool addSharedOptions(QCommandLineParser& parser);
    }
}
#endif
