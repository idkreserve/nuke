#include <stdio.h>
#include <stdlib.h>

#define AXISSIZE 99
/* #define VERBOSE */

struct nlist {
    ssize_t x, y;
    struct nlist *next;
};

static struct nlist *root;
static ssize_t radius;

size_t get_neighbors(ssize_t, ssize_t);

int main(int argc, char *argv[])
{
    struct nlist *np, *nprevp;
    ssize_t res_x = 0, res_y = 0;
    size_t max = 0, temp = 0, line;
    FILE *fp = NULL;
    int nread;

    if (argc != 3 || (fp = fopen(argv[1], "r")) == NULL || (radius = atoi(argv[2])) == 0) {
        if (argc != 3)
            fputs("Usage: nuke coords.txt 10\n", stderr);
        else if (fp == NULL)
            perror("Failed to open file");
        else if (radius == 0)
            fprintf(stderr, "Invalid number: %s\n", argv[2]);
        return 1;
    }
    
    if ((root = malloc(sizeof(*root))) == NULL)
        return 1;
    for (nprevp = NULL, np = root, line = 1; (nread = fscanf(fp, "%lu,%lu", &np->x, &np->y)) != EOF && nread == 2; nprevp = np, np = np->next, line++)
        if (np->x > AXISSIZE || np->y > AXISSIZE || (np->next = malloc(sizeof(*np))) == NULL) {
            fprintf(stderr, "Value too big in line %lu\n", line);
            return 1;
        }
    if (nprevp == NULL || (nread != EOF && nread != 2)) {
        fprintf(stderr, "Error while parsing %s in line %lu Max %lu\n", argv[1], line, (size_t) AXISSIZE);
        return 1;
    }
    free(np);
    nprevp->next = NULL;

    for (ssize_t y = 0; y < AXISSIZE; y++)
        for (ssize_t x = 0; x < AXISSIZE; x++)
            if ((temp = get_neighbors(x, y)) > max) {
                max = temp;
                res_x = x;
                res_y = y;
            }
    printf("X=%ld Y=%ld\tkilled=%lu\n", res_x, res_y, max);

    return 0;
}

size_t get_neighbors(ssize_t x, ssize_t y)
{
    struct nlist *np;
    size_t res = 0;

    for (np = root; np != NULL; np = np->next)
        if (np->x <= x + radius && np->y <= y + radius && np->x >= x - radius && np->y >= y - radius)
            res++;
    #ifdef VERBOSE
    fprintf(stderr, "x=%lu y=%lu total=%lu\n", x, y, res);
    #endif
    return res;
}
