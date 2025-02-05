#include <luisa/luisa-compute.h>

using namespace luisa;
using namespace luisa::compute;

int main() {

    auto module = xir::Module{};
    module.add_comment("My very simple test module.");
    module.set_name("TestModule");
    auto u32_zero = module.create_constant_zero(Type::of<uint>());
    auto f32_one = module.create_constant_one(Type::of<float>());
    auto bool_false = module.create_constant_zero(Type::of<bool>());
    bool_false->add_comment("bool constant false");

    auto b = xir::Builder{};

    auto f = module.create_callable(Type::of<float>());
    f->set_name("callable_function");
    f->set_location(__FILE__, __LINE__);
    f->add_comment("This is a callable function.");

    auto x = f->create_value_argument(Type::of<float>());
    auto y = f->create_value_argument(Type::of<float>());
    auto ray = f->create_value_argument(Type::of<Ray>());
    ray->add_comment("This is a ray...");

    b.set_insertion_point(f->create_body_block());
    auto add = b.call(Type::of<float>(), xir::ArithmeticOp::BINARY_MUL, {x, y});
    auto mul = b.call(Type::of<float>(), xir::ArithmeticOp::BINARY_ADD, {add, y});
    auto coord = module.create_dispatch_id();
    auto coord_x = b.call(Type::of<uint>(), xir::ArithmeticOp::EXTRACT, {coord, u32_zero});
    auto outline = b.outline();
    auto outline_body = outline->create_target_block();
    b.set_insertion_point(outline_body);
    auto switch_ = b.switch_(coord_x);
    switch_->add_comment("switch on x coordinate");
    switch_->add_comment("if (x == 0) { goto case 0; } else { goto default; }");
    auto switch_case_0 = switch_->create_case_block(0);
    switch_case_0->add_comment("switch case 0");
    b.set_insertion_point(switch_case_0);
    auto cond0 = b.call(Type::of<bool>(), xir::ArithmeticOp::BINARY_EQUAL, {coord_x, u32_zero});
    auto switch_default = switch_->create_default_block();
    auto switch_merge = switch_->create_merge_block();
    b.set_insertion_point(switch_merge);
    auto cond = b.phi(Type::of<bool>());
    cond->add_incoming(cond0, switch_case_0);
    cond->add_incoming(bool_false, switch_default);
    auto outline_merge = outline->create_merge_block();
    b.set_insertion_point(outline_merge);
    auto branch = b.if_(cond);
    auto true_block = branch->create_true_block();
    b.set_insertion_point(true_block);
    auto rq = b.alloca_local(Type::of<RayQueryAll>());
    b.print("({} + {}) * {} = {}", {x, y, y, mul});
    auto merge = branch->create_merge_block();
    b.set_insertion_point(merge);
    b.return_(mul);

    auto k = module.create_kernel();
    auto buffer = k->create_resource_argument(Type::of<Buffer<float>>());
    b.set_insertion_point(k->create_body_block());
    auto va = b.alloca_local(Type::of<float>());
    auto vb = b.alloca_local(Type::of<float>());
    b.store(va, f32_one);
    b.store(vb, f32_one);
    auto result = b.call(Type::of<float>(), f, {va, vb});
    b.call(xir::ResourceWriteOp::BUFFER_WRITE, {buffer, u32_zero, result});
    b.return_void();

    auto dummy = module.create_callable(nullptr);
    b.set_insertion_point(dummy->create_body_block());
    b.return_void();

    LUISA_INFO("IR:\n{}", xir::xir_to_text_translate(&module, true));
}
