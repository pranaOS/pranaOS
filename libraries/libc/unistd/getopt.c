#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *optarg = NULL;
int optind = 1;
int opterr = 1;
int optopt = 0;

/**
 * @brief get opt 
 * 
 * @param argc 
 * @param argv 
 * @param optstring 
 * @return int 
 */
int getopt(int argc, char *const argv[], const char *optstring) {
	return getopt_long(argc, argv, optstring, NULL, 0);
}

/**
 * @brief Get the opt long object
 * 
 * @param argc 
 * @param argv 
 * @param optstring 
 * @param longopts 
 * @param longindex 
 * @return int 
 */
int getopt_long(int argc, char *const argv[], const char *optstring, const struct option *longopts, int *longindex) {
	static char *nextchar = NULL;

	if (optind >= argc) {
		return -1;
	}

	do
	{
		if (!nextchar) {
			nextchar = argv[optind];
			if (*nextchar != '-') {
				return -1;
			}
			else {
				nextchar++;

				if (*nextchar == '\0') {
					return -1;
				}

				if (*nextchar == '-') {
					if (nextchar[1] == '\0') {
						optind++;
						return -1;
					}
					else if (longopts) {
						nextchar++;
						char tmp[strlen(nextchar) + 1];
						strcpy(tmp, nextchar);
						char *eq = strchr(tmp, '=');
						if (eq) {
							*eq = '\0';
							optarg = nextchar + (eq - tmp + 1);
						}
						else {
							optarg = NULL;
						}

						int found = -1;
						for (int index = 0; longopts[index].name; ++index) {
							if (!strcmp(longopts[index].name, tmp))
							{
								found = index;
							}
						}

						if (found == -1) {
							if (longindex) {
								*longindex = -1;
							}
							if (opterr) {
								fprintf(stderr, "unknown long argument: %s\n", tmp);
							}
							nextchar = NULL;
							optind++;
							optopt = '\0';
							return '?';
						}
						else {
							if (longindex) {
								*longindex = found;
							}
							if (longopts[found].has_arg == required_argument) {
								if (!optarg) {
									optarg = argv[optind + 1];
									optind++;
								}
							}
							nextchar = NULL;
							optind++;
							if (!longopts[found].flag) {
								return longopts[found].val;
							}
							else {
								*longopts[found].flag = longopts[found].val;
								return 0;
							}
						}
					}
				}
			}
		}

		if (*nextchar == '\0') {
			nextchar = NULL;
			optind++;
			continue;
		}

		if ((*nextchar < 'A' || *nextchar > 'z' || (*nextchar > 'Z' && *nextchar < 'a')) && (*nextchar != '?') && (*nextchar != '-')) {
			if (opterr) {
				fprintf(stderr, "Invalid option character: %c\n", *nextchar);
			}
			optopt = *nextchar;
			nextchar++;
			return '?';
		}

		char *opt = strchr(optstring, *nextchar);

		if (!opt) {
			if (opterr)
			{
				fprintf(stderr, "Invalid option character: %c\n", *nextchar);
			}
			optopt = *nextchar;
			nextchar++;
			return '?';
		}

		int optout = *nextchar;

		if (opt[1] == ':') {
			if (nextchar[1] != '\0') {
				optarg = &nextchar[1];
				nextchar = NULL;
				optind++;
			}
			else {
				optarg = argv[optind + 1];
				optind += 2;
				nextchar = NULL;
			}
		}
		else {
			nextchar++;
		}

		return optout;

	} while (optind < argc);

	return -1;
}