/**
 * @file crypt.cpp
 * @author Krisna Pranav
 * @brief crypt
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/base64.h>
#include <mods/types.h>
#include <libcrypto/hash/sha2.h>
#include <crypt.h>
#include <errno.h>
#include <string.h>

extern "C" {

static struct crypt_data crypt_data;

/**
 * @param key 
 * @param salt 
 * @return char* 
 */
char* crypt(char const* key, char const* salt)
{
    crypt_data.initialized = true;
    return crypt_r(key, salt, &crypt_data);
}

static constexpr size_t crypt_salt_max = 16;
static constexpr size_t sha_string_length = 44;

/**
 * @param key 
 * @param salt 
 * @param data 
 * @return char* 
 */
char* crypt_r(char const* key, char const* salt, struct crypt_data* data)
{
    if (!data->initialized) {
        errno = EINVAL;
        return nullptr;
    }

    if (salt[0] == '$') {
        if (salt[1] == '5') {
            char const* salt_value = salt + 3;
            size_t salt_len = min(strcspn(salt_value, "$"), crypt_salt_max);
            size_t header_len = salt_len + 3;

            bool fits = String(salt, header_len).copy_characters_to_buffer(data->result, sizeof(data->result));
            if (!fits) {
                errno = EINVAL;
                return nullptr;
            }
            data->result[header_len] = '$';

            Crypto::Hash::SHA256 sha;
            sha.update(key);
            sha.update((u8 const*)salt_value, salt_len);

            auto digest = sha.digest();
            auto string = encode_base64(ReadonlyBytes(digest.immutable_data(), digest.data_length()));

            fits = string.copy_characters_to_buffer(data->result + header_len + 1, sizeof(data->result) - header_len - 1);
            if (!fits) {
                errno = EINVAL;
                return nullptr;
            }

            return data->result;
        }
    }

    errno = EINVAL;
    return nullptr;
}

}
