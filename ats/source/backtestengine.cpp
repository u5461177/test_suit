//
// Created by Rong Zhou on 2018-08-09.
//

#include "backtestengine.h"
#include "csv.h"

namespace ats {

    using namespace std;
    using namespace io;

    bool BackTestEngine::load() {
        CSVReader<7> order_book("order_20180802_000002.csv");
        order_book.read_header(ignore_extra_column, "TradeDate", "RecvTime", "ApplSeqNum", "SecurityID", "Price",
                               "OrderQty", "Side");
        uint32_t order_date;
        uint64_t order_time;
        uint32_t order_sequence_id;
        string order_security_id;
        float order_price;
        uint32_t order_qty;
        int side;
        bool order_book_done = order_book.read_row(order_date, order_time, order_sequence_id, order_security_id,
                                                   order_price, order_qty, side);


        CSVReader<7> trade_book("trade_20180802_000002.csv");
        trade_book.read_header(ignore_extra_column, "TradeDate", "RecvTime", "SecurityID", "BidApplSeqNum",
                               "OfferApplSeqNum", "Price", "TradeQty");
        uint32_t trade_date;
        uint64_t trade_time;
        string trade_security_id;
        uint32_t bid_sequence_id;
        uint32_t ask_sequence_id;
        float trade_price;
        uint32_t trade_qty;
        bool trade_book_done = trade_book.read_row(trade_date, trade_time, trade_security_id, bid_sequence_id,
                                                   ask_sequence_id, trade_price, trade_qty);

        while (!order_book_done or !trade_book_done) {
            if (order_time <= trade_time) {
                auto order = make_pair(order_sequence_id, order_qty);
                if (side == 1) {
                    auto i = _bid_book.find(order_price);
                    if (i not_eq _bid_book.end()) {
                        i->second.push_back(order);
                    } else {
                        auto order_queue = OrderQueue{order};
                        _bid_book.insert(make_pair(order_price, order_queue));
                    }
                } else {
                    auto i = _ask_book.find(order_price);
                    if (i not_eq _ask_book.end()) {
                        i->second.push_back(order);
                    } else {
                        auto order_queue = OrderQueue{order};
                        _ask_book.insert(make_pair(order_price, order_queue));
                    }
                }
                order_book_done = order_book.read_row(order_date, order_time, order_sequence_id, order_security_id,
                                                      order_price, order_qty, side);
            } else {

            }
        }
        return true;
    }

}