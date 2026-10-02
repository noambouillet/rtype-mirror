# Download only the CSS files used by the documentation site.
set(theme_version "v2.5.0")
set(theme_directory "${CMAKE_CURRENT_LIST_DIR}/_theme")
set(theme_base_url "https://raw.githubusercontent.com/jothepro/doxygen-awesome-css/${theme_version}")

file(MAKE_DIRECTORY "${theme_directory}")

function(fetch_theme_file filename expected_sha256)
    set(destination "${theme_directory}/${filename}")
    if(EXISTS "${destination}")
        file(SHA256 "${destination}" actual_sha256)
        if(actual_sha256 STREQUAL expected_sha256)
            return()
        endif()
    endif()

    file(DOWNLOAD "${theme_base_url}/${filename}" "${destination}"
        EXPECTED_HASH "SHA256=${expected_sha256}"
        STATUS download_status
        TLS_VERIFY ON)
    list(GET download_status 0 status_code)
    list(GET download_status 1 status_message)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "Could not download ${filename}: ${status_message}")
    endif()
endfunction()

fetch_theme_file("doxygen-awesome.css"
    "ea37516ec7a7a7780719b9d9a285fe3268a8d5411523d8b93af98d77e67cd1a1")
fetch_theme_file("doxygen-awesome-sidebar-only.css"
    "909d17055552149555fc87c5e0cf171a41d09a9e358c389a51b25ddfafd462af")
