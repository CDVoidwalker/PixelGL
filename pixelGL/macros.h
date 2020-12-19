#ifndef PIXELGL_MACROS
#define PIXELGL_MACROS
#ifdef PIXELGL_DEBUG
#define vlog(x)         \
    do                  \
    {                   \
        printf(x);      \
        printf("\n");   \
        fflush(stdout); \
    } while (0)

#define vlogf(x, args...) \
    do                    \
    {                     \
        printf(x, args);  \
        printf("\n");     \
        fflush(stdout);   \
    } while (0)
#else
#define vlog(x)
#define vlogf(x, args...)
#endif
#define vclamp(val, min, max) \
    do                        \
    {                         \
        if (val < min)        \
            val = min;        \
        if (val > max)        \
            val = max;        \
    } while (0)

#define vthrow(error)           \
    do                          \
    {                           \
        fprintf(stderr, error); \
        exit(-1);               \
    } while (0)

#define vthrowf(error, args...)       \
    do                                \
    {                                 \
        fprintf(stderr, error, args); \
        exit(-1);                     \
    } while (0)
#endif