#include "otp/generator.h"
#include "app/storageprovider.h"
#include "app/cli.h"
#include "otp/dummy/generator.h"
#include "otp/oath/generator.h"
#include "otp/skey/generator.h"
#include "otp/steam/generator.h"

#include "shared-cli-options.h"

#include <QCoreApplication>
#include <QTimer>
#include <QtDebug>
#include <typeinfo>

#define TR_DOMAIN ("factorkey/samples/lookup-token-sample")

static QCommandLineOption HASH_ALGORITHM_OPTION(QStringList() << QLatin1String("a") << QLatin1String("algorithm"),
                                                QLatin1String("Specify hashing algorithm"),
                                                QLatin1String("hash"));
static QCommandLineOption HOTP_COUNTER_OPTION(QStringList() << QLatin1String("c") << QLatin1String("count"),
                                              QLatin1String("Specify HOTP token counter"),
                                              QLatin1String("count"));
static QCommandLineOption TOTP_EPOCH_OPTION(QStringList() << QLatin1String("e") << QLatin1String("epoch"),
                                            QLatin1String("Specify date and time of the token epoch as an ISO 8601 datetime."),
                                            QLatin1String("datetime"));
static QCommandLineOption SKEY_ENCODING_OPTION(QStringList() << QLatin1String("f") << QLatin1String("format") << QLatin1String("skey-format"),
                                               QLatin1String("Specify format of S/Key token; use hex for raw output."),
                                               QLatin1String("hex|words"));
static QCommandLineOption OATH_TOKEN_LENGTH_OPTION(QStringList() << QLatin1String("l") << QLatin1String("length"),
                                                   QLatin1String("Specify number of digits in the token"),
                                                   QLatin1String("length"));
static QCommandLineOption DUMMY_MESSAGE_OPTION(QStringList() << QLatin1String("m") << QLatin1String("message"),
                                               QLatin1String("Specify the message"),
                                               QLatin1String("message"));
static QCommandLineOption CREATE_TOKEN_OPTION(QStringList() << QLatin1String("n") << QLatin1String("new"),
                                              QLatin1String("Create a new token"),
                                              QLatin1String("type"));
static QCommandLineOption SECRET_OPTION(QStringList() << QLatin1String("p") << QLatin1String("secret") << QLatin1String("password"),
                                        QLatin1String("Specify the secret (password) to use"),
                                        QLatin1String("secret"));
static QCommandLineOption SECRET_BASE32_OPTION(QStringList() << QLatin1String("s") << QLatin1String("secret-encoding"),
                                               QLatin1String("Specify encoding of the token secret; use base32 for Google-Authenticator style tokens"),
                                               QLatin1String("base32|text"));
static QCommandLineOption TOTP_TIMESTEP_OPTION(QStringList() << QLatin1String("t") << QLatin1String("timestep"),
                                               QLatin1String("Specify timestep in milliseconds"),
                                               QLatin1String("msec"));

static bool close_backend(otp::storage::StorageProvider * storageBackend)
{
    storageBackend->close();
    storageBackend->deleteLater();
    return false;
}

static bool tokenSKeyEncoding(otp::skey::generator::SKeyTokenParameters * p,
                              const QString& tokenId,
                              const QCommandLineParser& options)
{
    if(options.isSet(SKEY_ENCODING_OPTION))
    {
        const auto s = options.value(SKEY_ENCODING_OPTION);
        if(s == QLatin1String("hex") && p->setSKeyEncoding(otp::skey::generator::SKeyEncodingType::Hex))
        {
            qDebug() << "Configuring raw/hexadecimal token format for:" << tokenId;
            return true;
        }
        if(s == QLatin1String("words") && p->setSKeyEncoding(otp::skey::generator::SKeyEncodingType::Words))
        {
            qDebug() << "Configuring words token format for:" << tokenId;
            return true;
        }
        qDebug() << "Failed to configure token format for:" << tokenId;
        return false;
    }
    else
    {
        return true;
    }
}

