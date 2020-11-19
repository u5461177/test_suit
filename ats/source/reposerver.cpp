// 
// Created by Ruiqi Zhang at 2019-06-11
//

#include "reposerver.h"

namespace ats 
{
	double RepoServer::get_repo_mean(const std::string& ticker)
	{
		if (_repo_mean.find(ticker) != _repo_mean.end())
			return _repo_mean[ticker];
		else
		{
			LOG(WARNING)<<ticker<<" not found.";
			return 0;
		}
	}

	double RepoServer::get_repo_std(const std::string& ticker)
	{
		if (_repo_std.find(ticker) != _repo_std.end())
			return _repo_std[ticker];
		else
		{
			LOG(WARNING)<<ticker<<" not found.";
			return 0;
		}
	}

	bool RepoServer::update_repo_data(std::string ticker, double price)
	{
		if (price < 0.0001)
		{
			//LOG(WARNING)<<"Abnormal price value!";
			return false;
		}
		// update variables
		if (_n_quote.find(ticker) != _n_quote.end())
		{
			_n_quote[ticker] += 1;
			_price_sum[ticker] += price;
			_price_square_sum[ticker] += price * price;
		}
		else
		{
			_n_quote[ticker] = 1;
			_price_sum[ticker] = price;
			_price_square_sum[ticker] = price * price;
		}
		_repo_mean[ticker] = _price_sum[ticker] / double(_n_quote[ticker]);
		_repo_std[ticker] = sqrt(_price_square_sum[ticker] * 1. / double (_n_quote[ticker]) - _repo_mean[ticker] * _repo_mean[ticker]);
		return true;
	}

}
