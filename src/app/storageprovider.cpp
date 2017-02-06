#include "storageprovider.h"

#include "config.h"
#include "otp/storage/storageprovider.h"
#include "otp/db/metadb.h"
#include "otp/db/storageprovider.h"

#if defined(FACTORKEY_WITH_KWALLET) && FACTORKEY_WITH_KWALLET
#include "wallet/secrets.h"
#endif

#if defined(FACTORKEY_WITH_QT_KEYCHAIN) && FACTORKEY_WITH_QT_KEYCHAIN
#include "keychain/secrets.h"
#endif

#if defined(FACTORKEY_WITH_QT_KEYCHAIN) && FACTORKEY_WITH_QT_KEYCHAIN
#define USE_QT_KEYCHAIN true
#else
#define USE_QT_KEYCHAIN false
#endif
#if defined(FACTORKEY_WITH_KWALLET) && FACTORKEY_WITH_KWALLET
#define USE_KWALLET true
#else
#define USE_KWALLET false
#endif

namespace otp
{
    namespace app
    {
        namespace storage
        {
            static bool useQtKeychain = USE_QT_KEYCHAIN;
            static bool useKwallet = USE_KWALLET;

            StorageBuilder::StorageBuilder() : m_dirSet(false), m_parent(nullptr) {}

            otp::storage::StorageProvider * StorageBuilder::build(void) const
            {
                QSharedPointer<otp::storage::db::MetadataFileDbBuilder> db(new otp::storage::db::MetadataFileDbBuilder());

                if(m_dirSet)
                {
                   db->setDataDirectory(m_dir);
                }
                else
                {
                    if(!db->setDataDirectory())
                    {
                        return nullptr;
                    }
                }

                db->setConnectionName(m_dbConnectionName);
                db->setDataFile(m_dbName);

                #if defined(FACTORKEY_WITH_QT_KEYCHAIN) && FACTORKEY_WITH_QT_KEYCHAIN
                if(useQtKeychain)
                {
                    QSharedPointer<otp::secrets::keychain::KeychainBuilder> secrets(new otp::secrets::keychain::KeychainBuilder());
                    secrets->setName(m_secretsDomain);
                    return new otp::storage::db::DbStorageProvider(db, secrets, m_parent);
                }
                #endif

                #if defined(FACTORKEY_WITH_KWALLET) && FACTORKEY_WITH_KWALLET
                if(useKwallet)
                {
                    QSharedPointer<otp::secrets::wallet::WalletBuilder> secrets(new otp::secrets::wallet::WalletBuilder());
                    secrets->setName(m_secretsDomain);
                    return new otp::storage::db::DbStorageProvider(db, secrets, m_parent);
                }
                #endif
                return nullptr;
            }

            void StorageBuilder::setSecretsDomain(const QString& domain)
            {
                m_secretsDomain = domain;
            }

            void StorageBuilder::setDbDirectory(const QDir& dir)
            {
                m_dir = dir;
                m_dirSet = true;
            }

            void StorageBuilder::setDbConnectionName(const QString& connectionName)
            {
                m_dbConnectionName = connectionName;
            }

            void StorageBuilder::setDbName(const QString& name)
            {
                m_dbName = name;
            }

            void StorageBuilder::setDbFile(const QFileInfo& dbFile)
            {
                setDbDirectory(dbFile.dir());
                setDbName(dbFile.fileName());
            }

            void StorageBuilder::setDbFile(const QString& dbFile)
            {
                setDbFile(QFileInfo(dbFile));
            }

            void StorageBuilder::setParent(QObject * parent)
            {
                m_parent = parent;
            }
        }
    }
}
