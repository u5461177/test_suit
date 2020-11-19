//
// Created by Rong Zhou on 2018-08-03.
//

#include "cryptohelper.h"
#include <chrono>
#include <random>
#include <cstring>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>


namespace ats {

    using namespace std;
    using namespace std::chrono;

    string generate_random_text(size_t size) {
        static const char charset[]{"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
        static uniform_int_distribution<> selector(0, 35);
        auto seed = high_resolution_clock::now().time_since_epoch().count();
        static mt19937 generator((unsigned) seed);
        string text;
        for (size_t i = 0; i < size; i++) {
            text += charset[selector(generator)];
        }
        return text;
    }

    string base64_encode(const string &input) {
        BIO *b64 = BIO_new(BIO_f_base64());
        BIO *bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
        BIO_write(bio, input.c_str(), (int) input.size());
        BIO_flush(bio);
        BUF_MEM *mem;
        BIO_get_mem_ptr(bio, &mem);
        BIO_set_close(bio, BIO_NOCLOSE);
        string output(mem->data, mem->length);
        free(mem->data);
        free(mem);
        BIO_free_all(bio);
        return output;
    }


    string base64_decode(const char * input) {
        BIO *b64 = NULL;
        BIO *bio = NULL;
        int in_length = (int)strlen(input);
        b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        bio = BIO_new_mem_buf(input, in_length);
        bio = BIO_push(b64, bio);
        char *output = new char[in_length+1];
        int out_length = BIO_read(bio, output, in_length);
        string out_str(output,output+strlen(output));
        BIO_free_all(bio);
        BIO_free_all(b64);
        free(output);
        return out_str;
    }


    vector<byte> base64_encode(const vector<byte> &input) {
        BIO *b64 = BIO_new(BIO_f_base64());
        BIO *bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);
        BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
        BIO_write(bio, &input.front(), (int) input.size());
        BIO_flush(bio);
        BUF_MEM *mem;
        BIO_get_mem_ptr(bio, &mem);
        BIO_set_close(bio, BIO_NOCLOSE);
        vector<byte> output(mem->length);
        memcpy(&output.front(), mem->data, mem->length);
        free(mem->data);
        free(mem);
        BIO_free_all(bio);
        return output;
    }

    string hmac_encode(const std::string &input) {
        constexpr char key[]{"zts.com.cn"};
        auto digest = HMAC(EVP_sha1(), key, (int) strlen(key), (unsigned char *) input.c_str(), input.size(), nullptr,
                           nullptr);
        char buffer[SHA_DIGEST_LENGTH * 2 + 1];
        for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
            sprintf(&buffer[i * 2], "%02x", (unsigned int) digest[i]);
        }
        return string(buffer);
    }

    vector<byte> hmac_encode(const vector<byte> &input) {
        constexpr char key[]{"zts.com.cn"};
        auto digest = HMAC(EVP_sha1(), key, (int) strlen(key), (unsigned char *) &input.front(), input.size(), nullptr,
                           nullptr);
        vector<byte> output(SHA_DIGEST_LENGTH * 2 + 1);
        for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
            sprintf((char *) &output[i * 2], "%02x", (unsigned int) digest[i]);
        }
        output.pop_back();
        return output;
    }

}