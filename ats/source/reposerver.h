// 
// Created by Ruiqi Zhang at 2019-06-06
//

#ifndef ATS_REPOSERVER_H
#define ATS_REPOSERVER_H

#include "singleton.h"
#include "types/markettype.h"
#include <map>
#include "glog/logging.h"
#include <cmath>
#include <shared_mutex>

namespace ats {

	using Ticker = std::pair<std::string, MarketType>;

	class RepoServer : public Singleton<RepoServer> 
	{
		private:
			std::map<std::string, int> _n_quote;
			std::map<std::string, double> _price_sum;
			std::map<std::string, double> _price_square_sum;
			std::map<std::string, double> _repo_mean;
			std::map<std::string, double> _repo_std;
		public:
			RepoServer() = default;
			~RepoServer() final = default;

			double get_repo_mean(const std::string& ticker);

			double get_repo_std(const std::string& ticker);

			bool update_repo_data(std::string ticker, double price);
	};

	const std::vector<Ticker> repo_ticker = {
		std::make_pair("204001", MarketType::SH),
		std::make_pair("204002", MarketType::SH),
        std::make_pair("204003", MarketType::SH),
        std::make_pair("204004", MarketType::SH),
        std::make_pair("204007", MarketType::SH),
        std::make_pair("204014", MarketType::SH),
        std::make_pair("204028", MarketType::SH),
        std::make_pair("204091", MarketType::SH),
        std::make_pair("204182", MarketType::SH),
        std::make_pair("131810", MarketType::SZ),
        std::make_pair("131811", MarketType::SZ),
        std::make_pair("131800", MarketType::SZ),
        std::make_pair("131809", MarketType::SZ),
        std::make_pair("131801", MarketType::SZ),
        std::make_pair("131802", MarketType::SZ),
        std::make_pair("131803", MarketType::SZ),
        std::make_pair("131805", MarketType::SZ),
        std::make_pair("131806", MarketType::SZ)
	};

}

#endif // ATS_REPOSERVER_H
