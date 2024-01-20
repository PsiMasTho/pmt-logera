#include "../../lib/io.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static FILE* write_to_tmpfile(const char* contents)
{
    FILE* file = tmpfile();
    assert(file != NULL);
    size_t len = strlen(contents);
    size_t written = fwrite(contents, 1, len, file);
    assert(written == len);
    rewind(file);
    assert(file != NULL);
    return file;
}

static void test_io_single_read(void)
{
    char test_text[] = "hello world";
    FILE* file = write_to_tmpfile(test_text);
    char terminator[] = "WOWO";
    terminated_reader reader = terminated_reader_create(file, terminator, sizeof(terminator));
    char buffer[1024];
    terminated_reader_read(&reader, buffer, 1024);
    assert(memcmp(buffer, "hello worldWOWO", (sizeof(test_text) - 1) + (sizeof(terminator)) - 1) == 0);
    assert(terminated_reader_done(&reader));
    assert(!terminated_reader_error(&reader));
    fclose(file);
}

static void test_io_multi_read(void)
{
    char test_text[] = "hello. how are you? i am fine. thank you. and you? i am fine too. goodbye. have a nice day. goodbye."\
                       "hello. how are you? i am fine. thank you. and you? i am fine too. goodbye. have a nice day. goodbye."\
                       "hello. how are you? i am fine. thank you. and you? i am fine too. goodbye. have a nice day. goodbye.";
    FILE* file = write_to_tmpfile(test_text);
    char terminator[] = "TERM";
    terminated_reader reader = terminated_reader_create(file, terminator, sizeof(terminator));
    char* dest = malloc(1024);
    char buffer[16];
    size_t total_read = 0;
    while (!terminated_reader_done(&reader))
    {
        size_t read = terminated_reader_read(&reader, buffer, 16);
        memcpy(dest + total_read, buffer, read);
        total_read += read;
    }

    char intended_result[] = "hello. how are you? i am fine. thank you. and you? i am fine too. goodbye. have a nice day. goodbye."\
                            "hello. how are you? i am fine. thank you. and you? i am fine too. goodbye. have a nice day. goodbye."\
                            "hello. how are you? i am fine. thank you. and you? i am fine too. goodbye. have a nice day. goodbye."\
                            "TERM";
    
    assert(memcmp(dest, intended_result, sizeof(intended_result)) == 0);
    free(dest);
    fclose(file);
}

void test_basename_from_path_unix(void)
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

void test_basename_from_path_windows(void)
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

void test_io(void)
{
    test_io_single_read();
    test_io_multi_read();
    test_basename_from_path_unix();
    test_basename_from_path_windows();
}
