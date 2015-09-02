project(poplite_world_list_game)

include_directories(gen)

# find_package( Boost 1.40 COMPONENTS serialization system thread serialization context log filesystem program_options REQUIRED )
# include_directories(${Boost_INCLUDE_DIR})

find_package(Qt5Widgets REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})

pop_add_object(RenderThread renderthread.hpp renderthread.cpp)
pop_add_object(MandelbrotWidget mandelbrotwidget.hpp mandelbrotwidget.cpp gen/renderthread.iface.hpp renderthread.cpp)
add_executable(mandelbrot main.cpp gen/mandelbrotwidget.iface.hpp gen/renderthread.iface.hpp)
target_link_libraries(mandelbrot ${Boost_LIBRARIES} ${Qt5Widgets_LIBRARIES})
