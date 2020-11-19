//
// Created by xhshu on 7/16/18.
//

#include "volumepredictor.h"
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include "configuration.h"
#include "glog/logging.h"

using namespace std;

namespace ats {
    VolumePredictor *VolumePredictor::get_instance() {
        return _instance;
    }

    VolumePredictor *VolumePredictor::_instance = nullptr;

    VolumePredictor::VolumePredictor() {
        _instance = this;
    }

    VolumePredictor::~VolumePredictor() {
        _instance = nullptr;
    }

    bool VolumePredictor::check_exist(const string &trade_date, const string &stock_code, double &total_predict_vol) {
        auto info = Configuration::instance().get_elastic_search_info();
        auto server_ip = info.first;
        auto server_port = info.second;
        ElasticSearch es(server_ip + ":" + std::to_string(server_port));
        // Get document.
        Json::Object jResult;
        if (!es.getDocument(trade_date.c_str(), "predict_vol", stock_code.c_str(), jResult)) {
            LOG(ERROR) << "Failed to get document. trade date is: " << trade_date << ", stock code is: " << stock_code;
            return false;
        } else {
            auto result = jResult["_source"].getObject();
            auto close_price = result["close_price"].getDouble();
            auto vol_result = result["freq_1"].getObject()["predict_volume"].getArray();
            for (const auto &vol:vol_result) {
                total_predict_vol = total_predict_vol + vol.getDouble() * close_price;
            }
            return true;
        }
    }

    void VolumePredictor::get_predict_data(uint64_t input_freq, string &trade_date, string &stock_code,
                                           vector<double> &predict_mktvol, vector<double> &expect_price_volatility,
                                           vector<uint64_t> &time_pieces, double &history_price) {
        auto info = Configuration::instance().get_elastic_search_info();
        auto server_ip = info.first;
        auto server_port = info.second;
        ElasticSearch es(server_ip + ":" + std::to_string(server_port));
        // Get document
        Json::Object jResult;
        Json::Array time_list;
        Json::Array predict_vol;
        Json::Array price_volatility;
        if (!es.getDocument(trade_date.c_str(), "predict_vol", stock_code.c_str(), jResult))
            LOG(ERROR) << "Failed to get document. trade date is: " << trade_date << ", stock code is: " << stock_code;
        auto result = jResult["_source"].getObject();
        if (input_freq == 1) {
            time_list = result["freq_1"].getObject()["time"].getArray();
            predict_vol = result["freq_1"].getObject()["predict_volume"].getArray();
            price_volatility = result["freq_1"].getObject()["price_volatility"].getArray();
            cout << "size of time list: " << time_list.size() << endl;
            assert(time_list.size() == 240);
            assert(predict_vol.size() == 240);
            assert(price_volatility.size() == 240);
        } else {
            LOG(ERROR) << "frequent parameter error";
        }
        history_price = result["close_price"].getDouble();
        for (const auto &mktvol:predict_vol) {
            predict_mktvol.push_back(mktvol.getDouble());
        }
        for (const auto &volatility:price_volatility) {
            expect_price_volatility.push_back(volatility.getDouble());
        }
        for (const auto &time:time_list) {
            time_pieces.push_back(time.getUnsignedInt());
        }
    }

    void VolumePredictor::get_predict_volume(uint64_t input_freq, string &trade_date, string &stock_code,
                                             vector<double> &predict_mktvol, vector<uint64_t> &time_pieces,
                                             double &history_price) {
        ElasticSearch es("10.25.24.184:9200");
        // Get document
        Json::Object jResult;
        Json::Array time_list;
        Json::Array predict_vol;
        Json::Array price_volatility;
        if (!es.getDocument(trade_date.c_str(), "predict_vol", stock_code.c_str(), jResult))
            LOG(ERROR) << "Failed to get document. trade date is: " << trade_date << ", stock code is: " << stock_code;
        auto result = jResult["_source"].getObject();
        if (input_freq == 1) {
            time_list = result["freq_1"].getObject()["time"].getArray();
            predict_vol = result["freq_1"].getObject()["predict_volume"].getArray();
            assert(time_list.size() == 240);
            assert(predict_vol.size() == 240);
        } else {
            LOG(ERROR) << "frequent parameter error";
        }
        history_price = result["close_price"].getDouble();
        for (const auto &mktvol:predict_vol) {
            predict_mktvol.push_back(mktvol.getDouble());
        }
        for (const auto &time:time_list) {
            time_pieces.push_back(time.getUnsignedInt());
        }
    }

}
