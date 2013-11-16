/**
 * natsort - Copyright (C) 2013 Olivier Brunel
 *
 * main.c
 * Copyright (C) 2013 Olivier Brunel <i.am.jack.mail@gmail.com>
 *
 * This file is part of natsort.
 *
 * natsort is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * natsort is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * natsort. If not, see http://www.gnu.org/licenses/
 */

#include "config.h"

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "sort.h"

#if defined(GIT_VERSION)
#undef PACKAGE_VERSION
#define PACKAGE_VERSION GIT_VERSION
#endif
#define PACKAGE_TAG             "Natural Sorting utility"

static DonnaSortOptions sort_options = DONNA_SORT_NATURAL_ORDER;
static int reverse = -1;

static void
show_version (void)
{
    puts (
        PACKAGE_NAME " - " PACKAGE_TAG " v" PACKAGE_VERSION "\n"
        "Copyright (C) 2013 Olivier Brunel\n"
        "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
        "This is free software: you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.");
    exit (0);
}

static void
show_help (const char *prgname)
{
    printf ("Usage:\n"
        " %s [OPTION..]\n"
        "\n"
        " -h, --help                Show this help and exit\n"
        " -V, --version             Show version information and exit\n"
        "\n"
        " -n, --no-natural-order    Disable natural order\n"
        " -i, --case-insensitive    Ignore case\n"
        " -f, --dot-first           Put items starting with a dot first\n"
        " -m, --dot-mixed           Treat items starting with a dot as others\n"
        " -I, --ignore-spunct       Ignore spaces & alphanumeric characters\n"
        " -r, --reverse             Reverse order\n"
        , prgname);
    exit (0);
}

static int
_strcmp_ext (const void *s1, const void *s2)
{
    /* by default we invert results (reverse==-1), because we actually print
     * then in reversed order */
    return reverse * strcmp_ext (* (char **) s1, * (char **) s2, sort_options);
}

int
main (int argc, char *argv[])
{
    char **items = NULL;
    size_t alloc = 0;
    size_t nb = 0;

    int o;
    int index = 0;
    struct option options[] = {
        { "help",               no_argument, 0, 'h' },
        { "version",            no_argument, 0, 'V' },
        { "no-natural-order",   no_argument, 0, 'n' },
        { "case-insensitive",   no_argument, 0, 'i' },
        { "dot-first",          no_argument, 0, 'f' },
        { "dot-mixed",          no_argument, 0, 'm' },
        { "ignore-spunct",      no_argument, 0, 'I' },
        { "reverse",            no_argument, 0, 'r' },
    };

    setlocale (LC_ALL, "");

    for (;;)
    {
        o = getopt_long (argc, argv, "hVdnifmIr", options, &index);
        if (o == -1)
            break;

        switch (o)
        {
            case 'h':
                show_help (argv[0]);
                /* not reached */
                break;
            case 'V':
                show_version ();
                /* not reached */
                break;
            case 'n':
                sort_options &= (unsigned int) ~DONNA_SORT_NATURAL_ORDER;
                break;
            case 'i':
                sort_options |= DONNA_SORT_CASE_INSENSITIVE;
                break;
            case 'f':
                sort_options |= DONNA_SORT_DOT_FIRST;
                break;
            case 'm':
                sort_options |= DONNA_SORT_DOT_MIXED;
                break;
            case 'I':
                sort_options |= DONNA_SORT_IGNORE_SPUNCT;
                break;
            case 'r':
                reverse = 1;
                break;
            case '?':   /* unknown option */
            default:
                return 1;
        }
    }

    /* read items from stdin */
    for (;;)
    {
        char *line;
        size_t l;
        ssize_t len;

        line = NULL;
        len = getline (&line, &l, stdin);
        if (len == -1)
        {
            free (line);
            break;
        }
        if (len > 1 && line[--len] == '\n')
            line[len] = '\0';

        if (nb >= alloc)
        {
            char **_items;

            alloc += 23;
            _items = realloc (items, sizeof (*items) * alloc);
            if (!_items)
            {
                while (nb > 0)
                    free (items[--nb]);
                free (items);
                return 1;
            }
            items = _items;
        }

        items[nb++] = line;
    }

    /* sort items */
    qsort (items, nb, sizeof (*items), _strcmp_ext);

    /* show (sorted) items */
    while (nb > 0)
    {
        puts (items[--nb]);
        free (items[nb]);
    }
    free (items);

    return 0;
}
