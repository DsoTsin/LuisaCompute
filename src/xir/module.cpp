#include <luisa/core/logging.h>
#include <luisa/xir/constant.h>
#include <luisa/xir/function.h>
#include <luisa/xir/module.h>

namespace luisa::compute::xir {

KernelFunction *Module::create_kernel() noexcept {
    auto f = pool()->create<KernelFunction>(this);
    f->add_to_list(_function_list);
    return f;
}

CallableFunction *Module::create_callable(const Type *ret_type) noexcept {
    auto f = pool()->create<CallableFunction>(this, ret_type);
    f->add_to_list(_function_list);
    return f;
}

ExternalFunction *Module::create_external_function(const Type *ret_type) noexcept {
    auto f = pool()->create<ExternalFunction>(this, ret_type);
    f->add_to_list(_function_list);
    return f;
}

Constant *Module::_get_or_create_constant(const Constant &temp) noexcept {
    auto [iter, success] = _hash_to_constant.try_emplace(temp.hash(), nullptr);
    if (success) {
        auto pooled_const = pool()->create<Constant>(this, temp.type(), temp.data(), temp.hash());
        iter->second = pooled_const;
        pooled_const->add_to_list(_constant_list);
    }
    return iter->second;
}

Constant *Module::create_constant(const Type *type, const void *data) noexcept {
    Constant temp{this, type, data};
    return _get_or_create_constant(temp);
}

Constant *Module::create_constant_zero(const Type *type) noexcept {
    Constant temp{this, type, Constant::ctor_tag_zero{}};
    return _get_or_create_constant(temp);
}

Constant *Module::create_constant_one(const Type *type) noexcept {
    Constant temp{this, type, Constant::ctor_tag_one{}};
    return _get_or_create_constant(temp);
}

Undefined *Module::create_undefined(const Type *type) noexcept {
    auto [iter, success] = _type_to_undefined.try_emplace(type, nullptr);
    if (success) {
        auto undef = pool()->create<Undefined>(this, type);
        iter->second = undef;
        undef->add_to_list(_undefined_list);
    }
    return iter->second;
}

SpecialRegister *Module::create_special_register(DerivedSpecialRegisterTag tag) noexcept {
    auto [iter, success] = _tag_to_special_register.try_emplace(tag, nullptr);
    if (success) {
        auto sreg = [tag, this]() noexcept -> SpecialRegister * {
            switch (tag) {
                case DerivedSpecialRegisterTag::THREAD_ID: return pool()->create<SPR_ThreadID>(this);
                case DerivedSpecialRegisterTag::BLOCK_ID: return pool()->create<SPR_BlockID>(this);
                case DerivedSpecialRegisterTag::WARP_LANE_ID: return pool()->create<SPR_WarpLaneID>(this);
                case DerivedSpecialRegisterTag::DISPATCH_ID: return pool()->create<SPR_DispatchID>(this);
                case DerivedSpecialRegisterTag::KERNEL_ID: return pool()->create<SPR_KernelID>(this);
                case DerivedSpecialRegisterTag::OBJECT_ID: return pool()->create<SPR_ObjectID>(this);
                case DerivedSpecialRegisterTag::BLOCK_SIZE: return pool()->create<SPR_BlockSize>(this);
                case DerivedSpecialRegisterTag::WARP_SIZE: return pool()->create<SPR_WarpSize>(this);
                case DerivedSpecialRegisterTag::DISPATCH_SIZE: return pool()->create<SPR_DispatchSize>(this);
                default: break;
            }
            LUISA_ERROR_WITH_LOCATION("Unsupported special register tag.");
        }();
        iter->second = sreg;
        sreg->add_to_list(_special_register_list);
    }
    return iter->second;
}

SPR_ThreadID *Module::create_thread_id() noexcept {
    auto sreg = create_special_register(DerivedSpecialRegisterTag::THREAD_ID);
    LUISA_DEBUG_ASSERT(sreg->isa<SPR_ThreadID>(), "Invalid special register type.");
    return static_cast<SPR_ThreadID *>(sreg);
}

SPR_BlockID *Module::create_block_id() noexcept {
    auto sreg = create_special_register(DerivedSpecialRegisterTag::BLOCK_ID);
    LUISA_DEBUG_ASSERT(sreg->isa<SPR_BlockID>(), "Invalid special register type.");
    return static_cast<SPR_BlockID *>(sreg);
}

SPR_WarpLaneID *Module::create_warp_lane_id() noexcept {
    auto sreg = create_special_register(DerivedSpecialRegisterTag::WARP_LANE_ID);
    LUISA_DEBUG_ASSERT(sreg->isa<SPR_WarpLaneID>(), "Invalid special register type.");
    return static_cast<SPR_WarpLaneID *>(sreg);
}

SPR_DispatchID *Module::create_dispatch_id() noexcept {
    auto sreg = create_special_register(DerivedSpecialRegisterTag::DISPATCH_ID);
    LUISA_DEBUG_ASSERT(sreg->isa<SPR_DispatchID>(), "Invalid special register type.");
    return static_cast<SPR_DispatchID *>(sreg);
}

SPR_KernelID *Module::create_kernel_id() noexcept {
    auto sreg = create_special_register(DerivedSpecialRegisterTag::KERNEL_ID);
    LUISA_DEBUG_ASSERT(sreg->isa<SPR_KernelID>(), "Invalid special register type.");
    return static_cast<SPR_KernelID *>(sreg);
}

SPR_ObjectID *Module::create_object_id() noexcept {
    auto sreg = create_special_register(DerivedSpecialRegisterTag::OBJECT_ID);
    LUISA_DEBUG_ASSERT(sreg->isa<SPR_ObjectID>(), "Invalid special register type.");
    return static_cast<SPR_ObjectID *>(sreg);
}

SPR_BlockSize *Module::create_block_size() noexcept {
    auto sreg = create_special_register(DerivedSpecialRegisterTag::BLOCK_SIZE);
    LUISA_DEBUG_ASSERT(sreg->isa<SPR_BlockSize>(), "Invalid special register type.");
    return static_cast<SPR_BlockSize *>(sreg);
}

SPR_WarpSize *Module::create_warp_size() noexcept {
    auto sreg = create_special_register(DerivedSpecialRegisterTag::WARP_SIZE);
    LUISA_DEBUG_ASSERT(sreg->isa<SPR_WarpSize>(), "Invalid special register type.");
    return static_cast<SPR_WarpSize *>(sreg);
}

SPR_DispatchSize *Module::create_dispatch_size() noexcept {
    auto sreg = create_special_register(DerivedSpecialRegisterTag::DISPATCH_SIZE);
    LUISA_DEBUG_ASSERT(sreg->isa<SPR_DispatchSize>(), "Invalid special register type.");
    return static_cast<SPR_DispatchSize *>(sreg);
}

}// namespace luisa::compute::xir
