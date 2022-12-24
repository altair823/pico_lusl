#include "version.h"

int read_version(binary_t* binary, version_t* version) {
    if (binary->data[0] != VERSION_INDICATOR){
        return -1;
    }
    version->major = binary->data[1];
    version->minor = binary->data[2];
    version->patch = binary->data[3];
    return 0;
}