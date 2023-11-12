#include <drogon/drogon.h>
#include "utils/Result.h"

using namespace drogon;

void register_aop();

int main() {
    //Load config file
    app().loadConfigFile("../config.json");
    app().registerBeginningAdvice([]() {
        LOG_INFO << "drogon application started on port " << app().getListeners()[0].toPort();
    });
    register_aop();
    app().run();
    return 0;
}


void register_aop() {
    //请求日志
    app().registerPreRoutingAdvice([](const HttpRequestPtr &req) {
        LOG_INFO << "[url] " << req->getMethodString() << ":" << req->getPath();
    });


    // token校验
    app().registerPreRoutingAdvice([](const HttpRequestPtr &req,
                                      FilterCallback &&fcb,
                                      FilterChainCallback &&fccb) {
        if (req->getPath() == "/account/login") {
            fccb();
            return;
        }


        if (req->getHeader("token").empty()) {
            fcb(Result::error("token not exist"));
            return;
        }
        fccb();
    });

}