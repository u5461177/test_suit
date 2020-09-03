//
// Created by xtgu on 8/28/19.
//

#ifndef AlgoapiOri_LIBALGO_H
#define AlgoapiOri_LIBALGO_H


#include "Algodatatype.h"
#include "Algospi.h"
#include "taskunit.h"
#include <memory>
#include <vector>

namespace ALGO {
	namespace API {
		struct Algoapi {

			static Algoapi *create(std::shared_ptr<Algospi> algospi, const std::string &algo_ip, int algo_port, const std::string &ws_ip, int ws_port);
			virtual void connect(const std::string &xtp_ip, int xtp_port, const std::string &username, const std::string &password) = 0;
			virtual std::string
				start_twap(const std::string &start_time, const std::string &end_time, const std::string &ticker,
					const SIDE &side,
					const MARKET &market, const std::string &business_type, int quantity,
					int minimal_trade_quantity) = 0;

			virtual std::string
				start_vwap(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, int quantity, int minimal_trade_quantity) = 0;

			virtual std::string
				start_is(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, int quantity, int minimal_trade_quantity) = 0;

			virtual std::string
				start_pov(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, int quantity, double participation_rate,
					int minimal_trade_quantity) = 0;

			virtual std::string
				start_random(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, int quantity, int num_of_tradings,
					int minimal_trade_quantity) = 0;

			virtual std::string
				start_iceberg(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, int quantity, int visible_volume) = 0;

			virtual std::string
				start_mstd(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, int quantity, int minimal_trade_quantity) = 0;

			virtual std::string
				start_ai(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, int quantity, int minimal_trade_quantity) = 0;

			virtual std::string start_tasks(const vector <shared_ptr<TaskUnit>> &tasks_list) = 0;

			virtual std::string start_amount_tasks(const vector<shared_ptr<TaskUnit>> &tasks_list) = 0;

			virtual std::string
				start_amount_vwap(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, double amount, int minimal_trade_amount) = 0;

			virtual std::string
				start_amount_twap(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, double amount, int minimal_trade_amount) = 0;

			virtual std::string
				start_amount_is(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, double amount, int minimal_trade_amount) = 0;

			virtual std::string
				start_amount_pov(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, double amount,
					double participation_rate, int minimal_trade_amount) = 0;

			virtual std::string
				start_amount_random(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, double amount, int num_of_tradings,
					int minimal_trade_amount) = 0;

			virtual std::string
				start_amount_iceberg(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, double amount, double visible_amount) = 0;


			virtual std::string
				start_amount_mstd(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, double amount, int minimal_trade_amount) = 0;

			virtual std::string
				start_amount_ai(const std::string &start_time, const std::string &end_time, const std::string &ticker, const SIDE &side,
					const MARKET &market, const std::string &business_type, double amount, int minimal_trade_amount) = 0;

			virtual std::string stop_task(int task_id) = 0;

			virtual std::string get_task_report() = 0;

			virtual std::string get_minute_report(int task_id) = 0;

			virtual std::string pause_task(int task_id) = 0;

			virtual std::string resume_task(int task_id) = 0;

			virtual void get_history_order(int task_id) = 0;

			virtual void get_history_trade(int task_id) = 0;

			virtual void subscribe_order_report() = 0;

			virtual void subscribe_trade_report() = 0;

			virtual void subscribe_task_report() = 0;

			virtual void subscribe_minute_report(int task_id) = 0;

			virtual void unsubscribe_order_report() = 0;

			virtual void unsubscribe_trade_report() = 0;

			virtual void unsubscribe_task_report() = 0;

			virtual void unsubscribe_minute_report(int task_id) = 0;

		};

	}
}


#endif //AlgoapiOri_LIBALGO_H
