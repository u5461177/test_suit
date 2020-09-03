//
// Created by xtgu.
//

#include "Algospi.h"
#include "json.hpp"
#include "Algodatatype.h"
#include "AlgospiSimple.h"
#include "libalgo.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace ALGO::API;
using json = nlohmann::json;
using namespace std;



void subscribe_task_report(json task, Algoapi &myapi) {
	if (task["code"].get<int>() == 0) {
		int task_id = task["task_id"].get<int>();
		cout << task_id << endl;
		myapi.subscribe_minute_report(task_id);
	}
	else {
		cout << task << endl;
	}
}


int addtasks(Algoapi &myapi) {
	//订阅
	myapi.subscribe_task_report();
	myapi.subscribe_order_report();
	myapi.subscribe_trade_report();

	//取消订阅
	//myapi.unsubscribe_task_report();
	//myapi.unsubscribe_order_report();
	//myapi.unsubscribe_trade_report();

	//订阅结果返回,true为订阅完成
	cout << myapi.issubcribedtask << endl;
	cout << myapi.issubcribedtrade << endl;
	cout << myapi.issubcribedorder << endl;

	string res;
	json param;

	//等待订阅完成
	while (!(myapi.issubcribedtask && myapi.issubcribedtrade && myapi.issubcribedorder)) {
		this_thread::sleep_for(chrono::seconds(1));
	}

	res = myapi.start_twap("09:40:00", "19:50:00", "131810", SIDE::SELL, MARKET::SZ, "REPO", 100, 10, 122);
	cout << res << endl;
	param = json::parse(res);
	subscribe_task_report(param, myapi);

	/*res = myapi.start_vwap("09:40:00", "19:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 100000, 200, 123);
	cout << res << endl;
	param = json::parse(res);
	subscribe_task_report(param, myapi);

	res = myapi.start_is("09:40:00", "19:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 100000, 200, 124);
	cout << res << endl;
	param = json::parse(res);
	subscribe_task_report(param, myapi);

	res = myapi.start_iceberg("09:40:00", "19:50:00", "000002", SIDE::BUY, MARKET::SZ, "CASH", 100000, 100, 125);
	cout << res << endl;
	param = json::parse(res);
	subscribe_task_report(param, myapi);

	res = myapi.start_random("09:40:00", "19:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 100000, 100, 200, 126);
	cout << res << endl;
	param = json::parse(res);
	subscribe_task_report(param, myapi);

	res = myapi.start_pov("09:40:00", "19:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 100000, 0.5, 200, 127);
	cout << res << endl;
	param = json::parse(res);
	subscribe_task_report(param, myapi);

	res = myapi.start_mstd("09:40:00", "19:50:00", "131810", SIDE::SELL, MARKET::SZ, "REPO", 100, 10, 128);
	cout << res << endl;
	param = json::parse(res);
	subscribe_task_report(param, myapi);

	res = myapi.start_ai("09:40:00", "19:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 100000, 200, 129);
	cout << res << endl;
	param = json::parse(res);
	subscribe_task_report(param, myapi);

	shared_ptr<TaskUnit> task1 = make_shared<TaskUnit>(1800, "09:40:00", "14:50:00", AlgoType::TWAP, "000002",
		SIDE::BUY, MARKET::SZ, "CASH");
	task1->setQuantity(10000);
	task1->setMinimalTradeQuantity(200);

	shared_ptr<TaskUnit> task2 = make_shared<TaskUnit>(1810, "09:40:00", "15:50:00", AlgoType::VWAP, "000002",
		SIDE::BUY, MARKET::SZ, "CASH");
	task2->setMinimalTradeQuantity(200);
	task2->setQuantity(10000);

	shared_ptr<TaskUnit> task3 = make_shared<TaskUnit>(1820, "09:40:00", "14:50:00", AlgoType::POV, "000002",
		SIDE::BUY, MARKET::SZ, "CASH");
	task3->setparticipationRate(0.5);
	task3->setMinimalTradeQuantity(200);
	task3->setQuantity(10000);

	shared_ptr<TaskUnit> task4 = make_shared<TaskUnit>(1830, "09:40:00", "10:50:00", AlgoType::IS, "000002",
		SIDE::SELL, MARKET::SZ, "CASH");
	task4->setMinimalTradeQuantity(200);
	task4->setQuantity(10000);

	shared_ptr<TaskUnit> task5 = make_shared<TaskUnit>(1840, "09:40:00", "11:10:00", AlgoType::ICEBERG, "000002",
		SIDE::SELL, MARKET::SZ, "CASH");
	task5->setVisibleVolume(200);
	task5->setMinimalTradeQuantity(200);
	task5->setQuantity(10000);

	shared_ptr<TaskUnit> task6 = make_shared<TaskUnit>(1850, "09:40:00", "13:50:00", AlgoType::RANDOM, "000002",
		SIDE::SELL, MARKET::SZ, "CASH");
	task6->setNumOfTradings(10);
	task6->setMinimalTradeQuantity(200);
	task6->setQuantity(10000);

	shared_ptr<TaskUnit> task7 = make_shared<TaskUnit>(1860, "09:40:00", "15:50:00", AlgoType::MSTD, "131810",
		SIDE::SELL, MARKET::SZ, "REPO");
	task7->setQuantity(100000);
	task7->setMinimalTradeQuantity(200);

	shared_ptr<TaskUnit> task8 = make_shared<TaskUnit>(1870, "09:40:00", "15:50:00", AlgoType::AIVWAP, "000002",
		SIDE::BUY, MARKET::SZ, "CASH");
	task8->setMinimalTradeQuantity(200);
	task8->setQuantity(10000);

	vector<shared_ptr<TaskUnit>> task_list;
	task_list.push_back(task1);
	task_list.push_back(task3);
	task_list.push_back(task2);
	task_list.push_back(task4);
	task_list.push_back(task5);
	task_list.push_back(task6);
	task_list.push_back(task7);
	task_list.push_back(task8);
	res = myapi.start_tasks(task_list);
	cout << res << endl;
	param = json::parse(res);

	for (auto &task : param) {
		subscribe_task_report(task, myapi);
	}*/


	////暂停
	//myapi.pause_task(task_id);
	////重新开始
	//myapi.resume_task(task_id);
	////停止
	//myapi.stop_task(task_id);

	this_thread::sleep_for(chrono::seconds(5));
	this_thread::sleep_for(chrono::seconds(5));
	this_thread::sleep_for(chrono::seconds(10000000));

	return 0;
};

