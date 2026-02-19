# CMake script to run an image comparison test
#
# This script:
# 1. Runs the example executable to generate a test image (SGI RGB format)
# 2. Compares the test image against the PNG control image
# 3. Reports pass/fail based on comparison result
#
# The executable receives TEST_BASE (no .rgb extension) as argv[1] so that:
#   - Single-output examples writing  argv[1]+".rgb"  produce TEST_BASE.rgb
#   - Multi-output examples writing   argv[1]+"_view.rgb" produce TEST_BASE_view.rgb
#   - Direct-write examples using argv[1] as full path produce TEST_BASE (no ext)
# After execution we search for the primary test image:
#   TEST_BASE.rgb  (most examples)
#   TEST_BASE      (direct-write examples like HelloCone)
#   TEST_BASE_*.rgb (first alphabetically, fallback for multi-view)

if(NOT DEFINED EXECUTABLE)
    message(FATAL_ERROR "EXECUTABLE not defined")
endif()
if(NOT DEFINED TEST_BASE)
    message(FATAL_ERROR "TEST_BASE not defined")
endif()
if(NOT DEFINED CONTROL_IMAGE)
    message(FATAL_ERROR "CONTROL_IMAGE not defined")
endif()
if(NOT DEFINED COMPARATOR)
    message(FATAL_ERROR "COMPARATOR not defined")
endif()
if(NOT DEFINED HASH_THRESHOLD)
    set(HASH_THRESHOLD 5)
endif()
if(NOT DEFINED RMSE_THRESHOLD)
    set(RMSE_THRESHOLD 5.0)
endif()
if(NOT DEFINED TEST_TIMEOUT)
    set(TEST_TIMEOUT 30)
endif()

# Create output directory
get_filename_component(TEST_DIR "${TEST_BASE}" DIRECTORY)
file(MAKE_DIRECTORY "${TEST_DIR}")

if(NOT EXISTS "${CONTROL_IMAGE}")
    message(FATAL_ERROR "Control image not found: ${CONTROL_IMAGE}")
endif()

# Run the example with the base name as argv[1]
message("Running example: ${EXECUTABLE}")
execute_process(
    COMMAND "${EXECUTABLE}" "${TEST_BASE}"
    RESULT_VARIABLE EXEC_RESULT
    OUTPUT_VARIABLE EXEC_OUTPUT
    ERROR_VARIABLE EXEC_ERROR
    TIMEOUT ${TEST_TIMEOUT}
    WORKING_DIRECTORY "${TEST_DIR}"
)

if(NOT EXEC_RESULT EQUAL 0)
    message(FATAL_ERROR "Example execution failed (code ${EXEC_RESULT})\nOutput: ${EXEC_OUTPUT}\nError: ${EXEC_ERROR}")
endif()

# Find the generated test image:
#  1. TEST_BASE.rgb   (append-.rgb examples: 09.1.Print, 11.2.ReadString, etc.)
#  2. TEST_BASE       (direct-write examples: 02.1.HelloCone)
#  3. First TEST_BASE_*.rgb (multi-view examples: fallback)
set(TEST_IMAGE "")
get_filename_component(TEST_BASENAME "${TEST_BASE}" NAME)
get_filename_component(TEST_DIR_ABS "${TEST_BASE}" DIRECTORY)

if(EXISTS "${TEST_BASE}.rgb")
    set(TEST_IMAGE "${TEST_BASE}.rgb")
elseif(EXISTS "${TEST_BASE}")
    set(TEST_IMAGE "${TEST_BASE}")
else()
    # Look for TEST_BASE_*.rgb
    file(GLOB _candidates "${TEST_BASE}_*.rgb")
    if(_candidates)
        list(SORT _candidates)
        list(GET _candidates 0 TEST_IMAGE)
    endif()
endif()

if(NOT TEST_IMAGE OR NOT EXISTS "${TEST_IMAGE}")
    message(FATAL_ERROR "Test image was not generated (looked for ${TEST_BASE}.rgb, ${TEST_BASE}, ${TEST_BASE}_*.rgb)")
endif()

# Run image comparator
message("Comparing images...")
message("  Control: ${CONTROL_IMAGE}")
message("  Test:    ${TEST_IMAGE}")
message("  Thresholds: hash=${HASH_THRESHOLD}, rmse=${RMSE_THRESHOLD}")

execute_process(
    COMMAND "${COMPARATOR}"
            "--threshold" "${HASH_THRESHOLD}"
            "--rmse" "${RMSE_THRESHOLD}"
            "--verbose"
            "${CONTROL_IMAGE}"
            "${TEST_IMAGE}"
    RESULT_VARIABLE COMPARE_RESULT
    OUTPUT_VARIABLE COMPARE_OUTPUT
    ERROR_VARIABLE COMPARE_ERROR
)

if(COMPARE_OUTPUT)
    message("${COMPARE_OUTPUT}")
endif()
if(COMPARE_ERROR)
    message("${COMPARE_ERROR}")
endif()

if(COMPARE_RESULT EQUAL 0)
    message("TEST PASSED: Images match within threshold")
elseif(COMPARE_RESULT EQUAL 1)
    message(FATAL_ERROR "TEST FAILED: Images differ beyond threshold")
else()
    message(FATAL_ERROR "TEST ERROR: Comparator failed with code ${COMPARE_RESULT}")
endif()
