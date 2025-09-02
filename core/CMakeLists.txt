set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE CORE CONFIGURE_DEPENDS src/ *.cpp)
file(GLOB_RECURSE HPP CONFIGURE_DEPENDS include/ *.hpp)
add_executable(vkRenderer ${CORE} ${HPP})
target_include_directories(vkRenderer PUBLIC include)

target_link_libraries(vkRenderer PUBLIC glfw)
target_include_directories(vkRenderer PUBLIC "${GLFW_SOURCE_DIR}/deps")

find_package(Vulkan REQUIRED)
target_link_libraries(vkRenderer PUBLIC Vulkan::Vulkan)
