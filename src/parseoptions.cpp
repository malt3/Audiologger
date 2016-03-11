#include <getopt.h>
#include <iostream>
#include <cstring>
#include "parseoptions.hpp"




int option_parser(int argc, char **argv, bool& help, bool& version, char*& title, char*& filename, char*& path){
    static struct option long_options[] = {
        {"title",     required_argument,       0, 't'},
        {"filename",     required_argument,       0, 'f'},
        {"path",  required_argument,  0, 'p'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'}
        //{"volume", required_argument, 0, 'v'},
        //{"duration", required_argument, 0, 'd'},
        //{"channels", required_argument, 0,  'c'},
    };
    int c;
    while (1){
        int option_index = 0;
        c = getopt_long (argc, argv, "t:f:p:hv",
                       long_options, &option_index);
        if (c == -1) {
            break;
        }
        switch (c) {
        /*case 0:*/
          /* If this option set a flag, do nothing else now. */
          /*if (long_options[option_index].flag != 0)
            break;
          printf ("option %s", long_options[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;*/

            case 't':
                title = new char[strlen(optarg)+1];
                strcpy(title, optarg);
                break;

            case 'f':
                filename = new char[strlen(optarg)+1];
                strcpy(filename, optarg);
                break;

            case 'p':
                path = new char[strlen(optarg)+1];
                strcpy(path, optarg);
                break;
            case 'h':
                help = true;
                break;
            case 'v':
                version = true;
                break;

            default:
                abort ();
        }
  }
    return optind;
}

void command_parser(int argc, char **argv, int position, command& com, int& number){
    static char commandStrings[][10] = {
        "create",
        "play",
        "list",
        "delete",
        "edit"
    };
    com = nocommand;
    if (argc <= position) {
        return;
    }else{
        for (int i = 0; i<5; i++) {
            if (strcmp(commandStrings[i], argv[position]) == 0) {
                com = (command)i;
                break;
            }
        }
    }
    if (argc > position+1) {
        number = atoi(argv[position+1]);
    }else{
        number = -1;
    }
    return;
}
