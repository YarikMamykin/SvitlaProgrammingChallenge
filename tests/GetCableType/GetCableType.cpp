#include <MockApiServer.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTest>

class GetCableType : public QObject {
  Q_OBJECT

private:
  std::tuple<QJsonObject, int, QNetworkReply::NetworkError>
  makeGetRequest(const QNetworkRequest& request) {

    QNetworkAccessManager manager;

    QNetworkReply* reply = manager.get(request);

    // Set up a QEventLoop to wait for the reply finished signal
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

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
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    loop.exec();

    auto replyBytes = reply->readAll();
    QJsonDocument response = QJsonDocument::fromJson(replyBytes);
    auto token = response.object()["jwtToken"].toString();

    // Clean up
    reply->deleteLater();
    return token;
  }

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

void GetCableType::getCableTypeByIdTest_data() {
  auto requestBodyRaw = QString(R"(
    {
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
      "metadata": { }
    })");

  auto responseBodyRaw = QString(R"(
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
    })");

  QTest::addColumn<QString>("userRole");
  QTest::addColumn<QString>("testId");
  QTest::addColumn<QJsonObject>("expectedResponseBody");
  QTest::addColumn<int>("expectedResultCode");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<test::api::MockApiServer::State>("apiState");

  QString testId{ "5f3bc9e2502422053e08f9f1" };
  auto validResponseBody =
      QJsonDocument::fromJson(responseBodyRaw.toUtf8()).object();

  QTest::newRow("Superuser valid request body 200")
      << "superuser" << testId << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Admin valid request body 200")
      << "admin" << testId << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("User valid request body 200")
      << "user" << testId << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  auto testIdTooLong = "5f3bc9e2502422053e08f9f19";
  QTest::newRow("Cable type id is too long 400")
      << "user" << testIdTooLong
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type id has invalid format"})")
             .object()
      << 400 << QNetworkReply::NetworkError::ProtocolInvalidOperationError
      << test::api::MockApiServer::State::Normal;

  auto testIdTooShort = "5f3d";
  QTest::newRow("Cable type id is too short 400")
      << "user" << testIdTooShort
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type id has invalid format"})")
             .object()
      << 400 << QNetworkReply::NetworkError::ProtocolInvalidOperationError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Missing token valid request body 401")
      << "" << testId
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Mismatch of customer and user ids 403")
      << "admin" << testId
      << QJsonDocument::fromJson(
             R"({"cause": "Attempt to access another customer data"})")
             .object()
      << 403 << QNetworkReply::NetworkError::ContentAccessDenied
      << test::api::MockApiServer::State::AttemptToAccessAnotherCustomerData;

  auto nonExistingTestId = "4f3bc9e2502422053e08f9f1";
  QTest::newRow("Customer by id doesn't exist 404")
      << "user" << nonExistingTestId
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type doesn't exists by specified id"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Database rejected transaction 417")
      << "user" << testId
      << QJsonDocument::fromJson(
             R"({"cause": "Database rejected transaction"})")
             .object()
      << 417 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRejectedTransaction;

  QTest::newRow("Database unhandled error 422")
      << "user" << testId
      << QJsonDocument::fromJson(R"({"cause": "Database unhandled error"})")
             .object()
      << 422 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseUnhandledError;

  QTest::newRow("Database request timeout 424")
      << "user" << testId
      << QJsonDocument::fromJson(R"({"cause": "Database request timeout"})")
             .object()
      << 424 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRequestTimeout;

  QTest::newRow("Database request timeout 500")
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
  auto token = loginUser(userRole);

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
  auto [responseObject, returnCode, networkError] = makeGetRequest(request);

  QCOMPARE(responseObject, expectedResponseBody);
  QCOMPARE(returnCode, expectedResultCode);
  QCOMPARE(networkError, expectedNetworkError);
}

