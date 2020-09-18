// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)


#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "Timestamp.h"

#include <cinttypes>
#include <sys/time.h>
#include <iostream>

using namespace gu;
using namespace std;

static_assert(sizeof(Timestamp) == sizeof(int64_t),
              "Timestamp is same size as int64_t");

Timestamp::Timestamp(): microSecondsSinceEpoch_(0)
{
}

Timestamp::Timestamp(int64_t microSecondsSinceEpochArg): microSecondsSinceEpoch_(microSecondsSinceEpochArg)
{
}

void Timestamp::swap(Timestamp& that)
{
    std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
}
void Timestamp::swap(int64_t microSeconds) {
    std::swap(microSecondsSinceEpoch_, microSeconds);
}

bool Timestamp::valid() const {
    return microSecondsSinceEpoch_ > 0;
}

Timestamp Timestamp::fromUnixTime(time_t t) {
    return fromUnixTime(t, 0);
}

Timestamp Timestamp::fromUnixTime(time_t t, int microseconds) {
    return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
}

Timestamp Timestamp::invalid() {
    return Timestamp();

}

time_t Timestamp::secondsSinceEpoch() const
{
    return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
}


string Timestamp::toString() const
{
    char buf[32] = {0};
    int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
}

string Timestamp::toFormattedString(bool showMicroseconds) const
{
    char buf[64] = {0};
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);

    if (showMicroseconds)
    {
        int microseconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
                 microseconds);
    }
    else
    {
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }
    return buf;
}

Timestamp Timestamp::now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

std::string Timestamp::getToday() {
    struct timeval tv;
    char buf[32] = {0};
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);
    snprintf(buf, sizeof(buf), "%4d%02d%02d",tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday);
    return buf;
}


/**
 * string example:20200918 08:12:49
 *
 **/
int Timestamp::toDayTimestamp(std::string &dayTimeStr, Timestamp &timestamp) {
    if(dayTimeStr.empty()){
        return 0;
    }
    try {
        tm tm_time;
        tm_time.tm_year = stoi(dayTimeStr.substr(0, 4))-1900;
        tm_time.tm_mon = stoi(dayTimeStr.substr(4, 2))-1;
        tm_time.tm_mday = stoi(dayTimeStr.substr(6, 2));
        tm_time.tm_hour = stoi(dayTimeStr.substr(9, 2))+8;
        tm_time.tm_min = stoi(dayTimeStr.substr(12, 2));
        tm_time.tm_sec = stoi(dayTimeStr.substr(15, 2));
        tm_time.tm_isdst = 0;
        time_t timet = mktime(&tm_time);

        timestamp.swap(timet * kMicroSecondsPerSecond);
        return 1;
    } catch (exception &e) {
        cout << "Error---- " << e.what() << endl;
        return 0;
    }

}

int Timestamp::toTodayTimestamp(std::string &timeStr, Timestamp &timestamp) {
    if (timeStr.empty()){
        return 0;
    }
    string todayStr = Timestamp::getToday();
    string todayTimeStr = todayStr + " " + timeStr;
    return toDayTimestamp(todayTimeStr, timestamp);

}


double Timestamp::timeDifference(Timestamp &high, Timestamp &low)
{
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

Timestamp Timestamp::addTime(Timestamp &timestamp, double seconds)
{
    int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
    return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}


 bool operator<(Timestamp &lhs, Timestamp &rhs)
{
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

bool operator==(Timestamp &lhs, Timestamp &rhs)
{
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

