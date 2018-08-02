#include <string>

#include <catch.hpp>
#include <common/Utils.hpp>

namespace cutils = ctb::common::utils;

TEST_CASE("Get filename from path") {
    const std::string path1 = "/tmp/a/b/foo.txt";

    REQUIRE(cutils::getFileName(path1) == "foo.txt");
    REQUIRE(cutils::getFileName("bar.txt") == "bar.txt");
    REQUIRE(cutils::getFileName("a") == "a");
    REQUIRE(cutils::getFileName("abc/d/foo.txt") == "foo.txt");
    REQUIRE(cutils::getFileName("abc/d/") == ".");  // Boost definiton
    REQUIRE(cutils::getFileName("") == "");

    SECTION("Consistency") {
        REQUIRE(cutils::getFileName(path1) == "foo.txt");
        REQUIRE(cutils::getFileName(path1) == "foo.txt");
        REQUIRE(cutils::getFileName(path1) == "foo.txt");
    }
}

TEST_CASE("Get directory path") {
    REQUIRE(cutils::getDirectoryPath("/tmp/a/b/foo.txt") == "/tmp/a/b");
    REQUIRE(cutils::getDirectoryPath("/tmp/a/b/") == "/tmp/a/b");
    REQUIRE(cutils::getDirectoryPath("/tmp/a/b") == "/tmp/a");

    REQUIRE(cutils::getDirectoryPath("") == "");
    REQUIRE(cutils::getDirectoryPath(".") == "");  // Boost definiton
    REQUIRE(cutils::getDirectoryPath("a/..") == "a");
    REQUIRE(cutils::getDirectoryPath("a") == "");
    REQUIRE(cutils::getDirectoryPath("abc/d/foo.txt") == "abc/d");
}

TEST_CASE("Get directory with slash path") {
    REQUIRE(cutils::getDirectoryPathWithSlash("/tmp/a/b/foo.txt") == "/tmp/a/b/");
    REQUIRE(cutils::getDirectoryPathWithSlash("/tmp/a/b/") == "/tmp/a/b/");
    REQUIRE(cutils::getDirectoryPathWithSlash("/tmp/a/b") == "/tmp/a/");

    REQUIRE(cutils::getDirectoryPathWithSlash("") == "");
    REQUIRE(cutils::getDirectoryPathWithSlash(".") == "");  // Boost definiton
    REQUIRE(cutils::getDirectoryPathWithSlash("a/..") == "a/");
    REQUIRE(cutils::getDirectoryPathWithSlash("a") == "");
    REQUIRE(cutils::getDirectoryPathWithSlash("abc/d/foo.txt") == "abc/d/");
}
