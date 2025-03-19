#pragma once

#include <luisa/ast/type_registry.h>
#include <luisa/xir/constant.h>
#include <luisa/xir/instructions/alloca.h>
#include <luisa/xir/instructions/arithmetic.h>
#include <luisa/xir/instructions/assert.h>
#include <luisa/xir/instructions/assume.h>
#include <luisa/xir/instructions/atomic.h>
#include <luisa/xir/instructions/branch.h>
#include <luisa/xir/instructions/break.h>
#include <luisa/xir/instructions/call.h>
#include <luisa/xir/instructions/cast.h>
#include <luisa/xir/instructions/clock.h>
#include <luisa/xir/instructions/continue.h>
#include <luisa/xir/instructions/gep.h>
#include <luisa/xir/instructions/if.h>
#include <luisa/xir/instructions/autodiff.h>
#include <luisa/xir/instructions/load.h>
#include <luisa/xir/instructions/loop.h>
#include <luisa/xir/instructions/outline.h>
#include <luisa/xir/instructions/phi.h>
#include <luisa/xir/instructions/print.h>
#include <luisa/xir/instructions/ray_query.h>
#include <luisa/xir/instructions/raster_discard.h>
#include <luisa/xir/instructions/return.h>
#include <luisa/xir/instructions/resource.h>
#include <luisa/xir/instructions/store.h>
#include <luisa/xir/instructions/switch.h>
#include <luisa/xir/instructions/thread_group.h>
#include <luisa/xir/instructions/unreachable.h>

namespace luisa::compute::xir {

/**
 * @class XIRBuilder
 * @brief Helper class for constructing XIR instructions and basic blocks.
 * 
 * The XIRBuilder provides a convenient interface for creating and inserting
 * instructions into basic blocks. It maintains an insertion point where new
 * instructions are added and provides methods for creating various types of
 * instructions.
 */
class LC_XIR_API XIRBuilder {

private:
    /** @brief The memory pool for allocating instructions */
    Pool *_pool = nullptr;
    
    /** @brief The current insertion point for new instructions */
    Instruction *_insertion_point = nullptr;

private:
    /**
     * @brief Creates and appends an instruction of the specified type.
     * 
     * @tparam T The instruction type to create
     * @tparam Args The argument types for the instruction constructor
     * @param args The arguments for the instruction constructor
     * @return A pointer to the created instruction
     */
    template<typename T, typename... Args>
    [[nodiscard]] auto _create_and_append_instruction(Args &&...args) noexcept -> T *;

public:
    /**
     * @brief Constructs a new XIRBuilder.
     */
    XIRBuilder() noexcept;
    
    /**
     * @brief Sets the insertion point to the specified instruction.
     * 
     * @param insertion_point The new insertion point
     */
    void set_insertion_point(Instruction *insertion_point) noexcept;
    
    /**
     * @brief Sets the insertion point to the end of the specified basic block.
     * 
     * @param block The basic block to set as the insertion point
     */
    void set_insertion_point(BasicBlock *block) noexcept;
    
    /**
     * @brief Gets the current insertion point.
     * 
     * @return The current insertion point
     */
    [[nodiscard]] auto insertion_point() noexcept -> Instruction * { return _insertion_point; }
    
    /**
     * @brief Gets the current insertion point (const version).
     * 
     * @return The current insertion point
     */
    [[nodiscard]] auto insertion_point() const noexcept -> const Instruction * { return _insertion_point; }

    /**
     * @brief Checks if the current insertion point is a terminator instruction.
     * 
     * @return True if the insertion point is a terminator instruction
     */
    [[nodiscard]] auto is_insertion_point_terminator() const noexcept {
        return _insertion_point != nullptr &&
               _insertion_point->is_terminator();
    }

public:
    /**
     * @brief Appends an instruction after the current insertion point.
     * 
     * @param inst The instruction to append
     */
    void append(Instruction *inst) noexcept;

    /**
     * @brief Creates an if instruction with the given condition.
     * 
     * @param cond The condition value
     * @return A pointer to the created if instruction
     */
    IfInst *if_(Value *cond) noexcept;
    
    /**
     * @brief Creates a switch instruction with the given value.
     * 
     * @param value The switch value
     * @return A pointer to the created switch instruction
     */
    SwitchInst *switch_(Value *value) noexcept;
    
