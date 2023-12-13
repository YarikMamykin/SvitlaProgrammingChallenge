#include "MockApiServer.h"

#include <QCoreApplication>
#include <QDebug>
#include <QHostAddress>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonObject>
#include <algorithm>
#include <qjsondocument.h>
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

  QHttpServerResponse makeResponse(QString&& rawBody,
                                   QHttpServerResponse::StatusCode statusCode) {
    return QHttpServerResponse(
        QJsonDocument::fromJson(rawBody.toUtf8()).object(), statusCode);
  }

  using State = test::api::MockApiServer::State;
  QHttpServerResponse responseByState(State state) {

    switch (state) {

    case State::Unauthorized:
      return makeResponse(R"({"cause": "Unauthorized"})",
                          QHttpServerResponse::StatusCode::Unauthorized);

    case State::AttemptToAccessAnotherCustomerData:
      return makeResponse(
          R"({"cause": "Attempt to access another customer data"})",
          QHttpServerResponse::StatusCode::Forbidden);

    case State::NonExistingCustomerId:
      return makeResponse(R"({"cause": "Non existing customer id specified"})",
                          QHttpServerResponse::StatusCode::NotFound);

    case State::CableTypeAlreadyExists:
      return makeResponse(R"({"cause": "Cable type already exists"})",
                          QHttpServerResponse::StatusCode::Conflict);

    case State::BusinessRulesViolated:
      return makeResponse(R"({"cause": "Business rules violated"})",
                          QHttpServerResponse::StatusCode::PreconditionFailed);

    case State::DatabaseRejectedTransaction:
      return makeResponse(R"({"cause": "Database rejected transaction"})",
                          QHttpServerResponse::StatusCode::ExpectationFailed);

    case State::DatabaseUnhandledError:
      return makeResponse(R"({"cause": "Database unhandled error"})",
                          QHttpServerResponse::StatusCode::UnprocessableEntity);

    case State::DatabaseRequestTimeout:
      return makeResponse(R"({"cause": "Database request timeout"})",
                          QHttpServerResponse::StatusCode::FailedDependency);

    case State::DatabaseConnectionError:
      return makeResponse(R"({"cause": "Database connection error"})",
                          QHttpServerResponse::StatusCode::InternalServerError);

    case State::TooLargePayload:
      return makeResponse(R"({"cause": "Too large payload"})",
                          QHttpServerResponse::StatusCode::InsufficientStorage);

    default:
      break;
    }

    return makeResponse(R"({"cause": "Unexpected error"})",
                        QHttpServerResponse::StatusCode::BadGateway);
  };

  bool validateRotationFrequencyUnitValues(QString&& value) noexcept {
    static constexpr std::array<std::string, 3> rotationFrequencyUnitValues = {
      "d", "w", "m"
    };

    return std::none_of(
        rotationFrequencyUnitValues.begin(),
        rotationFrequencyUnitValues.end(),
        [valueToValidate = value.toStdString()](const auto& value) -> bool {
          return value == valueToValidate;
        });
  }

  QString
  extractUserTokenFromHeaders(QList<QPair<QByteArray, QByteArray>>&& headers) {
    for (const auto& [key, value] : headers) {
      if ("Authorization" == key) {
        return value;
      }
    }
    return {};
  }

  static constexpr const char defaultCableTypeData[] = R"(
    {
      "id": "5f3bc9e2502422053e08f9f1",
      "identifier": "10-al-1c-trxple",
      "catid": 1622475,
      "diameter": {
        "published": {
          "value": 22.43,
          "unit": "mAh"
        },
        "actual": {
          "value": 22.43,
          "unit": "mAh"
        }
      },
      "conductor": {
        "number": 0,
        "size": {
          "value": 22.43,
          "unit": "mAh"
        }
      },
      "insulation": {
        "type": "string",
        "shield": "string",
        "jacket": "string",
        "thickness": {
          "value": 22.43,
          "unit": "mAh"
        }
      },
      "material": {
        "aluminum": 0,
        "copper": 0,
        "weight": {
          "net": {
            "value": 22.43,
            "unit": "mAh"
          },
          "calculated": {
            "value": 22.43,
            "unit": "mAh"
          }
        }
      },
      "currentPrice": {
        "value": 22.43,
        "unit": "USD"
      },
      "voltage": {
        "value": 22.43,
        "unit": "mAh"
      },
      "rotationFrequency": {
        "value": 22.43,
        "unit": "m"
      },
      "manufacturer": {
        "id": "5f3bc9e2502422053e08f9f1",
        "name": "Kerite"
      },
      "properties": [
        {
          "name": "manufacturedBy",
          "value": {
            "string": "string value",
            "number": 1234.56
          }
        }
      ],
      "customer": {
        "id": "5f3bc9e2502422053e08f9f1",
        "code": "bge"
      },
      "metadata": {
        "created": "2020-10-13T21:31:51.259Z", 
        "modified": "2020-10-13T21:31:51.259Z", 
        "user": { 
          "id": "5f3bc9e2502422053e08f9f1", 
          "username": "test@reelsense.io" 
        } 
      }
    })";
} // namespace

