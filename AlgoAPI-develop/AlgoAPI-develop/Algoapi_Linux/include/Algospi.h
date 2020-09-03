//
// Created by xtgu on 08/23/19.
//

#ifndef EASYWSCLIENT_ALGOSPI_H
#define EASYWSCLIENT_ALGOSPI_H

#include <string>

namespace ALGO {
	namespace API {
		using namespace std;

		class Algospi {
		public:
			Algospi() = default;
			virtual ~Algospi() = default;

		public:
			virtual void on_connect(const string &jsonstr) = 0;

			virtual void on_disconnect(const string &jsonstr) = 0;

			virtual void on_error(const string &jsonstr) = 0;

			virtual void on_order(const string &jsonstr) = 0;

			virtual void on_trade(const string &jsonstr) = 0;

			virtual void on_task_report(const string &jsonstr) = 0;

			virtual void on_minute_report(const string &jsonstr) = 0;

			virtual void on_message(const string &jsonstr) = 0;

		};
	}
}


#endif //EASYWSCLIENT_ALGOSPI_H
