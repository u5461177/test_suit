//
// Created by xhshu on 7/16/18.
//

#ifndef CPP_ELASTICSEARCH_VOLUMEPREDICTOR_H
#define CPP_ELASTICSEARCH_VOLUMEPREDICTOR_H

#include <iostream>
#include "elasticsearch.h"
#include "util/json.h"
#include "util/http.h"

namespace ats {
    class VolumePredictor {
    public:
        static VolumePredictor *get_instance();

    private:
        static VolumePredictor *_instance;

    public:
        VolumePredictor();

        virtual ~VolumePredictor();

    public:
        bool check_exist(const std::string &trade_date, const std::string &stock_code, double &total_predict_vol);

        void get_predict_data(uint64_t input_freq, std::string &trade_date, std::string &stock_code,
                              std::vector<double> &predict_mktvol, std::vector<double> &expect_price_volatility,
                              std::vector<uint64_t> &time_pieces, double &history_price);

        void get_predict_volume(uint64_t input_freq, std::string &trade_date, std::string &stock_code,
                                std::vector<double> &predict_mktvol, std::vector<uint64_t> &time_pieces,
                                double &history_price);
    };
}

#endif //CPP_ELASTICSEARCH_VOLUMEPREDICTOR_H
