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
//             class SanityTestMetadataStorageHandler: public otp::storage::db::MetadataStorageHandler
//             {
//             public:
//                 SanityTestMetadataStorageHandler(const otp::storage::db::MetadataStorageHandler& uut);
//                 virtual ~SanityTestMetadataStorageHandler();
//
//                 bool isParamNameValid(const QString & p) const;
//                 QString tableForParam(const QString& p) const;
//                 QString columnToParam(const QString& c) const;
//                 const QStringList keys(void) const;
//                 QSet<QString> columnsInTable(const QString& t) const;
//             private:
//                 const otp::storage::db::MetadataStorageHandler m_uut;
//             };

            class MetadataStorageHandlerSanityTest: public QObject
            {
                Q_OBJECT
            protected:
                void sanity(void);
                void sanity_data(void);
                void basicSanity(void);
            public:
                MetadataStorageHandlerSanityTest(const otp::storage::db::MetadataStorageHandler* uut, QObject * parent = 0);
                virtual ~MetadataStorageHandlerSanityTest();
            private:
                bool checkTableName(const QString& table);
                bool checkParamName(const QString& param);
                bool checkColumnName(const QString& column);
            private:
                const otp::storage::db::MetadataStorageHandler* m_uut;
//                 const SanityTestMetadataStorageHandler m_uut;
            };
        }
    }
}

#endif
