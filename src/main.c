// http://stackoverflow.com/questions/16997141/writing-structs-to-a-file-in-c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // open function
#include <unistd.h> // close function
#include <sys/stat.h>
#include <time.h>

#include "helpers.h"

#define DB_FILE "/tmp/batdb.dat"
#define META_FILE "/tmp/batdb.meta"

char *mainMenu = 
"1. Write data to database\n"
"2. Read data from database\n"
"0. Exit\n";

struct Entity {
    int id;
    int lastTime;
    char fullName[30];
    char email[30];
    char description[200];
};

struct Meta {
    int count;
};

void readFromDB(struct Meta *meta)
{
    struct Entity entity;
    int entitySize = sizeof(struct Entity);
    printf("READ >>> ");
    int c = getchar();
    char string[1];
    string[0] = c;

    int fd = open(DB_FILE, O_RDONLY);

    pread(fd, &entity, entitySize, entitySize * (atoi(string)));
    printf("id <<< %d\n", entity.id);
    printf("lastTime <<< %d\n", entity.lastTime);
    printf("fullName <<< %s\n", entity.fullName);
    printf("email <<< %s\n", entity.email);
    printf("description <<< %s\n", entity.description);

    close(fd);
}

void writeToDB(struct Meta *meta)
{
    int file_len = strlen(DB_FILE);
    char *datafile;
    datafile = (char*) malloc(file_len);
    strcpy(datafile, DB_FILE);

    FILE *fd;
    // Opening file
    fd = fopen(datafile, "a+b");
    if (fd == NULL)
        fatal("in main() while opening file");

    FILE *meta_fd;
    meta_fd = fopen(META_FILE, "wb");
    if (meta_fd == NULL)
        fatal("in writeToDB() while opening metafile");

    int pos_length = 3;
    int entitySize = sizeof(struct Entity);
    int rc, buf_len = entitySize + pos_length - 1 - sizeof(int) * 2;
    char buffer[buf_len];
    int i, y = 0;
    char positions[pos_length];
    memset(positions, 0, pos_length);

    printf("WRITE >>> ");
    scanf("%[^\n]s", buffer);


    for (i = 0; i < buf_len; i++)
    {
        if (buffer[i] == '|')
        {
            positions[y] = i;
            y++;
            if (y >= pos_length) {
                break;
            }
        }
    }

    for(i = 0; i < pos_length; i++)
    {
        if (positions[i] == 0)
        {
            printf("[WARNING] wrong positoins\n");
            return;
        }
    }


    struct Entity entity;
    entity.id = meta->count + 1;
    entity.lastTime = (int)time(NULL);

    memset(entity.fullName, 0, sizeof(entity.fullName));
    memset(entity.email, 0, sizeof(entity.email));
    memset(entity.description, 0, sizeof(entity.description));


    strncpy(entity.fullName, buffer, positions[0]);
    strncpy(entity.email, buffer + positions[0] + 1, positions[1]-positions[0] - 1);
    strncpy(entity.description, buffer + positions[1] + 1, positions[2]-positions[1] - 1);

    fwrite(&entity, entitySize, 1, fd);

    printf("OK, ID %d\n", meta->count);

    meta->count++;
    fwrite(meta, sizeof(struct Meta), 1, meta_fd);


    if (fclose(fd) == -1)
        fatal("in main while closing file");
}

int main(int argc, char *argv[])
{
    puts("DB start...");

    int userid, buffer_len = 100;
    char *metaData, buffer;

    // Meta file
    FILE *meta_fd;
    struct Meta meta;

    meta_fd = fopen(META_FILE, "rb");
    if (meta_fd == NULL) {
        meta_fd = fopen(META_FILE, "w+b");
        meta.count = 0;
        fwrite(&meta, sizeof(struct Meta), 1, meta_fd);
    } else {
        meta_fd = fopen(META_FILE, "r+b");
    }

    fread(&meta, sizeof(struct Meta), 1, meta_fd);

    if (fclose(meta_fd) == -1)
        fatal("in main while closing meta file");

    system("clear");

    int c;
    printf("%s", mainMenu);
    printf(">>> ");
    c = getchar();

    while(1)
    {
        if (c == '1')
        {
            while ((getchar()) != '\n');
            writeToDB(&meta);
        }
        else if (c == '2')
        {
            while ((getchar()) != '\n');
            readFromDB(&meta);
        }
        else if (c == 'q' || c == '0')
        {
            exit(0);
        }

        while ((getchar()) != '\n');
        printf(">>> ");
        c = getchar();
    }

    printf("Exit DB\n");

    return 0;
}