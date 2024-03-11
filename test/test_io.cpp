//          Copyright (C) 2022 PsiMasTho (1cbb875@gmail.com)
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "logera/errors.hpp"
#include "logera/io.hpp"

#include <cassert>
#include <string>

using namespace std;

static void test_basename_from_path_unix();

static void test_basename_from_path_windows();

static void test_readallf_big();

static void test_readallf();

static void write_file(char const* path, string const& contents);

void test_io()
{
    test_basename_from_path_unix();
    test_basename_from_path_windows();
    test_readallf_big();
    test_readallf();
}

static void test_basename_from_path_unix()
{
    string_view result;
    result = io::basename_from_path("/home/username/test.txt");
    assert(result == "test.txt");
    result = io::basename_from_path("/home/username/test");
    assert(result == "test");
    result = io::basename_from_path("/home/username/");
    assert(result == "");
    result = io::basename_from_path("/home/username");
    assert(result == "username");
    result = io::basename_from_path("test.txt");
    assert(result == "test.txt");
    result = io::basename_from_path("test");
    assert(result == "test");
    result = io::basename_from_path("");
    assert(result == "");
}

static void test_basename_from_path_windows()
{
    string_view result;
    result = io::basename_from_path("C:\\Users\\username\\test.txt");
    assert(result == "test.txt");
    result = io::basename_from_path("C:\\Users\\username\\test");
    assert(result == "test");
    result = io::basename_from_path("C:\\Users\\username\\");
    assert(result == "");
    result = io::basename_from_path("C:\\Users\\username");
    assert(result == "username");
    result = io::basename_from_path("test.txt");
    assert(result == "test.txt");
    result = io::basename_from_path("test");
    assert(result == "test");
    result = io::basename_from_path("");
    assert(result == "");
}

static void test_readallf_big()
{
    char const* path           = "test_readallf_big.txt";
    char const  outchars[]     = "abcdefghijklmnopqrstuvwxyz\n";
    int const   outchars_count = sizeof(outchars) - 1;

    string orig_contents;
    for (int i = 0; i < outchars_count * 10000; ++i)
        orig_contents.push_back(outchars[i % outchars_count]);
    write_file(path, orig_contents.c_str());

    string           buffer;
    error::container errors;

    assert(io::readallf(path, buffer, errors));
    assert(buffer.size() == orig_contents.size());
    assert(errors.empty());
    assert(buffer == orig_contents);

    remove(path);
}

static void test_readallf()
{
    string      orig_contents = "Hello, world! 1234567890";
    char const* path          = "test_readallf.txt";
    write_file(path, orig_contents.c_str());

    string           buffer;
    error::container errors;

    assert(io::readallf(path, buffer, errors));
    assert(buffer.size() == orig_contents.size());
    assert(errors.empty());

    remove(path);
}

static void write_file(char const* path, string const& contents)
{
    FILE* fp = fopen(path, "wb");
    assert(fp != NULL);
    fwrite(contents.c_str(), 1, contents.size(), fp);
    fclose(fp);
}
