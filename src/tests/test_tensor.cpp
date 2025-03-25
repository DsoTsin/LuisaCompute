#include <luisa/runtime/context.h>
#include <luisa/runtime/device.h>
#include <luisa/runtime/stream.h>
#include <luisa/dsl/sugar.h>
#include <luisa/core/clock.h>
#include <luisa/tensor/tensor_builder.h>
#include <luisa/tensor/pass/expr_topo.h>
#include <fstream>
using namespace luisa;
using namespace luisa::compute;

int main(int argc, char *argv[]) {
    auto ctx = Context(argv[0]);
    auto device = ctx.create_device("dx");
    auto stream = device.create_stream();

    TensorBuilder builder;
    TensorBuilder::set_thd_local(&builder);
    auto sizes = {1ull};
    auto a = builder.allocate_tensor(luisa::span{sizes.begin(), sizes.size()}, TensorElementType::Float32);
    auto b = builder.allocate_tensor(luisa::span{sizes.begin(), sizes.size()}, TensorElementType::Float32);
    auto c = builder.allocate_tensor(luisa::span{sizes.begin(), sizes.size()}, TensorElementType::Float32);
    auto d = builder.allocate_tensor(luisa::span{sizes.begin(), sizes.size()}, TensorElementType::Float32);
    auto e = builder.allocate_tensor(luisa::span{sizes.begin(), sizes.size()}, TensorElementType::Float32);
    auto a_expr = builder.root_expr().allocate_expr<TestExpr>(a, b, "A");
    auto d_expr = builder.root_expr().allocate_expr<TestExpr>(c, d, "D");
    auto c_expr = builder.root_expr().allocate_expr<TestExpr>(b, c, "C");
    auto b_expr = builder.root_expr().allocate_expr<TestExpr>(b, c, "B");
    auto e_expr = builder.root_expr().allocate_expr<TestExpr>(d, e, "E");
    auto sorted =  ExprTopo::topo_sort(
        builder.root_expr().expressions,
        builder.allocated_tensor().size());
    LUISA_INFO("size {}", sorted.size());
    for(auto& i : sorted) {
        LUISA_INFO("{}", static_cast<TestExpr*>(i)->name);
    }
}