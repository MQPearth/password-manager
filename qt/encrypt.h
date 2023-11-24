#pragma once
#include <qstring.h>
#include <cryptopp/aes.h>

using namespace CryptoPP;


class encrypt_util
{
public:

	static QString md5(const QString& input);

	static void calc_key_iv(const QString& keyText,
		CryptoPP::byte(&key)[AES::BLOCKSIZE], CryptoPP::byte(&iv)[AES::BLOCKSIZE]);

	static QString encrypt(const QString& plainText, const QString& key);

	static QString decrypt(const QString& cipherText, const QString& key);
};