    /**
     * @brief Creates a loop instruction.
     * 
     * @return A pointer to the created loop instruction
     */
    LoopInst *loop() noexcept;
    
    /**
     * @brief Creates a simple loop instruction (do-while).
     * 
     * @return A pointer to the created simple loop instruction
     */
    SimpleLoopInst *simple_loop() noexcept;

    /**
     * @brief Creates an unconditional branch instruction.
     * 
     * @param target The target basic block (optional)
     * @return A pointer to the created branch instruction
     */
    BranchInst *br(BasicBlock *target = nullptr) noexcept;
    
    /**
     * @brief Creates a conditional branch instruction.
     * 
     * @param cond The condition value
     * @param true_target The target block for the true condition (optional)
     * @param false_target The target block for the false condition (optional)
     * @return A pointer to the created conditional branch instruction
     */
    ConditionalBranchInst *cond_br(Value *cond, BasicBlock *true_target = nullptr, BasicBlock *false_target = nullptr) noexcept;

    /**
     * @brief Creates a break instruction.
     * 
     * @param target_block The target block to break to (optional)
     * @return A pointer to the created break instruction
     */
    BreakInst *break_(BasicBlock *target_block = nullptr) noexcept;
    
    /**
     * @brief Creates a continue instruction.
     * 
     * @param target_block The target block to continue to (optional)
     * @return A pointer to the created continue instruction
     */
    ContinueInst *continue_(BasicBlock *target_block = nullptr) noexcept;
    
    /**
     * @brief Creates an unreachable instruction.
     * 
     * @param message An optional message explaining why the code is unreachable
     * @return A pointer to the created unreachable instruction
     */
    UnreachableInst *unreachable_(luisa::string_view message = {}) noexcept;
    
    /**
     * @brief Creates a return instruction with a value.
     * 
     * @param value The value to return
     * @return A pointer to the created return instruction
     */
    ReturnInst *return_(Value *value) noexcept;
    
    /**
     * @brief Creates a void return instruction.
     * 
     * @return A pointer to the created return instruction
     */
    ReturnInst *return_void() noexcept;
    
    /**
     * @brief Creates a raster discard instruction.
     * 
     * @return A pointer to the created raster discard instruction
     */
    RasterDiscardInst *raster_discard() noexcept;

    /**
     * @brief Creates an assert instruction.
     * 
     * @param condition The condition to assert
     * @param message An optional message for the assertion
     * @return A pointer to the created assert instruction
     */
    AssertInst *assert_(Value *condition, luisa::string_view message = {}) noexcept;
    
    /**
     * @brief Creates an assume instruction.
     * 
     * @param condition The condition to assume
     * @param message An optional message for the assumption
     * @return A pointer to the created assume instruction
     */
    AssumeInst *assume_(Value *condition, luisa::string_view message = {}) noexcept;

    /**
     * @brief Creates a function call instruction.
     * 
     * @param type The result type of the call
     * @param callee The function to call
     * @param arguments The arguments to pass to the function
     * @return A pointer to the created call instruction
     */
    CallInst *call(const Type *type, Function *callee, luisa::span<Value *const> arguments) noexcept;
    
    /**
     * @brief Creates a function call instruction with initializer list arguments.
     * 
     * @param type The result type of the call
     * @param callee The function to call
     * @param arguments The arguments to pass to the function
     * @return A pointer to the created call instruction
     */
    CallInst *call(const Type *type, Function *callee, std::initializer_list<Value *> arguments) noexcept;

    /**
     * @brief Creates an autodiff intrinsic instruction.
     * 
     * @param type The result type of the intrinsic
     * @param op The autodiff intrinsic operation
     * @param arguments The arguments for the intrinsic
     * @return A pointer to the created autodiff intrinsic instruction
     */
    AutodiffIntrinsicInst *call(const Type *type, AutodiffIntrinsicOp op, luisa::span<Value *const> arguments) noexcept;
    
    /**
     * @brief Creates an autodiff intrinsic instruction with initializer list arguments.
     * 
     * @param type The result type of the intrinsic
     * @param op The autodiff intrinsic operation
     * @param arguments The arguments for the intrinsic
     * @return A pointer to the created autodiff intrinsic instruction
     */
    AutodiffIntrinsicInst *call(const Type *type, AutodiffIntrinsicOp op, std::initializer_list<Value *> arguments) noexcept;

