//
// Created by xtgu on 8/23/19.
//

#ifndef AlgoapiOri_ALGOSPISIMPLE_H
#define AlgoapiOri_ALGOSPISIMPLE_H

#include "Algospi.h"


namespace ALGO {
    namespace API {
        class AlgospiSimple: public Algospi {
            void on_connect(const string &jsonstr) override;

            void on_disconnect(const string &jsonstr) override;

            void on_error(const string &jsonstr) override;

            void on_order(const string &jsonstr) override;

            void on_trade(const string &jsonstr) override;

            void on_task_report(const string &jsonstr) override;

            void on_minute_report(const string &jsonstr) override;

            void on_message(const string &jsonstr) override;

			void on_taskid(const string &jsonstr) override;

			void on_subscribe_result(const string &jsonstr) override;

			void on_unsubscribe_result(const string &jsonstr) override;

        };
    }
}


#endif //AlgoapiOri_ALGOSPISIMPLE_H
