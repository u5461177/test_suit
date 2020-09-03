//
// Created by xtgu on 4/2/19.
//

#ifndef PROJECT_TASKUNIT_H
#define PROJECT_TASKUNIT_H

#include <string>
#include "Algodatatype.h"


namespace ALGO {
    namespace API {
        class TaskUnit {
        public:
            TaskUnit(uint64_t id, const std::string &start_time, const std::string &end_time,
                     const AlgoType &task_type,
                     const std::string &ticker, const SIDE &side, const MARKET &market,const std::string &business_type);

            ~TaskUnit() = default;

        public:
            void setMinimalTradeQuantity(int minmal_trade_quantity);

            void setparticipationRate(double participationg_rate);

            void setNumOfTradings(int num_of_tradings);


            void setVisibleVolume(int visible_volume);

            uint64_t getId();

            const std::string getBusinessType();


            void setQuantity(int quantity);

            void setAmount(double amount);

            double getAmount();

            void setMinimalTradeAmount(double minimal_trade_amount);

            double getMinimalTradeAmount();

            const std::string getStartTime();

            const std::string getEndTime();

            const AlgoType getTaskType();

            const std::string getTicker();

            const SIDE getSide();

            const MARKET getMarket();

            int getQuantity();

            int getMinimalTradeQuantity();

            double getParticipationRate();

            int getNumOfTradings();


            int getVisibleVolume();

            void setVisibleAmount( double visible_amount);

            double getVisibleAmount();

        private:
            std::string _start_time;
            std::string _end_time;
            AlgoType _task_type;
            std::string _ticker;
            SIDE _side;
            MARKET _market;
            int _quantity;
            std::string _business_type;
            double _amount;
            int _minimal_trade_quantity;
            double _minimal_trade_amount;
            double _participation_rate;
            int _num_of_tradings;
            int _visible_volume;
            double _visible_amount;
            uint64_t _id;
        };

        inline const std::string TaskUnit::getBusinessType() {
            return _business_type;
        }

        inline void TaskUnit::setQuantity(int quantity) {
            _quantity = quantity;
        }

        inline void TaskUnit::setAmount(double amount) {
            _amount = amount;
        }

        inline double TaskUnit::getAmount() {
            return _amount;
        }

        inline void TaskUnit::setMinimalTradeAmount(double minimal_trade_amount) {
            _minimal_trade_amount = minimal_trade_amount;
        }

        inline double TaskUnit::getMinimalTradeAmount() {
            return  _minimal_trade_amount;
        }

        inline void TaskUnit::setMinimalTradeQuantity(int minmal_trade_quantity) {
            _minimal_trade_quantity = minmal_trade_quantity;
        }

        inline void TaskUnit::setparticipationRate(double participationg_rate) {
            _participation_rate = participationg_rate;
        }

        inline void TaskUnit::setNumOfTradings(int num_of_tradings) {
            _num_of_tradings = num_of_tradings;
        }

        inline void TaskUnit::setVisibleVolume(int visible_volume) {
            _visible_volume = visible_volume;
        }

        inline uint64_t TaskUnit::getId() {
            return _id;
        }

        inline const std::string TaskUnit::getStartTime() {
            return _start_time;

        }

        inline const std::string TaskUnit::getEndTime() {
            return _end_time;
        }

        inline const AlgoType TaskUnit::getTaskType() {
            return _task_type;
        }

        inline const std::string TaskUnit::getTicker() {
            return _ticker;
        }

        inline const MARKET TaskUnit::getMarket() {
            return _market;
        }

        inline const SIDE TaskUnit::getSide() {
            return _side;
        }

        inline int TaskUnit::getQuantity() {
            return _quantity;
        }

        inline int TaskUnit::getMinimalTradeQuantity() {
            return _minimal_trade_quantity;
        }

        inline double TaskUnit::getParticipationRate() {
            return _participation_rate;
        }


        inline int TaskUnit::getNumOfTradings() {
            return _num_of_tradings;
        }

        inline int TaskUnit::getVisibleVolume() {
            return _visible_volume;
        }

       inline void TaskUnit::setVisibleAmount(double visible_amount) {
            _visible_amount = visible_amount;
        }

        inline double TaskUnit::getVisibleAmount() {
            return _visible_amount;
        }
    }
}
#endif //PROJECT_TASKUNIT_H