    /**
     * @brief Creates an atomic instruction.
     * 
     * @param type The result type of the atomic operation
     * @param op The atomic operation
     * @param base The base pointer for the atomic operation
     * @param indices The indices for the atomic operation
     * @param values The values for the atomic operation
     * @return A pointer to the created atomic instruction
     */
    AtomicInst *call(const Type *type, AtomicOp op, Value *base, luisa::span<Value *const> indices, luisa::span<Value *const> values) noexcept;
    
    /**
     * @brief Creates an atomic instruction with initializer list values.
     * 
     * @param type The result type of the atomic operation
     * @param op The atomic operation
     * @param base The base pointer for the atomic operation
     * @param indices The indices for the atomic operation
     * @param values The values for the atomic operation
     * @return A pointer to the created atomic instruction
     */
    AtomicInst *call(const Type *type, AtomicOp op, Value *base, luisa::span<Value *const> indices, std::initializer_list<Value *> values) noexcept;

    /**
     * @brief Creates a thread group instruction.
     * 
     * @param type The result type of the thread group operation
     * @param op The thread group operation
     * @param operands The operands for the thread group operation
     * @return A pointer to the created thread group instruction
     */
    ThreadGroupInst *call(const Type *type, ThreadGroupOp op, luisa::span<Value *const> operands) noexcept;
    
    /**
     * @brief Creates a thread group instruction with initializer list operands.
     * 
     * @param type The result type of the thread group operation
     * @param op The thread group operation
     * @param operands The operands for the thread group operation
     * @return A pointer to the created thread group instruction
     */
    ThreadGroupInst *call(const Type *type, ThreadGroupOp op, std::initializer_list<Value *> operands) noexcept;

    /**
     * @brief Creates an arithmetic instruction.
     * 
     * @param type The result type of the arithmetic operation
     * @param op The arithmetic operation
     * @param operands The operands for the arithmetic operation
     * @return A pointer to the created arithmetic instruction
     */
    ArithmeticInst *call(const Type *type, ArithmeticOp op, luisa::span<Value *const> operands) noexcept;
    
    /**
     * @brief Creates an arithmetic instruction with initializer list operands.
     * 
     * @param type The result type of the arithmetic operation
     * @param op The arithmetic operation
     * @param operands The operands for the arithmetic operation
     * @return A pointer to the created arithmetic instruction
     */
    ArithmeticInst *call(const Type *type, ArithmeticOp op, std::initializer_list<Value *> operands) noexcept;

    /**
     * @brief Creates a resource query instruction.
     * 
     * @param type The result type of the resource query
     * @param op The resource query operation
     * @param operands The operands for the resource query
     * @return A pointer to the created resource query instruction
     */
    ResourceQueryInst *call(const Type *type, ResourceQueryOp op, luisa::span<Value *const> operands) noexcept;
    
    /**
     * @brief Creates a resource query instruction with initializer list operands.
     * 
     * @param type The result type of the resource query
     * @param op The resource query operation
     * @param operands The operands for the resource query
     * @return A pointer to the created resource query instruction
     */
    ResourceQueryInst *call(const Type *type, ResourceQueryOp op, std::initializer_list<Value *> operands) noexcept;

    /**
     * @brief Creates a resource read instruction.
     * 
     * @param type The result type of the resource read
     * @param op The resource read operation
     * @param operands The operands for the resource read
     * @return A pointer to the created resource read instruction
     */
    ResourceReadInst *call(const Type *type, ResourceReadOp op, luisa::span<Value *const> operands) noexcept;
    
    /**
     * @brief Creates a resource read instruction with initializer list operands.
     * 
     * @param type The result type of the resource read
     * @param op The resource read operation
     * @param operands The operands for the resource read
     * @return A pointer to the created resource read instruction
     */
    ResourceReadInst *call(const Type *type, ResourceReadOp op, std::initializer_list<Value *> operands) noexcept;

    /**
     * @brief Creates a resource write instruction.
     * 
     * @param op The resource write operation
     * @param operands The operands for the resource write
     * @return A pointer to the created resource write instruction
     */
    ResourceWriteInst *call(ResourceWriteOp op, luisa::span<Value *const> operands) noexcept;
    
