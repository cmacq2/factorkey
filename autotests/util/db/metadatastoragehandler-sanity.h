#ifndef FACTORKEY_AUTOTESTS_SANITY_CHECKS_METADATASTORAGEHANDLER_H
#define FACTORKEY_AUTOTESTS_SANITY_CHECKS_METADATASTORAGEHANDLER_H

#include "otp/db/metadatastoragehandler.h"
#include <QObject>

namespace sanity
{
    namespace storage
    {
        namespace db
        {
            class MetadataStorageHandlerSanityTest: public QObject
            {
                Q_OBJECT
            protected:
                void sanity(void);
                void sanity_data(void);
                void basicSanity(void);
            public:
                MetadataStorageHandlerSanityTest(const QSharedPointer<otp::storage::db::MetadataStorageHandler> uut, QObject * parent = 0);
                virtual ~MetadataStorageHandlerSanityTest();
            private:
                bool checkTableName(const QString& table);
                bool checkParamName(const QString& param);
                bool checkColumnName(const QString& column);
            private:
                const QSharedPointer<otp::storage::db::MetadataStorageHandler> m_uut;
            };
        }
    }
}

#endif
