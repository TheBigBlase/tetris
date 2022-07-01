install(
    TARGETS tetris_exe
    RUNTIME COMPONENT tetris_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
