
function(SETUP_HAL_SIMD)

    if(NOT DEFINED SIMD)
        set(SIMD OFF)
    endif()

    string(TOUPPER "${SIMD}" SIMD)

    if(SIMD STREQUAL "OFF")

        message(STATUS "HAL SIMD: OFF")
        add_compile_definitions(HAL_SIMD_FORCE_SCALAR)

    elseif(SIMD STREQUAL "ON")

        message(STATUS "HAL SIMD: ON")

    elseif(SIMD STREQUAL "AUTO")

        if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|amd64")

            set(HAL_CPU_TEST "${CMAKE_BINARY_DIR}/hal_cpu_features.cpp")

            file(WRITE "${HAL_CPU_TEST}" "
#include <cstdio>

int main()
{
#if defined(__GNUC__) || defined(__clang__)
    if (__builtin_cpu_supports(\"avx512f\"))
        std::printf(\"avx512f\\n\");

    if (__builtin_cpu_supports(\"avx2\"))
        std::printf(\"avx2\\n\");

    if (__builtin_cpu_supports(\"fma\"))
        std::printf(\"fma\\n\");

    if (__builtin_cpu_supports(\"sse4.2\"))
        std::printf(\"sse4.2\\n\");
#endif
    return 0;
}
")

            try_run(
                HAL_CPU_RUN_RESULT
                HAL_CPU_COMPILE_RESULT
                "${CMAKE_BINARY_DIR}/hal_cpu_test"
                "${HAL_CPU_TEST}"
                OUTPUT_VARIABLE HAL_CPU_OUTPUT
            )

            if(HAL_CPU_COMPILE_RESULT)

                string(FIND "${HAL_CPU_OUTPUT}" "avx512f" HAL_HAS_AVX512)
                string(FIND "${HAL_CPU_OUTPUT}" "avx2"    HAL_HAS_AVX2)
                string(FIND "${HAL_CPU_OUTPUT}" "fma"     HAL_HAS_FMA)
                string(FIND "${HAL_CPU_OUTPUT}" "sse4.2"  HAL_HAS_SSE42)

                if(HAL_HAS_AVX512 GREATER -1)

                    check_cxx_compiler_flag(
                        "-mavx512f"
                        HAL_COMPILER_HAS_AVX512
                    )

                    if(HAL_COMPILER_HAS_AVX512)
                        message(STATUS "HAL SIMD: AVX-512")
                        add_compile_options(-mavx512f)
                    endif()

                elseif(HAL_HAS_AVX2 GREATER -1)

                    check_cxx_compiler_flag(
                        "-mavx2"
                        HAL_COMPILER_HAS_AVX2
                    )

                    if(HAL_COMPILER_HAS_AVX2)

                        message(STATUS "HAL SIMD: AVX2")
                        add_compile_options(-mavx2)

                        if(HAL_HAS_FMA GREATER -1)
                            check_cxx_compiler_flag(
                                "-mfma"
                                HAL_COMPILER_HAS_FMA
                            )

                            if(HAL_COMPILER_HAS_FMA)
                                add_compile_options(-mfma)
                            endif()
                        endif()

                    endif()

                elseif(HAL_HAS_SSE42 GREATER -1)

                    check_cxx_compiler_flag(
                        "-msse4.2"
                        HAL_COMPILER_HAS_SSE42
                    )

                    if(HAL_COMPILER_HAS_SSE42)
                        message(STATUS "HAL SIMD: SSE4.2")
                        add_compile_options(-msse4.2)
                    endif()

                else()

                    message(STATUS
                        "HAL SIMD: no supported x86 SIMD detected"
                    )

                endif()

            else()

                message(WARNING
                    "HAL SIMD: CPU detection failed, using compiler default"
                )

            endif()

        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")

            message(STATUS "HAL SIMD: ARM64 NEON/AdvSIMD")

        else()

            message(STATUS
                "HAL SIMD: unknown architecture, compiler default"
            )

        endif()

    else()

        message(FATAL_ERROR
            "SIMD must be OFF, ON or AUTO"
        )

    endif()

endfunction()