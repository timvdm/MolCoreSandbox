set(tests
    smilesformat
    dietzmolecule
   )

foreach(test ${tests})
  add_executable(${test}_test ${test}.cpp)
  target_link_libraries(${test}_test molcore dl)
  add_test(${test}_test ${test}_test)
endforeach()
