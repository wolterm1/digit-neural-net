cmake_minimum_required(VERSION 3.15)


project(
  MatrixLinAlgProject
  VERSION 1.0
  LANGUAGES CXX)


# ASAN
set(ENABLE_ASAN ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)

fetchcontent_declare(
  lin-alg
  GIT_REPOSITORY https://github.com/wolterm1/lin-alg.git
  GIT_TAG master
)
fetchcontent_makeavailable(lin-alg)

# SFML
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
fetchcontent_declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
fetchcontent_makeavailable(SFML)


add_library(components
    Components/button.cpp
    Components/button.hpp
    Components/button_grid.hpp
    Components/button_grid.cpp
    Components/probability_histogram.hpp
    Components/probability_histogram.cpp
)
target_link_libraries(components PUBLIC SFML::Graphics PUBLIC lin-alg)
target_include_directories(components PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Components)




add_library(neural-net
    neural_net/classification_metrics.cpp
    neural_net/classification_metrics.hpp
    neural_net/confusion_matrix.cpp
    neural_net/confusion_matrix.hpp
    neural_net/helper.cpp
    neural_net/helper.hpp
    neural_net/net.cpp
    neural_net/net.hpp
    neural_net/optimizer.cpp
    neural_net/optimizer.hpp
)

target_link_libraries(neural-net PUBLIC lin-alg)
target_include_directories(neural-net PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/neural_net)




add_executable(nn_test src/nntest.cpp)
add_executable(loadEval src/loadEval.cpp)
set(MY_TARGETS nn_test loadEval)
foreach(target ${MY_TARGETS})
  target_link_libraries(${target} PRIVATE neural-net)

  if(ENABLE_ASAN)
    target_compile_options(${target} PRIVATE -fsanitize=address)
    target_link_options(${target} PRIVATE -fsanitize=address)
  endif()
endforeach()


add_executable(main src/main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics components neural-net)
# SFML END
