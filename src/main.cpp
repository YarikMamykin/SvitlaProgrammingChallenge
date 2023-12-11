#include "MockApiServer.h"

#include <QCoreApplication>

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);
  test::api::MyRestApiServer restApiServer;
  return app.exec();
}
