#pragma once
#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonDocument>
#include <QJsonObject>

namespace test::api {
  class MockApiServer {

  public:
    enum class State {
      Normal,
      Unauthorized,
      AttemptToAccessAnotherCustomerData,
      NonExistingCustomerId,
      CableTypeAlreadyExists,
      BusinessRulesViolated,
      DatabaseRejectedTransaction,
      DatabaseUnhandledError,
      DatabaseRequestTimeout,
      DatabaseConnectionError,
      TooLargePayload
    };

    MockApiServer(State state = State::Normal);
    ~MockApiServer() = default;

  private:
    QHttpServer m_server;
  };
} // namespace test::api
