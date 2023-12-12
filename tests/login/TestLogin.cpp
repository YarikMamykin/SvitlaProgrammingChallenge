#include <MockApiServer.h>
#include <QObject>
#include <QTest>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlogging.h>
#include <qtestcase.h>

class TestLogin : public QObject {
  Q_OBJECT

private:
  std::tuple<QJsonDocument, QNetworkReply::NetworkError>
  makeRequest(QUrl&& url) {

    QNetworkAccessManager manager;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QString("application/json"));
    QNetworkReply* reply = manager.get(request);

    // Set up a QEventLoop to wait for the reply finished signal
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();

    auto replyBytes = reply->readAll();
    qDebug() << replyBytes;
    QJsonDocument replyData = QJsonDocument::fromJson(replyBytes);
    auto result = std::make_tuple(replyData, reply->error());

    // Clean up
    reply->deleteLater();

    return result;
  }
private slots:
  void loginTest_data();
  void loginTest();
};

void TestLogin::loginTest_data() {
  QTest::addColumn<QString>("userrole");
  QTest::addColumn<QString>("expectedToken");

  QTest::newRow("superuserLogin")
      << "superuser"
      << "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
         "eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5M"
         "DIy"
         "fQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c";

  QTest::newRow("adminLogin")
      << "admin"
      << "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
         "fyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5M"
         "DIy"
         "fQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c";

  QTest::newRow("userLogin")
      << "user"
      << "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
         "gyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiaWF0IjoxNTE2MjM5M"
         "DIy"
         "fQ.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c";
}

void TestLogin::loginTest() {
  QFETCH(QString, userrole);
  QFETCH(QString, expectedToken);

  test::api::MyRestApiServer apiServer;
  auto [response, error] = makeRequest(
      QUrl(QString("http://localhost:8080/login/%1").arg(userrole)));
  QCOMPARE(error, QNetworkReply::NetworkError::NoError);
  QVERIFY(response.isObject());
  QVERIFY(response.object().contains("jwtToken"));
  QCOMPARE(response.object()["jwtToken"], expectedToken);
}

QTEST_MAIN(TestLogin)
#include "TestLogin.moc"
