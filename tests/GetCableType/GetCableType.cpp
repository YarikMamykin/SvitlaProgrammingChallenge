#include <MockApiServer.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTest>
#include <utils.h>

class GetCableType : public QObject {
  Q_OBJECT

private slots:
  void getCableTypeByIdTest_data();
  void getCableTypeByIdTest();

  void getCableTypeByIdentifierTest_data();
  void getCableTypeByIdentifierTest();

  void getCableTypeByCatIdTest_data();
  void getCableTypeByCatIdTest();

  void getCableTypeByIdentifierAndCustomerCodeTest_data();
  void getCableTypeByIdentifierAndCustomerCodeTest();

  void getCableTypeByCatIdAndCustomerCodeTest_data();
  void getCableTypeByCatIdAndCustomerCodeTest();
};

namespace {
  static constexpr char responseBodyRaw[] = R"(
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

void GetCableType::getCableTypeByIdTest_data() {
  QTest::addColumn<QString>("userRole");
  QTest::addColumn<QString>("testId");
  QTest::addColumn<QJsonObject>("expectedResponseBody");
  QTest::addColumn<int>("expectedResultCode");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<test::api::MockApiServer::State>("apiState");

  QString testId{ "5f3bc9e2502422053e08f9f1" };
  auto validResponseBody = QJsonDocument::fromJson(responseBodyRaw).object();

  QTest::newRow("Superuser makes valid request, created cable type object "
                "returned in response and response code 200")
      << "superuser" << testId << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Admin makes valid request, created cable type object returned "
                "in response and response code 200")
      << "admin" << testId << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("User makes valid request, created cable type object returned "
                "in response and response code 200")
      << "user" << testId << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  auto testIdTooLong = "5f3bc9e2502422053e08f9f19";
  QTest::newRow("Request URL contains cable type id that is too long, error "
                "message with response code 400 returned")
      << "user" << testIdTooLong
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type id has invalid format"})")
             .object()
      << 400 << QNetworkReply::NetworkError::ProtocolInvalidOperationError
      << test::api::MockApiServer::State::Normal;

  auto testIdTooShort = "5f3d";
  QTest::newRow("Request URL contains cable type id that is too short, error "
                "message with response code 400 returned")
      << "user" << testIdTooShort
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type id has invalid format"})")
             .object()
      << 400 << QNetworkReply::NetworkError::ProtocolInvalidOperationError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("No token provided in request, error message with response "
                "code 401 returned (no permissions)")
      << "" << testId
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Request with mismatch of customer and user ids, error message "
                "with response code 403 returned")
      << "admin" << testId
      << QJsonDocument::fromJson(
             R"({"cause": "Attempt to access another customer data"})")
             .object()
      << 403 << QNetworkReply::NetworkError::ContentAccessDenied
      << test::api::MockApiServer::State::AttemptToAccessAnotherCustomerData;

  auto nonExistingTestId = "4f3bc9e2502422053e08f9f1";
  QTest::newRow("Request with non existing customer id, error message with "
                "response code 404 returned")
      << "user" << nonExistingTestId
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type doesn't exists by specified id"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Database rejected transaction, error message with response "
                "code 417 returned")
      << "user" << testId
      << QJsonDocument::fromJson(
             R"({"cause": "Database rejected transaction"})")
             .object()
      << 417 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRejectedTransaction;

  QTest::newRow(
      "Database unhandled error, error message with response code 422 returned")
      << "user" << testId
      << QJsonDocument::fromJson(R"({"cause": "Database unhandled error"})")
             .object()
      << 422 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseUnhandledError;

  QTest::newRow(
      "Database request timeout, error message with response code 424 returned")
      << "user" << testId
      << QJsonDocument::fromJson(R"({"cause": "Database request timeout"})")
             .object()
      << 424 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRequestTimeout;

  QTest::newRow("Database connection error, error message with response code "
                "500 returned")
      << "user" << testId
      << QJsonDocument::fromJson(R"({"cause": "Database connection error"})")
             .object()
      << 500 << QNetworkReply::NetworkError::InternalServerError
      << test::api::MockApiServer::State::DatabaseConnectionError;
}

