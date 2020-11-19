//
// Created by Rong Zhou on 2018-08-03.
//

#ifndef ATS_ERRORCODE_H
#define ATS_ERRORCODE_H

#include <string>
#include <system_error>

namespace ats {

    enum class ErrorCode {
        NO_ERROR = 0,
        WRONG_FORMAT = 101,
        UNKNOWN_SECURITY = 102,
        USER_NOT_EXIST = 103,
        WRONG_PASSWORD = 104,
        WRONG_SERVER_IP = 105,
        WRONG_SESSION_ID = 106,
        WRONG_TASK_ID = 107,
        WRONG_TIME_RANGE = 108,
        PERMISSION_DENIED = 109,
        WRONG_ID = 110,
        WRONG_TASK_TYPE = 111,
        WRONG_BUSINESS_TYPE = 112,
        WRONG_MARKET_TYPE = 113,
        UNKNOWN_ERROR = 114,
        WRONG_INPUT_TYPE = 115,
        FAILED_TO_RECONNECT =116,
        ILLEGAL_SESSION = 117,
        CANNOT_GET_MAC = 118,
        FAILED_TO_CREATE_TASK = 201,
        FAILED_TO_RESUME_TASK = 202,
        FAILED_TO_PAUSE_TASK = 203,
        FAILED_TO_STOP_TASK = 204,
        TASK_ALREADY_RESUMED = 205,
        TASK_ALREADY_PAUSED = 206,
        TASK_ALREADY_STOPPED = 207,
        INSUFFICIENT_TASK_QUANTITY = 301,
        INVALID_QUANTITY_PARAMETER = 302,
        INVALID_TIME_PARAMETER = 303,
        INVALID_MINIMAL_TRADE_QUANTITY_PARAMETER = 304,
        INVALID_MINIMAL_TRADE_AMOUNT_PARAMETER = 305,
        TOTAL_ALGO_ORDER_EXCEEDS_AVAILABLE_ASSET = 306,
        TOTAL_ALGO_ORDER_EXCEEDS_AVAILABLE_QUANTITY = 307,
        INVALID_SIDE_TYPE_PARAMETER = 308,
        FAIL_TO_SUBSCRIBE_MARKET_DATA = 309,
        TIME_OUT = 310,
        CANNOT_GET_TICKER_QUOTE = 311,
        CANNOT_FIND_TRADABLE_ASSET = 312,
        CANNOT_FIND_SELLABLE_QUANTITY = 313,
        QUANTITY_TOO_LARGE = 314,
        ALGORITHM_PARAMETER_ERROR = 315,
        ALGORITHM_EXECUTE_ERROR = 316,
        ESTIMATED_VOLUME_UNAVAILABLE = 317,
        ESTIMATED_VOLUME_INSUFFICIENT = 318,
        INVALID_VISIBLE_PARAMETERS = 319,
        INVALID_NUM_OF_TRADINGS_PARAMETER = 320,
        INVALID_AMOUNT_PARAMETER = 321,
        INVALID_PARTICIPATION_RATE_PARAMETER = 322,
        CANNOT_LOAD_AIVWAP_MODEL = 323,
        CANCEL_ORDER_ERROR = 324,
        OVER_TASKS_LIMIT = 325,
        WRONG_END_TIME = 326
    };

    class ErrorCategory  {

    public:
        ErrorCategory();

        explicit ErrorCategory(ErrorCode errorCode);

        std::string message() const;

        ErrorCode &get_error_code();

        int value() const;

        //std::error_code make_error_code(ErrorCode code);


    private:

        ErrorCode _errorCode;

    };

    inline ErrorCode& ErrorCategory::get_error_code(){
        return _errorCode;
    }

}

//namespace std {
//
//    template<>
//    struct is_error_code_enum<ats::ErrorCode> : true_type {
//    };
//
//}

#endif //ATS_ERRORCODE_H
