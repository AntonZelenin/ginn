#ifndef __ARGS_H__
#define __ARGS_H__

#include <getopt.h>
#include <iostream>

#include "native_client/deepspeech.h"

char* model = NULL;

char* alphabet = NULL;

char* lm = NULL;

char* trie = NULL;

char* audio = NULL;

bool show_times = false;

bool has_versions = false;

void PrintHelp(const char* bin)
{
    std::cout <<
    "Usage: " << bin << " --model MODEL --alphabet ALPHABET [--lm LM --trie TRIE]\n"
    "\n"
    "Running DeepSpeech inference.\n"
    "\n"
    "	--model MODEL		Path to the model (protocol buffer binary file)\n"
    "	--alphabet ALPHABET	Path to the configuration file specifying the alphabet used by the network\n"
    "	--lm LM			Path to the language model binary file\n"
    "	--trie TRIE		Path to the language model trie file created with native_client/generate_trie\n"
    "	-t			Run in benchmark mode, output mfcc & inference time\n"
    "	--help			Show help\n"
    "	--version		Print version and exits\n";
    DS_PrintVersions();
    exit(1);
}

bool ProcessArgs(int argc, char** argv)
{
    const char* const short_opts = "m:a:l:r:w:thv";
    const option long_opts[] = {
            {"model", required_argument, nullptr, 'm'},
            {"alphabet", required_argument, nullptr, 'a'},
            {"lm", required_argument, nullptr, 'l'},
            {"trie", required_argument, nullptr, 'r'},
            {"t", no_argument, nullptr, 't'},
            {"help", no_argument, nullptr, 'h'},
            {"version", no_argument, nullptr, 'v'},
            {nullptr, no_argument, nullptr, 0}
    };

    while (true)
    {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt)
        {
        case 'm':
            model     = optarg;
            break;

        case 'a':
            alphabet     = optarg;
            break;

        case 'l':
            lm     = optarg;
            break;

        case 'r':
            trie     = optarg;
            break;

        case 't':
            show_times = true;
            break;

        case 'v':
            has_versions = true;
            break;

        case 'h': // -h or --help
        case '?': // Unrecognized option
        default:
            PrintHelp(argv[0]);
            break;
        }
    }

    if (has_versions) {
        DS_PrintVersions();
        return false;
    }

    if (!model || !alphabet) {
        PrintHelp(argv[0]);
        return false;
    }

    return true;
}

#endif // __ARGS_H__