void GetCableType::getCableTypeByIdTest() {
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
      test::utils::makeGetRequest(request);

  QCOMPARE(responseObject, expectedResponseBody);
  QCOMPARE(returnCode, expectedResultCode);
  QCOMPARE(networkError, expectedNetworkError);
}

void GetCableType::getCableTypeByIdentifierTest_data() {
  QTest::addColumn<QString>("userRole");
  QTest::addColumn<QString>("testIdentifier");
  QTest::addColumn<QJsonObject>("expectedResponseBody");
  QTest::addColumn<int>("expectedResultCode");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<test::api::MockApiServer::State>("apiState");

  QString testId{ "10-al-1c-trxple" };
  auto validResponseBody = QJsonDocument::fromJson(responseBodyRaw).object();

  QTest::newRow("Superuser makes valid request, created cable type object "
                "returned in response and response code 200")
      << "superuser" << testId << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Admin makes valid request, created cable type object returned "
                "in response and response code 200")
      << "admin" << testId << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("User makes valid request, created cable type object returned "
                "in response and response code 200")
      << "user" << testId << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("No token provided in request, error message with response "
                "code 401 returned (no permissions)")
      << "" << testId
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  auto nonExistingTestIdentifier = "4f3bc9e2502422053e08f9f1";
  QTest::newRow("Request with non existing customer id, error message with "
                "response code 404 returned")
      << "user" << nonExistingTestIdentifier
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type not found by identifier"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Database rejected transaction, error message with response "
                "code 417 returned")
      << "user" << testId
      << QJsonDocument::fromJson(
             R"({"cause": "Database rejected transaction"})")
             .object()
      << 417 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRejectedTransaction;

  QTest::newRow(
      "Database unhandled error, error message with response code 422 returned")
      << "user" << testId
      << QJsonDocument::fromJson(R"({"cause": "Database unhandled error"})")
             .object()
      << 422 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseUnhandledError;

  QTest::newRow(
      "Database request timeout, error message with response code 424 returned")
      << "user" << testId
      << QJsonDocument::fromJson(R"({"cause": "Database request timeout"})")
             .object()
      << 424 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRequestTimeout;

  QTest::newRow("Database connection error, error message with response code "
                "500 returned")
      << "user" << testId
      << QJsonDocument::fromJson(R"({"cause": "Database connection error"})")
             .object()
      << 500 << QNetworkReply::NetworkError::InternalServerError
      << test::api::MockApiServer::State::DatabaseConnectionError;
}

void GetCableType::getCableTypeByIdentifierTest() {
  QFETCH(QString, userRole);
  QFETCH(QString, testIdentifier);
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
      QUrl(QString("http://localhost:8080/cable/type/identifier/%1")
               .arg(testIdentifier)));
  if (not userRole.isEmpty()) {
    request.setRawHeader("Authorization", token.toLocal8Bit());
  }
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    QString("application/json"));
  auto [responseObject, returnCode, networkError] =
      test::utils::makeGetRequest(request);

  QCOMPARE(responseObject, expectedResponseBody);
  QCOMPARE(returnCode, expectedResultCode);
  QCOMPARE(networkError, expectedNetworkError);
}

void GetCableType::getCableTypeByCatIdTest_data() {
  QTest::addColumn<QString>("userRole");
  QTest::addColumn<int>("catid");
  QTest::addColumn<QJsonObject>("expectedResponseBody");
  QTest::addColumn<int>("expectedResultCode");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<test::api::MockApiServer::State>("apiState");

  const int catid{ 1622475 };
  auto validResponseBody = QJsonDocument::fromJson(responseBodyRaw).object();

  QTest::newRow("Superuser makes valid request, created cable type object "
                "returned in response and response code 200")
      << "superuser" << catid << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Admin makes valid request, created cable type object returned "
                "in response and response code 200")
      << "admin" << catid << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("User makes valid request, created cable type object returned "
                "in response and response code 200")
      << "user" << catid << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("No token provided in request, error message with response "
                "code 401 returned (no permissions)")
      << "" << catid
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  auto nonExistingTestIdentifier = 104;
  QTest::newRow("Request with non existing customer id, error message with "
                "response code 404 returned")
      << "user" << nonExistingTestIdentifier
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type not found by catid"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Database rejected transaction, error message with response "
                "code 417 returned")
      << "user" << catid
      << QJsonDocument::fromJson(
             R"({"cause": "Database rejected transaction"})")
             .object()
      << 417 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRejectedTransaction;

  QTest::newRow(
      "Database unhandled error, error message with response code 422 returned")
      << "user" << catid
      << QJsonDocument::fromJson(R"({"cause": "Database unhandled error"})")
             .object()
      << 422 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseUnhandledError;

  QTest::newRow(
      "Database request timeout, error message with response code 424 returned")
      << "user" << catid
      << QJsonDocument::fromJson(R"({"cause": "Database request timeout"})")
             .object()
      << 424 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRequestTimeout;

  QTest::newRow("Database connection error, error message with response code "
                "500 returned")
      << "user" << catid
      << QJsonDocument::fromJson(R"({"cause": "Database connection error"})")
             .object()
      << 500 << QNetworkReply::NetworkError::InternalServerError
      << test::api::MockApiServer::State::DatabaseConnectionError;
}

