find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(
  recorder
  main.cpp
  program.cpp
  channel.cpp
  epg.cpp
  channel_model.cpp)

target_link_libraries(recorder Qt5::Widgets pugixml)
