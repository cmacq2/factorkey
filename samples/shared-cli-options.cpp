#include "shared-cli-options.h"

namespace samples
{
    namespace cli
    {
        bool addSharedOptions(QCommandLineParser& parser)
        {
            QCommandLineOption entry(QStringList() << QLatin1String("I") << ENTRY_ID_OPTION << QLatin1String("token-id") << QLatin1String("token"),
                                     QLatin1String("Specify token ID"),
                                     QLatin1String("id"));
            QCommandLineOption file(QStringList() << QLatin1String("D") << STORAGE_FILE_OPTION << QLatin1String("database"),
                                    QLatin1String("Specify token database <db> to use"),
                                    QLatin1String("db"));
            parser.addVersionOption();
            parser.addHelpOption();
            return /*parser.addOption(type) &&*/ parser.addOption(entry) && parser.addOption(file);
        }
    }
}
