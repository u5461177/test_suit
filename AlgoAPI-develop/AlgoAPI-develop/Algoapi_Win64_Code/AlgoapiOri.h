//
// Created by galen on 12/17/18.
//

#ifndef EASYWSCLIENT_AlgoapiOri_H
#define EASYWSCLIENT_AlgoapiOri_H
#include "libalgo.h"
#include "Algospi.h"
#include "taskunit.h"
#include <iostream>
#include <string>
#include <thread>
#include "Algodatatype.h"
#include <cstdlib>
#include <cerrno>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <sstream>
#include <iostream>
#include <memory>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

namespace ALGO {
	namespace API {
		using namespace std;
		using websocketpp::lib::placeholders::_1;
		using websocketpp::lib::placeholders::_2;
		using websocketpp::lib::bind;
		class AlgoapiOri : public Algoapi {
		private:
			typedef websocketpp::client<websocketpp::config::asio_tls_client> wssclient;
			typedef websocketpp::client<websocketpp::config::asio_client> client;
			typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;
			shared_ptr<Algospi> _algospi;
			string _algo_ip;
			int _algo_port;
			string _ws_ip;
			int _ws_port;
			shared_ptr <curlpp::Easy> _httpclient;
			shared_ptr <std::list<std::string>> _request_headers;
			shared_ptr <wssclient> _wss;
			shared_ptr <client> _ws;
			wssclient::connection_ptr _cons;
			client::connection_ptr _con;
			websocketpp::connection_hdl _hdl;
			string _session_id;
			string _username;
			thread _thread;
			const string market_str[2] = { "SH", "SZ" };
			const string side_str[2] = { "BUY", "SELL" };
			bool _isconnected = false;
			string _subscribe_type;
			uint64_t _id;


		public:
			AlgoapiOri(shared_ptr<Algospi> algospi, const string &algo_ip, int algo_port, const string &ws_ip, int ws_port);

			virtual ~AlgoapiOri();

			void connect(const string &xtp_ip, int xtp_port, const string &username, const string &password);

			void disconnect();

			bool isconnect();

			string
				start_vwap(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, int quantity, int minimal_trade_quantity, uint64_t _id) override;

			string
				start_twap(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, int quantity, int minimal_trade_quantity, uint64_t _id) override;

			string
				start_is(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, int quantity, int minimal_trade_quantity, uint64_t _id) override;

			string
				start_pov(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, int quantity, double participation_rate,
					int minimal_trade_quantity, uint64_t _id) override;

			string
				start_random(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, int quantity, int num_of_tradings,
					int minimal_trade_quantity, uint64_t _id) override;

			string
				start_iceberg(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, int quantity, int visible_volume, uint64_t _id) override;

			string
				start_mstd(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, int quantity, int minimal_trade_quantity, uint64_t _id) override;

			string
				start_ai(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, int quantity, int minimal_trade_quantity, uint64_t _id) override;

			string start_tasks(const vector <shared_ptr<TaskUnit>> &tasks_list) override;

			string start_amount_tasks(const vector<shared_ptr<TaskUnit>> &tasks_list) override;

			string
				start_amount_vwap(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, double amount, int minimal_trade_amount, uint64_t _id) override;

			string
				start_amount_twap(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, double amount, int minimal_trade_amount, uint64_t _id) override;

			string
				start_amount_is(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, double amount, int minimal_trade_amount, uint64_t _id) override;

			string
				start_amount_pov(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, double amount,
					double participation_rate, int minimal_trade_amount, uint64_t _id) override;

			string
				start_amount_random(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, double amount, int num_of_tradings,
					int minimal_trade_amount, uint64_t _id) override;

			string
				start_amount_iceberg(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, double amount, double visible_amount, uint64_t _id) override;


			string
				start_amount_mstd(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, double amount, int minimal_trade_amount, uint64_t _id) override;

			string
				start_amount_ai(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
					const MARKET &market, const string &business_type, double amount, int minimal_trade_amount, uint64_t _id) override;

			string stop_task(int task_id) override;

			string get_task_report() override;

			string get_minute_report(int task_id) override;

			string pause_task(int task_id) override;

			string resume_task(int task_id) override;

			void get_history_order(int task_id) override;

			void get_history_trade(int task_id) override;

			void subscribe_order_report() override;

			void subscribe_trade_report () override;

			void subscribe_task_report() override;

			void subscribe_minute_report(int task_id) override;

			void unsubscribe_order_report() override;

			void unsubscribe_trade_report() override;

			void unsubscribe_task_report() override;

			void unsubscribe_minute_report(int task_id) override;

		private:
			string login(const string &xtp_ip, int xtp_port, const string &username, const string &password);

			string logout();

			void pre_handle_message(const string &jsonstr);

			string send_post_request(const string &func, const string &jsonstr);

			string send_get_request(const string &func, const string &jsonstr);

			void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg);

			bool verify_certificate(const char *hostname, bool preverified, boost::asio::ssl::verify_context &ctx);

			context_ptr on_tls_init(const char *hostname, websocketpp::connection_hdl);

			void send(const string &msg);

		};
	}
}


#endif //EASYWSCLIENT_AlgoapiOri_H
