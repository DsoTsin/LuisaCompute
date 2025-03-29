#pragma once
#include <luisa/tensor/tensor.h>
#include <luisa/ast/usage.h>
#include <luisa/vstl/functional.h>
#include <luisa/tensor/fused_activation.h>
namespace luisa::compute {
// clang-format off
#define LUISA_COMPUTE_TENSOR_EXPRRESSIONS \
    ScopeExpr,                          \
    SetValueExpr,                       \
    LCGRandExpr,                        \
    GEMMExpr,                           \
    ConvExpr,                           \
    TestExpr
// clang-format on

#define LUISA_MAKE_TENSOR_EXPR_DECL(CMD) class CMD;
LUISA_MAP(LUISA_MAKE_TENSOR_EXPR_DECL, LUISA_COMPUTE_TENSOR_EXPRRESSIONS)
#undef LUISA_MAKE_TENSOR_EXPR_DECL

struct TensorExprVisitor {
#define LUISA_MAKE_TENSOR_EXPR_VISITOR_INTERFACE(CMD) \
    virtual void visit(const CMD *) noexcept = 0;
    LUISA_MAP(LUISA_MAKE_TENSOR_EXPR_VISITOR_INTERFACE, LUISA_COMPUTE_TENSOR_EXPRRESSIONS)
#undef LUISA_MAKE_TENSOR_EXPR_VISITOR_INTERFACE
    virtual ~TensorExprVisitor() noexcept = default;
};

class TensorExpr {
    uint64_t _idx;
    //     TensorExpr *_last = nullptr;
    //     TensorExpr *_next = nullptr;
    // public:
    //     LC_TENSOR_API std::pair<TensorExpr *, TensorExpr *> remove_self() noexcept;
    //     LC_TENSOR_API void add_after(TensorExpr *expr) noexcept;
    //     LC_TENSOR_API void add_before(TensorExpr *expr) noexcept;
    //     [[nodiscard]] auto last() const noexcept { return _last; }
    //     [[nodiscard]] auto next() const noexcept { return _next; }
public:
    virtual void get_tensors(vstd::FuncRef<void(TensorData *, Usage usage)> callback) noexcept = 0;
    TensorExpr(uint64_t idx) noexcept : _idx(idx) {}
    enum struct Tag : uint32_t {
#define LUISA_MAKE_TENSOR_EXPR_TAG(Cmd) E##Cmd,
        LUISA_MAP(LUISA_MAKE_TENSOR_EXPR_TAG, LUISA_COMPUTE_TENSOR_EXPRRESSIONS)
#undef LUISA_MAKE_TENSOR_EXPR_TAG
    };
    [[nodiscard]] virtual Tag tag() noexcept = 0;
    [[nodiscard]] uint64_t idx() const noexcept { return _idx; }
    virtual void accept(TensorExprVisitor *) const noexcept = 0;
    virtual ~TensorExpr() noexcept = default;
};

template<typename Derive, TensorExpr::Tag _tag>
class TensorExprCRTPDerive : public TensorExpr {
protected:
    using BaseClass = typename TensorExprCRTPDerive<Derive, _tag>;
public:
    TensorExprCRTPDerive(uint64_t idx) noexcept : TensorExpr(idx) {}
    Tag tag() noexcept override {
        return _tag;
    }
    static constexpr TensorExpr::Tag const_tag = _tag;
    virtual ~TensorExprCRTPDerive() noexcept = default;
    void accept(TensorExprVisitor *visitor) const noexcept override;
};

#define LUISA_TENSOR_EXPR_CLASS_INHERIT(ClassName) ClassName final : public TensorExprCRTPDerive<ClassName, TensorExpr::Tag::E##ClassName>
class LC_TENSOR_API LUISA_TENSOR_EXPR_CLASS_INHERIT(ScopeExpr) {
public:
    struct Arg {
        TensorData *tensor;
        Usage usage;
    };
    luisa::vector<Arg> arguments;
    luisa::vector<TensorExpr *> expressions;
    ScopeExpr(uint64_t idx) noexcept;
    ~ScopeExpr() noexcept;
    template<typename T, typename... Args>
        requires(std::is_base_of_v<TensorExpr, T> && luisa::is_constructible_v<T, uint64_t, Args && ...>)
    T *allocate_expr(Args &&...args) noexcept;
    void get_tensors(vstd::FuncRef<void(TensorData *, Usage usage)> callback) noexcept override {
        for (auto &i : arguments) {
            callback(i.tensor, i.usage);
        }
    }
};
class LUISA_TENSOR_EXPR_CLASS_INHERIT(SetValueExpr) {
public:
    TensorData *tensor_data;
    uint value;
    SetValueExpr(
        uint64_t idx,
        TensorData *tensor_data,
        uint32_t value) noexcept
        : BaseClass(idx),
          tensor_data(tensor_data),
          value(value) {}
    void get_tensors(vstd::FuncRef<void(TensorData *, Usage usage)> callback) noexcept override {
        callback(tensor_data, Usage::WRITE);
    }
};
class LUISA_TENSOR_EXPR_CLASS_INHERIT(LCGRandExpr) {
public:
    TensorData *tensor_data;
    uint64_t group_size;
    uint seed;
    LCGRandExpr(
        uint64_t idx,
        TensorData *tensor_data,
        uint64_t group_size,
        uint seed) noexcept
        : BaseClass(idx),
          tensor_data(tensor_data),
          group_size(group_size),
          seed(seed) {}
    void get_tensors(vstd::FuncRef<void(TensorData *, Usage usage)> callback) noexcept override {
        callback(tensor_data, Usage::WRITE);
    }
};
class LC_TENSOR_API LUISA_TENSOR_EXPR_CLASS_INHERIT(GEMMExpr) {
public:
    TensorData *lhs_tensor;
    TensorData *rhs_tensor;
    TensorData *output_tensor;
    FusedActivation fused_activation;
    GEMMExpr(
        uint64_t idx,
        TensorData *lhs_tensor,
        TensorData *rhs_tensor,
        FusedActivation const &fused_activation,
        TensorElementType out_type) noexcept;
    void get_tensors(vstd::FuncRef<void(TensorData *, Usage usage)> callback) noexcept override {
        callback(lhs_tensor, Usage::READ);
        callback(rhs_tensor, Usage::READ);
        callback(output_tensor, Usage::WRITE);
    }
};

class LC_TENSOR_API LUISA_TENSOR_EXPR_CLASS_INHERIT(ConvExpr) {
public:
    TensorData *input_tensor;
    TensorData *weight_tensor;
    TensorData *out_tensor;
    FusedActivation fused_activation;
    luisa::fixed_vector<uint, 3> filter_size;
    luisa::fixed_vector<uint, 3> dilation;
    luisa::fixed_vector<uint, 3> start_paddings;
    luisa::fixed_vector<uint, 3> end_paddings;
    void get_tensors(vstd::FuncRef<void(TensorData *, Usage usage)> callback) noexcept override {
        callback(input_tensor, Usage::READ);
        callback(weight_tensor, Usage::READ);
        callback(out_tensor, Usage::WRITE);
    }
    ConvExpr(
        uint64_t idx,
        TensorData *input_tensor,
        TensorData *weight_tensor,
        FusedActivation const &fused_activation,
        luisa::span<uint const> filter_size,
        luisa::span<uint const> dilation,
        luisa::span<uint const> start_paddings,
        luisa::span<uint const> end_paddings,
        TensorElementType out_type) noexcept;
    [[nodiscard]] uint dimension() const noexcept { return filter_size.size(); }
    static luisa::variant<TensorData *, luisa::string> get_output_tensor(TensorData *input_tensor, TensorData *filter_tensor) noexcept;
};

class LC_TENSOR_API LUISA_TENSOR_EXPR_CLASS_INHERIT(TestExpr) {
public:
    TensorData *input;
    TensorData *output;
    luisa::string_view name;
    TestExpr(
        uint64_t idx,
        TensorData *input,
        TensorData *output,
        luisa::string_view name) noexcept;
    void get_tensors(vstd::FuncRef<void(TensorData *, Usage usage)> callback) noexcept override {
        callback(input, Usage::READ);
        callback(output, Usage::WRITE);
    }
};

#undef LUISA_TENSOR_EXPR_CLASS_INHERIT
template<typename Derive, TensorExpr::Tag _tag>
inline void TensorExprCRTPDerive<Derive, _tag>::accept(TensorExprVisitor *visitor) const noexcept {
    visitor->visit(static_cast<Derive const *>(this));
}
}// namespace luisa::compute
