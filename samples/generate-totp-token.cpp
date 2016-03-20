#include "otp/token.h"

#include <QString>
#include <QtDebug>

/**
 * Example to generate a TOTP token using default settings.
 * Usage:
 *   $ generate-totp-token-sample mysecretkey
 *
 * See otp/token.h for more advanced options and other token types.
 *
 * @param argc number of arguments passed to main in `argv'
 * @param argv command line arguments passed to the sample app.
 * @return 0 (success)
 */
int main(int argc, const char** argv)
{
    QString secret(QLatin1String("Hello, world!"));

    if(argc > 1)
    {
        // secret = QString::fromUtf8(argv[1]);
        secret = QString::fromLocal8Bit(argv[1]);
    }

    const otp::Algorithm& algorithm = otp::hmacAlgorithm(); /* use default hashing algorithm */
    const otp::Key& keyEncoder = otp::createKey(); /* use default text codec for encoding the key */
    const otp::Message& message = otp::totpMessage(); /* use default epoch and default time step */
    const otp::Encoder& encoder = otp::otpEncoder(); /* use default length and default locale */

    qDebug() << "Token is:" << otp::token(secret, message, keyEncoder, algorithm, encoder);
    return 0;
}