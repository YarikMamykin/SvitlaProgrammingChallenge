#include "MockApiServer.h"

#include <QCoreApplication>
#include <QHostAddress>
#include <QHttpServerRequest>
#include <QHttpServerResponse>

namespace test::api {
  MyRestApiServer::MyRestApiServer() {
    m_server.route("/exit", []() -> QHttpServerResponse {
      QCoreApplication::exit(0);
      return "";
    });
    m_server.listen(QHostAddress::LocalHost, 8080);
  }

} // namespace test::api
