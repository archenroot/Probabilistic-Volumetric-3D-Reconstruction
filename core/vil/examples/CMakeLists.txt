# core/vil/examples/CMakeLists.txt

link_libraries( ${VXL_LIB_PREFIX}vil ${VXL_LIB_PREFIX}vcl )

add_executable( vil_example1 example1.cxx )
add_executable( vil_example2 example2.cxx )
add_executable( vil_example3 example3.cxx )
add_executable( vil_example4 example4.cxx )
add_executable( vil_example5 example5.cxx )
add_executable( vil_example6 example6.cxx )
add_executable( vil_example_rgb example_rgb.cxx )
add_executable( vil_file_example1 file_example1.cxx )
add_executable( vil_file_example2 file_example2.cxx )
add_executable( vil_file_example3 file_example3.cxx )
add_executable( vil_file_example4 file_example4.cxx )
add_executable( vil_file_example5 file_example5.cxx )
add_executable( vil_image_copy vil_image_copy.cxx )
add_executable( vil_image_convert vil_image_convert.cxx )
add_executable( vil_rotate_image vil_rotate_image.cxx )
add_executable( vil_resample_image vil_resample_image.cxx )
add_executable( vil_example_interpolate example_interpolate.cxx )
add_executable( vil_color_tables vil_color_tables.cxx )
