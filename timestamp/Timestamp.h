
// Author: guxt 2020/09/18

#ifndef MUDUO_BASE_TIMESTAMP_H
#define MUDUO_BASE_TIMESTAMP_H



#include "operators.hpp"
/***
 * string example:20200918 08:12:49
 */

namespace gu
{

    class Timestamp :public boost::equality_comparable<Timestamp>,public boost::less_than_comparable<Timestamp>
    {
    public:
        Timestamp();
        explicit Timestamp(int64_t microSecondsSinceEpochArg);
        friend bool operator<(Timestamp &lhs, Timestamp &rhs);
        friend bool operator==(Timestamp &lhs, Timestamp &rhs);

        void swap(Timestamp& that);
        void swap(int64_t microSeconds);
        std::string toString() const;
        std::string toFormattedString(bool showMicroseconds = true) const;

        bool valid() const;
        // for internal usage.
        int64_t microSecondsSinceEpoch() const;
        time_t secondsSinceEpoch() const;
        static Timestamp now();
        static Timestamp invalid();
        static Timestamp fromUnixTime(time_t t);
        static Timestamp fromUnixTime(time_t t, int microseconds);
        static std::string getToday();
        static int toTodayTimestamp(std::string &timeStr, Timestamp &timestamp);
        static int toDayTimestamp(std::string &dayTimeStr, Timestamp &timestamp);
        static double timeDifference(Timestamp &high, Timestamp &low);
        static Timestamp addTime(Timestamp &timestamp, double seconds);
        static const int kMicroSecondsPerSecond = 1000 * 1000;

    private:
        int64_t microSecondsSinceEpoch_;
    };

    inline int64_t Timestamp::microSecondsSinceEpoch() const {
        return microSecondsSinceEpoch_;
    }


}  // namespace muduo

#endif  // MUDUO_BASE_TIMESTAMP_H
