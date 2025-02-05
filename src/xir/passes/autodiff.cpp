#include <luisa/core/logging.h>
#include <luisa/xir/module.h>
#include <luisa/xir/builder.h>
#include <luisa/xir/undefined.h>
#include <luisa/xir/instructions/autodiff.h>
#include <luisa/xir/passes/autodiff.h>
#include <luisa/xir/passes/dom_tree.h>
#include "helpers.h"

namespace luisa::compute::xir {

struct TransformAdScope {
    Function *function{};
    AutodiffScopeInst *ad_scope{};
    luisa::vector<BasicBlock *> ad_blocks{};
    luisa::vector<const Instruction *> requires_grads{};
    luisa::vector<const Instruction *> intermediate{};
    void inline_all_callables() {}
    void locate_intermediates() {
    }
    void promote_intermediates() {
    }
    void generate_backward() {
    }
    void run() {
        inline_all_callables();
        locate_intermediates();
        promote_intermediates();
        generate_backward();
    }
};

struct AutodiffPass {
    Function *function{};
    AutodiffOptions options{};
    auto locate_autodiff_scopes() {
        auto def = function->definition();
        auto dom = compute_dom_tree(def);
        luisa::vector<std::pair<AutodiffScopeInst *, luisa::vector<BasicBlock *>>> ad_scopes;
        luisa::unordered_set<BasicBlock *> visited;
        def->traverse_basic_blocks([&](BasicBlock *block) {
            if (visited.contains(block)) {
                return;
            }
            block->traverse_instructions([&](Instruction *inst) {
                auto tag = inst->derived_instruction_tag();
                if (tag == DerivedInstructionTag::AUTODIFF_SCOPE) {
                    auto ad_scope = static_cast<AutodiffScopeInst *>(inst);

                    LUISA_INFO("Found autodiff scope: {}", ad_scope->name().value_or("unnamed"));
                    auto ad_blocks = luisa::vector<BasicBlock *>{};
                    block->traverse_successors(false, [&](BasicBlock *succ) {
                        ad_blocks.emplace_back(succ);
                    });
                }
            });
            visited.emplace(block);
        });

        return ad_scopes;
    }
    void run() {
        if (!function->definition()) {
            return;
        }
        auto scopes = locate_autodiff_scopes();
        for (auto scope : scopes) {
            // TransformAdScope transform{function, scope};
            // transform.run();
        }
    }
};

LC_XIR_API void autodiff_pass_run_on_function(Function *function, const AutodiffOptions &options) noexcept {
    AutodiffPass pass{function, options};
    pass.run();
}

LC_XIR_API void autodiff_pass_run_on_module(Module *module, const AutodiffOptions &options) noexcept {
    for (auto &func : module->function_list()) {
        autodiff_pass_run_on_function(&func, options);
    }
}

}// namespace luisa::compute::xir