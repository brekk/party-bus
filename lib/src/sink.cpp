#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void partybus__sink__open(const char *path) {
    /* append mode needs no setup */
    (void)path;
}

void partybus__sink__write(const char *path, const char *line) {
    FILE *f = fopen(path, "a");
    if (!f) { fprintf(stderr, "cannot open %s\n", path); return; }
    fputs(line, f);
    fputc('\n', f);
    fclose(f);
}

void partybus__sink__end(const char *path) {
    FILE *f = fopen(path, "a");
    if (!f) return;
    fputc('\n', f);
    fclose(f);
}

#ifdef __cplusplus
}
#endif
