#include "MockApiServer.h"

#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonObject>
#include <stdexcept>
#include <unordered_map>

namespace {
  /*
   * Predefined map of user ids to their hardcoded JWT token.
   * Enough for test purpose to imitate logged in user.
   */
  static const std::unordered_map<QString, QString> users = {
    {"superuser",
     "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
     "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIy"
     "fQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c"},

    {    "admin",
     "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
     "fyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIy"
     "fQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c"},

    {     "user",
     "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
     "gyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5MDIy"
     "fQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c"},
  };
} // namespace

namespace test::api {
  MockApiServer::MockApiServer() {
    m_server.route(
        "/login/<arg>",
        QHttpServerRequest::Method::Get,
        [](const QString& id) -> QHttpServerResponse {
          try {

            const auto& userToken = users.at(id);
            QJsonObject responseBody;
            responseBody["jwtToken"] = userToken;
            return responseBody;

          } catch (const std::out_of_range& idError) {
            return QHttpServerResponse::StatusCode::InternalServerError;
          }
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
