

// Fixes warning
#undef ERROR

//#define ERROR(s) do { fprintf(stderr, "%s\n", (s)); exit(-1); } while(0)
#define INFO(s) do { fprintf(stdout, "INFO: %s in %s:%d\n", (s), __FILE__, __LINE__); } while(0)
#define INFO_COUNT(s, c) do { fprintf(stderr, "INFO: %s %d in %s:%d\n", (s), (c), __FILE__, __LINE__); } while(0)
#define ERROR(s) do { fprintf(stderr, "ERROR: %s in %s:%d\n", (s), __FILE__, __LINE__); exit(-1); } while(0)
#define ERROR_COUNT(s, c) do { fprintf(stderr, "ERROR: %s %d in %s:%d\n", (s), (c), __FILE__, __LINE__); exit(-1); } while(0)
#define ERROR2(...) do { fprintf(stderr, __VA_ARGS__); exit(-1); } while(0)
#define PERROR(s) do { perror((s)); exit(-1); } while(0)
#define USAGE_ERROR(s) do { fprintf(stderr, "%s\n", (s)); print_help_and_exit(argv[0]); } while(0)
