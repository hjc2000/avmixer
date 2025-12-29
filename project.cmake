add_executable(${ProjectName})
target_import_src(${ProjectName})
target_import_cli11(${ProjectName} PUBLIC)
target_import_ffmpeg_wrapper(${ProjectName} PUBLIC)
