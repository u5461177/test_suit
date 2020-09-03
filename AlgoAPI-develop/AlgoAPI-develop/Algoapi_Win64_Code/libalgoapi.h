#include<memory>
#include<string>
#include <iostream>
#include "include/Algospi.h"
#ifndef ALGO_EXPORT
#define ALGO_API __declspec(dllexport)
#else
#define ALGO_API __declspec(dllimport)
#endif

namespace ALGO {
	namespace API {
		using namespace std;
		struct Foo {

			static Foo* create(shared_ptr<Algospi> algospi, const string &algo_ip, int algo_port, const string &ws_ip, int ws_port);

			virtual string start_vwap(const string &start_time, const string &end_time, const string &ticker, const SIDE &side, const MARKET &market, const string &business_type, int quantity, int  minimal_trade_quantity) = 0;			
		};

	};
};
