#include <luisa/luisa-compute.h>

using namespace luisa;
using namespace luisa::compute;

struct Juice {
    std::array<float3x3, 10> m;
};

struct Apple {
    int x;
    bool b;
    float2 y;
    Juice j;
};

LUISA_STRUCT(Juice, m) {};
LUISA_STRUCT(Apple, x, b, y, j) {};

auto dump(const xir::AggregateFieldBitmask &m) noexcept {
    luisa::string s;
    for (auto i = m.size_buckets(); i != 0u; i--) {
        auto bucket = m.raw_bits()[i - 1];
        for (auto j = 64u; j != 0u; j--) {
            if ((i - 1u) * 64u + j - 1u < m.size()) {
                s.append((bucket & (1ull << (j - 1u))) ? "1" : "0");
            }
        }
    }
    return s;
}

int main() {
    xir::AggregateFieldBitmask m1{Type::of<Apple>()};
    m1.access(2, 0).set();
    m1.access(3, 0, 2, 1).set();
    m1.access(3).set();
    LUISA_INFO("m1      = {}", dump(m1));
    xir::AggregateFieldBitmask m2{Type::of<Apple>()};
    m2.access(1).set();
    m2.access(2).set();
    m2.access(3, 0, 6).set();
    LUISA_INFO("m2      = {}", dump(m2));
    LUISA_INFO("m1 | m2 = {}", dump(m1 | m2));
    LUISA_INFO("m1 & m2 = {}", dump(m1 & m2));
    LUISA_INFO("m1 ^ m2 = {}", dump(m1 ^ m2));
}