void GetCableType::getCableTypeByIdentifierTest_data() {
  auto requestBodyRaw = QString(R"(
    {
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
      "metadata": { }
    })");

  auto responseBodyRaw = QString(R"(
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
    })");

  QTest::addColumn<QString>("userRole");
  QTest::addColumn<QString>("testIdentifier");
  QTest::addColumn<QJsonObject>("expectedResponseBody");
  QTest::addColumn<int>("expectedResultCode");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<test::api::MockApiServer::State>("apiState");

  QString testId{ "10-al-1c-trxple" };
  auto validResponseBody =
      QJsonDocument::fromJson(responseBodyRaw.toUtf8()).object();

  QTest::newRow("Superuser valid request body 200")
      << "superuser" << testId << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Admin valid request body 200")
      << "admin" << testId << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("User valid request body 200")
      << "user" << testId << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Missing token valid request body 401")
      << "" << testId
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  auto nonExistingTestIdentifier = "4f3bc9e2502422053e08f9f1";
  QTest::newRow("Customer by id doesn't exist 404")
      << "user" << nonExistingTestIdentifier
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type not found by identifier"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Database rejected transaction 417")
      << "user" << testId
      << QJsonDocument::fromJson(
             R"({"cause": "Database rejected transaction"})")
             .object()
      << 417 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRejectedTransaction;

  QTest::newRow("Database unhandled error 422")
      << "user" << testId
      << QJsonDocument::fromJson(R"({"cause": "Database unhandled error"})")
             .object()
      << 422 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseUnhandledError;

  QTest::newRow("Database request timeout 424")
      << "user" << testId
      << QJsonDocument::fromJson(R"({"cause": "Database request timeout"})")
             .object()
      << 424 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRequestTimeout;

  QTest::newRow("Database request timeout 500")
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
  auto token = loginUser(userRole);

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
  auto [responseObject, returnCode, networkError] = makeGetRequest(request);

  QCOMPARE(responseObject, expectedResponseBody);
  QCOMPARE(returnCode, expectedResultCode);
  QCOMPARE(networkError, expectedNetworkError);
}

void GetCableType::getCableTypeByCatIdTest_data() {
  auto requestBodyRaw = QString(R"(
    {
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
      "metadata": { }
    })");

  auto responseBodyRaw = QString(R"(
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
    })");

  QTest::addColumn<QString>("userRole");
  QTest::addColumn<int>("catid");
  QTest::addColumn<QJsonObject>("expectedResponseBody");
  QTest::addColumn<int>("expectedResultCode");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<test::api::MockApiServer::State>("apiState");

  const int catid{ 1622475 };
  auto validResponseBody =
      QJsonDocument::fromJson(responseBodyRaw.toUtf8()).object();

  QTest::newRow("Superuser valid request body 200")
      << "superuser" << catid << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Admin valid request body 200")
      << "admin" << catid << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("User valid request body 200")
      << "user" << catid << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Missing token valid request body 401")
      << "" << catid
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  auto nonExistingTestIdentifier = 104;
  QTest::newRow("Customer by id doesn't exist 404")
      << "user" << nonExistingTestIdentifier
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type not found by catid"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Database rejected transaction 417")
      << "user" << catid
      << QJsonDocument::fromJson(
             R"({"cause": "Database rejected transaction"})")
             .object()
      << 417 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRejectedTransaction;

  QTest::newRow("Database unhandled error 422")
      << "user" << catid
      << QJsonDocument::fromJson(R"({"cause": "Database unhandled error"})")
             .object()
      << 422 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseUnhandledError;

  QTest::newRow("Database request timeout 424")
      << "user" << catid
      << QJsonDocument::fromJson(R"({"cause": "Database request timeout"})")
             .object()
      << 424 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRequestTimeout;

  QTest::newRow("Database request timeout 500")
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
  auto token = loginUser(userRole);

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
  auto [responseObject, returnCode, networkError] = makeGetRequest(request);

  QCOMPARE(responseObject, expectedResponseBody);
  QCOMPARE(returnCode, expectedResultCode);
  QCOMPARE(networkError, expectedNetworkError);
}

