message(STATUS "Resolving GIT Version")
set(GITVERSION "unknown")
find_package(Git)
if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --dirty --always --abbrev=8
    WORKING_DIRECTORY "${local_dir}"
    OUTPUT_VARIABLE GITVERSION
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message( STATUS "GIT hash: ${GITVERSION}")
else()
  message(STATUS "GIT not found")
endif()

string(TIMESTAMP _time_stamp)

configure_file(${local_dir}/cmake/gitversion.h.in ${output_dir}/gitversion.h @ONLY)
