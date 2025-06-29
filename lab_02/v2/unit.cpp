#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "function.h"
volatile double minInMassiv = 1.7976931348623158e+308;
volatile double maxInMassiv = -1.7976931348623158e+308;
volatile double averageInMassiv = 0;
volatile int n = 0;


TEST_CASE("Test min_max 1") {
    double testArray[] = { 1.0, 2.0, 3.0, 4.0 };
    n = 4;

    minInMassiv = 1.7976931348623158e+308;
    maxInMassiv = -1.7976931348623158e+308;

    min_max(static_cast<LPVOID>(testArray));

    REQUIRE(minInMassiv == 1.0);
    REQUIRE(maxInMassiv == 4.0);
}


TEST_CASE("Test min_max 2") {
    double testArray[] = { 2.0, 2.0, 2.0, 2.0 };
    n = 4;

    minInMassiv = 1.7976931348623158e+308;
    maxInMassiv = -1.7976931348623158e+308;

    min_max(static_cast<LPVOID>(testArray));

    REQUIRE(minInMassiv == 2.0);
    REQUIRE(maxInMassiv == 2.0);
}

TEST_CASE("Test average 1") {
    double testArray[] = { 1.0, 2.0, 3.0, 4.0 };
    n = 4;

    averageInMassiv = 0;

    average(static_cast<LPVOID>(testArray));

    REQUIRE(averageInMassiv == Approx(2.5).epsilon(0.01));
}



TEST_CASE("Test average 2") {
    double testArray[] = { 3.0, 3.0, 3.0, 3.0, 3.0 };
    n = 5;

    averageInMassiv = 0;


    average(static_cast<LPVOID>(testArray));

    REQUIRE(averageInMassiv == Approx(3.0).epsilon(0.01));
}