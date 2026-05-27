set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Clean, modern FetchContent approach
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# Bring gtest and gtest_main targets into scope
FetchContent_MakeAvailable(googletest)

# --- Test Support Library ---
add_library(oss_test_support STATIC
    support/test_fixture.cpp
    support/test_runtime.cpp
)
target_include_directories(oss_test_support PRIVATE
    ${CMAKE_SOURCE_DIR}/Server
    ${CMAKE_SOURCE_DIR}/Server/Headers
)
target_link_libraries(oss_test_support PUBLIC gtest)
# Added HEADLESS_CI_BUILD to support utilities
target_compile_definitions(oss_test_support PRIVATE ONLINE_SHOPPING_UNIT_TEST HEADLESS_CI_BUILD)

# --- Source File Mappings ---
set(CLIENT_PROD_SOURCES
    ${CMAKE_SOURCE_DIR}/Client/Headers/Client.cpp
)

set(SERVER_PROD_SOURCES
    ${CMAKE_SOURCE_DIR}/Server/Headers/Server.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Thread.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Person.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Customer.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Employee.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Admin.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Complaint_Base.cpp
    ${CMAKE_SOURCE_DIR}/Server/Headers/Complaint_E.cpp
)

set(TEST_CLIENT_SOURCES
    unit/test_admin.cpp
    unit/test_admin_coverage.cpp
    unit/test_admin_home_cases.cpp
    unit/test_branch_targets.cpp
    unit/test_branch_coverage_boost.cpp
    unit/test_cash.cpp
    unit/test_client.cpp
    unit/test_customer.cpp
    unit/test_customer_coverage.cpp
    unit/test_employee.cpp
    unit/test_employee_coverage.cpp
    unit/test_person.cpp
    unit/test_person_coverage.cpp
    unit/test_person_deep_branches.cpp
)

set(TEST_SERVER_SOURCES
    unit/test_complaint_base.cpp
    unit/test_complaint_coverage.cpp
    unit/test_complaint_customer.cpp
    unit/test_complaint_employee.cpp
    unit/test_integration_network.cpp
    unit/test_server_branch_boost.cpp
    unit/test_server.cpp
    unit/test_thread.cpp
)

# --- testClient Target ---
add_executable(testClient ${TEST_CLIENT_SOURCES} ${CLIENT_PROD_SOURCES} ${SERVER_PROD_SOURCES})
target_include_directories(testClient PRIVATE
    ${CMAKE_SOURCE_DIR}/Server
    ${CMAKE_SOURCE_DIR}/Server/Headers
    ${CMAKE_SOURCE_DIR}/Client
    ${CMAKE_SOURCE_DIR}/Client/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/support
    ${CMAKE_CURRENT_SOURCE_DIR}/unit
)
target_link_libraries(testClient PRIVATE gtest gtest_main ws2_32 oss_test_support)
# CRITICAL FIX: Added HEADLESS_CI_BUILD definition to stop hanging inputs/loops
target_compile_definitions(testClient PRIVATE ONLINE_SHOPPING_UNIT_TEST HEADLESS_CI_BUILD)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(testClient PRIVATE
        "-Wl,--wrap=getch"
        "-Wl,--wrap=_getch"
        "-Wl,--wrap=system"
    )
endif()

add_test(NAME testClient COMMAND testClient)

# --- testServer Target ---
add_executable(testServer ${TEST_SERVER_SOURCES} ${SERVER_PROD_SOURCES} ${CLIENT_PROD_SOURCES})

target_include_directories(testServer PRIVATE
    ${CMAKE_SOURCE_DIR}/Server
    ${CMAKE_SOURCE_DIR}/Server/Headers
    ${CMAKE_SOURCE_DIR}/Client
    ${CMAKE_SOURCE_DIR}/Client/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/support
    ${CMAKE_CURRENT_SOURCE_DIR}/unit
)

target_link_libraries(testServer PRIVATE
    gtest
    gtest_main
    ws2_32
    oss_test_support
)
# CRITICAL FIX: Added HEADLESS_CI_BUILD definition to stop hanging inputs/loops
target_compile_definitions(testServer PRIVATE ONLINE_SHOPPING_UNIT_TEST HEADLESS_CI_BUILD)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(testServer PRIVATE
        "-Wl,--wrap=getch"
        "-Wl,--wrap=_getch"
        "-Wl,--wrap=system"
    )
endif()

add_test(NAME testServer COMMAND testServer)

# REMOVED: add_custom_target(coverage) to prevent duplicate runs 
# and file access conflicts with your Jenkinsfile process.
