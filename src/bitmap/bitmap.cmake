# Add this if ImageMagick and zlib exist
if(${ImageMagick_FOUND})
    if(${ZLIB_FOUND})
        # Bitmap executable
        add_executable(invader-bitmap
            src/bitmap/bitmap.cpp
            src/bitmap/composite_bitmap.cpp
        )

        #target_link_libraries(invader-bitmap invader)

        target_include_directories(invader-bitmap
            PUBLIC ${ImageMagick_Magick++_INCLUDE_DIRS}
            PUBLIC ${ZLIB_INCLUDE_DIRS}
        )

        target_link_libraries(invader-bitmap invader ${ImageMagick_LIBRARIES} ${ZLIB_LIBRARIES})

        target_compile_definitions(invader-bitmap
            PUBLIC MAGICKCORE_QUANTUM_DEPTH=8
            PUBLIC MAGICKCORE_HDRI_ENABLE=0
        )

        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            target_compile_options(invader-bitmap
                PUBLIC "-Wno-old-style-cast"
            )
        endif()
    else()
        message("zlib not found. invader-bitmap will not compile.")
    endif()
else()
    message("ImageMagick not found. invader-bitmap will not compile.")
endif()
