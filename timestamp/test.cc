#include "Timestamp.h"
#include <iostream>
#include <thread>

using namespace gu;
using namespace std;

void timestamp_test() {
    Timestamp ts(Timestamp::now());
    cout << ts.toFormattedString(false) << endl;
    this_thread::sleep_for(2s);
    Timestamp ts_end(Timestamp::now());
    cout << ts_end.toFormattedString(false) << endl;
    double diff = Timestamp::timeDifference(ts_end, ts);
    cout << "Diffrent is " << diff  <<  endl;
    Timestamp::addTime(ts_end, 10);
    cout << ts_end.toFormattedString(false) << endl;
    Timestamp ts3;
    string str1 = ts_end.toFormattedString(false);
    string str2 = "09:34-22";
    Timestamp::toTodayTimestamp(str2, ts3);
    cout << ts3.toFormattedString(false) << endl;
    Timestamp::toDayTimestamp(str1, ts3);
    cout << ts3.toFormattedString(false) << endl;

}