static bool tokenSecretEncoding(otp::generator::TokenParameters * p,
                                const QString& tokenId,
                                const QCommandLineParser& options)
{
    if(options.isSet(SECRET_BASE32_OPTION))
    {
        const auto s = options.value(SECRET_BASE32_OPTION);
        if(s == QLatin1String("base32") && p->setSecretEncodingType(otp::generator::EncodingType::Base32))
        {
            qDebug() << "Configuring base32 encoded secret type for:" << tokenId;
            return true;
        }
        if(s == QLatin1String("text") && p->setSecretEncodingType(otp::generator::EncodingType::Text))
        {
            qDebug() << "Configuring plain text secret type for:" << tokenId;
            return true;
        }
        qDebug() << "Failed to configure secret type for:" << tokenId;
        return false;
    }
    else
    {
        return true;
    }
}

static bool tokenSecretOrExit(otp::generator::TokenParameters * p,
                              const QString& tokenId,
                              const QCommandLineParser& options,
                              const std::function<void(bool, const QString&, otp::generator::TokenParameters *)>& secret_result)
{
    if(options.isSet(SECRET_OPTION))
    {
        const auto s = options.value(SECRET_OPTION);
        if(!s.isNull() && !s.isEmpty() &&
            QObject::connect(p, &otp::generator::TokenParameters::secretUpdated, secret_result) &&
            p->setSecret(s))
        {
            qDebug() << "Configuring secret for:" << tokenId;
            return true;
        }
        else
        {
            qDebug() << "Failed to configure secret for:" << tokenId;
            return false;
        }
    }
    else
    {
        // Defer committing the token until after this function returns.
        QTimer::singleShot(0, [tokenId, p, secret_result]() -> void
        {
            secret_result(true, tokenId, p);
        });
        return true;
    }
}

static bool tokenHash(otp::generator::GenericTokenParameters * p,
                      const QString& tokenId,
                      const QCommandLineParser& options)
{
    if(options.isSet(HASH_ALGORITHM_OPTION))
    {
        const auto s = options.value(HASH_ALGORITHM_OPTION);
        if(!s.isNull() && !s.isEmpty() && p->setHashAlgorithm(s))
        {
            qDebug() << "Configuring hashing algorithm for:" << tokenId;
            return true;
        }
        else
        {
            qDebug() << "Failed to configure hashing algorithm for:" << tokenId;
            return false;
        }
    }
    else
    {
        return true;
    }
}

static bool tokenLength(otp::oath::generator::GenericOTPParameters * p,
                        const QString& tokenId,
                        const QCommandLineParser& options)
{
    if(options.isSet(OATH_TOKEN_LENGTH_OPTION))
    {
        const auto s = options.value(OATH_TOKEN_LENGTH_OPTION);
        if(!s.isNull() && !s.isEmpty())
        {
            bool ok = false;
            uint length = s.toUInt(&ok);
            if(ok && p->setTokenLength(length))
            {
                qDebug() << "Configuring token length for:" << tokenId;
                return true;
            }
        }

        qDebug() << "Failed to configure token length for:" << tokenId;
        return false;
    }
    else
    {
        return true;
    }
}

static bool tokenCounter(otp::oath::generator::HOTPTokenParameters * p,
                         const QString& tokenId,
                         const QCommandLineParser& options)
{
    if(options.isSet(HOTP_COUNTER_OPTION))
    {
        const auto s = options.value(HOTP_COUNTER_OPTION);
        if(!s.isNull() && !s.isEmpty())
        {
            bool ok = false;
            quint64 count = s.toULongLong(&ok);
            if(ok && p->setTokenCounter(count))
            {
                qDebug() << "Configuring token counter for:" << tokenId;
                return true;
            }
        }

        qDebug() << "Failed to configure token counter for:" << tokenId;
        return false;
    }
    else
    {
        return true;
    }
}

