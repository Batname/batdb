// http://stackoverflow.com/questions/16997141/writing-structs-to-a-file-in-c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // open function
#include <unistd.h> // close function
#include <sys/stat.h>

#include "helpers.h"

#define DB_FILE "/tmp/batdb.dat"

char *mainMenu = 
"1. Write data to database\n"
"2. Read data from database\n"
"0. Exit\n";

struct Entity {
    int id;
    long lastTime;
    char fullName[30];
    char email[30];
    char description[200];
};

void readFromDB(int fd)
{
    printf("READ >>>");
}

void writeToDB(int fd)
{
    int pos_length = 5;
    int entitySize = sizeof(struct Entity);
    int rc, buf_len = entitySize + pos_length - 1;
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
            printf("position is %d value is %d\n", y, i);
            y++;
            if (y >= pos_length) {
                break;
            }
        }
    }

    for(i = 0; i < pos_length; i++)
    {
        printf("pos: %d\n", positions[i]);
        if (positions[i] == 0)
        {
            printf("[WARNING] wrong positoins\n");
            return;
        }
    }


    struct Entity entity;
    entity.id = 10;
    entity.lastTime = 123;

    memset(entity.fullName, 0, sizeof(entity.fullName));
    memset(entity.email, 0, sizeof(entity.email));
    memset(entity.description, 0, sizeof(entity.description));


    strncpy(entity.fullName, buffer + positions[1] + 1, positions[2]-positions[1] - 1);
    strncpy(entity.email, buffer + positions[2] + 1, positions[3]-positions[2] - 1);
    strncpy(entity.description, buffer + positions[3] + 1, positions[4]-positions[3] - 1);

    // writing data
    if(write(fd, &entity, entitySize) == -1) 
        fatal("in main while writing entity to file");
    write(fd, "\n", 1);
}

int main(int argc, char *argv[])
{
    puts("DB start...");

    int userid, fd, file_len, buffer_len = 100;
    char *metaData, *datafile, buffer;

    file_len = strlen(DB_FILE);

    datafile = (char*) malloc(file_len);
    strcpy(datafile, DB_FILE);

    // Opening file
    fd = open(datafile,O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
    if (fd == -1)
        fatal("in main() while opening file");

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
            writeToDB(fd);
        }
        else if (c == '2')
        {
            while ((getchar()) != '\n');
            readFromDB(fd);
        }
        else if (c == 'q')
        {
            exit(0);
        }

        while ((getchar()) != '\n');
        printf(">>> ");
        c = getchar();
    }

    if (close(fd) == -1)
        fatal("in main while closing file");

    // printf("Saved to DB\n");
    // free(metaData);
    // free(datafile);
    // free(buffer);

    return 0;
}