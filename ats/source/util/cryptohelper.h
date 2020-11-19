//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_CRYPTOHELPER_H
#define ATS_CRYPTOHELPER_H

#include <string>
#include <vector>
#include <cstddef>

namespace ats {

    std::string generate_random_text(size_t size = 32);

    std::string base64_encode(const std::string &input);

    std::vector<std::byte> base64_encode(const std::vector<std::byte> &input);

    std::string hmac_encode(const std::string &input);

    std::vector<std::byte> hmac_encode(const std::vector<std::byte> &input);


    std::string base64_decode(const char * input);


}

#endif //ATS_CRYPTOHELPER_H
