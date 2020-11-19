#include "configuration.h"
#include "redisserver.h"
#include "quoteserver.h"
#include "restserver.h"
#include "reposerver.h"
#include "data_store/dataStorage.h"


using namespace ats;
using namespace std::chrono;

int main(int argc, char **argv) {
    GLogHelper gh(argv[0], "/home/data/muse_log/data");
#ifdef MY_DEBUG
    //Debug
    LOG(INFO) << "Using Debug Mode";
#else
    LOG(INFO) << "Using Release Mode" ;
#endif
    if (argc > 1 && strcmp(argv[1], "release") == 0) {
        if (Configuration::instance().load("config_release.json")) {
            LOG(INFO) << "load release configuration successfully";
        } else {
            LOG(ERROR) << "failed to load release configuration";
            return EXIT_FAILURE;
        }
    } else {
        if (Configuration::instance().load("config.json")) {
            LOG(INFO) << "load configuration successfully";
        } else {
            LOG(ERROR) << "failed to load configuration";
            google::ShutdownGoogleLogging();
            return EXIT_FAILURE;
        }
    }

    if(Configuration::instance().get_current_store()) {

        DataStorage::get_instance()->start();
    }

    RedisServer::instance().connect();
    if (QuoteServer::instance().connect()) {
        LOG(INFO) << "connect quote server successfully";
    } else {
        LOG(ERROR) << "failed to connect quote server and keep trying";
        while (!QuoteServer::instance().connect()) {
            std::this_thread::sleep_for(1s);
        }
        LOG(INFO) << "connect quote server successfully";
    }

    //  subscribe market_data for repos
    for (auto ticker : repo_ticker)
        QuoteServer::instance().subscribe_market_data(ticker);

    RESTServer::instance().start();
    RESTServer::instance().stop();

    // unsubscribe market_data for repos
    for (auto ticker : repo_ticker)
        QuoteServer::instance().unsubscribe_market_data(ticker);

    QuoteServer::instance().disconnect();
    RedisServer::instance().disconnect();

    return EXIT_SUCCESS;
}
