#include "../../lib/errors.h"
#include "../../lib/io.h"
#include "../../lib/opaque_vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void test_basename_from_path_unix(void);

static void test_basename_from_path_windows(void);

static void test_readallf_big(void);

static void test_readallf(void);

static void write_file(char const* path, char const* contents);

// ----------------------------------------------------------------------------

void test_io(void)
{
    test_basename_from_path_unix();
    test_basename_from_path_windows();
    test_readallf_big();
    test_readallf();
}

// ----------------------------------------------------------------------------

static void test_basename_from_path_unix(void)
{
#define BUFFER_SIZE 1024
    char buffer[BUFFER_SIZE];
    basename_from_path("/home/username/test.txt", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "test.txt") == 0);
    basename_from_path("/home/username/test", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "test") == 0);
    basename_from_path("/home/username/", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "") == 0);
    basename_from_path("/home/username", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "username") == 0);
    basename_from_path("test.txt", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "test.txt") == 0);
    basename_from_path("test", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "test") == 0);
    basename_from_path("", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "") == 0);
}

static void test_basename_from_path_windows(void)
{
#define BUFFER_SIZE 1024
    char buffer[BUFFER_SIZE];
    basename_from_path("C:\\Users\\username\\test.txt", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "test.txt") == 0);
    basename_from_path("C:\\Users\\username\\test", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "test") == 0);
    basename_from_path("C:\\Users\\username\\", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "") == 0);
    basename_from_path("C:\\Users\\username", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "username") == 0);
    basename_from_path("test.txt", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "test.txt") == 0);
    basename_from_path("test", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "test") == 0);
    basename_from_path("", buffer, BUFFER_SIZE);
    assert(strcmp(buffer, "") == 0);
}

static void test_readallf_big(void)
{
    char const* path = "test_readallf_big.txt";
    char const outchars[] = "abcdefghijklmnopqrstuvwxyz\n";
    int const outchars_count = sizeof(outchars) - 1;

    opaque_vector orig_contents = opaque_vector_create(sizeof(char), opaque_vector_destroy_trivial);
    for (int i = 0; i < outchars_count * 10000; ++i)
        opaque_vector_push(&orig_contents, &outchars[i % outchars_count]);
    opaque_vector_push(&orig_contents, &(char){ '\0' });
    write_file(path, orig_contents.begin);

    opaque_vector buffer = opaque_vector_create(sizeof(char), opaque_vector_destroy_trivial);
    opaque_vector errors = opaque_vector_create(sizeof(file_error), file_error_destroy);

    assert(readallf(path, &buffer, &errors));
    assert(opaque_vector_size(&buffer) == opaque_vector_size(&orig_contents));
    assert(opaque_vector_size(&errors) == 0);
    assert(memcmp(buffer.begin, orig_contents.begin, opaque_vector_size(&buffer)) == 0);

    opaque_vector_destroy(&orig_contents);
    opaque_vector_destroy(&buffer);
    opaque_vector_destroy(&errors);
    remove(path);
}

static void test_readallf(void)
{
    char const orig_contents[] = "Hello, world! 1234567890";
    char const* path = "test_readallf.txt";
    write_file(path, orig_contents);

    opaque_vector buffer = opaque_vector_create(sizeof(char), opaque_vector_destroy_trivial);
    opaque_vector errors = opaque_vector_create(sizeof(file_error), file_error_destroy);

    assert(readallf(path, &buffer, &errors));
    assert(opaque_vector_size(&buffer) == sizeof(orig_contents));
    assert(opaque_vector_size(&errors) == 0);

    opaque_vector_destroy(&buffer);
    opaque_vector_destroy(&errors);
    remove(path);
}

static void write_file(char const* path, char const* contents)
{
    FILE* fp = fopen(path, "wb");
    assert(fp != NULL);
    fwrite(contents, 1, strlen(contents), fp);
    fclose(fp);
}
