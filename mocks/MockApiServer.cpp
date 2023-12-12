#include "MockApiServer.h"

#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>
#include <QHttpServerRequest>
#include <QHttpServerResponse>

namespace test::api {
  MyRestApiServer::MyRestApiServer() {
    m_server.route("/exit", []() -> QHttpServerResponse {
      QCoreApplication::exit(0);
      return "";
    });

    m_server.route(
        "/cable/type",
        QHttpServerRequest::Method::Get,
        [](const QHttpServerRequest&) -> QHttpServerResponse { return "LEL"; });
    m_server.route(
        "/cable/type",
        QHttpServerRequest::Method::Post,
        [](const QHttpServerRequest&) -> QHttpServerResponse { return "LEL"; });

    m_server.route("/cable/type/id/<arg>",
                   QHttpServerRequest::Method::Get,
                   [](const QString& id) -> QHttpServerResponse {
                     qDebug() << id;
                     return "KEK";
                   });
    m_server.route("/cable/type/id/<arg>",
                   QHttpServerRequest::Method::Put,
                   [](const QString& id) -> QHttpServerResponse {
                     qDebug() << id;
                     return "KEK";
                   });
    m_server.route("/cable/type/id/<arg>",
                   QHttpServerRequest::Method::Delete,
                   [](const QString& id) -> QHttpServerResponse {
                     qDebug() << id;
                     return "KEK";
                   });
    m_server.route("/cable/type/identifier/<arg>",
                   QHttpServerRequest::Method::Get,
                   [](const QString& identifier) -> QHttpServerResponse {
                     qDebug() << identifier;
                     return "KEK";
                   });
    m_server.route("/cable/type/customer/code/<arg>",
                   QHttpServerRequest::Method::Get,
                   [](const QString& code) -> QHttpServerResponse {
                     qDebug() << code;
                     return "KEK";
                   });
    m_server.route("/cable/type/identifier/<arg>/customer/code/<arg>",
                   QHttpServerRequest::Method::Get,
                   [](const QString& identifier,
                      const QString& code) -> QHttpServerResponse {
                     qDebug() << identifier << " " << code;
                     return "KEK";
                   });
    m_server.route("/cable/type/count/references/<arg>",
                   QHttpServerRequest::Method::Get,
                   [](const QString& ref) -> QHttpServerResponse {
                     qDebug() << ref;
                     return "KEK";
                   });
    m_server.route("/cable/type/history/summary/<arg>",
                   QHttpServerRequest::Method::Get,
                   [](const QString& id) -> QHttpServerResponse {
                     qDebug() << id;
                     return "KEK";
                   });
    m_server.route("/cable/type/history/document/<arg>",
                   QHttpServerRequest::Method::Get,
                   [](const QString& id) -> QHttpServerResponse {
                     qDebug() << id;
                     return "KEK";
                   });
    m_server.route("/cable/type/created/between/<arg>/<arg>",
                   QHttpServerRequest::Method::Get,
                   [](const QString& startTime,
                      const QString& endTime) -> QHttpServerResponse {
                     qDebug() << startTime << " " << endTime;
                     return "KEK";
                   });
    m_server.route("/cable/type/modified/between/<arg>/<arg>",
                   QHttpServerRequest::Method::Get,
                   [](const QString& startTime,
                      const QString& endTime) -> QHttpServerResponse {
                     qDebug() << startTime << " " << endTime;
                     return "KEK";
                   });
    m_server.listen(QHostAddress::LocalHost, 8080);
  }

} // namespace test::api
