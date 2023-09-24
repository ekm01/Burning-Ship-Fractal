#include "bmp/bmpp.h"
#include "burningshipd/burningship.h"
#include "burningshipd/burningshipp.h"
#include <getopt.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define STD_VERSION "0"
#define STD_FUNCTION_CALL_NUMBER "1"
#define STD_START_COORDINATE "-2,1"
#define STD_SIZE "1024,1024"
#define STD_ITERATION_LIMIT "255"
#define STD_RES "0.003"
#define STD_FILENAME "test"

unsigned
argumentCheckerDs(char *str) { // checks if a string has a certain char in it
  if (strchr(str, ',')) {
    return 1;
  }
  return 0;
}

void usage() {
  fprintf(stderr,
          "\n\n"
          "                                       User Manual: BURNING SHIP "
          "FRACTAL\n\n"
          "NAME\n"
          "   burningship - an executable file to draw the famous fractal "
          "created by Michael Michelitsch and Otto E. Rössler.\n\n"
          "SYNOPSIS\n"
          "   ./burningship [burningship options]\n"
          "   ./burningship -V [integer]\n"
          "   ./burningship -B [unsigned integer]\n"
          "   ./burningship -s [float],[float]\n"
          "   ./burningship -d [size_t],[size_t]\n"
          "   ./burningship -n [unsigned integer]\n"
          "   ./burningship -r [float]\n"
          "   ./burningship -o [string]\n"
          "   ./burningship -h\n"
          "   ./burningship --help\n\n"
          "DESCRIPTION\n"
          "   ./burningship is a command to execute  the burningship  file. "
          "Each option (besides \"-V,-B,-o,-h,--help\"),\n"
          "   if provided, is a necessary parameter to draw the desired part "
          "of the fractal in desired size and detail.\n"
          "   If the options are not provided, the fractal is drawn  with  "
          "default parameters (see DEFAULT PARAMETERS).\n"
          "   After the execution, a \".bmp\" file is created by means of "
          "which the fractal image can be displayed.\n\n"
          "OPTIONS (for the data types see SYNOPSIS)\n"
          "   -V<int>     —The implementation to be used. There  are two "
          "versions: V0 is the default implementation. V1\n"
          "                is the parallel implementation of the same function "
          "which is faster.\n\n"
          "   -B<int>     —If set, the average runtime of the specified "
          "implementation is measured and output. The argument\n"
          "                of this option specifies the number of repetitions "
          "of the function call.\n\n"
          "   -s<real>,<imag>     —This is the start  coordinate of the the  "
          "desired part of the fractal. <real> refers to\n"
          "                        x coordinate,  whereas <imag> refers to y "
          "coordinate. Calculation is done in  imaginary\n"
          "                        coordinate  plane.  Note  that as the x "
          "value is incremented, the user can  inspect the\n"
          "                        part which is more on the right side of the "
          "x-axis, otherwise what's  more on the left.\n"
          "                        In contrast, as the y value is incremented, "
          "the user can inspect the part which is more\n"
          "                        on the downside of the y-axis, otherwise "
          "what's more above.\n\n"
          "   -d<int>,<int>       —Width and height of the image. Both must be "
          "positive. As inremented, a bigger part of the\n"
          "                        fractal can be displayed.\n\n"
          "   -n<int>      —Maximum number of iterations per image  pixel (per "
          "coordinate).As this is incremented, image\n"
          "                 becomes more detailed colorwise. It must be "
          "positive.\n\n"
          "   -r<float>    —This indicates the zoom in/out ratio. If it is "
          "incremented,the image is zoomed out, otherwise\n"
          "                 zoomed in. It should be positive, unless an upside "
          "down burning ship fractal is desired.\n\n"
          "   -o<file>     —Name of the output file (.bmp).\n\n"
          "   -h           —This displays the User Manuals.\n\n"
          "   --help       —This displays the User Manuals.\n\n"
          "DEFAULT PARAMETERS\n"
          "   -V<int>: <int> = 0\n\n"
          "   -B<int>: <int> = 1\n\n"
          "   -s<real>,<imag>: <real> = -2.0, <imag> = 1.0\n\n"
          "   -d<int>,<int>: <int> = 1024, <int> = 1024\n\n"
          "   -n<int>: <int> = 255\n\n"
          "   -r<float>: <float> = 0.003\n\n"
          "   -o<file>: <file> = test.bmp\n\n"
          "EXAMPLES (options and their arguments can be written seperately, as "
          "well as together, see (2) and (3))\n"
          "   (1) ./burningship\n\n"
          "   (2) ./burningship -V 1 -B 42\n\n"
          "   (3) ./burningship -o realship -V1 -B 1 -d1024,1024 -s-1.82,0.04 "
          "-r 0.00012 -n 255 (give it a try)\n\n"
          "   (4) ./burningship -r32.32 -B123\n\n");
}

