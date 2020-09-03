#include <memory>
#include "AlgoapiOri.h"
#include "json.hpp"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::placeholders::_3;
using websocketpp::lib::bind;

namespace ALGO {
	namespace API {
		using json = nlohmann::json;
		using namespace std;
		Algoapi * Algoapi::create(shared_ptr<Algospi> algospi, const string &algo_ip, int algo_port, const string &ws_ip, int ws_port) {
			return new AlgoapiOri(algospi, algo_ip, algo_port, ws_ip, ws_port);

		}
		static string myreplace(const string &inStr, const string &toreplace, const string &replace) {
			int pos = 0;
			string r = inStr;
			while ((pos = r.find(toreplace)) >= 0) {
				r = r.replace(pos, toreplace.length(), replace);
			}
			return r;
		}

		AlgoapiOri::AlgoapiOri(shared_ptr<Algospi> algospi, const string &algo_ip, int algo_port, const string &ws_ip, int ws_port) {
			_algospi = algospi;
			_algo_ip = algo_ip;
			_algo_port = algo_port;
			_ws_ip = ws_ip;
			_ws_port = ws_port;
			_httpclient = make_shared<curlpp::Easy>();
			_request_headers = make_shared<std::list<std::string>>();
			_request_headers->push_back("Content-Type: application/json");
			_request_headers->push_back("Accept: application/json");
		}

		AlgoapiOri::~AlgoapiOri() {
			if (_thread.joinable()) {
				_thread.join();
			}
		}

		void AlgoapiOri::connect(const string &xtp_ip, int xtp_port, const string &username, const string &password) {

			string sessionid = login(xtp_ip, xtp_port, username, password);
			string url = "";
			if (_ws_ip.find("smart") != std::string::npos) {
				url = "wss://" + _ws_ip + ":" + to_string(_ws_port) + "/report?session_id=" + sessionid;
				_wss = make_shared<wssclient>();
				try {
					// Set logging to be pretty verbose (everything except message payloads)
					_wss->set_access_channels(websocketpp::log::alevel::none);
					_wss->clear_access_channels(websocketpp::log::alevel::none);
					_wss->set_error_channels(websocketpp::log::elevel::all);
					_wss->init_asio();
					_wss->set_message_handler(std::bind(&AlgoapiOri::on_message, this, _1, _2));
					_wss->set_tls_init_handler(
						websocketpp::lib::bind(std::bind(&AlgoapiOri::on_tls_init, this, _1, _2), _ws_ip.c_str(),
							::_1));

					websocketpp::lib::error_code ec;
					_cons = _wss->get_connection(url, ec);
					if (ec) {
						std::cout << "could not create connection because: " << ec.message() << std::endl;
						return;
					}
					_hdl = _cons->get_handle();
					_wss->connect(_cons);
					_wss->get_alog().write(websocketpp::log::alevel::app, "Connecting to " + url);
					_thread = thread(&wssclient::run, _wss);

					while (!isconnect()) {
						cout << "connecting" << endl;
						this_thread::sleep_for(chrono::seconds(1));
					}

				}
				catch (websocketpp::exception const &e) {
					std::cout << "exception:" << e.what() << std::endl;
				}
			}
			else {
				url = "ws://" + _ws_ip + ":" + to_string(_ws_port) + "/report?session_id=" + sessionid;
				_ws = make_shared<client>();
				try {
					_ws->set_access_channels(websocketpp::log::alevel::none);
					_ws->clear_access_channels(websocketpp::log::alevel::none);
					_ws->set_error_channels(websocketpp::log::elevel::all);
					_ws->init_asio();
					_ws->set_message_handler(std::bind(&AlgoapiOri::on_message, this, _1, _2));

					websocketpp::lib::error_code ec;
					_con = _ws->get_connection(url, ec);
					if (ec) {
						std::cout << "could not create connection because: " << ec.message() << std::endl;
						return;
					}

					_hdl = _con->get_handle();
					_ws->connect(_con);
					_ws->get_alog().write(websocketpp::log::alevel::app, "Connecting to " + url);
					_thread = thread(&client::run, _ws);

					while (!isconnect()) {
						cout << "connecting" << endl;
						this_thread::sleep_for(chrono::seconds(1));
					}

				}
				catch (websocketpp::exception const &e) {
					std::cout << "exception:" << e.what() << std::endl;
				}
			}


		}