int addAmounttasks(Algoapi &myapi) {
	//订阅
	myapi.subscribe_task_report();
	myapi.subscribe_order_report();
	myapi.subscribe_trade_report();

	//取消订阅
	//myapi.unsubscribe_task_report();
	//myapi.unsubscribe_order_report();
	//myapi.unsubscribe_trade_report();

	//订阅结果返回,true为订阅完成
	cout << myapi.issubcribedtask << endl;
	cout << myapi.issubcribedtrade << endl;
	cout << myapi.issubcribedorder << endl;
	cout << myapi.issubcribedminute << endl;

	string res;
	json param;

	//等待订阅完成
	while (!(myapi.issubcribedtask && myapi.issubcribedtrade && myapi.issubcribedorder)) {
		this_thread::sleep_for(chrono::seconds(1));
	}

	res = myapi.start_amount_twap("09:40:00", "15:50:00", "131810", SIDE::SELL, MARKET::SZ, "REPO", 100000.0, 3000, 133);
	cout << res << endl;
	param = json::parse(res);
	subscribe_task_report(param, myapi);

	//res = myapi.start_amount_vwap("09:40:00", "14:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 10000.0, 200, 134);
	//cout << res << endl;
	//param = json::parse(res); subscribe_task_report(param, myapi);
	//subscribe_task_report(param, myapi);

	//res = myapi.start_amount_is("09:40:00", "14:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 10000.0, 200, 135);
	//cout << res << endl;
	//param = json::parse(res);
	//subscribe_task_report(param, myapi);

	//res = myapi.start_amount_iceberg("09:40:00", "14:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 10000.0, 10000, 136);
	//cout << res << endl;
	//param = json::parse(res);
	//subscribe_task_report(param, myapi);

	//res = myapi.start_amount_random("09:40:00", "14:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 10000.0, 100, 200, 137);
	//cout << res << endl;
	//param = json::parse(res);
	//subscribe_task_report(param, myapi);

	//res = myapi.start_amount_pov("09:40:00", "14:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 10000.0, 0.5, 200, 138);
	//cout << res << endl;
	//param = json::parse(res);
	//subscribe_task_report(param, myapi);

	//res = myapi.start_amount_mstd("09:40:00", "14:50:00", "131810", SIDE::SELL, MARKET::SZ, "REPO", 100000.0, 3000, 139);
	//cout << res << endl;
	//param = json::parse(res);
	//subscribe_task_report(param, myapi);

	//res = myapi.start_amount_ai("09:40:00", "14:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 10000.0, 200, 140);
	//cout << res << endl;
	//param = json::parse(res); subscribe_task_report(param, myapi);
	//subscribe_task_report(param, myapi);

	//shared_ptr<TaskUnit> task1 = make_shared<TaskUnit>(5000, "09:40:00", "14:50:00", AlgoType::TWAP, "000002",
	//	SIDE::BUY, MARKET::SZ, "CASH");
	//task1->setAmount(10000.0);
	//task1->setMinimalTradeAmount(100.0);

	//shared_ptr<TaskUnit> task2 = make_shared<TaskUnit>(5010, "09:40:00", "14:50:00", AlgoType::VWAP, "000002",
	//	SIDE::BUY, MARKET::SZ, "CASH");
	//task2->setMinimalTradeAmount(100.0);
	//task2->setAmount(10000.0);

	//shared_ptr<TaskUnit> task3 = make_shared<TaskUnit>(5020, "09:40:00", "14:50:00", AlgoType::POV, "000002",
	//	SIDE::BUY, MARKET::SZ, "CASH");
	//task3->setparticipationRate(0.5);
	//task3->setMinimalTradeAmount(100.0);
	//task3->setAmount(10000.0);

	//shared_ptr<TaskUnit> task4 = make_shared<TaskUnit>(5030, "09:40:00", "14:50:00", AlgoType::IS, "000002",
	//	SIDE::BUY, MARKET::SZ, "CASH");
	//task4->setMinimalTradeAmount(100.0);
	//task4->setAmount(10000.0);

	//shared_ptr<TaskUnit> task5 = make_shared<TaskUnit>(5040, "09:40:00", "14:50:00", AlgoType::ICEBERG, "000002",
	//	SIDE::BUY, MARKET::SZ, "CASH");
	//task5->setVisibleAmount(5000.0);
	//task5->setAmount(100000.0);

	//shared_ptr<TaskUnit> task6 = make_shared<TaskUnit>(5050, "09:40:00", "14:50:00", AlgoType::RANDOM, "000002",
	//	SIDE::BUY, MARKET::SZ, "CASH");
	//task6->setNumOfTradings(32);
	//task6->setMinimalTradeAmount(100);
	//task6->setAmount(100000.0);

	//shared_ptr<TaskUnit> task7 = make_shared<TaskUnit>(5060, "09:40:00", "14:50:00", AlgoType::MSTD, "131810",
	//	SIDE::SELL, MARKET::SZ, "REPO");
	//task7->setAmount(10000.0);
	//task7->setMinimalTradeAmount(1000.0);

	//shared_ptr<TaskUnit> task8 = make_shared<TaskUnit>(5070, "09:40:00", "14:50:00", AlgoType::AIVWAP, "000002",
	//	SIDE::BUY, MARKET::SZ, "CASH");
	//task2->setMinimalTradeAmount(100.0);
	//task2->setAmount(10000.0);

	//vector<shared_ptr<TaskUnit>> task_list;
	//task_list.push_back(task1);
	//task_list.push_back(task3);
	//task_list.push_back(task2);
	//task_list.push_back(task4);
	//task_list.push_back(task5);
	//task_list.push_back(task6);
	//task_list.push_back(task7);
	//task_list.push_back(task8);
	//res = myapi.start_amount_tasks(task_list);
	//cout << res << endl;
	//param = json::parse(res);

	//for (auto &task : param) {
	//	subscribe_task_report(task, myapi);
	//}
	//

	////暂停
	//myapi.pause_task(task_id);
	////重新开始
	//myapi.resume_task(task_id);
	////停止
	//myapi.stop_task(1000000044);

	this_thread::sleep_for(chrono::seconds(5));
	this_thread::sleep_for(chrono::seconds(5));
	this_thread::sleep_for(chrono::seconds(10000000));
	return 0;
};


int main(int argc, char **argv) {
	auto myapi = Algoapi::create(make_shared<AlgospiSimple>(), "算法交易ip", 端口, "算法交易ip", 28085);

	myapi->connect("XTP ip", 端口, "账户", "密码");
	addtasks(*myapi);
	//addAmounttasks(*myapi);

	this_thread::sleep_for(chrono::seconds(100));
	//test->disconnect();
	return 0;
}
