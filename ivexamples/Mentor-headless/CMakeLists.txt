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
add_headless_example(02.3.Trackball)
add_headless_example(02.4.Examiner)
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
add_headless_example(05.6.TransformOrdering)
add_headless_example(06.1.Text)
add_headless_example(06.2.Simple3DText)
add_headless_example(06.3.Complex3DText)
add_headless_example(07.1.BasicTexture)
add_headless_example(07.2.TextureCoordinates)
add_headless_example(07.3.TextureFunction)
add_headless_example(08.1.BSCurve)
add_headless_example(08.2.UniCurve)
add_headless_example(08.3.BezSurf)
add_headless_example(08.4.TrimSurf)
add_headless_example(09.1.Print)
add_headless_example(09.2.Texture)
add_headless_example(09.3.Search)
add_headless_example(09.4.PickAction)
add_headless_example(09.5.GenSph)
add_headless_example(10.1.addEventCB)
add_headless_example(10.5.SelectionCB)
add_headless_example(10.6.PickFilterTopLevel)
add_headless_example(10.7.PickFilterManip)
add_headless_example(11.1.ReadFile)
add_headless_example(11.2.ReadString)
add_headless_example(12.1.FieldSensor)
add_headless_example(12.2.NodeSensor)
add_headless_example(12.3.AlarmSensor)
add_headless_example(12.4.TimerSensor)
add_headless_example(13.1.GlobalFlds)
add_headless_example(13.2.ElapsedTime)
add_headless_example(13.3.TimeCounter)
add_headless_example(13.4.Gate)
add_headless_example(13.5.Boolean)
add_headless_example(13.6.Calculator)
add_headless_example(13.7.Rotor)
add_headless_example(13.8.Blinker)

# Chapter 14: NodeKits
add_headless_example(14.1.FrolickingWords)
add_headless_example(14.3.Balance)

# Chapter 15: Manipulators
add_headless_example(15.1.ConeRadius)
add_headless_example(15.2.SliderBox)
add_headless_example(15.3.AttachManip)
add_headless_example(15.4.Customize)

# Chapter 17: OpenGL Integration
add_headless_example(17.2.GLCallback)

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