static bool tokenTimeStep(otp::oath::generator::TOTPTokenParameters * p,
                          const QString& tokenId,
                          const QCommandLineParser& options)
{
    if(options.isSet(TOTP_TIMESTEP_OPTION))
    {
        const auto s = options.value(TOTP_TIMESTEP_OPTION);
        if(!s.isNull() && !s.isEmpty())
        {
            bool ok = false;
            quint64 msec = s.toULongLong(&ok);
            if(ok && p->setTokenTimeStep(msec))
            {
                qDebug() << "Configuring token timestep for:" << tokenId;
                return true;
            }
        }

        qDebug() << "Failed to configure token timestep for:" << tokenId;
        return false;
    }
    else
    {
        return true;
    }
}

static bool tokenEpoch(otp::oath::generator::TOTPTokenParameters * p,
                       const QString& tokenId,
                       const QCommandLineParser& options)
{
    if(options.isSet(TOTP_EPOCH_OPTION))
    {
        const auto s = options.value(TOTP_EPOCH_OPTION);
        if(!s.isNull() && !s.isEmpty())
        {
            bool ok = false;
            const auto epoch = QDateTime::fromString(s, Qt::ISODate);
            if(ok && p->setTokenEpoch(epoch))
            {
                qDebug() << "Configuring token epoch for:" << tokenId;
                return true;
            }
        }

        qDebug() << "Failed to configure token epoch for:" << tokenId;
        return false;
    }
    else
    {
        return true;
    }
}

static bool tokenMessage(otp::dummy::generator::DummyParameters * p,
                         const QString& tokenId,
                         const QCommandLineParser& options)
{
    if(options.isSet(DUMMY_MESSAGE_OPTION))
    {
        const auto s = options.value(DUMMY_MESSAGE_OPTION);
        if(!s.isNull() && !s.isEmpty() && p->setTokenMessage(s))
        {
            qDebug() << "Configuring token message for:" << tokenId;
            return true;
        }
        else
        {
            qDebug() << "Failed to configure token message for:" << tokenId;
            return false;
        }
    }
    else
    {
        return true;
    }
}

template<typename T>
static inline bool set_parameters(T* p,
                                  const QString& tokenId,
                                  const QCommandLineParser&,
                                  const std::function<void(bool, const QString&, otp::generator::TokenParameters *)>&);

template<>
bool set_parameters(otp::dummy::generator::DummyParameters * p,
                    const QString& tokenId,
                    const QCommandLineParser& options,
                    const std::function<void(bool, const QString&, otp::generator::TokenParameters *)>& secret_result)
{
    return p &&
        tokenMessage(p, tokenId, options) &&
        tokenHash(p, tokenId, options) &&
        tokenSecretEncoding(p, tokenId, options) &&
        tokenSecretOrExit(p, tokenId, options, secret_result);
}

template<>
bool set_parameters(otp::steam::generator::SteamGuardParameters * p,
                    const QString& tokenId,
                    const QCommandLineParser& options,
                    const std::function<void(bool, const QString&, otp::generator::TokenParameters *)>& secret_result)
{
    return p &&
        tokenSecretEncoding(p, tokenId, options) &&
        tokenSecretOrExit(p, tokenId, options, secret_result);
}

template<>
bool set_parameters(otp::skey::generator::SKeyTokenParameters * p,
                    const QString& tokenId,
                    const QCommandLineParser& options,
                    const std::function<void(bool, const QString&, otp::generator::TokenParameters *)>& secret_result)
{
    return p &&
        tokenSKeyEncoding(p, tokenId, options) &&
        tokenSecretEncoding(p, tokenId, options) &&
        tokenSecretOrExit(p, tokenId, options, secret_result);
}

template<>
bool set_parameters(otp::oath::generator::TOTPTokenParameters * p,
                    const QString& tokenId,
                    const QCommandLineParser& options,
                    const std::function<void(bool, const QString&, otp::generator::TokenParameters *)>& secret_result)
{
    return p &&
        tokenEpoch(p, tokenId, options) &&
        tokenTimeStep(p, tokenId, options) &&
        tokenLength(p, tokenId, options) &&
        tokenHash(p, tokenId, options) &&
        tokenSecretEncoding(p, tokenId, options) &&
        tokenSecretOrExit(p, tokenId, options, secret_result);
}

