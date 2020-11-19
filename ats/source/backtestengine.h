//
// Created by Rong Zhou on 2018-08-09.
//

#ifndef ATS_BACKTESTENGINE_H
#define ATS_BACKTESTENGINE_H

#include "singleton.h"
#include <vector>
#include <map>

namespace ats {

    class BackTestEngine : public Singleton<BackTestEngine> {
        using Order = std::pair<uint32_t, uint32_t>;
        using OrderQueue = std::vector<Order>;

    public:
        BackTestEngine() = default;

        ~BackTestEngine() final = default;

    public:
        bool load();

    private:
        std::map<float, OrderQueue> _ask_book;
        std::map<float, OrderQueue> _bid_book;
    };

}

#endif //ATS_BACKTESTENGINE_H
