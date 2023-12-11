#pragma once
#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonDocument>
#include <QJsonObject>

namespace test::api {
  class MyRestApiServer {

  public:
    MyRestApiServer();
    ~MyRestApiServer() = default;

  private:
    QHttpServer m_server;
  };
} // namespace test::api
