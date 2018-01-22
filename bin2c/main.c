//
//  main.c
//  bin2c - Convert a binary file to a C array for inclusion in programs
//
//  Created by Stéphane Peter on 10/21/13.
//  Copyright (c) 2013 Catloaf Software, LLC. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

typedef enum
{
    VTChar,
    VTNSString
} VariableType;

typedef struct
{
    VariableType vtype;
    int static_keyword;
    int null_terminate;
    int line_len;
} OutOptions;

static void print_usage(const char *argv0)
{
    fprintf(stderr,
            "Usage: %s [-i inputfile] [-o output.h] [-l line_len] [-t type] [-s0h] -a var_name\n"
            "\t-s make the generated variable static\n"
            "\t-0 add a null-char at the end of the generated array; only valid with -t char\n"
            "\t-h show this help\n"
            "\ttype can be: char (unsigned char array, default), nsstring (Objective-C NSString constant)\n",
            argv0);
    exit(1);
}

static void print_c_str (FILE* in, FILE* out, const char* array, const OutOptions* opts)
{
    unsigned char c;
    unsigned int l = 0;
    const char* const static_str = (opts->static_keyword ? "static " : "");
    int nullchar_printed = !opts->null_terminate;
    int eof;

    fprintf(out, "%sconst unsigned char %s[] = {", static_str, array);

    c = fgetc(in);
    eof = feof(in);
    while (!eof || !nullchar_printed) {
        if (eof) {
            c = 0;
            nullchar_printed = 1;
        }

        if ((l % opts->line_len) == 0) {
            fprintf(out, "\n\t");
        }
        l++;
        fprintf(out, "0x%02x", c);
        c = fgetc(in);

        eof = feof(in);
        if (!eof || !nullchar_printed) {
            fprintf(out, ",");
        }
    }
    if (l) {
        fprintf(out, "\n");
    }
    fprintf(out, "};\n");
    fprintf(out, "%sconst unsigned int %s_len = %d;\n", static_str, array, l);
}

static void print_nsstring (FILE* in, FILE* out, const char* array, const OutOptions* opts)
{
    unsigned char c;
    unsigned int l = 0;

    if (opts->static_keyword)
    {
        fprintf(out, "static ");
    }
    fprintf(out, "const NSString *%s = \n\t@\"", array);

    c = fgetc(in);
    while (!feof(in)) {
        l++;
        fprintf(out, "\\x%02x", c);
        if ((l % opts->line_len) == 0) {
            fprintf(out, "\"\n\t\"");
        }
        c = fgetc(in);
    }
    fprintf(out, "\";\n");
}

void bin2c(const char *infile, const char *outfile, const char *array, const OutOptions* opts)
{
    FILE *in, *out;

    in = infile ? fopen(infile, "rb") : stdin;
    if (in) {
        out = outfile ? fopen(outfile, "w") : stdout;
        if (out) {
            if (infile)
                fprintf(out, "// Imported from file '%s'\n", infile);
            if (opts->vtype == VTChar) {
                print_c_str(in, out, array, opts);
            } else { // NSString
                print_nsstring(in, out, array, opts);
            }

            if (out != stdout)
                fclose(out);
        } else {
            fprintf(stderr, "Failed to open file for writing: %s", outfile);
        }
        if (in != stdin)
            fclose(in);
    } else{
        fprintf(stderr, "Failed to open file for reading: %s", infile);
    }
}

int main(int argc,  char * const argv[])
{
    const char *infile = NULL, *outfile = NULL, *array = NULL;
    int line_len = 80;
    OutOptions opts;
    int opt;

    opts.vtype = VTChar;
    opts.static_keyword = 0;
    opts.line_len = 80;
    opts.null_terminate = 0;

    while( (opt = getopt(argc, argv, "i:o:a:l:t:s0h")) != -1) {
        switch (opt) {
            case 'i':
                infile = strdup(optarg);
                break;
            case 'o':
                outfile = strdup(optarg);
                break;
            case 'a':
                array = strdup(optarg);
                break;
            case 'l':
                opts.line_len = atoi(optarg);
                break;
            case 't':
                if (!strcmp(optarg, "char")) {
                    opts.vtype = VTChar;
                } else if (!strcmp(optarg, "nsstring")) {
                    opts.vtype = VTNSString;
                } else {
                    print_usage(argv[0]);
                }
                break;
            case '0':
                opts.null_terminate = 1;
                break;
            case 's':
                opts.static_keyword = 1;
                break;
            case 'h':
            default:
                print_usage(argv[0]);
                break;
        }
    }

    if (!array) {
        print_usage(argv[0]);
    } else {
        bin2c(infile, outfile, array, &opts);
    }

    return 0;
}
