#include "utils.h"

namespace test::utils {
  std::tuple<QJsonObject, int, QNetworkReply::NetworkError>
  makeGetRequest(const QNetworkRequest& request) {

    QNetworkAccessManager manager;

    QNetworkReply* reply = manager.get(request);

    // Set up a QEventLoop to wait for the reply finished signal
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();

    auto replyBytes = reply->readAll();
    QJsonDocument replyData = QJsonDocument::fromJson(replyBytes);
    auto result = std::make_tuple(
        replyData.object(),
        reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute)
            .toInt(),
        reply->error());

    // Clean up
    reply->deleteLater();

    return result;
  }

  std::tuple<QJsonObject, int, QNetworkReply::NetworkError>
  makePostRequest(const QNetworkRequest& request, const QByteArray& data) {

    QNetworkAccessManager manager;

    QNetworkReply* reply = manager.post(request, data);

    // Set up a QEventLoop to wait for the reply finished signal
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();

    auto replyBytes = reply->readAll();
    QJsonDocument replyData = QJsonDocument::fromJson(replyBytes);
    auto result = std::make_tuple(
        replyData.object(),
        reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute)
            .toInt(),
        reply->error());

    // Clean up
    reply->deleteLater();

    return result;
  }

  std::tuple<QJsonObject, int, QNetworkReply::NetworkError>
  makePutRequest(const QNetworkRequest& request, const QByteArray& data) {

    QNetworkAccessManager manager;

    QNetworkReply* reply = manager.put(request, data);

    // Set up a QEventLoop to wait for the reply finished signal
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();

    auto replyBytes = reply->readAll();
    QJsonDocument replyData = QJsonDocument::fromJson(replyBytes);
    auto result = std::make_tuple(
        replyData.object(),
        reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute)
            .toInt(),
        reply->error());

    // Clean up
    reply->deleteLater();

    return result;
  }

  std::tuple<QJsonObject, int, QNetworkReply::NetworkError>
  makeDeleteRequest(const QNetworkRequest& request) {

    QNetworkAccessManager manager;

    QNetworkReply* reply = manager.deleteResource(request);

    // Set up a QEventLoop to wait for the reply finished signal
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();

    auto replyBytes = reply->readAll();
    QJsonDocument replyData = QJsonDocument::fromJson(replyBytes);
    auto result = std::make_tuple(
        replyData.object(),
        reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute)
            .toInt(),
        reply->error());

    // Clean up
    reply->deleteLater();

    return result;
  }

  QString loginUser(const QString& userRole) {
    QNetworkAccessManager manager;

    QNetworkRequest request(
        QUrl(QString("http://localhost:8080/login/%1").arg(userRole)));
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QString("application/json"));
    QNetworkReply* reply = manager.get(request);

    // Set up a QEventLoop to wait for the reply finished signal
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();

    auto replyBytes = reply->readAll();
    QJsonDocument response = QJsonDocument::fromJson(replyBytes);
    auto token = response.object()["jwtToken"].toString();

    // Clean up
    reply->deleteLater();
    return token;
  }
} // namespace test::utils