		string AlgoapiOri::login(const string &xtp_ip, int xtp_port, const string &username, const string &password) {
			json jsonstr;
			jsonstr["username"] = username;
			jsonstr["password"] = password;
			jsonstr["server_ip"] = xtp_ip;
			jsonstr["server_port"] = xtp_port;
			jsonstr["key"] = "";
			string res = send_post_request("login", jsonstr.dump());
			auto param = json::parse(res);
			_session_id = param["session_id"].get<string>();
			_username = username;
			return _session_id;
		}

		void AlgoapiOri::pre_handle_message(const string &jsonstr) {
			if (jsonstr == "welcome") {
				_isconnected = true;
				_algospi->on_connect("{\"msg\":\"welcome\"}");
				return;
			}
			string jsonstr2 = myreplace(myreplace(jsonstr, "[", ""), "]", "");
			auto param = json::parse(jsonstr2);
			auto report_type = param["report_type"].get<string>();
			if (report_type == "task_report") {
				_algospi->on_task_report(jsonstr2);
			}
			else if (report_type == "minute_report") {
				_algospi->on_minute_report(jsonstr2);
			}
			else if (report_type == "trade_report") {
				_algospi->on_trade(jsonstr2);
			}
			else if (report_type == "order_report") {
				_algospi->on_order(jsonstr2);
			}
			else {
				_algospi->on_error(jsonstr);
			}
		}

		string AlgoapiOri::send_post_request(const string &func, const string &jsonstr) {
			string url;
			if (_algo_ip.find("smart") != std::string::npos)
				url = "https://" + _algo_ip + ":" + to_string(_algo_port) + "/" + func;
			else
				url = "http://" + _algo_ip + ":" + to_string(_algo_port) + "/" + func;
			std::ostringstream response;
			_httpclient = make_shared<curlpp::Easy>();
			_httpclient->setOpt(new curlpp::options::Url(url));
			_httpclient->setOpt(new curlpp::options::Verbose(false));
			_httpclient->setOpt(new curlpp::options::HttpHeader(*_request_headers));
			_httpclient->setOpt(new curlpp::options::PostFields(jsonstr));
			_httpclient->setOpt(new curlpp::options::PostFieldSize(jsonstr.length()));
			_httpclient->setOpt(new curlpp::options::WriteStream(&response));
			_httpclient->perform();

			string res = response.str();
			res = myreplace(res, "\\", "");
			return res;
		}

		string AlgoapiOri::send_get_request(const string &func, const string &jsonstr) {
			string url;
			if (_algo_ip.find("smart") != std::string::npos)
				url = "https://" + _algo_ip + ":" + to_string(_algo_port) + "/" + func + "?";
			else
				url = "http://" + _algo_ip + ":" + to_string(_algo_port) + "/" + func + "?";
			auto param = json::parse(jsonstr);
			for (auto i = param.begin(); i != param.end(); i++) {
				url += i.key() + "=";
				string value = i.value().dump();
				value = myreplace(value, "\"", "");
				url += value + "&";
			}
			url = url.substr(0, url.size() - 1);

			std::ostringstream response;
			_httpclient = make_shared<curlpp::Easy>();
			_httpclient->setOpt(new curlpp::options::Url(url));
			_httpclient->setOpt(new curlpp::options::Verbose(true));
			_httpclient->setOpt(new curlpp::options::HttpHeader(*_request_headers));
			_httpclient->setOpt(new curlpp::options::WriteStream(&response));
			_httpclient->perform();
			string res = response.str();
			res = myreplace(res, "\\", "");
			return res;
		}

		string
			AlgoapiOri::start_vwap(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
				const MARKET &market, const string &business_type, int quantity,
				int minimal_trade_quantity) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "VWAP";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["quantity"] = quantity;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_quantity"] = minimal_trade_quantity;
			return send_post_request("start_task", jsonstr.dump());

		}

