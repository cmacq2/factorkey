#include "otp/generator.h"
#include "otp/skey/generator.h"
#include "app/storageprovider.h"
#include "app/cli.h"

#include "shared-cli-options.h"

#include <QCoreApplication>
#include <QTimer>
#include <QtDebug>

#define TR_DOMAIN ("factorkey/samples/lookup-token-sample")

static const QCommandLineOption SKEY_CHALLENGE_OPTION(QStringList() << QLatin1String("c") << QLatin1String("challenge"),
                                                      QLatin1String("Specify S/Key challenge to respond to."),
                                                      QLatin1String("challenge"));
static const QCommandLineOption LIST_ENTRIES_OPTION(QStringList() << QLatin1String("l") << QLatin1String("list"),
                                                    QLatin1String("List available token entries (IDs)"));

static bool close_backend(otp::storage::StorageProvider * storageBackend)
{
    storageBackend->close();
    storageBackend->deleteLater();
    return false;
}

static bool tokenChallenge(otp::skey::generator::SKeyTokenParameters * p, const QString& tokenId, const QCommandLineParser& options)
{
    if(p)
    {
        const auto s = options.value(SKEY_CHALLENGE_OPTION);
        if(!s.isNull() && !s.isEmpty() && p->setChallenge(s))
        {
            return true;
        }
        else
        {
            qDebug() << "Failed to determine the challenge for S/Key token:" << tokenId;
            return false;
        }
    }
    else
    {
        return true;
    }
}

static bool list_tokens(QObject * owner, otp::storage::StorageProvider * storageBackend, int& result)
{
    QStringList ls;
    if(storageBackend->entries(ls))
    {
        for(const auto& e: ls)
        {
            qDebug() << e;
        }
        result = 0;
    }
    else
    {
        qDebug() << "Failed to list (all) tokens";
        result = 1;
    }

    close_backend(storageBackend);
    owner->deleteLater();
    return true;
}

static bool generate_token(QObject * owner, otp::storage::StorageProvider * storageBackend,
                           const QCommandLineParser& options,
                           const otp::generator::TokenGenerator::TokenResult& receive_token_result)
{
    const QString tokenId = options.value(samples::cli::ENTRY_ID_OPTION);

    if(tokenId.isNull() || tokenId.isEmpty())
    {
        qDebug() << "Token id is required!";
        return false;
    }

    otp::storage::Storage * storageItem = storageBackend->lookup(tokenId);
    if(!storageItem)
    {
        return close_backend(storageBackend);
    }

    otp::generator::TokenParameters * tokenParameters = otp::generator::TokenParameters::create(storageItem, owner);
    if(!tokenParameters || !tokenChallenge(qobject_cast<otp::skey::generator::SKeyTokenParameters *>(tokenParameters), tokenId, options))
    {
        storageItem->deleteLater();
        return close_backend(storageBackend);
    }

    otp::generator::TokenGenerator * tokenGenerator = tokenParameters->generator(owner);
    if(!tokenGenerator)
    {
        tokenParameters->deleteLater();
        storageItem->deleteLater();
        return close_backend(storageBackend);
    }

    bool ok = QObject::connect(tokenGenerator, &otp::generator::TokenGenerator::tokenGenerated, receive_token_result);
    if(!ok)
    {
        tokenGenerator->deleteLater();
        tokenParameters->deleteLater();
        storageItem->deleteLater();
        return close_backend(storageBackend);
    }

    tokenGenerator->generateToken();
    return ok;
}

static bool accept_options(QObject * owner, const QCommandLineParser& options, int& result)
{
    otp::app::storage::StorageBuilder storageConfig;

    const auto dbFile = options.value(samples::cli::STORAGE_FILE_OPTION);
    if(!dbFile.isNull() && !dbFile.isEmpty())
    {
        storageConfig.setDbFile(dbFile);
    }
    storageConfig.setSecretsDomain(QLatin1String("lookup-token-sample-domain"));
    storageConfig.setParent(owner);

    otp::storage::StorageProvider * storageBackend = storageConfig.build();
    if(!storageBackend)
    {
        return false;
    }

    if(!storageBackend->open())
    {
        storageBackend->deleteLater();
        return false;
    }

    bool ok;
    if (options.isSet(LIST_ENTRIES_OPTION))
    {
        ok = list_tokens(owner, storageBackend, result);
        if(!ok)
        {
            qDebug() << "Failed to list your tokens";
        }
    }
    else
    {
        ok = generate_token(owner, storageBackend, options, [&result, owner](bool ok, const QString& token) -> void
        {
            if(ok)
            {
                qDebug() << "Current token value:" << token;
                result = 0;
            }
            else
            {
                qDebug() << "Failed to generate your token";
                result = 1;
            }
            owner->deleteLater();
        });
        if(!ok)
        {
            qDebug() << "Failed to generate your token";
        }
    }

    return ok;
}

void run(const QCommandLineParser& options, int& result)
{
    QObject * owner = new QObject();

    bool cleanup_prepared = QObject::connect(owner, &QObject::destroyed, [&result](QObject *) -> void
    {
        QCoreApplication::exit(result);
    });

    bool triggered = cleanup_prepared && accept_options(owner, options, result);

    if(!triggered)
    {
        if(cleanup_prepared)
        {
            owner->deleteLater();
        }
        else
        {
            delete owner;
            QCoreApplication::exit(result);
        }
    }
}

int main(int argc, char** argv)
{
    const otp::app::cli::ApplicationDecorator metadata([](QCoreApplication& app) -> void
    {
        app.setApplicationVersion(QLatin1String("0.1"));
    });
    const otp::app::cli::RegisterCommandLineOptions options([](QCommandLineParser& cli) -> bool
    {
        cli.setApplicationDescription(QCoreApplication::translate(TR_DOMAIN, "Generate one time password (OTP) tokens"));
        return cli.addOption(SKEY_CHALLENGE_OPTION) && cli.addOption(LIST_ENTRIES_OPTION) && samples::cli::addSharedOptions(cli);
    });

    return otp::app::cli::skeleton_main(argc, argv, options, run, metadata);
}
