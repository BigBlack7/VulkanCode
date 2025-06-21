set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE CORE ./ *.cpp)
add_executable(engine ${CORE})

find_package(Vulkan REQUIRED)
target_link_libraries(engine PUBLIC Vulkan::Vulkan)