    /**
     * @brief Creates a resource write instruction with initializer list operands.
     * 
     * @param op The resource write operation
     * @param operands The operands for the resource write
     * @return A pointer to the created resource write instruction
     */
    ResourceWriteInst *call(ResourceWriteOp op, std::initializer_list<Value *> operands) noexcept;

    /**
     * @brief Creates a cast instruction.
     * 
     * @param type The target type of the cast
     * @param op The cast operation
     * @param value The value to cast
     * @return A pointer to the created cast instruction
     */
    CastInst *cast_(const Type *type, CastOp op, Value *value) noexcept;

    /**
     * @brief Creates a static cast instruction.
     * 
     * @param type The target type of the cast
     * @param value The value to cast
     * @return A pointer to the created cast instruction
     */
    Instruction *static_cast_(const Type *type, Value *value) noexcept;
    
    /**
     * @brief Creates a bitwise cast instruction.
     * 
     * @param type The target type of the cast
     * @param value The value to cast
     * @return A pointer to the created cast instruction
     */
    CastInst *bit_cast_(const Type *type, Value *value) noexcept;

    /**
     * @brief Creates a static cast instruction if necessary.
     * 
     * If the value already has the target type, it is returned as is.
     * Otherwise, a static cast instruction is created.
     * 
     * @param type The target type of the cast
     * @param value The value to cast
     * @return The original value or a new cast instruction
     */
    Value *static_cast_if_necessary(const Type *type, Value *value) noexcept;
    
    /**
     * @brief Creates a bitwise cast instruction if necessary.
     * 
     * If the value already has the target type, it is returned as is.
     * Otherwise, a bitwise cast instruction is created.
     * 
     * @param type The target type of the cast
     * @param value The value to cast
     * @return The original value or a new cast instruction
     */
    Value *bit_cast_if_necessary(const Type *type, Value *value) noexcept;

    /**
     * @brief Creates a phi instruction.
     * 
     * @param type The result type of the phi node
     * @param incomings The incoming values and blocks
     * @return A pointer to the created phi instruction
     */
    PhiInst *phi(const Type *type, luisa::span<const PhiIncoming> incomings = {}) noexcept;
    
    /**
     * @brief Creates a phi instruction with initializer list incomings.
     * 
     * @param type The result type of the phi node
     * @param incomings The incoming values and blocks
     * @return A pointer to the created phi instruction
     */
    PhiInst *phi(const Type *type, std::initializer_list<PhiIncoming> incomings) noexcept;

    /**
     * @brief Creates a print instruction.
     * 
     * @param format The format string
     * @param values The values to print
     * @return A pointer to the created print instruction
     */
    PrintInst *print(luisa::string format, luisa::span<Value *const> values) noexcept;
    
    /**
     * @brief Creates a print instruction with initializer list values.
     * 
     * @param format The format string
     * @param values The values to print
     * @return A pointer to the created print instruction
     */
    PrintInst *print(luisa::string format, std::initializer_list<Value *> values) noexcept;

    /**
     * @brief Creates an alloca instruction.
     * 
     * @param type The type of the allocated memory
     * @param space The allocation space (local or shared)
     * @return A pointer to the created alloca instruction
     */
    AllocaInst *alloca_(const Type *type, AllocSpace space) noexcept;
    
    /**
     * @brief Creates a local alloca instruction.
     * 
     * @param type The type of the allocated memory
     * @return A pointer to the created alloca instruction
     */
    AllocaInst *alloca_local(const Type *type) noexcept;
    
    /**
     * @brief Creates a shared alloca instruction.
     * 
     * @param type The type of the allocated memory
     * @return A pointer to the created alloca instruction
     */
    AllocaInst *alloca_shared(const Type *type) noexcept;

    /**
     * @brief Creates a GEP (get element pointer) instruction.
     * 
     * @param type The result type of the GEP
     * @param base The base pointer
     * @param indices The indices for the GEP
     * @return A pointer to the created GEP instruction
     */
    GEPInst *gep(const Type *type, Value *base, luisa::span<Value *const> indices) noexcept;
    
    /**
     * @brief Creates a GEP instruction with initializer list indices.
     * 
     * @param type The result type of the GEP
     * @param base The base pointer
     * @param indices The indices for the GEP
     * @return A pointer to the created GEP instruction
     */
    GEPInst *gep(const Type *type, Value *base, std::initializer_list<Value *> indices) noexcept;

