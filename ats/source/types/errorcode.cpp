//
// Created by Rong Zhou on 2018-08-03.
//

#include "errorcode.h"

namespace ats {

    using namespace std;

    ErrorCategory::ErrorCategory() {
        _errorCode = ErrorCode::NO_ERROR;
    }

    ErrorCategory::ErrorCategory(ErrorCode errorCode):_errorCode(errorCode) {

    }

    string ErrorCategory::message() const {
        switch (_errorCode) {
            case ErrorCode::NO_ERROR:
                return "success";
            case ErrorCode::WRONG_FORMAT:
                return "wrong format";
            case ErrorCode::UNKNOWN_SECURITY:
                return "unknown security";
            case ErrorCode::USER_NOT_EXIST:
                return "user not exist";
            case ErrorCode::WRONG_PASSWORD:
                return "wrong password";
            case ErrorCode::WRONG_SERVER_IP:
                return "wrong server ip";
            case ErrorCode::WRONG_SESSION_ID:
                return "wrong session id";
            case ErrorCode::WRONG_TASK_ID:
                return "wrong task id";
            case ErrorCode::WRONG_TIME_RANGE:
                return "wrong time range";
            case ErrorCode::PERMISSION_DENIED:
                return "permission denied";
            case ErrorCode::WRONG_ID:
                return "wrong id";
            case ErrorCode::WRONG_TASK_TYPE:
                return "wrong task type";
            case ErrorCode::WRONG_BUSINESS_TYPE:
                return "wrong business type";
            case ErrorCode::WRONG_MARKET_TYPE:
                return "wrong market type";
            case ErrorCode::WRONG_INPUT_TYPE:
                return "has wrong type in input data";
            case ErrorCode::FAILED_TO_RESUME_TASK:
                return "failed to resume task";
            case ErrorCode::FAILED_TO_PAUSE_TASK:
                return "failed to pause task";
            case ErrorCode::FAILED_TO_STOP_TASK:
                return "failed to stop task";
            case ErrorCode::FAILED_TO_CREATE_TASK:
                return "failed to create task";
            case ErrorCode::TASK_ALREADY_RESUMED:
                return "task already resumed";
            case ErrorCode::TASK_ALREADY_PAUSED:
                return "task already paused";
            case ErrorCode::TASK_ALREADY_STOPPED:
                return "task already stopped";
            case ErrorCode::INSUFFICIENT_TASK_QUANTITY:
                return "insufficient task quantity";
            case ErrorCode::INVALID_QUANTITY_PARAMETER:
                return "invalid quantity parameter ";
            case ErrorCode::INVALID_TIME_PARAMETER:
                return "The end time needs to be 5 minutes longer than the start time;"
                       " Time needs to be in continuous trading hours";
            case ErrorCode::INVALID_MINIMAL_TRADE_QUANTITY_PARAMETER:
                return "invalid minimal trade quantity parameter";
            case ErrorCode::INVALID_MINIMAL_TRADE_AMOUNT_PARAMETER:
                return "invalid minimal trade amount parameter";
            case ErrorCode::TOTAL_ALGO_ORDER_EXCEEDS_AVAILABLE_ASSET:
                return "total algo order exceeds available asset";
            case ErrorCode::TOTAL_ALGO_ORDER_EXCEEDS_AVAILABLE_QUANTITY:
                return "total algo order exceeds available quantity";
            case ErrorCode::INVALID_SIDE_TYPE_PARAMETER:
                return "invalid side type parameter";
            case ErrorCode::FAIL_TO_SUBSCRIBE_MARKET_DATA:
                return "fail to subscribe market data";
            case ErrorCode::TIME_OUT:
                return "time out";
            case ErrorCode::CANNOT_GET_TICKER_QUOTE:
                return "cannot get ticker quote";
            case ErrorCode::CANNOT_FIND_TRADABLE_ASSET:
                return "cannot find tradable asset";
            case ErrorCode::CANNOT_FIND_SELLABLE_QUANTITY:
                return "cannot find sellable quantity";
            case ErrorCode::QUANTITY_TOO_LARGE:
                return "the quantity is too large to be fully traded";
            case ErrorCode::ALGORITHM_PARAMETER_ERROR:
                return "algorithm parameter error";
            case ErrorCode::ALGORITHM_EXECUTE_ERROR:
                return "algorithm execute error";
            case ErrorCode::ESTIMATED_VOLUME_UNAVAILABLE:
                return "estimated volume is not available";
            case ErrorCode::ESTIMATED_VOLUME_INSUFFICIENT:
                return "estimated volume is insufficient";
            case ErrorCode::INVALID_VISIBLE_PARAMETERS:
                return "invalid visible parameters";
            case ErrorCode::INVALID_NUM_OF_TRADINGS_PARAMETER:
                return "invalid num of tradings parameter";
            case ErrorCode::INVALID_AMOUNT_PARAMETER:
                return "invalid amount parameter";
            case ErrorCode::INVALID_PARTICIPATION_RATE_PARAMETER:
                return "invalid participation rate parameter";
            case ErrorCode::CANCEL_ORDER_ERROR:
                return "can not cancel order in system!!";
            case ErrorCode::OVER_TASKS_LIMIT:
                return "Exceeded the limit of tasks number in each tasks unit!!";
            case ErrorCode::ILLEGAL_SESSION:
                return "This device is not the log-in one !!!";
            case ErrorCode::CANNOT_GET_MAC:
                return "Can not Get Mac!!";
            case ErrorCode::WRONG_END_TIME:
                return "Wrong end time, end time is earlier than now !!";
            default:
                return "unknown error";
        }
    }


    int ErrorCategory::value() const{
        return (int)_errorCode;
    }


}