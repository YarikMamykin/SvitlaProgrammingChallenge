#include <MockApiServer.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTest>
#include <utils.h>

class DeleteCableType : public QObject {
  Q_OBJECT

private slots:
  void deleteCableTypeTest_data();
  void deleteCableTypeTest();
};

void DeleteCableType::deleteCableTypeTest_data() {
  QTest::addColumn<QString>("userRole");
  QTest::addColumn<QString>("testId");
  QTest::addColumn<QJsonObject>("expectedResponseBody");
  QTest::addColumn<int>("expectedResultCode");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<test::api::MockApiServer::State>("apiState");

  QString validTestId{ "5f3bc9e2502422053e08f9f1" };
  QTest::newRow("Superuser valid id 200")
      << "superuser" << validTestId << QJsonObject{} << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Admin valid id 200")
      << "admin" << validTestId << QJsonObject{} << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  auto testIdTooLong = "5f3bc9e2502422053e08f9f19";
  QTest::newRow("Cable type id is too long 400")
      << "admin" << testIdTooLong
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type id has invalid format"})")
             .object()
      << 400 << QNetworkReply::NetworkError::ProtocolInvalidOperationError
      << test::api::MockApiServer::State::Normal;

  auto testIdTooShort = "5f3d";
  QTest::newRow("Cable type id is too short 400")
      << "admin" << testIdTooShort
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type id has invalid format"})")
             .object()
      << 400 << QNetworkReply::NetworkError::ProtocolInvalidOperationError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("User valid request body 401")
      << "user" << validTestId
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Missing token valid request body 401")
      << "" << validTestId
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QString nonExistingTestId{ "4f3bc9e2502422053e08f9f1" };
  QTest::newRow("Customer by id doesn't exist 404")
      << "admin" << nonExistingTestId
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type doesn't exists by specified id"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Cable is referenced by other entities 412")
      << "admin" << validTestId
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type is referenced by other entities"})")
             .object()
      << 412 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::CableTypeReferencedByOtherEntities;

  QTest::newRow("Database rejected transaction 417")
      << "admin" << validTestId
      << QJsonDocument::fromJson(
             R"({"cause": "Database rejected transaction"})")
             .object()
      << 417 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRejectedTransaction;

  QTest::newRow("Database unhandled error 422")
      << "admin" << validTestId
      << QJsonDocument::fromJson(R"({"cause": "Database unhandled error"})")
             .object()
      << 422 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseUnhandledError;

  QTest::newRow("Database request timeout 424")
      << "admin" << validTestId
      << QJsonDocument::fromJson(R"({"cause": "Database request timeout"})")
             .object()
      << 424 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRequestTimeout;

  QTest::newRow("Database request timeout 500")
      << "admin" << validTestId
      << QJsonDocument::fromJson(R"({"cause": "Database connection error"})")
             .object()
      << 500 << QNetworkReply::NetworkError::InternalServerError
      << test::api::MockApiServer::State::DatabaseConnectionError;
}

void DeleteCableType::deleteCableTypeTest() {
  QFETCH(QString, userRole);
  QFETCH(QString, testId);
  QFETCH(QJsonObject, expectedResponseBody);
  QFETCH(int, expectedResultCode);
  QFETCH(QNetworkReply::NetworkError, expectedNetworkError);
  QFETCH(test::api::MockApiServer::State, apiState);

  test::api::MockApiServer apiServer{ apiState };
  auto token = test::utils::loginUser(userRole);

  if (not userRole.isEmpty()) {
    QCOMPARE(155ul, token.size());
  }

  QNetworkRequest request(
      QUrl(QString("http://localhost:8080/cable/type/id/%1").arg(testId)));
  if (not userRole.isEmpty()) {
    request.setRawHeader("Authorization", token.toLocal8Bit());
  }
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    QString("application/json"));
  auto [responseObject, returnCode, networkError] =
      test::utils::makeDeleteRequest(request);

  QCOMPARE(responseObject, expectedResponseBody);
  QCOMPARE(returnCode, expectedResultCode);
  QCOMPARE(networkError, expectedNetworkError);
}

QTEST_MAIN(DeleteCableType)
#include "DeleteCableType.moc"