    /**
     * @brief Creates a load instruction.
     * 
     * @param type The type of the loaded value
     * @param variable The variable to load from
     * @return A pointer to the created load instruction
     */
    LoadInst *load(const Type *type, Value *variable) noexcept;
    
    /**
     * @brief Creates a store instruction.
     * 
     * @param variable The variable to store to
     * @param value The value to store
     * @return A pointer to the created store instruction
     */
    StoreInst *store(Value *variable, Value *value) noexcept;

    /**
     * @brief Creates a clock instruction.
     * 
     * @return A pointer to the created clock instruction
     */
    ClockInst *clock() noexcept;

    /**
     * @brief Creates an outline instruction.
     * 
     * @return A pointer to the created outline instruction
     */
    OutlineInst *outline() noexcept;

    /**
     * @brief Creates an autodiff scope instruction.
     * 
     * @return A pointer to the created autodiff scope instruction
     */
    AutodiffScopeInst *autodiff_scope() noexcept;

    /**
     * @brief Creates a ray query loop instruction.
     * 
     * @return A pointer to the created ray query loop instruction
     */
    RayQueryLoopInst *ray_query_loop() noexcept;
    
    /**
     * @brief Creates a ray query dispatch instruction.
     * 
     * @param query_object The ray query object
     * @return A pointer to the created ray query dispatch instruction
     */
    RayQueryDispatchInst *ray_query_dispatch(Value *query_object) noexcept;

    /**
     * @brief Creates a ray query object read instruction.
     * 
     * @param type The result type of the read
     * @param op The ray query object read operation
     * @param operands The operands for the read
     * @return A pointer to the created ray query object read instruction
     */
    RayQueryObjectReadInst *call(const Type *type, RayQueryObjectReadOp op, luisa::span<Value *const> operands) noexcept;
    
    /**
     * @brief Creates a ray query object read instruction with initializer list operands.
     * 
     * @param type The result type of the read
     * @param op The ray query object read operation
     * @param operands The operands for the read
     * @return A pointer to the created ray query object read instruction
     */
    RayQueryObjectReadInst *call(const Type *type, RayQueryObjectReadOp op, std::initializer_list<Value *> operands) noexcept;
    
    /**
     * @brief Creates a ray query object write instruction.
     * 
     * @param op The ray query object write operation
     * @param operands The operands for the write
     * @return A pointer to the created ray query object write instruction
     */
    RayQueryObjectWriteInst *call(RayQueryObjectWriteOp op, luisa::span<Value *const> operands) noexcept;
    
    /**
     * @brief Creates a ray query object write instruction with initializer list operands.
     * 
     * @param op The ray query object write operation
     * @param operands The operands for the write
     * @return A pointer to the created ray query object write instruction
     */
    RayQueryObjectWriteInst *call(RayQueryObjectWriteOp op, std::initializer_list<Value *> operands) noexcept;

    /**
     * @brief Creates a ray query pipeline instruction.
     * 
     * @param query_object The ray query object
     * @param on_surface The surface callback function
     * @param on_procedural The procedural callback function
     * @param captured_args The captured arguments
     * @return A pointer to the created ray query pipeline instruction
     */
    RayQueryPipelineInst *ray_query_pipeline(Value *query_object = nullptr,
                                             Function *on_surface = nullptr,
                                             Function *on_procedural = nullptr,
                                             luisa::span<Value *const> captured_args = {}) noexcept;

    /**
     * @brief Creates an atomic fetch-add instruction.
     * 
     * @param type The result type of the atomic operation
     * @param base The base pointer
     * @param indices The indices for the atomic operation
     * @param value The value to add
     * @return A pointer to the created atomic instruction
     */
    AtomicInst *atomic_fetch_add(const Type *type, Value *base, luisa::span<Value *const> indices, Value *value) noexcept;
    
    /**
     * @brief Creates an atomic fetch-sub instruction.
     * 
     * @param type The result type of the atomic operation
     * @param base The base pointer
     * @param indices The indices for the atomic operation
     * @param value The value to subtract
     * @return A pointer to the created atomic instruction
     */
    AtomicInst *atomic_fetch_sub(const Type *type, Value *base, luisa::span<Value *const> indices, Value *value) noexcept;
    
