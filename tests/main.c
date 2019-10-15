#include "functionality_tests.h"
#include "unit_tests.h"
#include <stdio.h>

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
	for (i; i < argc; i++, argv++){
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

int main(int argc, char **argv){
	parse_cmd_options(argc, argv);
	if (po.error == 1) {
		return 1;
	}
	if (po.show_help == 1) {
		return 0;
	}
	if (po.func_tests) {
		test_functionality(po.verbose_mode);
	}
	if (po.unit_tests) {
		test_units(po.verbose_mode);
	}
	return 0;
}
