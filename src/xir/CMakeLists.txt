set(LUISA_COMPUTE_XIR_SOURCES
        argument.cpp
        basic_block.cpp
        builder.cpp
        constant.cpp
        function.cpp
        instruction.cpp
        metadata.cpp
        module.cpp
        pool.cpp
        special_register.cpp
        undefined.cpp
        use.cpp
        user.cpp
        value.cpp

        # derived instructions
        instructions/alloca.cpp
        instructions/arithmetic.cpp
        instructions/assert.cpp
        instructions/assume.cpp
        instructions/atomic.cpp
        instructions/autodiff.cpp
        instructions/branch.cpp
        instructions/break.cpp
        instructions/call.cpp
        instructions/cast.cpp
        instructions/clock.cpp
        instructions/continue.cpp
        instructions/gep.cpp
        instructions/if.cpp
        instructions/load.cpp
        instructions/loop.cpp
        instructions/op_name_map.cpp
        instructions/outline.cpp
        instructions/phi.cpp
        instructions/print.cpp
        instructions/raster_discard.cpp
        instructions/ray_query.cpp
        instructions/resource.cpp
        instructions/return.cpp
        instructions/store.cpp
        instructions/switch.cpp
        instructions/thread_group.cpp
        instructions/unreachable.cpp

        # derived metadata
        metadata/location.cpp
        metadata/comment.cpp
        metadata/name.cpp

        # translators
        translators/ast2xir.cpp
        translators/json2xir.cpp
        translators/xir2json.cpp
        translators/xir2text.cpp

        # passes
        passes/helpers.cpp
        passes/dce.cpp
        passes/dom_tree.cpp
        passes/call_graph.cpp
        passes/outline.cpp
        passes/sroa.cpp
        passes/trace_gep.cpp
        passes/aggregate_field_bitmask.cpp
        passes/pointer_usage.cpp
        passes/local_store_forward.cpp
        passes/local_load_elimination.cpp
        passes/early_return_elimination.cpp
        passes/lower_ray_query_loop.cpp
        passes/mem2reg.cpp
        passes/autodiff.cpp
        passes/transpose_gep.cpp
        passes/unused_callable_removal.cpp
        passes/reg2mem.cpp
        passes/promote_ref_arg.cpp
)

add_library(luisa-compute-xir SHARED ${LUISA_COMPUTE_XIR_SOURCES})
target_link_libraries(luisa-compute-xir
        PUBLIC luisa-compute-ast
        PRIVATE luisa-compute-ext-yyjson-interface)

target_compile_definitions(luisa-compute-xir PUBLIC LUISA_ENABLE_XIR=1)
target_compile_definitions(luisa-compute-xir PRIVATE LC_XIR_EXPORT_DLL=1)

set_target_properties(luisa-compute-xir PROPERTIES
        UNITY_BUILD ${LUISA_COMPUTE_ENABLE_UNITY_BUILD}
        OUTPUT_NAME lc-xir)

luisa_compute_install(xir)

if (LUISA_COMPUTE_ENABLE_XIR_TESTS)
    add_executable(test_aggregate_field_bitmasks tests/test_aggregate_field_bitmasks.cpp)
    target_link_libraries(test_aggregate_field_bitmasks PRIVATE luisa-compute-xir)

    add_executable(test_mem2reg tests/test_mem2reg.cpp)
    target_link_libraries(test_mem2reg PRIVATE luisa::compute)
endif ()
