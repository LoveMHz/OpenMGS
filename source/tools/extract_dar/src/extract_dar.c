#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include <sys/stat.h>

#include "mgs_archive.h"

const char *argp_program_version = "extract_dar 1.0";
const char *argp_program_bug_address = "<dustin@devholden.com>";

/* Program documentation. */
static char doc[] = "extract_dar -- Metal Gear Solid PC DAR archive extract";

/* A description of the arguments we accept. */
static char args_doc[] = "DAR_ARCHIVE";

/* The options we understand. */
static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output"},
    {"quiet", 'q', 0, 0, "Don't produce any output"},
    {"silent", 's', 0, OPTION_ALIAS},
    {0}};

/* Used by main to communicate with parse_opt. */
struct arguments {
  char *args[1]; /* arg1 & arg2 */
  int silent, verbose;
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we know is a pointer to our
   * arguments structure. */
  struct arguments *arguments = state->input;

  switch (key) {
  case 'q':
  case 's':
    arguments->silent = 1;
    break;
  case 'v':
    arguments->verbose = 1;
    break;
  case ARGP_KEY_ARG:
    if (state->arg_num >= 1)
      /* Too many arguments. */
      argp_usage(state);

    arguments->args[state->arg_num] = arg;
    break;
  case ARGP_KEY_END:
    if (state->arg_num < 1)
      /* Not enough arguments. */
      argp_usage(state);
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

// remove_ext: removes the "extension" from a file spec.
//   mystr is the string to process.
//   dot is the extension separator.
//   sep is the path separator (0 means to ignore).
// Returns an allocated string identical to the original but
//   with the extension removed. It must be freed when you're
//   finished with it.
// If you pass in NULL or the new string can't be allocated,
//   it returns NULL.
char *remove_ext(char *mystr, char dot, char sep) {
  char *retstr, *lastdot, *lastsep;

  // Error checks and allocate string.
  if (mystr == NULL)
    return NULL;

  if ((retstr = malloc(strlen(mystr) + 1)) == NULL)
    return NULL;

  // Make a copy and find the relevant characters.
  strcpy(retstr, mystr);
  lastdot = strrchr(retstr, dot);
  lastsep = (sep == 0) ? NULL : strrchr(retstr, sep);

  // If it has an extension separator.
  if (lastdot != NULL) {
    // and it's before the extenstion separator.
    if (lastsep != NULL) {
      if (lastsep < lastdot) {
        // then remove it.
        *lastdot = '\0';
      }
    } else {
      // Has extension separator with no path separator.
      *lastdot = '\0';
    }
  }

  // Return the modified string.
  return retstr;
}

/* Our argp parser. */
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv) {
  struct arguments arguments;

  /* Default values. */
  arguments.silent = 0;
  arguments.verbose = 0;

  /* Parse our arguments; every option seen by parse_opt will be reflected in
   * arguments. */
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  char *real_path = realpath("./", NULL);

  /* Create output folder */
  char *output_folder = remove_ext(arguments.args[0], '.', '/');
  struct stat st = {0};

  if (stat(output_folder, &st) == -1) {
    mkdir(output_folder, 0700);
  }

  struct DAR_ARCHIVE *archive = openArchive(arguments.args[0]);
  for (uint32_t index = 0; index < archive->count; index++) {
    // Output file name
    const size_t path_size =
        strlen(output_folder) + strlen(archive->entries[index].name) + 2;
    char *path = malloc(path_size);

    // Build output filename
    snprintf(path, path_size, "%s/%s", output_folder,
             archive->entries[index].name);

    // Write file
    FILE *write_ptr = fopen(path, "wb");
    fwrite(archive->entries[index].data, archive->entries[index].size, 1,
           write_ptr);

    // Free resources
    fclose(write_ptr);
    free(path);
  }

  return 0;
  // struct DAR_ARCHIVE *archive =
  // openArchive("/home/dustin/.wine/drive_c/Program Files (x86)/Metal Gear
  // Solid/stage/s19br/stg_tex1.dar");
}
