cmake_minimum_required(VERSION 3.2)

option(HUNTER_STATUS_DEBUG "..." ON)
set(HUNTER_CONFIGURATION_TYPES Release CACHE STRING "...")

include("${CMAKE_CURRENT_LIST_DIR}/../HunterGate.cmake")
HunterGate(
    URL "${HUNTER_HOST_URL}"
    SHA1 "${HUNTER_HOST_SHA1}"
)

project(LLVMHost)

hunter_add_package(LLVM)