namespace test::api {
  MockApiServer::MockApiServer(State state) {
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
        QHttpServerRequest::Method::Post,
        [state](const QHttpServerRequest& request) -> QHttpServerResponse {
          auto token = extractUserTokenFromHeaders(request.headers());
          if (token != users.at("superuser") and token != users.at("admin")) {
            return responseByState(State::Unauthorized);
          }

          if (State::Normal != state) {
            return responseByState(state);
          }

          QJsonObject requestBody =
              QJsonDocument::fromJson(request.body()).object();

          if (requestBody.contains("id")) {
            return makeResponse(R"({"cause": "id provided in request"})",
                                QHttpServerResponse::StatusCode::BadRequest);
          }

          if (not requestBody.contains("catid") or
              not requestBody.contains("identifier")) {
            return makeResponse(R"({"cause": "missing required keys"})",
                                QHttpServerResponse::StatusCode::BadRequest);
          }

          if (requestBody.contains("rotationFrequency") and
              (not requestBody["rotationFrequency"].isObject() or
               not requestBody["rotationFrequency"].toObject().contains(
                   "unit"))) {
            return makeResponse(
                R"({"cause": "rotationFrequency invalid specification"})",
                QHttpServerResponse::StatusCode::BadRequest);
          }

          if (requestBody.contains("rotationFrequency") and
              validateRotationFrequencyUnitValues(
                  requestBody["rotationFrequency"]
                      .toObject()["unit"]
                      .toString())) {
            return makeResponse(
                R"({"cause": "rotationFrequency.unit has invalid value"})",
                QHttpServerResponse::StatusCode::BadRequest);
          }

          const auto metadataRawJson = QString(
              R"({ 
                  "created": "2020-10-13T21:31:51.259Z", 
                  "modified": "2020-10-13T21:31:51.259Z", 
                  "user": { 
                    "id": "5f3bc9e2502422053e08f9f1", 
                    "username": "test@reelsense.io" 
                  } 
                })");

          QJsonObject metadataGenerated =
              QJsonDocument::fromJson(metadataRawJson.toUtf8()).object();
          QJsonObject responseBody = requestBody;
          responseBody["metadata"] = metadataGenerated;
          responseBody["id"] = "5f3bc9e2502422053e08f9f1";

          return responseBody;
        });

    m_server.route(
        "/cable/type/id/<arg>",
        QHttpServerRequest::Method::Get,
        [state](const QString& id,
                const QHttpServerRequest& request) -> QHttpServerResponse {
          auto token = extractUserTokenFromHeaders(request.headers());
          if (token.isEmpty()) {
            return responseByState(State::Unauthorized);
          }

          if (State::Normal != state) {
            return responseByState(state);
          }

          auto response =
              QJsonDocument::fromJson(defaultCableTypeData).object();

          auto idSize = id.size();
          auto correctIdSize = response["id"].toString().size();
          if (idSize < correctIdSize or idSize > correctIdSize) {
            return makeResponse(
                R"({"cause": "Cable type id has invalid format"})",
                QHttpServerResponse::StatusCode::BadRequest);
          }

          if (id != response["id"].toString()) {
            return makeResponse(
                R"({"cause": "Cable type doesn't exists by specified id"})",
                QHttpServerResponse::StatusCode::NotFound);
          }

          return response;
        });

    m_server.route(
        "/cable/type/id/<arg>",
        QHttpServerRequest::Method::Delete,
        [state](const QString& id,
                const QHttpServerRequest& request) -> QHttpServerResponse {
          auto token = extractUserTokenFromHeaders(request.headers());
          if (token.isEmpty() or
              (token != users.at("superuser") and token != users.at("admin"))) {
            return responseByState(State::Unauthorized);
          }

          if (State::CableTypeReferencedByOtherEntities == state) {
            return makeResponse(
                R"({"cause": "Cable type is referenced by other entities"})",
                QHttpServerResponse::StatusCode::PreconditionFailed);
          }

          if (State::Normal != state) {
            return responseByState(state);
          }

          auto response =
              QJsonDocument::fromJson(defaultCableTypeData).object();

          auto idSize = id.size();
          auto correctIdSize = response["id"].toString().size();
          if (idSize < correctIdSize or idSize > correctIdSize) {
            return makeResponse(
                R"({"cause": "Cable type id has invalid format"})",
                QHttpServerResponse::StatusCode::BadRequest);
          }

          if (id != response["id"].toString()) {
            return makeResponse(
                R"({"cause": "Cable type doesn't exists by specified id"})",
                QHttpServerResponse::StatusCode::NotFound);
          }

          return QJsonDocument::fromJson("{}").object();
        });

    m_server.route(
        "/cable/type/id/<arg>",
        QHttpServerRequest::Method::Put,
        [state](const QString& id,
                const QHttpServerRequest& request) -> QHttpServerResponse {
          auto token = extractUserTokenFromHeaders(request.headers());
          if (token != users.at("superuser") and token != users.at("admin")) {
            return responseByState(State::Unauthorized);
          }

          if (State::Normal != state) {
            return responseByState(state);
          }

          QJsonObject requestBody =
              QJsonDocument::fromJson(request.body()).object();

          if (not requestBody.contains("id")) {
            return makeResponse(R"({"cause": "id not provided in request"})",
                                QHttpServerResponse::StatusCode::BadRequest);
          }

          if (id != requestBody["id"].toString()) {
            return makeResponse(
                R"({"cause": "id mismatch for URL and request body"})",
                QHttpServerResponse::StatusCode::BadRequest);
          }

          if (not requestBody.contains("catid") or
              not requestBody.contains("identifier")) {
            return makeResponse(R"({"cause": "missing required keys"})",
                                QHttpServerResponse::StatusCode::BadRequest);
          }

          QJsonObject defaultCableType =
              QJsonDocument::fromJson(defaultCableTypeData).object();

          if (defaultCableType["id"] != requestBody["id"] or
              defaultCableType["catid"] != requestBody["catid"] or
              defaultCableType["identifier"] != requestBody["identifier"]) {
            return makeResponse(
                R"({"cause": "Attempt to change immutable keys"})",
                QHttpServerResponse::StatusCode::PreconditionFailed);
          }

          if (requestBody.contains("rotationFrequency") and
              (not requestBody["rotationFrequency"].isObject() or
               not requestBody["rotationFrequency"].toObject().contains(
                   "unit"))) {
            return makeResponse(
                R"({"cause": "rotationFrequency invalid specification"})",
                QHttpServerResponse::StatusCode::BadRequest);
          }

          if (requestBody.contains("rotationFrequency") and
              validateRotationFrequencyUnitValues(
                  requestBody["rotationFrequency"]
                      .toObject()["unit"]
                      .toString())) {
            return makeResponse(
                R"({"cause": "rotationFrequency.unit has invalid value"})",
                QHttpServerResponse::StatusCode::BadRequest);
          }

          return requestBody;
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
