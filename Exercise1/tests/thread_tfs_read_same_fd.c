#include "../fs/operations.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define THREADS 15

/*
This program tests if the tfs_read function is
thread-safe by making THREADS threads reading from
the same file descriptor. If the it wasn't thread-
-safe, in the final string duplicates would show.
*/

char final[100];

void* read(void* fhandle) {

    int f = *(int*)fhandle;

    char buffer[1];
    ssize_t r; 

    while ((r = tfs_read(f, buffer, sizeof(buffer))) > 0) {
        strncat(final, buffer, (size_t)r);
    }
    
    return NULL;
}

int main() {

    char *str_ext_file = "123456789abcdefghijklmnopqrstuvwxyz";
    char *path_copied_file = "/f1";
    char *path_src = "tests/tfs_read.txt";
    pthread_t tid[THREADS];

    assert(tfs_init(NULL) != -1);

    int f;

    f = tfs_copy_from_external_fs(path_src, path_copied_file);
    assert(f != -1);

    f = tfs_open(path_copied_file, TFS_O_CREAT);
    assert(f != -1);

    //read((void*)&f);
    for (int i = 0; i < THREADS; i++) {
        pthread_create(&tid[i], NULL, read, (void*)&f);
    }

    for (int i = 0; i < THREADS; i++){
        pthread_join(tid[i], NULL);
    }
 
    tfs_close(f);

    tfs_destroy();

    size_t final_size = strlen(final);
    size_t size = strlen(str_ext_file);
    assert(final_size == size);

    printf("Successful test.\n");
    return 0;

}
