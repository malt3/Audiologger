#pragma once

/* The option_parser function takes argc and argv with a number of arguments tht get set by it (passed by refernce). It returns the position in argv where the lookup stopped.*/
enum command {create, play, lst, del, edit, nocommand};

int option_parser(int argc, char **argv, bool& help, bool& version, char*& title, char*& filename, char*& path);
void command_parser(int argc, char **argv, int position, command& com, int& number);