void GetCableType::getCableTypeByCatIdTest() {
  QFETCH(QString, userRole);
  QFETCH(int, catid);
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
      QUrl(QString("http://localhost:8080/cable/type/catid/%1").arg(catid)));
  if (not userRole.isEmpty()) {
    request.setRawHeader("Authorization", token.toLocal8Bit());
  }
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    QString("application/json"));
  auto [responseObject, returnCode, networkError] =
      test::utils::makeGetRequest(request);

  QCOMPARE(responseObject, expectedResponseBody);
  QCOMPARE(returnCode, expectedResultCode);
  QCOMPARE(networkError, expectedNetworkError);
}

void GetCableType::getCableTypeByIdentifierAndCustomerCodeTest_data() {
  QTest::addColumn<QString>("userRole");
  QTest::addColumn<QString>("testIdentifier");
  QTest::addColumn<QString>("testCustomerCode");
  QTest::addColumn<QJsonObject>("expectedResponseBody");
  QTest::addColumn<int>("expectedResultCode");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<test::api::MockApiServer::State>("apiState");

  QString testIdentifier{ "10-al-1c-trxple" };
  QString testCustomerCode{ "bge" };

  auto validResponseBody = QJsonDocument::fromJson(responseBodyRaw).object();

  QTest::newRow("Superuser makes valid request, created cable type object "
                "returned in response and response code 200")
      << "superuser" << testIdentifier << testCustomerCode << validResponseBody
      << 200 << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Admin makes valid request, error message with response code "
                "401 returned (no permissions)")
      << "admin" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("User makes valid request, error message with response code "
                "401 returned (no permissions)")
      << "user" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("No token provided in request, error message with response "
                "code 401 returned (no permissions)")
      << "" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QString nonExistingTestIdentifier{ "11-al-1c-trxple" };
  QTest::newRow("Request with non existing identifier, error message with "
                "response code 404 returned")
      << "superuser" << nonExistingTestIdentifier << testCustomerCode
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type not found by identifier"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QString nonExistingTestCustomerCode{ "abc" };
  QTest::newRow("Request with non existing customer id, error message with "
                "response code 404 returned")
      << "superuser" << testIdentifier << nonExistingTestCustomerCode
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type not found by customer code"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Database rejected transaction, error message with response "
                "code 417 returned")
      << "superuser" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(
             R"({"cause": "Database rejected transaction"})")
             .object()
      << 417 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRejectedTransaction;

  QTest::newRow(
      "Database unhandled error, error message with response code 422 returned")
      << "superuser" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Database unhandled error"})")
             .object()
      << 422 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseUnhandledError;

  QTest::newRow(
      "Database request timeout, error message with response code 424 returned")
      << "superuser" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Database request timeout"})")
             .object()
      << 424 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRequestTimeout;

  QTest::newRow("Database connection error, error message with response code "
                "500 returned")
      << "superuser" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Database connection error"})")
             .object()
      << 500 << QNetworkReply::NetworkError::InternalServerError
      << test::api::MockApiServer::State::DatabaseConnectionError;
}

