add_library(Template Template.c)
target_include_directories(Template 
  PUBLIC
  ${PROJECT_SOURCE_DIR}/src/Template
)