void GetCableType::getCableTypeByIdentifierAndCustomerCodeTest_data() {
  auto requestBodyRaw = QString(R"(
    {
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
      "metadata": { }
    })");

  auto responseBodyRaw = QString(R"(
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
    })");

  QTest::addColumn<QString>("userRole");
  QTest::addColumn<QString>("testIdentifier");
  QTest::addColumn<QString>("testCustomerCode");
  QTest::addColumn<QJsonObject>("expectedResponseBody");
  QTest::addColumn<int>("expectedResultCode");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<test::api::MockApiServer::State>("apiState");

  QString testIdentifier{ "10-al-1c-trxple" };
  QString testCustomerCode{ "bge" };

  auto validResponseBody =
      QJsonDocument::fromJson(responseBodyRaw.toUtf8()).object();

  QTest::newRow("Superuser valid request body 200")
      << "superuser" << testIdentifier << testCustomerCode << validResponseBody
      << 200 << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Missing token valid request body 401")
      << "admin" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Missing token valid request body 401")
      << "user" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Missing token valid request body 401")
      << "" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QString nonExistingTestIdentifier{ "11-al-1c-trxple" };
  QTest::newRow("Cable type by identifier doesn't exist 404")
      << "superuser" << nonExistingTestIdentifier << testCustomerCode
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type not found by identifier"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QString nonExistingTestCustomerCode{ "abc" };
  QTest::newRow("Cable type by customer code doesn't exist 404")
      << "superuser" << testIdentifier << nonExistingTestCustomerCode
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type not found by customer code"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Database rejected transaction 417")
      << "superuser" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(
             R"({"cause": "Database rejected transaction"})")
             .object()
      << 417 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRejectedTransaction;

  QTest::newRow("Database unhandled error 422")
      << "superuser" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Database unhandled error"})")
             .object()
      << 422 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseUnhandledError;

  QTest::newRow("Database request timeout 424")
      << "superuser" << testIdentifier << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Database request timeout"})")
             .object()
      << 424 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRequestTimeout;

  QTest::newRow("Database request timeout 500")
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
  auto token = loginUser(userRole);

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
  auto [responseObject, returnCode, networkError] = makeGetRequest(request);

  QCOMPARE(responseObject, expectedResponseBody);
  QCOMPARE(returnCode, expectedResultCode);
  QCOMPARE(networkError, expectedNetworkError);
}

void GetCableType::getCableTypeByCatIdAndCustomerCodeTest_data() {
  auto requestBodyRaw = QString(R"(
    {
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
      "metadata": { }
    })");

  auto responseBodyRaw = QString(R"(
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
    })");

  QTest::addColumn<QString>("userRole");
  QTest::addColumn<int>("catid");
  QTest::addColumn<QString>("testCustomerCode");
  QTest::addColumn<QJsonObject>("expectedResponseBody");
  QTest::addColumn<int>("expectedResultCode");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<test::api::MockApiServer::State>("apiState");

  int catid{ 1622475 };
  QString testCustomerCode{ "bge" };

  auto validResponseBody =
      QJsonDocument::fromJson(responseBodyRaw.toUtf8()).object();

  QTest::newRow("Superuser valid request body 200")
      << "superuser" << catid << testCustomerCode << validResponseBody << 200
      << QNetworkReply::NetworkError::NoError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Missing token valid request body 401")
      << "admin" << catid << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Missing token valid request body 401")
      << "user" << catid << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Missing token valid request body 401")
      << "" << catid << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Unauthorized"})").object() << 401
      << QNetworkReply::NetworkError::AuthenticationRequiredError
      << test::api::MockApiServer::State::Normal;

  int nonExistingTestCatId{ 192 };
  QTest::newRow("Cable type by identifier doesn't exist 404")
      << "superuser" << nonExistingTestCatId << testCustomerCode
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type not found by identifier"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QString nonExistingTestCustomerCode{ "abc" };
  QTest::newRow("Cable type by customer code doesn't exist 404")
      << "superuser" << catid << nonExistingTestCustomerCode
      << QJsonDocument::fromJson(
             R"({"cause": "Cable type not found by customer code"})")
             .object()
      << 404 << QNetworkReply::NetworkError::ContentNotFoundError
      << test::api::MockApiServer::State::Normal;

  QTest::newRow("Database rejected transaction 417")
      << "superuser" << catid << testCustomerCode
      << QJsonDocument::fromJson(
             R"({"cause": "Database rejected transaction"})")
             .object()
      << 417 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRejectedTransaction;

  QTest::newRow("Database unhandled error 422")
      << "superuser" << catid << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Database unhandled error"})")
             .object()
      << 422 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseUnhandledError;

  QTest::newRow("Database request timeout 424")
      << "superuser" << catid << testCustomerCode
      << QJsonDocument::fromJson(R"({"cause": "Database request timeout"})")
             .object()
      << 424 << QNetworkReply::NetworkError::UnknownContentError
      << test::api::MockApiServer::State::DatabaseRequestTimeout;

  QTest::newRow("Database request timeout 500")
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
  auto token = loginUser(userRole);

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
  auto [responseObject, returnCode, networkError] = makeGetRequest(request);

  QCOMPARE(responseObject, expectedResponseBody);
  QCOMPARE(returnCode, expectedResultCode);
  QCOMPARE(networkError, expectedNetworkError);
}

QTEST_MAIN(GetCableType)
#include "GetCableType.moc"
