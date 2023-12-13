#pragma once
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QObject>

namespace test::utils {

  std::tuple<QJsonObject, int, QNetworkReply::NetworkError>
  makeGetRequest(const QNetworkRequest& request);

  std::tuple<QJsonObject, int, QNetworkReply::NetworkError>
  makePostRequest(const QNetworkRequest& request, const QByteArray& data);

  std::tuple<QJsonObject, int, QNetworkReply::NetworkError>
  makePutRequest(const QNetworkRequest& request, const QByteArray& data);

  std::tuple<QJsonObject, int, QNetworkReply::NetworkError>
  makeDeleteRequest(const QNetworkRequest& request);

  QString loginUser(const QString& userRole);

} // namespace test::utils
