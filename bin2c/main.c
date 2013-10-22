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

static void print_usage(const char *argv0)
{
    fprintf(stderr,
            "Usage: %s [-i inputfile] [-o output.h] [-l line_len] [-t type] -a var_name\n"
            "\ttype can be: char (unsigned char array, default), nsstring (Objective-C NSString constant)\n",
            argv0);
    exit(1);
}

void bin2c(const char *infile, const char *outfile, const char *array, int line_len, VariableType vtype)
{
    FILE *in, *out;
    
    in = infile ? fopen(infile, "rb") : stdin;
    if (in) {
        out = outfile ? fopen(outfile, "w") : stdout;
        if (out) {
            unsigned char c;
            unsigned int l = 0;
            
            if (infile)
                fprintf(out, "// Imported from file '%s'\n", infile);
            if (vtype == VTChar) {
                fprintf(out, "static const unsigned char %s[] = \n\t\"", array);
            } else { // NSString
                fprintf(out, "static const NSString *%s = \n\t@\"", array);
            }
            
            while (!feof(in)) {
                c = fgetc(in);
                l++;
                fprintf(out, "\\x%02x", c);
                if ((l % line_len) == 0) {
                    fprintf(out, "\"\n\t\"");
                }
            }
            fprintf(out, "\";\n\n");
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
    VariableType vtype = VTChar;
    int opt;
    
    while( (opt = getopt(argc, argv, "i:o:a:l:t:")) != -1) {
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
                line_len = atoi(optarg);
                break;
            case 't':
                if (!strcmp(optarg, "char")) {
                    vtype = VTChar;
                } else if (!strcmp(optarg, "nsstring")) {
                    vtype = VTNSString;
                } else {
                    print_usage(argv[0]);
                }
                break;
            default:
                print_usage(argv[0]);
                break;
        }
    }
    
    if (!array) {
        print_usage(argv[0]);
    } else {
        bin2c(infile, outfile, array, line_len, vtype);
    }
    
    return 0;
}

