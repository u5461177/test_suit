//
// Created by xtgu on 8/23/19.
//

#include <iostream>
#include "AlgospiSimple.h"

using namespace ALGO::API;
using namespace std;

void AlgospiSimple::on_connect(const string &jsonstr) {
	cout << "on_connect" << endl;
	cout << jsonstr << endl;

}

void AlgospiSimple::on_disconnect(const string &jsonstr) {
	cout << "on_disconnect" << endl;
	cout << jsonstr << endl;
}

void AlgospiSimple::on_error(const string &jsonstr) {
	cout << "on_error" << endl;
	cout << jsonstr << endl;

}

void AlgospiSimple::on_order(const string &jsonstr) {
	cout << "on_order" << endl;
	cout << jsonstr << endl;

}

void AlgospiSimple::on_trade(const string &jsonstr) {
	cout << "on_trade" << endl;
	cout << jsonstr << endl;

}

void AlgospiSimple::on_task_report(const string &jsonstr) {
	cout << "on_task_report" << endl;
	cout << jsonstr << endl;

}

void AlgospiSimple::on_minute_report(const string &jsonstr) {
	cout << "on_minute_report" << endl;
	cout << jsonstr << endl;

}

void AlgospiSimple::on_message(const string &jsonstr) {
	cout << "on_error" << endl;
	cout << jsonstr << endl;

}

void AlgospiSimple::on_taskid(const string &jsonstr) {
	cout << "on_taskid" << endl;
	cout << jsonstr << endl;

}

void AlgospiSimple::on_subscribe_result(const string &jsonstr) {
	cout << "on_subscribe_result" << endl;
	cout << jsonstr << endl;

}

void AlgospiSimple::on_unsubscribe_result(const string &jsonstr) {
	cout << "on_unsubscribe_result" << endl;
	cout << jsonstr << endl;

}
