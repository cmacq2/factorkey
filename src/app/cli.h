#ifndef FACTORKEY_APP_CLI_H
#define FACTORKEY_APP_CLI_H

#include <functional>

#include <QCommandLineParser>
#include <QCoreApplication>


namespace otp
{
    namespace app
    {
        namespace cli
        {
            typedef std::function<bool(QCommandLineParser&)> RegisterCommandLineOptions;
            typedef std::function<void(QCoreApplication&)> ApplicationDecorator;
            typedef std::function<void(QCommandLineParser&, int&)> Application;

            int skeleton_main(int argc, char* argv[], const RegisterCommandLineOptions& options, const Application& appLogic);
            int skeleton_main(int argc, char* argv[], const RegisterCommandLineOptions& options, const Application& appLogic, const ApplicationDecorator& metadata);
        }
    }
}
#endif
