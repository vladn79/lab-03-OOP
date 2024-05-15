cmake_minimum_required(VERSION 3.27)
project(lab03_oop)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab03_oop
        recursive_alg/point.h
        recursive_alg/point.cpp
        recursive_alg/ConvexHull.h
        recursive_alg/ConvexHull.cpp

        graham_alg/convexhull_graham.h
        graham_alg/convexhull_graham.cpp
        graham_alg/point_for_graham.h

        andrew_jarvis_alg/Point_for_Jarvis.h
        andrew_jarvis_alg/ConvexHullJarvis.h
        andrew_jarvis_alg/ConvexHullJarvis.cpp


        kirkpatrick_alg/Point_kirkpatrick.h
        kirkpatrick_alg/convexhull_kirkpatrick.h
        kirkpatrick_alg/convexhull_kirkpatrick.cpp


        main.cpp
        fortunes_alg/math.h
        fortunes_alg/math.cpp
        fortunes_alg/data_structures.h
        fortunes_alg/data_structures.cpp
        fortunes_alg/realization.h
        fortunes_alg/realization.cpp

)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/Users/38093/OneDrive/Документи/libs/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.1/lib/cmake/SFML)

find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/Users/38093/OneDrive/Документи/libs/SFML-2.6.1-windows-gcc-13.1.0-mingw-64-bit/SFML-2.6.1/include)

target_link_libraries(lab03_oop sfml-system sfml-window sfml-graphics sfml-audio)
