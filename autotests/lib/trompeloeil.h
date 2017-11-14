#ifndef FACTORKEY_AUTOTEST_LIBS_TROMPE_L_OEIL_H
#define FACTORKEY_AUTOTEST_LIBS_TROMPE_L_OEIL_H
#include "trompeloeil/include/trompeloeil.hpp"

#include <functional>
#include <QByteArray>
#include <QException>

namespace autotests
{
    namespace integration
    {
        class ExpectationViolation
        {
        public:
            ExpectationViolation(trompeloeil::severity severity, char const *file, unsigned long line, std::string const &msg);
            QByteArray report(void) const;
        private:
            trompeloeil::severity m_severity;
            const char * m_file;
            unsigned long m_line;
            const std::string m_msg;
        };

        class ExpectationViolationException: public QException
        {
        public:
            ExpectationViolationException(const ExpectationViolation& v);
            void raise() const;
            ExpectationViolationException * clone() const;
            QByteArray report(void) const;
        private:
            const ExpectationViolation m_violation;
        };

        typedef std::function<void(trompeloeil::severity severity, char const *file, unsigned long line, std::string const &msg)> ViolationsReporter;

        ViolationsReporter setup(std::vector<ExpectationViolation>& violations);

        bool verifyExpectations(const std::vector<ExpectationViolation>& violations);
    }
}
#endif
