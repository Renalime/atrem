#include <stdio.h>
#include "token_list_tests.h"
#include "alteration_list_tests.h"
#include "../src/atrem.h"
#include "irviz/print_ir.h"

void parse_cmd_options(int argc, char **argv);
void show_help();

struct program_options{
	unsigned char func_tests : 1;
	unsigned char unit_tests : 1;
	unsigned char verbose_mode : 1;
	unsigned char show_help : 1;
	unsigned char error : 1;
};

static struct program_options po = {
	0,
	0,
	0,
	0,
	0
};

void show_help(){
	printf("-h: Show help\n"
	"-f: Print functionality tests\n"
	"-u: Print unit tests\n"
	"-v: Verbose mode\n");
}

void parse_cmd_options(int argc, char **argv){
	int i = 1;
	argv++;
	for (; i < argc; i++, argv++){
		while (((++(*argv))[0]) != '\0'){
			switch ((*argv)[0]){
			case 'f':
				po.func_tests = 1;
				break;
			case 'u':
				po.unit_tests = 1;
				break;
			case 'v':
				po.verbose_mode = 1;
				break;
			case 'h':
				po.show_help = 1;
				show_help();
				return;
			default:
				printf("Unknown command line argument\n");
				show_help();
				po.error = 1;
				return;
			}
		}
	}
}

void atrem_test()
{
	atrem_regex ar;
	char *s = "q(a{,2}b{3,}c{2})?[^]0-9A-Z][[:punct:][:alnum:]]";
	puts(s);
	ar = atrem_parse_regex(s);
	print_ir(ar);
	atrem_dispose_regex(ar);
}

int main(int argc, char **argv){
	parse_cmd_options(argc, argv);
	if (po.error == 1) {
		return 1;
	}
	if (po.show_help == 1) {
		return 0;
	}
	atrem_test();
	do_token_list_tests();
	do_alteration_list_tests();
	return 0;
}
