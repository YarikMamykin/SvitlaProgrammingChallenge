#include <MockApiServer.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTest>
#include <utils.h>

class UpdateCableType : public QObject {
  Q_OBJECT

private slots:
  void updateCableTypeTest_data();
  void updateCableTypeTest();
};

namespace {
  static constexpr char requestBodyRaw[] = R"(
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
        "value": 20.43,
        "unit": "mAh"
      },
      "rotationFrequency": {
        "value": 22.43,
        "unit": "d"
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
      "metadata": { }
  })";
}

void UpdateCableType::updateCableTypeTest_data() {

  QTest::addColumn<QString>("userRole");
  QTest::addColumn<QJsonObject>("requestBody");
  QTest::addColumn<QJsonObject>("expectedResponseBody");
  QTest::addColumn<int>("expectedResultCode");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<test::api::MockApiServer::State>("apiState");
  QTest::addColumn<QString>("testId");

  /*
   * API Mock simply sends back request body if it fits requirements.
   */
  auto validRequestBody = QJsonDocument::fromJson(requestBodyRaw).object();
  auto validResponseBody = QJsonDocument::fromJson(requestBodyRaw).object();

  QString testId{ "5f3bc9e2502422053e08f9f1" };

  QTest::newRow("Superuser valid request body 200")
      << "superuser" << validRequestBody << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal << testId;

  QTest::newRow("Admin valid request body 200")
      << "admin" << validRequestBody << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal << testId;

  auto requestBodyWithInvalidRotationFrequencyUnit = validRequestBody;
  auto rotationFrequencyObject =
      requestBodyWithInvalidRotationFrequencyUnit["rotationFrequency"]
          .toObject();
  rotationFrequencyObject["unit"] = "g";
  requestBodyWithInvalidRotationFrequencyUnit["rotationFrequency"] =
      rotationFrequencyObject;
  QTest::newRow("Rotation frequency unit value is invalid 400")
      << "admin" << requestBodyWithInvalidRotationFrequencyUnit
      << QJsonDocument::fromJson(
             R"({"cause": "rotationFrequency.unit has invalid value"})")
             .object()
      << 400 << QNetworkReply::NetworkError::ProtocolInvalidOperationError
      << test::api::MockApiServer::State::Normal << testId;

  auto requestBodyWithoutRequiredKeys = validRequestBody;
  requestBodyWithoutRequiredKeys.remove("identifier");
  requestBodyWithoutRequiredKeys.remove("catid");
  QTest::newRow("Request without required keys 400")
      << "admin" << requestBodyWithoutRequiredKeys
      << QJsonDocument::fromJson(R"({"cause": "missing required keys"})")
             .object()
      << 400 << QNetworkReply::NetworkError::ProtocolInvalidOperationError
      << test::api::MockApiServer::State::Normal << testId;

  QString otherTestId{ "4f3bc9e2502422053e08f9f2" };
  QTest::newRow("Request with mismatching ids in body and URL 400")
      << "admin" << validRequestBody
      << QJsonDocument::fromJson(
             R"({"cause": "id mismatch for URL and request body"})")
             .object()
      << 400 << QNetworkReply::NetworkError::ProtocolInvalidOperationError
      << test::api::MockApiServer::State::Normal << otherTestId;

  QTest::newRow("User valid request body 401")
      << "user" << validRequestBody
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal << testId;

  QTest::newRow("Missing token valid request body 401")
      << "" << validRequestBody
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal << testId;

  QTest::newRow("Mismatch of customer and user ids 403")
      << "admin" << validRequestBody
      << QJsonDocument::fromJson(
             R"({"cause": "Attempt to access another customer data"})")
             .object()
      << 403 << QNetworkReply::NetworkError::ContentAccessDenied
      << test::api::MockApiServer::State::AttemptToAccessAnotherCustomerData
      << testId;

  QTest::newRow("Customer by id doesn't exist 404")
      << "admin" << validRequestBody
      << QJsonDocument::fromJson(
             R"({"cause": "Non existing customer id specified"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::NonExistingCustomerId << testId;

  QTest::newRow("Customer by id doesn't exist 409")
      << "admin" << validRequestBody
      << QJsonDocument::fromJson(R"({"cause": "Cable type already exists"})")
             .object()
      << 409 << QNetworkReply::NetworkError::ContentConflictError
      << test::api::MockApiServer::State::CableTypeAlreadyExists << testId;

  QTest::newRow("Business rules violated 412")
      << "admin" << validRequestBody
      << QJsonDocument::fromJson(R"({"cause": "Business rules violated"})")
             .object()
      << 412 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::BusinessRulesViolated << testId;

  QTest::newRow("Database rejected transaction 417")
      << "admin" << validRequestBody
      << QJsonDocument::fromJson(
             R"({"cause": "Database rejected transaction"})")
             .object()
      << 417 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRejectedTransaction << testId;

  QTest::newRow("Database unhandled error 422")
      << "admin" << validRequestBody
      << QJsonDocument::fromJson(R"({"cause": "Database unhandled error"})")
             .object()
      << 422 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseUnhandledError << testId;

  QTest::newRow("Database request timeout 424")
      << "admin" << validRequestBody
      << QJsonDocument::fromJson(R"({"cause": "Database request timeout"})")
             .object()
      << 424 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRequestTimeout << testId;

  QTest::newRow("Database request timeout 500")
      << "admin" << validRequestBody
      << QJsonDocument::fromJson(R"({"cause": "Database connection error"})")
             .object()
      << 500 << QNetworkReply::NetworkError::InternalServerError
      << test::api::MockApiServer::State::DatabaseConnectionError << testId;

  QTest::newRow("Database request timeout 507")
      << "admin" << validRequestBody
      << QJsonDocument::fromJson(R"({"cause": "Too large payload"})").object()
      << 507 << QNetworkReply::NetworkError::UnknownServerError
      << test::api::MockApiServer::State::TooLargePayload << testId;
}

void UpdateCableType::updateCableTypeTest() {
  QFETCH(QString, userRole);
  QFETCH(QJsonObject, requestBody);
  QFETCH(QJsonObject, expectedResponseBody);
  QFETCH(int, expectedResultCode);
  QFETCH(QNetworkReply::NetworkError, expectedNetworkError);
  QFETCH(test::api::MockApiServer::State, apiState);
  QFETCH(QString, testId);

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
      test::utils::makePutRequest(request, QJsonDocument(requestBody).toJson());

  QCOMPARE(responseObject, expectedResponseBody);
  QCOMPARE(returnCode, expectedResultCode);
  QCOMPARE(networkError, expectedNetworkError);
}

QTEST_MAIN(UpdateCableType)
#include "UpdateCableType.moc"
