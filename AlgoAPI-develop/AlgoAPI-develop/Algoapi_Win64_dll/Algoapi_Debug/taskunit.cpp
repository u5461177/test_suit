//
// Created by xtgu on 4/2/19.
//

#include "Algodatatype.h"
#include "taskunit.h"
#include <string>
#include <cstring>


namespace ALGO {
	namespace API {
		using namespace std;

		TaskUnit::TaskUnit(uint64_t id, const string &start_time, const string &end_time,
			const AlgoType &task_type,
			const string &ticker, const SIDE &side, const MARKET &market, const string &business_type) {
			_id = id;
			_start_time = start_time;
			_end_time = end_time;
			_task_type = task_type;
			_ticker = ticker;
			_side = side;
			_market = market;
			_business_type = business_type;
			_quantity = 10;
			_amount = 10.0;
			_minimal_trade_quantity = 200;
			_minimal_trade_amount = 1000.0;
			_participation_rate = 0.5;
			_num_of_tradings = 100;
			_visible_volume = 100;
		}
	}
}

