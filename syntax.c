#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <errno.h>

#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define MAGENTA 0x5
#define YELLOW 0x6
#define WHITE 0x7
#define GRAY 0x8
#define LIGHTBLUE 0x9
#define LIGHTGREEN 0xa
#define LIGHTCYAN 0xb
#define LIGHTRED 0xc
#define LIGHTMAGENTA 0xd
#define LIGHTYELLOW 0xe
#define LIGHTWHITE 0xf

#define default_color WHITE

struct
{
    char *keyword;
    int color;
} keywords[] = {
    {"break", MAGENTA},
    {"continue", MAGENTA},
    {"return", MAGENTA},

    {"const", LIGHTBLUE},
    {"volatile", LIGHTBLUE},
    {"extern", LIGHTBLUE},
    {"static", LIGHTBLUE},

    {"char", LIGHTBLUE},
    {"int", LIGHTBLUE},
    {"short", LIGHTBLUE},
    {"float", LIGHTBLUE},
    {"double", LIGHTBLUE},
    {"long", LIGHTBLUE},
    {"bool", LIGHTBLUE},
    {"void", LIGHTBLUE},

    {"double_t", LIGHTGREEN},
    {"float_t", LIGHTGREEN},
    {"fpos_t", LIGHTGREEN},
    {"max_align_t", LIGHTGREEN},
    {"mbstate_t", LIGHTGREEN},
    {"nullptr_t", LIGHTGREEN},
    {"ptrdiff_t", LIGHTGREEN},
    {"sig_atomic_t", LIGHTGREEN},
    {"size_t", LIGHTGREEN},
    {"time_t", LIGHTGREEN},
    {"wchar_t", LIGHTGREEN},
    {"wint_t", LIGHTGREEN},
    {"FILE", LIGHTGREEN},

    {"uint8_t", LIGHTGREEN},
    {"uint16_t", LIGHTGREEN},
    {"uint32_t", LIGHTGREEN},
    {"uint64_t", LIGHTGREEN},

    {"int8_t", LIGHTGREEN},
    {"int16_t", LIGHTGREEN},
    {"int32_t", LIGHTGREEN},
    {"int64_t", LIGHTGREEN},

    {"struct", LIGHTYELLOW},
    {"enum", LIGHTYELLOW},
    {"union", LIGHTYELLOW},
    {"typedef", LIGHTYELLOW},
    {"unsigned", LIGHTBLUE},
    {"signed", LIGHTBLUE},
    {"sizeof", LIGHTBLUE},
    {"register", LIGHTBLUE},
    {"do", LIGHTYELLOW},
    {"if", LIGHTYELLOW},
    {"while", LIGHTYELLOW},
    {"switch", LIGHTYELLOW},
    {"for", LIGHTYELLOW},
    {"case", LIGHTYELLOW},
    {"default", LIGHTYELLOW},
    {"goto", LIGHTYELLOW},

    {"include", LIGHTCYAN},
    {"pragma", LIGHTCYAN},
    {"define", LIGHTCYAN},
    {"ifdef", LIGHTCYAN},
    {"ifndef", LIGHTCYAN},
    {"endif", LIGHTCYAN},
    {"undef", LIGHTCYAN},
    {"error", LIGHTCYAN},

    {"main", YELLOW},

    {"0x", LIGHTGREEN},
    {"0o", LIGHTGREEN},
    {"0b", LIGHTGREEN},

};

struct
{
    char *keyword;
    int color;
} comment[] = {
    {"//", LIGHTGREEN},
};

void SetColor(int color_hex)
{
    HANDLE hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleColor, (color_hex / 16) << 4 | (color_hex % 16));
}

int FindString(char *str, char *find)

{
    int n = strlen(find);
    for (int i = 0; i < strlen(str); i++)
    {
        if (!strncmp(str + i, find, n))
        {
            return i;
        }
    }
    return -1;
}

int is_separator(int c) // Taken from https://github.com/antirez/kilo/blob/69c3ce609d1e8df3956cba6db3d296a7cf3af3de/kilo.c#L366
{
    return c == '\0' || isspace(c) || strchr("#,.()+-/*=~[];", c) != NULL;
}

void Help()
{
    printf("\nSyntax highlighting for C/C++\n\nUsage: syntax <file>\n\nExample:\nsyntax program.c\nCopyright (c) 2021 anic17 Software\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2 || !strcmp(argv[1], "--help") || !strcmp(argv[1], "/?"))
    {
        Help();
        return 0;
    }
    FILE *fp = fopen(argv[1], "rb");

    if (!fp)
    {
        printf("%s: %s", argv[1], strerror(errno));
        return errno;
    }
    size_t keyword_size = sizeof(keywords) / sizeof(keywords[0]);
    size_t comment_size = sizeof(comment) / sizeof(comment[0]);

    DWORD l_mode;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hStdout, &l_mode);
    SetConsoleMode(hStdout, l_mode | 0x4 | 0x8);

    char *line = (char *)malloc(sizeof(char) * FILENAME_MAX);
    // Make syntax highlighting

    int pos;
    SetColor(default_color);
    while (fgets(line, FILENAME_MAX, fp))
    {
        line[strcspn(line, "\n")] = 0;

        for (int i = 0; i < strlen(line); i++)
        {
            for (int k = 0; k < keyword_size; k++)
            {

                if (pos = FindString(line + i, keywords[k].keyword) != -1 &&
                          is_separator(line[i + pos - 1]) && is_separator(line[i + pos + strlen(keywords[k].keyword)]))
                {

                    SetColor(keywords[k].color);

                    printf("%.*s", strlen(keywords[k].keyword), line + i);

                    i += strlen(keywords[k].keyword) + pos - 1;

                    SetColor(default_color);

                    break;
                }
            }
            putchar(line[i]);
        }
        putchar('\n');
    }
    SetColor(0x07);
    fclose(fp);
    return 0;
}