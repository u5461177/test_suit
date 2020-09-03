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
	myapi.subscribe_task_report();
	myapi.subscribe_order_report();
	myapi.subscribe_trade_report();

	string res;
	json param;

	for (int i = 0; i < 1; i++) {
		res = myapi.start_twap("09:40:00", "19:50:00", "131810", SIDE::SELL, MARKET::SZ, "REPO", 100, 10);
		cout << res << endl;
		param = json::parse(res);
		subscribe_task_report(param, myapi);

		res = myapi.start_vwap("09:40:00", "19:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 100000, 200);
		cout << res << endl;
		param = json::parse(res);
		subscribe_task_report(param, myapi);

		res = myapi.start_is("09:40:00", "19:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 100000, 200);
		cout << res << endl;
		param = json::parse(res);
		subscribe_task_report(param, myapi);

		res = myapi.start_iceberg("09:40:00", "19:50:00", "000002", SIDE::BUY, MARKET::SZ, "CASH", 100000, 100);
		cout << res << endl;
		param = json::parse(res);
		subscribe_task_report(param, myapi);

		res = myapi.start_random("09:40:00", "19:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 100000, 100, 200);
		cout << res << endl;
		param = json::parse(res);
		subscribe_task_report(param, myapi);

		res = myapi.start_pov("09:40:00", "19:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 100000, 0.5, 200);
		cout << res << endl;
		param = json::parse(res);
		subscribe_task_report(param, myapi);

		res = myapi.start_mstd("09:40:00", "19:50:00", "131810", SIDE::SELL, MARKET::SZ, "REPO", 100, 10);
		cout << res << endl;
		param = json::parse(res);
		subscribe_task_report(param, myapi);

		res = myapi.start_ai("09:40:00", "19:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 100000, 200);
		cout << res << endl;
		param = json::parse(res);
		subscribe_task_report(param, myapi);

		//shared_ptr<TaskUnit> task1 = make_shared<TaskUnit>(1800, "09:40:00", "14:50:00", AlgoType::TWAP, "000002",
		//	SIDE::BUY, MARKET::SZ, "CASH");
		//task1->setQuantity(10000);
		//task1->setMinimalTradeQuantity(200);

		//shared_ptr<TaskUnit> task2 = make_shared<TaskUnit>(1810, "09:40:00", "15:50:00", AlgoType::VWAP, "000002",
		//	SIDE::BUY, MARKET::SZ, "CASH");
		//task2->setMinimalTradeQuantity(200);
		//task2->setQuantity(10000);

		//shared_ptr<TaskUnit> task3 = make_shared<TaskUnit>(1820, "09:40:00", "14:50:00", AlgoType::POV, "000002",
		//	SIDE::BUY, MARKET::SZ, "CASH");
		//task3->setparticipationRate(0.5);
		//task3->setMinimalTradeQuantity(200);
		//task3->setQuantity(10000);

		//shared_ptr<TaskUnit> task4 = make_shared<TaskUnit>(1830, "09:40:00", "10:50:00", AlgoType::IS, "000002",
		//	SIDE::SELL, MARKET::SZ, "CASH");
		//task4->setMinimalTradeQuantity(200);
		//task4->setQuantity(10000);

		//shared_ptr<TaskUnit> task5 = make_shared<TaskUnit>(1840, "09:40:00", "11:10:00", AlgoType::ICEBERG, "000002",
		//	SIDE::SELL, MARKET::SZ, "CASH");
		//task5->setVisibleVolume(200);
		//task5->setMinimalTradeQuantity(200);
		//task5->setQuantity(10000);

		//shared_ptr<TaskUnit> task6 = make_shared<TaskUnit>(1850, "09:40:00", "13:50:00", AlgoType::RANDOM, "000002",
		//	SIDE::SELL, MARKET::SZ, "CASH");
		//task6->setNumOfTradings(1000);
		//task6->setMinimalTradeQuantity(200);
		//task6->setQuantity(10000);

		//shared_ptr<TaskUnit> task7 = make_shared<TaskUnit>(1860, "09:40:00", "15:50:00", AlgoType::MSTD, "131810",
		//	SIDE::SELL, MARKET::SZ, "REPO");
		//task7->setQuantity(100000);
		//task7->setMinimalTradeQuantity(200);

		//shared_ptr<TaskUnit> task8 = make_shared<TaskUnit>(1810, "09:40:00", "15:50:00", AlgoType::AIVWAP, "000002",
		//	SIDE::BUY, MARKET::SZ, "CASH");
		//task8->setMinimalTradeQuantity(200);
		//task8->setQuantity(10000);

		//vector<shared_ptr<TaskUnit>> task_list;
		//task_list.push_back(task1);
		//task_list.push_back(task3);
		//task_list.push_back(task2);
		//task_list.push_back(task4);
		//task_list.push_back(task5);
		//task_list.push_back(task6);
		//task_list.push_back(task7);
		//task_list.push_back(task8);
		//res = myapi.start_tasks(task_list);
		//cout << res << endl;
		//param = json::parse(res);

		//for (auto &task : param) {
		//	subscribe_task_report(task, myapi);
		//}
	}

	////��ͣ
	//myapi.pause_task(task_id);
	////���¿�ʼ
	//myapi.resume_task(task_id);
	////ֹͣ
	//myapi.stop_task(task_id);

	this_thread::sleep_for(chrono::seconds(5));
	this_thread::sleep_for(chrono::seconds(5));
	this_thread::sleep_for(chrono::seconds(10000000));

	return 0;
};

int addAmounttasks(Algoapi &myapi) {
	myapi.subscribe_task_report();
	myapi.subscribe_order_report();
	myapi.subscribe_trade_report();

	string res;
	json param;

	for (int i = 0; i < 1; i++) {
		//res = myapi.start_amount_twap("09:40:00", "14:50:00", "131810", SIDE::SELL, MARKET::SZ, "REPO", 100000.0, 3000);
		//cout << res << endl;
		//param = json::parse(res);
		//subscribe_task_report(param, myapi);

		//res = myapi.start_amount_vwap("09:40:00", "14:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 10000.0, 200);
		//cout << res << endl;
		//param = json::parse(res); subscribe_task_report(param, myapi);
		//subscribe_task_report(param, myapi);

		//res = myapi.start_amount_is("09:40:00", "14:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 10000.0, 200);
		//cout << res << endl;
		//param = json::parse(res);
		//subscribe_task_report(param, myapi);

		res = myapi.start_amount_iceberg("09:40:00", "14:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 10000.0,
			10000);
		cout << res << endl;
		param = json::parse(res);
		subscribe_task_report(param, myapi);

		res = myapi.start_amount_random("09:40:00", "14:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 10000.0, 100,
			200);
		cout << res << endl;
		param = json::parse(res);
		subscribe_task_report(param, myapi);

		//res = myapi.start_amount_pov("09:40:00", "14:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 10000.0, 0.5,
		//	200);
		//cout << res << endl;
		//param = json::parse(res);
		//subscribe_task_report(param, myapi);

		//res = myapi.start_amount_mstd("09:40:00", "14:50:00", "131810", SIDE::SELL, MARKET::SZ, "REPO", 100000.0, 3000);
		//cout << res << endl;
		//param = json::parse(res);
		//subscribe_task_report(param, myapi);

		//res = myapi.start_amount_ai("09:40:00", "14:50:00", "600000", SIDE::BUY, MARKET::SH, "CASH", 10000.0, 200);
		//cout << res << endl;
		//param = json::parse(res); subscribe_task_report(param, myapi);
		//subscribe_task_report(param, myapi);

		//shared_ptr<TaskUnit> task1 = make_shared<TaskUnit>(6000, "09:40:00", "14:50:00", AlgoType::TWAP, "000002",
		//	SIDE::BUY, MARKET::SZ, "CASH");
		//task1->setAmount(10000.0);
		//task1->setMinimalTradeAmount(100.0);

		//shared_ptr<TaskUnit> task2 = make_shared<TaskUnit>(6010, "09:40:00", "14:50:00", AlgoType::VWAP, "000002",
		//	SIDE::BUY, MARKET::SZ, "CASH");
		//task2->setMinimalTradeAmount(100.0);
		//task2->setAmount(10000.0);

		//shared_ptr<TaskUnit> task3 = make_shared<TaskUnit>(6020, "09:40:00", "14:50:00", AlgoType::POV, "000002",
		//	SIDE::BUY, MARKET::SZ, "CASH");
		//task3->setparticipationRate(0.5);
		//task3->setMinimalTradeAmount(100.0);
		//task3->setAmount(10000.0);

		//shared_ptr<TaskUnit> task4 = make_shared<TaskUnit>(6030, "09:40:00", "14:50:00", AlgoType::IS, "000002",
		//	SIDE::BUY, MARKET::SZ, "CASH");
		//task4->setMinimalTradeAmount(100.0);
		//task4->setAmount(10000.0);

		//shared_ptr<TaskUnit> task5 = make_shared<TaskUnit>(6040, "09:40:00", "14:50:00", AlgoType::ICEBERG, "000002",
		//	SIDE::BUY, MARKET::SZ, "CASH");
		//task5->setVisibleAmount(5000.0);
		//task5->setAmount(100000.0);

		//shared_ptr<TaskUnit> task6 = make_shared<TaskUnit>(6050, "09:40:00", "14:50:00", AlgoType::RANDOM, "000002",
		//	SIDE::BUY, MARKET::SZ, "CASH");
		//task6->setNumOfTradings(32);
		//task6->setMinimalTradeAmount(100);
		//task6->setAmount(100000.0);

		//shared_ptr<TaskUnit> task7 = make_shared<TaskUnit>(6060, "09:40:00", "14:50:00", AlgoType::MSTD, "131810",
		//	SIDE::SELL, MARKET::SZ, "REPO");
		//task7->setAmount(10000.0);
		//task7->setMinimalTradeAmount(1000.0);

		//shared_ptr<TaskUnit> task8 = make_shared<TaskUnit>(6010, "09:40:00", "14:50:00", AlgoType::AIVWAP, "000002",
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
	}

	//myapi.pause_task(task_id);
	//myapi.resume_task(task_id);
	//myapi.stop_task(task_id);

	this_thread::sleep_for(chrono::seconds(5));
	this_thread::sleep_for(chrono::seconds(5));
	this_thread::sleep_for(chrono::seconds(10000000));
	return 0;
};


int main(int argc, char **argv) {

	auto myapi = Algoapi::create(make_shared<AlgospiSimple>(), "smart.qilu-dev.com", 22225, "smart.qilu-dev.com", 22225);

	myapi->connect("120.27.164.69", 6001, "15003916", "gS9SYHOz");
	addtasks(*myapi);
	addAmounttasks(*myapi);

	this_thread::sleep_for(chrono::seconds(100));
	//test->disconnect();
	return 0;
}
