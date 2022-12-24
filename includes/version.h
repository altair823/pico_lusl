#include "binary.h"

#define VERSION_INDICATOR 0x1


typedef struct {
    char major;
    char minor;
    char patch;
} version_t;

/// Reads the version from the binary.
/// @param binary The binary to read from.
/// @param version The version to read into.
/// @return 0 on success, -1 on failure.
int read_version(binary_t* binary, version_t* version);