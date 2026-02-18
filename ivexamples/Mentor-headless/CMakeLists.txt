cmake_minimum_required(VERSION 3.0)

project(MentorHeadlessExamples)

# Find Coin package
# First try to find Coin in the build tree
set(Coin_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../build")
find_package(Coin QUIET)

if(NOT Coin_FOUND)
    # Try to use the library from the build directory directly
    set(COIN_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include")
    set(COIN_BUILD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../build/include")
    set(COIN_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../build/lib")
    
    # Add include directories
    include_directories(${COIN_BUILD_INCLUDE_DIR})
    include_directories(${COIN_INCLUDE_DIR})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    
    # Find required libraries
    find_package(OpenGL REQUIRED)
    
    # Link directories
    link_directories(${COIN_LIB_DIR})
    
    # Macro to add headless examples
    macro(add_headless_example name)
        add_executable(${name} ${name}.cpp)
        target_include_directories(${name} PRIVATE 
            ${COIN_BUILD_INCLUDE_DIR}
            ${COIN_INCLUDE_DIR} 
            ${CMAKE_CURRENT_SOURCE_DIR})
        target_link_libraries(${name} Coin ${OPENGL_LIBRARIES})
        set_target_properties(${name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        )
    endmacro()
else()
    # Use found Coin package
    include_directories(${Coin_INCLUDE_DIRS})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    
    macro(add_headless_example name)
        add_executable(${name} ${name}.cpp)
        target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
        target_link_libraries(${name} Coin::Coin)
        set_target_properties(${name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        )
    endmacro()
endif()

# Add headless examples
add_headless_example(02.1.HelloCone)
add_headless_example(02.2.EngineSpin)
add_headless_example(03.1.Molecule)
add_headless_example(03.2.Robot)
add_headless_example(03.3.Naming)
add_headless_example(04.1.Cameras)
add_headless_example(04.2.Lights)
add_headless_example(05.1.FaceSet)
add_headless_example(05.2.IndexedFaceSet)
add_headless_example(05.3.TriangleStripSet)
add_headless_example(05.4.QuadMesh)
add_headless_example(05.5.Binding)
add_headless_example(06.1.Text)
add_headless_example(06.2.Simple3DText)
add_headless_example(07.1.BasicTexture)
add_headless_example(09.1.Print)
add_headless_example(09.3.Search)
add_headless_example(11.1.ReadFile)
add_headless_example(11.2.ReadString)

# Create output directory for rendered images
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/output")

# Add a custom target to run all examples and generate images
add_custom_target(run_examples
    COMMAND ${CMAKE_COMMAND} -E echo "Running headless examples..."
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/output"
    DEPENDS 02.1.HelloCone 02.2.EngineSpin 03.1.Molecule 03.2.Robot
    COMMENT "Building all headless examples"
)

# Individual run targets
add_custom_target(run_HelloCone
    COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/output && ${CMAKE_CURRENT_BINARY_DIR}/bin/02.1.HelloCone
    DEPENDS 02.1.HelloCone
    COMMENT "Running HelloCone example"
)

add_custom_target(run_EngineSpin
    COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/output && ${CMAKE_CURRENT_BINARY_DIR}/bin/02.2.EngineSpin
    DEPENDS 02.2.EngineSpin
    COMMENT "Running EngineSpin example"
)

add_custom_target(run_Molecule
    COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/output && ${CMAKE_CURRENT_BINARY_DIR}/bin/03.1.Molecule
    DEPENDS 03.1.Molecule
    COMMENT "Running Molecule example"
)

add_custom_target(run_Robot
    COMMAND cd ${CMAKE_CURRENT_BINARY_DIR}/output && ${CMAKE_CURRENT_BINARY_DIR}/bin/03.2.Robot
    DEPENDS 03.2.Robot
    COMMENT "Running Robot example"
)
