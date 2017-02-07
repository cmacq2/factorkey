#include "cli.h"

#include <QTimer>

#include "debugger.h"

namespace otp
{
    namespace app
    {
        namespace cli
        {
            static int internal_skeleton_main(QCoreApplication& app, const RegisterCommandLineOptions& options, const Application& appLogic)
            {
                int result = -1;
                QCommandLineParser cli;

                if(options(cli))
                {
                    QTimer::singleShot(0, [&app, &cli, &result, &appLogic]() -> void
                    {
                        cli.process(app);
                        appLogic(cli, result);
                    });
                    return QCoreApplication::exec();
                }
                else
                {
                    return result;
                }
            }

            int skeleton_main(int argc, char* argv[], const RegisterCommandLineOptions& options, const Application& appLogic)
            {
                if(otp::app::secureAgainstDebugger())
                {
                    QCoreApplication app(argc, argv);
                    return internal_skeleton_main(app, options, appLogic);
                }
                else
                {
                    return -2;
                }
            }

            int skeleton_main(int argc, char* argv[], const RegisterCommandLineOptions& options, const Application& appLogic, const ApplicationDecorator& metadata)
            {
                if(otp::app::secureAgainstDebugger())
                {
                    QCoreApplication app(argc, argv);
                    metadata(app);
                    return internal_skeleton_main(app, options, appLogic);
                }
                else
                {
                    return -2;
                }
            }
        }
    }
}
