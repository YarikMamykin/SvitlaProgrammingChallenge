#pragma once
#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonDocument>
#include <QJsonObject>

namespace test::api {
  class MockApiServer {

  public:
    MockApiServer();
    ~MockApiServer() = default;

  private:
    QHttpServer m_server;
  };
} // namespace test::api
