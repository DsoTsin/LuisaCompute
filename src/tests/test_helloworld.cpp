#include <luisa/luisa-compute.h>

using namespace luisa;
using namespace luisa::compute;

int main(int argc, char *argv[]) {
    Context ctx(argv[0]);
    Device device = ctx.create_default_device();    
}
