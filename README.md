# SvitlaProgrammingChallenge

## Description

This repository contains implementation of integration tests for `create`, `read`, `update` and `delete` operations
on cable type entity provided in this response [sample](https://api-dev.reelsense.io/docs/#tag/CableType/operation/getCableTypes).

With no access to API itself for testing purposes API Mock was implemented.
API Mock immitates behaviour needed for tests.
For simplicity, login process is implemented straightforward by giving hardcoded authorisation token respectively to user id provided in request.
Afterwards, token retrieved this way is used for test requests to endpoints listed below.

For tests that require specific situations e.g. `database connection error` Mock API can accept specific `State` flag,
which will indicate what response is desired according to request and Mock API `State`.

## Dependencies

1. cmake >= 3.20
2. qt >= 6.4 (modules used: `Qt6::Core`, `Qt6::Test`, `Qt6::HttpServer`)
   Note: `Qt6::HttpServer` might require additional package installation depending on distribution you use.
   E.g. Archlinux requires `qt6-httpserver`

## Build

1. cmake -S . -B build
2. cmake --build build/ --parallel
3. cmake --install build/;

## Run tests

1. ctest --test-dir build/tests/ --verbose

## List of implemented endpoints and test cases for them

- /cable/type (POST)
  Creates cable type. Information is provided with request body.

  Test cases:

1.  Superuser makes valid request, created cable type object returned in response and response code 200
2.  Admin makes valid request, created cable type object returned in response and response code 200
3.  ID present in request, error message with response code 400 returned
4.  Rotation frequency unit value is invalid, error message with response code 400 returned
5.  Request without required keys, error message with response code 400 returned
6.  User makes valid request, error message with response code 401 returned (no permissions)
7.  No token provided in request, error message with response code 401 returned (no permissions)
8.  Request with mismatch of customer and user ids, error message with response code 403 returned
9.  Request with non existing customer id, error message with response code 404 returned
10. Request sent with already existing cable type, error message with response code 409 returned
11. Request sent with business rules violated, error message with response code 412 returned
12. Database rejected transaction, error message with response code 417 returned
13. Database unhandled error, error message with response code 422 returned
14. Database request timeout, error message with response code 424 returned
15. Database connection error, error message with response code 500 returned
16. Too large payload, error message with response code 507 returned

- /cable/type/id/{id} (PUT)
  Updates cable type by `id`.

  Test cases:

1.  Superuser makes valid request, created cable type object returned in response and response code 200
2.  Admin makes valid request, created cable type object returned in response and response code 200
3.  Rotation frequency unit value is invalid, error message with response code 400 returned
4.  Request without required keys, error message with response code 400 returned
5.  Request with mismatching ids in body and URL, error message with response code 400 returned
6.  User makes valid request, error message with response code 401 returned (no permissions)
7.  No token provided in request, error message with response code 401 returned (no permissions)
8.  Request with mismatch of customer and user ids, error message with response code 403 returned
9.  Request with non existing customer id, error message with response code 404 returned
10. Request sent with already existing cable type, error message with response code 409 returned
11. Request sent with business rules violated, error message with response code 412 returned
12. Database rejected transaction, error message with response code 417 returned
13. Database unhandled error, error message with response code 422 returned
14. Database request timeout, error message with response code 424 returned
15. Database connection error, error message with response code 500 returned
16. Too large payload, error message with response code 507 returned

- /cable/type/id/{id} (GET)
  Provides data about cable type by `id`.

  Test cases:

1. Superuser makes valid request, created cable type object returned in response and response code 200
2. Admin makes valid request, created cable type object returned in response and response code 200
3. User makes valid request, created cable type object returned in response and response code 200
4. Request URL contains cable type id that is too long, error message with response code 400 returned
5. Request URL contains cable type id that is too short, error message with response code 400 returned
6. No token provided in request, error message with response code 401 returned (no permissions)
7. Request with mismatch of customer and user ids, error message with response code 403 returned
8. Request with non existing customer id, error message with response code 404 returned
9. Database rejected transaction, error message with response code 417 returned
10. Database unhandled error, error message with response code 422 returned
11. Database request timeout, error message with response code 424 returned
12. Database connection error, error message with response code 500 returned

- /cable/type/identifier/{identifier} (GET)
  Provides data about cable type by `identifier`.

  Test cases:

1. Superuser makes valid request, created cable type object returned in response and response code 200
2. Admin makes valid request, created cable type object returned in response and response code 200
3. User makes valid request, created cable type object returned in response and response code 200
4. No token provided in request, error message with response code 401 returned (no permissions)
5. Request with non existing customer id, error message with response code 404 returned
6. Database rejected transaction, error message with response code 417 returned
7. Database unhandled error, error message with response code 422 returned
8. Database request timeout, error message with response code 424 returned
9. Database connection error, error message with response code 500 returned

- /cable/type/catid/{catid} (GET)
  Provides data about cable type by `catid`.

  Test cases:

1. Superuser makes valid request, created cable type object returned in response and response code 200
2. Admin makes valid request, created cable type object returned in response and response code 200
3. User makes valid request, created cable type object returned in response and response code 200
4. No token provided in request, error message with response code 401 returned (no permissions)
5. Request with non existing customer id, error message with response code 404 returned
6. Database rejected transaction, error message with response code 417 returned
7. Database unhandled error, error message with response code 422 returned
8. Database request timeout, error message with response code 424 returned
9. Database connection error, error message with response code 500 returned

- /cable/type/identifier/{identifier}/customer/code/{code} (GET)
  Provides data about cable type by `identifier` and `customer code`.

  Test cases:

1. Superuser makes valid request, created cable type object returned in response and response code 200
2. Admin makes valid request, error message with response code 401 returned (no permissions)
3. User makes valid request, error message with response code 401 returned (no permissions)
4. No token provided in request, error message with response code 401 returned (no permissions)
5. Request with non existing identifier, error message with response code 404 returned
6. Request with non existing customer id, error message with response code 404 returned
7. Database rejected transaction, error message with response code 417 returned
8. Database unhandled error, error message with response code 422 returned
9. Database request timeout, error message with response code 424 returned
10. Database connection error, error message with response code 500 returned

- /cable/type/catid/{catid}/customer/code/{code} (GET)
  Provides data about cable type by `catid` and `customer code`.

  Test cases:

1. Superuser makes valid request, created cable type object returned in response and response code 200
2. Admin makes valid request, error message with response code 401 returned (no permissions)
3. User makes valid request, error message with response code 401 returned (no permissions)
4. No token provided in request, error message with response code 401 returned (no permissions)
5. Request with non existing catid, error message with response code 404 returned
6. Request with non existing customer id, error message with response code 404 returned
7. Database rejected transaction, error message with response code 417 returned
8. Database unhandled error, error message with response code 422 returned
9. Database request timeout, error message with response code 424 returned
10. Database connection error, error message with response code 500 returned

- /cable/type/id/{id} (DELETE)
  Removes data about cable type by `id`.

  Test cases:

1. Superuser makes valid request, created cable type object returned in response and response code 200
2. Admin makes valid request, created cable type object returned in response and response code 200
3. Request URL contains cable type id that is too long, error message with response code 400 returned
4. Request URL contains cable type id that is too short, error message with response code 400 returned
5. User makes valid request, error message with response code 401 returned (no permissions)
6. No token provided in request, error message with response code 401 returned (no permissions)
7. Request with non existing customer id, error message with response code 404 returned
8. Request with cable type id which is referenced by other entities, error message with response code 412 returned
9. Database rejected transaction, error message with response code 417 returned
10. Database unhandled error, error message with response code 422 returned
11. Database request timeout, error message with response code 424 returned
12. Database connection error, error message with response code 500 returned

- /login/{userrole}
  This endpoint is tested as proof of concept for different userroles applied
  in test requests for endpoints listed above.

  Test cases:

1. `superuser` login, `superuser` token returned
2. `admin` login, `admin` token returned
3. `user` login, `user` token returned