		string
			AlgoapiOri::start_twap(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
				const MARKET &market, const string &business_type, int quantity,
				int minimal_trade_quantity) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "TWAP";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["quantity"] = quantity;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_quantity"] = minimal_trade_quantity;
			return send_post_request("start_task", jsonstr.dump());
		}

		string
			AlgoapiOri::start_is(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
				const MARKET &market, const string &business_type, int quantity, int minimal_trade_quantity) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "IS";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["quantity"] = quantity;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_quantity"] = minimal_trade_quantity;
			return send_post_request("start_task", jsonstr.dump());
		}

		string
			AlgoapiOri::start_pov(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
				const MARKET &market, const string &business_type, int quantity, double participation_rate,
				int minimal_trade_quantity) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "POV";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["quantity"] = quantity;
			jsonstr["participation_rate"] = participation_rate;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_quantity"] = minimal_trade_quantity;
			return send_post_request("start_task", jsonstr.dump());
		}

		string
			AlgoapiOri::start_random(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
				const MARKET &market, const string &business_type, int quantity, int num_of_tradings,
				int minimal_trade_quantity) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "RANDOM";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["quantity"] = quantity;
			jsonstr["num_of_tradings"] = num_of_tradings;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_quantity"] = minimal_trade_quantity;
			return send_post_request("start_task", jsonstr.dump());
		}

		string
			AlgoapiOri::start_iceberg(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
				const MARKET &market, const string &business_type, int quantity, int visible_volume) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "ICEBERG";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["quantity"] = quantity;
			jsonstr["business_type"] = business_type;
			jsonstr["visible_volume"] = visible_volume;
			jsonstr["session_id"] = _session_id;
			return send_post_request("start_task", jsonstr.dump());
		}

		string
			AlgoapiOri::start_mstd(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
				const MARKET &market, const string &business_type, int quantity, int minimal_trade_quantity) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "MSTD";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["quantity"] = quantity;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_quantity"] = minimal_trade_quantity;
			return send_post_request("start_task", jsonstr.dump());
		}

		string
			AlgoapiOri::start_ai(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
				const MARKET &market, const string &business_type, int quantity, int minimal_trade_quantity) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "AIVWAP";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["quantity"] = quantity;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_quantity"] = minimal_trade_quantity;
			return send_post_request("start_task", jsonstr.dump());
		}
		string AlgoapiOri::start_tasks(const vector<shared_ptr<TaskUnit>> &tasks_list) {
			json tasks_array = json::array();
			for (const auto &task : tasks_list) {
				json jsonstr;
				jsonstr["id"] = task->getId();
				jsonstr["start_time"] = task->getStartTime();
				jsonstr["end_time"] = task->getEndTime();
				jsonstr["task_type"] = algoType_to_String(task->getTaskType());
				jsonstr["ticker"] = task->getTicker();
				jsonstr["side"] = side_str[task->getSide()];
				jsonstr["market"] = market_str[task->getMarket()];
				jsonstr["quantity"] = task->getQuantity();
				jsonstr["business_type"] = task->getBusinessType();
				jsonstr["visible_volume"] = task->getVisibleVolume();
				jsonstr["minimal_trade_quantity"] = task->getMinimalTradeQuantity();
				jsonstr["participation_rate"] = task->getParticipationRate();
				jsonstr["num_of_tradings"] = task->getNumOfTradings();
				jsonstr["session_id"] = _session_id;
				tasks_array.push_back(jsonstr);

			}
			return send_post_request("start_tasks", tasks_array.dump());

		}

		string
			AlgoapiOri::start_amount_vwap(const string &start_time, const string &end_time, const string &ticker,
				const SIDE &side,
				const MARKET &market, const string &business_type, double amount,
				int minimal_trade_amount) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "VWAP";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["amount"] = amount;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_amount"] = minimal_trade_amount;
			return send_post_request("start_amount_task", jsonstr.dump());

		}

		string
			AlgoapiOri::start_amount_twap(const string &start_time, const string &end_time, const string &ticker,
				const SIDE &side,
				const MARKET &market, const string &business_type, double amount,
				int minimal_trade_amount) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "TWAP";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["amount"] = amount;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_amount"] = minimal_trade_amount;
			return send_post_request("start_amount_task", jsonstr.dump());
		}

		string
			AlgoapiOri::start_amount_is(const string &start_time, const string &end_time, const string &ticker,
				const SIDE &side,
				const MARKET &market, const string &business_type, double amount,
				int minimal_trade_amount) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "IS";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["amount"] = amount;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_amount"] = minimal_trade_amount;
			return send_post_request("start_amount_task", jsonstr.dump());
		}

		string
			AlgoapiOri::start_amount_pov(const string &start_time, const string &end_time, const string &ticker,
				const SIDE &side,
				const MARKET &market, const string &business_type, double amount,
				double participation_rate,
				int minimal_trade_amount) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "POV";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["amount"] = amount;
			jsonstr["participation_rate"] = participation_rate;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_amount"] = minimal_trade_amount;
			return send_post_request("start_amount_task", jsonstr.dump());
		}

		string
			AlgoapiOri::start_amount_random(const string &start_time, const string &end_time, const string &ticker,
				const SIDE &side,
				const MARKET &market, const string &business_type, double amount,
				int num_of_tradings,
				int minimal_trade_amount) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "RANDOM";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["amount"] = amount;
			jsonstr["num_of_tradings"] = num_of_tradings;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_amount"] = minimal_trade_amount;
			return send_post_request("start_amount_task", jsonstr.dump());
		}

		string
			AlgoapiOri::start_amount_iceberg(const string &start_time, const string &end_time, const string &ticker,
				const SIDE &side,
				const MARKET &market, const string &business_type, double amount,
				double visible_amount) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "ICEBERG";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["amount"] = amount;
			jsonstr["business_type"] = business_type;
			jsonstr["visible_amount"] = visible_amount;
			jsonstr["session_id"] = _session_id;
			return send_post_request("start_amount_task", jsonstr.dump());
		}

		string
			AlgoapiOri::start_amount_mstd(const string &start_time, const string &end_time, const string &ticker,
				const SIDE &side,
				const MARKET &market, const string &business_type, double amount,
				int minimal_trade_amount) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "MSTD";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["amount"] = amount;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_amount"] = minimal_trade_amount;
			return send_post_request("start_amount_task", jsonstr.dump());
		}

		string
			AlgoapiOri::start_amount_ai(const string &start_time, const string &end_time, const string &ticker, const SIDE &side,
				const MARKET &market, const string &business_type, double amount, int minimal_trade_amount) {
			json jsonstr;
			jsonstr["start_time"] = start_time;
			jsonstr["end_time"] = end_time;
			jsonstr["task_type"] = "AIVWAP";
			jsonstr["ticker"] = ticker;
			jsonstr["side"] = side_str[side];
			jsonstr["market"] = market_str[market];
			jsonstr["business_type"] = business_type;
			jsonstr["amount"] = amount;
			jsonstr["session_id"] = _session_id;
			jsonstr["minimal_trade_amount"] = minimal_trade_amount;
			return send_post_request("start_amount_task", jsonstr.dump());

		}
		string AlgoapiOri::start_amount_tasks(const vector<shared_ptr<TaskUnit>> &tasks_list) {

			json tasks_array = json::array();


			for (const auto &task : tasks_list) {
				json jsonstr;
				jsonstr["id"] = task->getId();
				jsonstr["start_time"] = task->getStartTime();
				jsonstr["end_time"] = task->getEndTime();
				jsonstr["task_type"] = algoType_to_String(task->getTaskType());
				jsonstr["ticker"] = task->getTicker();
				jsonstr["side"] = side_str[task->getSide()];
				jsonstr["market"] = market_str[task->getMarket()];
				jsonstr["amount"] = task->getAmount();
				jsonstr["business_type"] = task->getBusinessType();
				jsonstr["visible_amount"] = task->getVisibleAmount();
				jsonstr["minimal_trade_amount"] = task->getMinimalTradeAmount();
				jsonstr["participation_rate"] = task->getParticipationRate();
				jsonstr["num_of_tradings"] = task->getNumOfTradings();
				jsonstr["session_id"] = _session_id;
				tasks_array.push_back(jsonstr);

			}
			return send_post_request("start_amount_tasks", tasks_array.dump());

		}


		void AlgoapiOri::disconnect() {
			string res = logout();
			if (_ws_ip.find("smart") != std::string::npos)
				_wss->close(_hdl, websocketpp::close::status::normal, "");
			else
				_ws->close(_hdl, websocketpp::close::status::normal, "");
			std::cout << "disconnect" << std::endl;
			std::cout << res << std::endl;
		}

		string AlgoapiOri::logout() {
			json jsonstr;
			jsonstr["session_id"] = _session_id;
			string res = send_post_request("logout", jsonstr.dump());
			return res;
		}

		string AlgoapiOri::get_task_report() {
			json jsonstr;
			jsonstr["username"] = _username;
			jsonstr["session_id"] = _session_id;
			string res = send_get_request("get_task_report", jsonstr.dump());
			return res;
		}

		string AlgoapiOri::get_minute_report(int task_id) {
			json jsonstr;
			jsonstr["username"] = _username;
			jsonstr["session_id"] = _session_id;
			jsonstr["task_id"] = task_id;
			string res = send_get_request("get_minute_report", jsonstr.dump());
			return res;
		}

		string AlgoapiOri::stop_task(int task_id) {
			json jsonstr;
			jsonstr["session_id"] = _session_id;
			jsonstr["task_id"] = task_id;
			string res = send_post_request("stop_task", jsonstr.dump());
			return res;
		}

		string AlgoapiOri::pause_task(int task_id) {
			json jsonstr;
			jsonstr["session_id"] = _session_id;
			jsonstr["task_id"] = task_id;
			string res = send_post_request("pause_task", jsonstr.dump());
			return res;
		}

		string AlgoapiOri::resume_task(int task_id) {
			json jsonstr;
			jsonstr["session_id"] = _session_id;
			jsonstr["task_id"] = task_id;
			string res = send_post_request("resume_task", jsonstr.dump());
			return res;
		}


		void AlgoapiOri::get_history_order(int task_id) {
			cout << "to be implemented" << endl;
		}

		void AlgoapiOri::get_history_trade(int task_id) {
			cout << "to be implemented" << endl;
		}

		void AlgoapiOri::subscribe_order_report() {
			send("{\"command\":\"subscribe_order_report\"}");
		}

		void AlgoapiOri::subscribe_trade_report() {
			send("{\"command\":\"subscribe_trade_report\"}");
		}

		void AlgoapiOri::subscribe_task_report() {
			send("{\"command\":\"subscribe_task_report\"}");
		}

		void AlgoapiOri::subscribe_minute_report(int task_id) {
			send("{\"command\":\"subscribe_minute_report\",\"task_id\":" + to_string(task_id) + "}");
		}

		void AlgoapiOri::unsubscribe_order_report() {
			send("{\"command\":\"unsubscribe_order_report\"}");
		}

		void AlgoapiOri::unsubscribe_trade_report() {
			send("{\"command\":\"unsubscribe_trade_report\"}");
		}

		void AlgoapiOri::unsubscribe_task_report() {
			send("{\"command\":\"unsubscribe_task_report\"}");
		}

		void AlgoapiOri::unsubscribe_minute_report(int task_id) {
			send("{\"command\":\"unsubscribe_minute_report\",\"task_id\":" + to_string(task_id) + "}");
		}

		void AlgoapiOri::on_message(websocketpp::connection_hdl hdl, client::message_ptr msg) {
			pre_handle_message(msg->get_payload());
		}

		bool
			AlgoapiOri::verify_certificate(const char *hostname, bool preverified, boost::asio::ssl::verify_context &ctx) {
			return true;
		}

		AlgoapiOri::context_ptr AlgoapiOri::on_tls_init(const char *hostname, websocketpp::connection_hdl) {
			context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(
				boost::asio::ssl::context::sslv23);

			try {
				ctx->set_options(boost::asio::ssl::context::default_workarounds |
					boost::asio::ssl::context::no_sslv2 |
					boost::asio::ssl::context::no_sslv3 |
					boost::asio::ssl::context::single_dh_use);


				ctx->set_verify_mode(boost::asio::ssl::verify_peer);
				ctx->set_verify_callback(
					websocketpp::lib::bind(std::bind(&AlgoapiOri::verify_certificate, this, _1, _2, _3), hostname,
						::_1, ::_2));

			}
			catch (std::exception &e) {
				std::cout << "exception:" << e.what() << std::endl;
			}
			return ctx;
		}

		void AlgoapiOri::send(const std::string &msg) {
			websocketpp::lib::error_code ec;
			if (_ws_ip.find("com") != std::string::npos)
				_wss->send(_hdl, msg, websocketpp::frame::opcode::text, ec);
			else
				_ws->send(_hdl, msg, websocketpp::frame::opcode::text, ec);
		}

		bool AlgoapiOri::isconnect() {
			return _isconnected;
		}

	}
}