    /**
     * @brief Creates an atomic fetch-and instruction.
     * 
     * @param type The result type of the atomic operation
     * @param base The base pointer
     * @param indices The indices for the atomic operation
     * @param value The value to AND with
     * @return A pointer to the created atomic instruction
     */
    AtomicInst *atomic_fetch_and(const Type *type, Value *base, luisa::span<Value *const> indices, Value *value) noexcept;
    
    /**
     * @brief Creates an atomic fetch-or instruction.
     * 
     * @param type The result type of the atomic operation
     * @param base The base pointer
     * @param indices The indices for the atomic operation
     * @param value The value to OR with
     * @return A pointer to the created atomic instruction
     */
    AtomicInst *atomic_fetch_or(const Type *type, Value *base, luisa::span<Value *const> indices, Value *value) noexcept;
    
    /**
     * @brief Creates an atomic fetch-xor instruction.
     * 
     * @param type The result type of the atomic operation
     * @param base The base pointer
     * @param indices The indices for the atomic operation
     * @param value The value to XOR with
     * @return A pointer to the created atomic instruction
     */
    AtomicInst *atomic_fetch_xor(const Type *type, Value *base, luisa::span<Value *const> indices, Value *value) noexcept;
    
    /**
     * @brief Creates an atomic fetch-min instruction.
     * 
     * @param type The result type of the atomic operation
     * @param base The base pointer
     * @param indices The indices for the atomic operation
     * @param value The value to compare with
     * @return A pointer to the created atomic instruction
     */
    AtomicInst *atomic_fetch_min(const Type *type, Value *base, luisa::span<Value *const> indices, Value *value) noexcept;
    
    /**
     * @brief Creates an atomic fetch-max instruction.
     * 
     * @param type The result type of the atomic operation
     * @param base The base pointer
     * @param indices The indices for the atomic operation
     * @param value The value to compare with
     * @return A pointer to the created atomic instruction
     */
    AtomicInst *atomic_fetch_max(const Type *type, Value *base, luisa::span<Value *const> indices, Value *value) noexcept;
    
    /**
     * @brief Creates an atomic exchange instruction.
     * 
     * @param type The result type of the atomic operation
     * @param base The base pointer
     * @param indices The indices for the atomic operation
     * @param value The value to exchange with
     * @return A pointer to the created atomic instruction
     */
    AtomicInst *atomic_exchange(const Type *type, Value *base, luisa::span<Value *const> indices, Value *value) noexcept;
    
    /**
     * @brief Creates an atomic compare-exchange instruction.
     * 
     * @param type The result type of the atomic operation
     * @param base The base pointer
     * @param indices The indices for the atomic operation
     * @param expected The expected value
     * @param desired The desired value
     * @return A pointer to the created atomic instruction
     */
    AtomicInst *atomic_compare_exchange(const Type *type, Value *base, luisa::span<Value *const> indices, Value *expected, Value *desired) noexcept;

    /**
     * @brief Creates a shader execution reorder instruction.
     * 
     * @return A pointer to the created thread group instruction
     */
    ThreadGroupInst *shader_execution_reorder() noexcept;
    
    /**
     * @brief Creates a shader execution reorder instruction with hint.
     * 
     * @param hint The reordering hint
     * @param hint_bits The hint bits
     * @return A pointer to the created thread group instruction
     */
    ThreadGroupInst *shader_execution_reorder(Value *hint, Value *hint_bits) noexcept;
    
    /**
     * @brief Creates a synchronize block instruction.
     * 
     * @return A pointer to the created thread group instruction
     */
    ThreadGroupInst *synchronize_block() noexcept;
    
    /**
     * @brief Creates a raster quad derivative in X direction instruction.
     * 
     * @param type The result type
     * @param value The value to compute the derivative of
     * @return A pointer to the created thread group instruction
     */
    ThreadGroupInst *raster_quad_ddx(const Type *type, Value *value) noexcept;
    
    /**
     * @brief Creates a raster quad derivative in Y direction instruction.
     * 
     * @param type The result type
     * @param value The value to compute the derivative of
     * @return A pointer to the created thread group instruction
     */
    ThreadGroupInst *raster_quad_ddy(const Type *type, Value *value) noexcept;
};

}// namespace luisa::compute::xir