template<>
bool set_parameters(otp::oath::generator::HOTPTokenParameters * p,
                    const QString& tokenId,
                    const QCommandLineParser& options,
                    const std::function<void(bool, const QString&, otp::generator::TokenParameters *)>& secret_result)
{
    return p &&
        tokenCounter(p, tokenId, options) &&
        tokenLength(p, tokenId, options) &&
        tokenHash(p, tokenId, options) &&
        tokenSecretEncoding(p, tokenId, options) &&
        tokenSecretOrExit(p, tokenId, options, secret_result);
}

static otp::storage::StorageProvider * openStorage(const QCommandLineParser& options, QObject * owner)
{
    otp::app::storage::StorageBuilder storageConfig;

    const auto dbFile = options.value(samples::cli::STORAGE_FILE_OPTION);
    if(!dbFile.isNull() && !dbFile.isEmpty())
    {
        storageConfig.setDbFile(dbFile);
    }

    storageConfig.setSecretsDomain(QLatin1String("lookup-token-sample-domain"));
    storageConfig.setParent(owner);

    return storageConfig.build();
}

static bool edit_token(const QString& tokenId,
                       otp::storage::Storage * storage,
                       QObject * owner,
                       const QCommandLineParser& options,
                       const std::function<void(bool, const QString&, otp::generator::TokenParameters *)>& secret_result)
{
    switch(storage->type())
    {
        case otp::storage::OTPTokenType::HOTP:
            return set_parameters(otp::oath::generator::HOTPTokenParameters::from(storage, owner),
                                  tokenId,
                                  options,
                                  secret_result);
        case otp::storage::OTPTokenType::TOTP:
            return set_parameters(otp::oath::generator::TOTPTokenParameters::from(storage, owner),
                                  tokenId,
                                  options,
                                  secret_result);
        case otp::storage::OTPTokenType::SKey:
            return set_parameters(otp::skey::generator::SKeyTokenParameters::from(storage, owner),
                                  tokenId,
                                  options,
                                  secret_result);
        case otp::storage::OTPTokenType::SteamGuard:
            return set_parameters(otp::steam::generator::SteamGuardParameters::from(storage, owner),
                                  tokenId,
                                  options,
                                  secret_result);
        case otp::storage::OTPTokenType::DummyHMAC:
            return set_parameters(otp::dummy::generator::DummyParameters::from(storage, owner),
                                  tokenId,
                                  options,
                                  secret_result);
        default:
            Q_ASSERT_X(false, Q_FUNC_INFO, "Unsupported token type!");
            return false;
    }
}


static bool edit_token(const QString& tokenId,
                       otp::storage::StorageProvider * storageBackend,
                       QObject * owner,
                       const QCommandLineParser& options, const std::function<void(bool, const QString&, otp::generator::TokenParameters *)>& secret_result)
{
    auto storage = storageBackend->lookup(tokenId);
    if(storage)
    {
        return edit_token(tokenId, storage, owner, options, secret_result);
    }
    else
    {
        qDebug() << "No such token:" << tokenId;
        return close_backend(storageBackend);
    }
}

