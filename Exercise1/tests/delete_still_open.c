#include "../fs/operations.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/*
This test tests the permanent deletion
of a file in the file system. The unlink
function only returns unsuccessful if the
target file is pointing to an inode with
the hard link count = 1 and is not a 
symbolic link.
*/

int main() {
    char *path = "/f1";

    tfs_params params = tfs_default_params();
    params.max_inode_count = 2;
    params.max_block_count = 2;
    assert(tfs_init(&params) != -1);

    // Create file
    int fd = tfs_open(path, TFS_O_CREAT);
    assert(fd != -1);

    const char write_contents[] = "Hello World!";

    // Write to file
    assert(tfs_write(fd, write_contents, sizeof(write_contents)));

    // Unlink file
    assert(tfs_unlink(path) == -1);

    // Create new file with the same name
    fd = tfs_open(path, TFS_O_CREAT);
    assert(fd != -1);

    // Check if new file is empty
    char read_contents[sizeof(write_contents)];
    tfs_read(fd, read_contents, sizeof(read_contents));
    assert(!strcmp(read_contents, "Hello World!"));

    printf("Successful test.\n");
}
