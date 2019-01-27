#1.1 My project is based on the version of lunar.If your ros is different version,please change the CMakeLists.txt .Please make "set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /opt/ros/lunar)" to "set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /opt/ros/your_ros_dis)"

#2.1 Build and Run
  $cd bintong
  $mkdir build
  $cd build
  $cmake ..
  $make
  $cd devel
  $source setup.bash
  $roslaunch bintong main.launch