static bool create_token(const QString& tokenId,
                         otp::storage::StorageProvider * storageBackend,
                         QObject * owner,
                         const QCommandLineParser& options, const std::function<void(bool, const QString&, otp::generator::TokenParameters *)>& secret_result)
{
    bool ok = false, validType = false;
    const QString tokenType = options.value(CREATE_TOKEN_OPTION);

    if(tokenType == QLatin1String("hotp"))
    {
        validType = true;
        ok = set_parameters(otp::oath::generator::HOTPTokenParameters::create(tokenId, storageBackend, owner),
                            tokenId,
                            options,
                            secret_result);
    }

    if(tokenType == QLatin1String("totp"))
    {
        validType = true;
        ok = set_parameters(otp::oath::generator::TOTPTokenParameters::create(tokenId, storageBackend, owner),
                            tokenId,
                            options,
                            secret_result);
    }

    if(tokenType == QLatin1String("skey"))
    {
        validType = true;
        ok = set_parameters(otp::skey::generator::SKeyTokenParameters::create(tokenId, storageBackend, owner),
                            tokenId,
                            options,
                            secret_result);
    }

    if(tokenType == QLatin1String("steam"))
    {
        validType = true;
        ok = set_parameters(otp::steam::generator::SteamGuardParameters::create(tokenId, storageBackend, owner),
                            tokenId,
                            options,
                            secret_result);
    }

    if(tokenType == QLatin1String("dummy"))
    {
        validType = true;
        ok = set_parameters(otp::dummy::generator::DummyParameters::create(tokenId, storageBackend, owner),
                            tokenId,
                            options,
                            secret_result);
    }

    if(!ok)
    {
        if(validType)
        {
            qDebug() << "Unable to configure token:" << tokenId;
        }
        else
        {
            qDebug() << "Invalid token type for token:" << tokenId;
        }
        return close_backend(storageBackend);
    }

    return ok;
}

static bool accept_options(QObject * owner,
                           const QCommandLineParser& options,
                           const std::function<void(bool, const QString&, otp::generator::TokenParameters *)>& secret_result)
{
    const QString tokenId = options.value(samples::cli::ENTRY_ID_OPTION);
    if(tokenId.isNull() || tokenId.isEmpty())
    {
        qDebug() << "Token id is required!";
        return false;
    }

    auto storageBackend = openStorage(options, owner);
    if(!storageBackend)
    {
        qDebug() << "Failed to open storage backend";
        return false;
    }

    if(!storageBackend->open())
    {
        storageBackend->deleteLater();
        qDebug() << "Failed to open storage backend";
        return false;
    }

    return options.isSet(CREATE_TOKEN_OPTION) ?
        create_token(tokenId, storageBackend, owner, options, secret_result) :
        edit_token(tokenId, storageBackend, owner, options, secret_result);
}

void run(const QCommandLineParser& options, int& result)
{
    QObject * owner = new QObject();

    bool cleanup_prepared = QObject::connect(owner, &QObject::destroyed, [&result](QObject *) -> void
    {
        QCoreApplication::exit(result);
    });

    bool triggered = cleanup_prepared && accept_options(owner, options, [&result, owner](bool ok, const QString& tokenId, otp::generator::TokenParameters * p) -> void
    {
        if(ok)
        {
            if(p->commit())
            {
                qDebug() << "Sucessfully configured your token:" << tokenId;
                result = 0;
            }
            else
            {
                qDebug() << "Failed to commit configuration for:" << tokenId;
                result = 1;
            }
        }
        else
        {
            qDebug() << "Failed to configure token secret for:" << tokenId;
            result = 1;
        }
        owner->deleteLater();
    });

    if(!triggered)
    {
        qDebug() << "Failed to configure your token";
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
        cli.setApplicationDescription(QCoreApplication::translate(TR_DOMAIN, "Configure parameters of one time password (OTP) tokens"));

        return cli.addOption(HASH_ALGORITHM_OPTION) &&
        cli.addOption(HOTP_COUNTER_OPTION) &&
        cli.addOption(TOTP_EPOCH_OPTION) &&
        cli.addOption(SKEY_ENCODING_OPTION) &&
        cli.addOption(OATH_TOKEN_LENGTH_OPTION) &&
        cli.addOption(DUMMY_MESSAGE_OPTION) &&
        cli.addOption(CREATE_TOKEN_OPTION) &&
        cli.addOption(SECRET_OPTION) &&
        cli.addOption(SECRET_BASE32_OPTION) &&
        cli.addOption(TOTP_TIMESTEP_OPTION) &&
        samples::cli::addSharedOptions(cli);
    });

    return otp::app::cli::skeleton_main(argc, argv, options, run, metadata);
}
