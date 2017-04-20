#define YATF_MAIN
#include <yatf.h>
#include <cstdio>

void console_init();

int main(int argc, const char *argv[]) {
    console_init();
    return yatf::main(printf, argc, argv);
}

