// http://stackoverflow.com/questions/16997141/writing-structs-to-a-file-in-c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // open function
#include <unistd.h> // close function
#include <sys/stat.h>

#include "helpers.h"

#define DB_FILE "/tmp/batdb.dat"

struct Entity {
    int id;
    long lastTime;
    char fullName[30];
    char email[30];
    char description[200];
};

int main(int argc, char *argv[])
{
    puts("DB start...");

    int userid, fd, file_len, buffer_len = 100;
    char *buffer, *datafile;

    file_len = strlen(DB_FILE);

    buffer = (char*) malloc(buffer_len);
    datafile = (char*) malloc(file_len);
    strcpy(datafile, DB_FILE);

    if (argc < 2)
        usage(argv[0], datafile);


    strncpy(buffer, argv[1], buffer_len);

    printf("[DEBUG] buffer@ %p: \'%s\'\n", buffer, buffer);
    printf("[DEBUG] datafile @ %p: \'%s\'\n", datafile, datafile);

    // TEST CODE
    struct Entity entity;
    int entitySize = sizeof(entity);
    entity.id = 10;
    entity.lastTime = 123;

    memset(entity.fullName, 0, sizeof(entity.fullName));
    memset(entity.email, 0, sizeof(entity.email));
    memset(entity.description, 0, sizeof(entity.description));


    char *fullName = "Denis Dubinin";
    char *email = "dadubinin@gmail.com";
    char *description = "long description";

    strncpy(entity.fullName, fullName, strlen(fullName));
    strncpy(entity.email, email, strlen(email));
    strncpy(entity.description, description, strlen(description));

    printf("[DEBUG] entity@ %p: \n", &entity);

    // Opening file
    fd = open(datafile,O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
    if (fd == -1)
        fatal("in main() while opening file");

    // writing data
    if(write(fd, &entity, entitySize) == -1) 
        fatal("in main while writing entity to file");
    write(fd, "\n", 1);

    if (close(fd) == -1)
        fatal("in main while closing file");

    printf("Saved to DB\n");
    free(buffer);
    free(datafile);

    return 0;
}