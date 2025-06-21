cmake_minimum_required(VERSION 3.20)
project(VulkanStudy LANGUAGES CXX)

include(ProcessorCount)
ProcessorCount(N)

if(N EQUAL 0)
    set(N 24)
endif()

set(CMAKE_BUILD_PARALLEL_LEVEL ${N})

add_subdirectory(core)

# add_subdirectory(thirdparty/glm)
# add_subdirectory(thirdparty/glfw)