void GetCableType::getCableTypeByIdentifierAndCustomerCodeTest() {
  QFETCH(QString, userRole);
  QFETCH(QString, testIdentifier);
  QFETCH(QString, testCustomerCode);
  QFETCH(QJsonObject, expectedResponseBody);
  QFETCH(int, expectedResultCode);
  QFETCH(QNetworkReply::NetworkError, expectedNetworkError);
  QFETCH(test::api::MockApiServer::State, apiState);

  test::api::MockApiServer apiServer{ apiState };
  auto token = test::utils::loginUser(userRole);

  if (not userRole.isEmpty()) {
    QCOMPARE(155ul, token.size());
  }

  QNetworkRequest request(QUrl(
      QString("http://localhost:8080/cable/type/identifier/%1/customer/code/%2")
          .arg(testIdentifier)
          .arg(testCustomerCode)));
  if (not userRole.isEmpty()) {
    request.setRawHeader("Authorization", token.toLocal8Bit());
  }
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    QString("application/json"));
  auto [responseObject, returnCode, networkError] =
      test::utils::makeGetRequest(request);

  QCOMPARE(responseObject, expectedResponseBody);
  QCOMPARE(returnCode, expectedResultCode);
  QCOMPARE(networkError, expectedNetworkError);
}

void GetCableType::getCableTypeByCatIdAndCustomerCodeTest_data() {
  QTest::addColumn<QString>("userRole");
  QTest::addColumn<int>("catid");
  QTest::addColumn<QString>("testCustomerCode");
  QTest::addColumn<QJsonObject>("expectedResponseBody");
  QTest::addColumn<int>("expectedResultCode");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<test::api::MockApiServer::State>("apiState");

  int catid{ 1622475 };
  QString testCustomerCode{ "bge" };

  auto validResponseBody = QJsonDocument::fromJson(responseBodyRaw).object();

  QTest::newRow("Superuser makes valid request, created cable type object "
                "returned in response and response code 200")
      << "superuser" << catid << testCustomerCode << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Admin makes valid request, error message with response code "
                "401 returned (no permissions)")
      << "admin" << catid << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("User makes valid request, error message with response code "
                "401 returned (no permissions)")
      << "user" << catid << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("No token provided in request, error message with response "
                "code 401 returned (no permissions)")
      << "" << catid << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  int nonExistingTestCatId{ 192 };
  QTest::newRow("Request with non existing catid, error message with response "
                "code 404 returned")
      << "superuser" << nonExistingTestCatId << testCustomerCode
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type not found by identifier"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QString nonExistingTestCustomerCode{ "abc" };
  QTest::newRow("Request with non existing customer id, error message with "
                "response code 404 returned")
      << "superuser" << catid << nonExistingTestCustomerCode
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type not found by customer code"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Database rejected transaction, error message with response "
                "code 417 returned")
      << "superuser" << catid << testCustomerCode
      << QJsonDocument::fromJson(
             R"({"cause": "Database rejected transaction"})")
             .object()
      << 417 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRejectedTransaction;

  QTest::newRow(
      "Database unhandled error, error message with response code 422 returned")
      << "superuser" << catid << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Database unhandled error"})")
             .object()
      << 422 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseUnhandledError;

  QTest::newRow(
      "Database request timeout, error message with response code 424 returned")
      << "superuser" << catid << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Database request timeout"})")
             .object()
      << 424 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRequestTimeout;

  QTest::newRow("Database connection error, error message with response code "
                "500 returned")
      << "superuser" << catid << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Database connection error"})")
             .object()
      << 500 << QNetworkReply::NetworkError::InternalServerError
      << test::api::MockApiServer::State::DatabaseConnectionError;
}

void GetCableType::getCableTypeByCatIdAndCustomerCodeTest() {
  QFETCH(QString, userRole);
  QFETCH(int, catid);
  QFETCH(QString, testCustomerCode);
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
      QUrl(QString("http://localhost:8080/cable/type/catid/%1/customer/code/%2")
               .arg(catid)
               .arg(testCustomerCode)));
  if (not userRole.isEmpty()) {
    request.setRawHeader("Authorization", token.toLocal8Bit());
  }
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    QString("application/json"));
  auto [responseObject, returnCode, networkError] =
      test::utils::makeGetRequest(request);

  QCOMPARE(responseObject, expectedResponseBody);
  QCOMPARE(returnCode, expectedResultCode);
  QCOMPARE(networkError, expectedNetworkError);
}

QTEST_MAIN(GetCableType)
#include "GetCableType.moc"
