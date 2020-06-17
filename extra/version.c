#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (2 != argc) {
        printf("Usage: %s <output file>\n", argv[0]);
        return 1;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    FILE *fd = fopen(argv[1], "w");
    if (NULL == fd) {
        fprintf(stderr, "Cannot open the file <%s> for writing.\n", argv[1]);
        return 1;
    }
    fprintf(fd, "#ifndef VERSION\n");
    fprintf(fd,"#define VERSION \"%d-%02d-%02d %02d:%02d:%02d\"\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(fd, "#endif\n");
    fclose(fd);
    return 0;
}