int main(int argc, char **argv) {
  char *v_value = NULL;
  char *b_value = NULL;
  char *s_value = NULL;
  char *d_value = NULL;
  char *n_value = NULL;
  char *r_value = NULL;
  char *o_value = NULL;
  int bflag = 0;
  int hflag = 0;
  int c;
  opterr = 0;

  struct option long_option[] = {{"help", no_argument, &hflag, 1}};

  int option_index = 0;

  while ((c = getopt_long(argc, argv, "V:B::s:d:n:r:o:h", long_option,
                          &option_index)) != -1) {
    switch (c) {
    case 0:
      if (long_option[option_index].flag != 0)
        break;
    case 'h':
      hflag = 1;
      break;

    case 'V':
      v_value = optarg;
      break;
    case 'B':
      bflag = 1;
      if (optarg == NULL && optind < argc && argv[optind][0] != '-') {
        optarg = argv[optind++];
      }
      b_value = optarg;
      break;

    case 's':
      s_value = optarg;
      break;

    case 'd':
      d_value = optarg;
      break;

    case 'n':
      n_value = optarg;
      break;

    case 'r':
      r_value = optarg;
      break;

    case 'o':
      o_value = optarg;
      break;

    default:
      fprintf(stderr, "Unavailable option. See the user manual:\n\n");
      usage();
      return 1;
      break;
    }
  }

  // Initialize null values with standard values.
  if (v_value == NULL) {
    v_value = STD_VERSION;
  }

  if (b_value == NULL) {
    b_value = STD_FUNCTION_CALL_NUMBER;
  }

  if (s_value == NULL) {
    s_value = STD_START_COORDINATE;
  }

  if (d_value == NULL) {
    d_value = STD_SIZE;
  }

  if (n_value == NULL) {
    n_value = STD_ITERATION_LIMIT;
  }

  if (r_value == NULL) {
    r_value = STD_RES;
  }

  if (o_value == NULL) {
    o_value = STD_FILENAME;
  }

  if (hflag) {
    usage();
    return 0;
  }

  // getting the option arguments from the user and parsing them to
  // corresponding

  if (argumentCheckerDs(s_value) == 0 && argumentCheckerDs(d_value) == 0) {
    fprintf(stderr, "False argument for the options -d and -s. If you want "
                    "more info, see the user manual (type --help or -h).\n");
    return 1;
  } else if (argumentCheckerDs(d_value) == 0) {
    fprintf(stderr, "False argument for the option -d. If you want more info, "
                    "see the user manual (type --help or -h).\n");
    return 1;
  } else if (argumentCheckerDs(s_value) == 0) {
    fprintf(stderr, "False argument for the option -s. If you want more info, "
                    "see the user manual (type --help or -h).\n");
    return 1;
  }
  unsigned version;

  sscanf(v_value, "%u", &version);

  unsigned function_call_number;

  sscanf(b_value, "%u", &function_call_number);

  float start_real, start_imag;
  sscanf(s_value, "%f,%f", &start_real, &start_imag);

  // check if start has a valid value

  if (isfinite(start_real) == 0 && isfinite(start_imag) == 0) {
    fprintf(stderr, "Given values do not have the type float. If you want more "
                    "info, see the user manual (type --help or -h).\n");
    return 1;
  } else if (isfinite(start_real) == 0) {
    fprintf(stderr, "<real> value does not have the type float. If you want "
                    "more info, see the user manual (type --help or -h).\n");
    return 1;
  } else if (isfinite(start_imag) == 0) {
    fprintf(stderr, "<imag> value does not have the type float. If you want "
                    "more info, see the user manual (type --help or -h).\n");
    return 1;
  }

  float complex start = start_real + start_imag * I;

  // check if width > 0 and height > 0

  long long int widthArg, heightArg;

  sscanf(d_value, "%lld,%lld", &widthArg, &heightArg);

  size_t width, height;

  if (widthArg < 0 || heightArg < 0) {
    fprintf(stderr, "Size and/or width cannot be negative numbers. If you want "
                    "more info, see the user manual (type --help or -h).\n");
    return 1;
  } else if (widthArg == 0 || heightArg == 0) {
    fprintf(stderr, "Size and/or width cannot be zero. If you want more info, "
                    "see the user manual (type --help or -h).\n");
    return 1;
  } else {
    width = widthArg;
    height = heightArg;
    size_t checkVar = width * height;
    if (checkVar / width != height) {
      fprintf(stderr, "Couldn't allocate enough space for the given "
                      "dimensions, try something smaller.\n");
      return 1;
    }
  }
  float res;

  sscanf(r_value, "%f", &res);

  // check if res has a valid value

  if (isfinite(res) == 0) {
    fprintf(stderr, "res value does not have the type float. If you want more "
                    "info, see the user manual (type --help or -h).\n");
    return 1;
  }

  long long int nArg;

  sscanf(n_value, "%lld", &nArg);

  unsigned n;

  if (nArg < 0) {
    fprintf(stderr, "n cannot be negative. If you want more info, see the user "
                    "manual (type --help or -h).\n");
    return 1;
  } else if (nArg > UINT32_MAX) {
    fprintf(stderr,
            "n cannot exceed the max value of unsigned integer (2^32-1). If "
            "you want more info, see the user manual (type --help or -h).\n");
    return 1;
  } else {
    n = nArg;
  }

  unsigned char *img;

  if (!(version == 0 || version == 1)) {
    fprintf(stderr, "Version can only be 0 or 1. If you want more info, see "
                    "the user manual (type --help or -h).\n");
    return 1;
  } else {
    img = (unsigned char *)malloc(width * height * 3);
    if (img == NULL) {
      fprintf(stderr, "Couldn't allocate enough space for the given "
                      "dimensions, try something smaller.\n");
      return 1;
    }
  }

  // Performance analysis
  if (bflag) {
    if (version == 0) {
      double maxTime = 0.0;
      for (int i = 0; i < function_call_number; i++) {
        struct timespec startTime;
        clock_gettime(CLOCK_MONOTONIC, &startTime);
        burning_ship(start, width, height, res, n, img);
        struct timespec endTime;
        clock_gettime(CLOCK_MONOTONIC, &endTime);
        maxTime += endTime.tv_sec - startTime.tv_sec +
                   1e-9 * (endTime.tv_nsec - startTime.tv_nsec);
      }
      double averageTime = maxTime / function_call_number;
      printf("Average time of the operation takes %f seconds (Function was "
             "called %d time(s).)\n",
             averageTime, function_call_number);

    } else {
      double maxTime = 0.0;
      for (int i = 0; i < function_call_number; i++) {
        struct timespec startTime;
        clock_gettime(CLOCK_MONOTONIC, &startTime);
        burning_ship_p(start, width, height, res, n, img);
        struct timespec endTime;
        clock_gettime(CLOCK_MONOTONIC, &endTime);
        maxTime += endTime.tv_sec - startTime.tv_sec +
                   1e-9 * (endTime.tv_nsec - startTime.tv_nsec);
      }
      double averageTime = maxTime / function_call_number;
      printf("Average time of the operation takes %f seconds (Function was "
             "called %d time(s).)\n",
             averageTime, function_call_number);
    }
    bmpImage_p image;
    bmpImageSet_p(&image, width, height, img);
    size_t fileNameSize = strlen(o_value) + 5;
    char fileName[fileNameSize];
    snprintf(fileName, fileNameSize, "%s.bmp", o_value);
    bmpImageWrite_p(&image, fileName);
    free(img);
    return 0;
  }

  // Normal execution
  else {
    if (version == 0) {
      burning_ship(start, width, height, res, n, img);
    } else {
      burning_ship_p(start, width, height, res, n, img);
    }
    bmpImage_p image;
    struct timespec startTime;
    clock_gettime(CLOCK_MONOTONIC, &startTime);
    bmpImageSet_p(&image, width, height, img);
    size_t fileNameSize = strlen(o_value) + 5;
    char fileName[fileNameSize];
    snprintf(fileName, fileNameSize, "%s.bmp", o_value);
    bmpImageWrite_p(&image, fileName);
    struct timespec endTime;
    clock_gettime(CLOCK_MONOTONIC, &endTime);
    double maxTime = endTime.tv_sec - startTime.tv_sec +
                     1e-9 * (endTime.tv_nsec - startTime.tv_nsec);
    printf("Time of the operation takes %f seconds (Function was called %d "
           "time(s).)\n",
           maxTime, function_call_number);
    free(img);
    return 0;
  }
  return 0;
}
