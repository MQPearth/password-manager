#include <drogon/drogon.h>
#include "utils/Result.h"

using namespace drogon;

void register_aop();

int main() {
    //Load config file
    app().loadConfigFile("../config.json");
    register_aop();
    app().run();
    return 0;
}


void register_aop() {
    app().registerBeginningAdvice([]() {
        LOG_INFO << "drogon application started on port " << app().getListeners()[0].toPort();
    });

    //请求日志
    app().registerPreRoutingAdvice([](const HttpRequestPtr &req) {
        LOG_INFO << "[url] " << req->getMethodString() << ":" << req->getPath();
    });

}