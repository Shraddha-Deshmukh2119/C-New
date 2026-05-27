set(GTEST_ROOT "${CMAKE_BINARY_DIR}/_deps/googletest-src")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

if(EXISTS "${GTEST_ROOT}/CMakeLists.txt")
    add_subdirectory(${GTEST_ROOT} ${CMAKE_BINARY_DIR}/_deps/googletest-build EXCLUDE_FROM_ALL)
else()
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(googletest)
endif()

add_library(oss_test_support STATIC test/support/test_runtime.cpp)

set(CLIENT_PROD_SOURCES
    ${CMAKE_SOURCE_DIR}/Client/Headers/Client.cpp
    ${CMAKE_SOURCE_DIR}/Client/Headers/Person.cpp
    ${CMAKE_SOURCE_DIR}/Client/Headers/Customer.cpp
    ${CMAKE_SOURCE_DIR}/Client/Headers/Employee.cpp
    ${CMAKE_SOURCE_DIR}/Client/Headers/Admin.cpp
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
    test/unit/test_admin.cpp
    test/unit/test_admin_coverage.cpp
    test/unit/test_admin_home_cases.cpp
    test/unit/test_branch_targets.cpp
    test/unit/test_cash.cpp
    test/unit/test_client.cpp
    test/unit/test_customer.cpp
    test/unit/test_customer_coverage.cpp
    test/unit/test_employee.cpp
    test/unit/test_employee_coverage.cpp
    test/unit/test_person.cpp
    test/unit/test_person_coverage.cpp
    test/unit/test_person_deep_branches.cpp
)

set(TEST_SERVER_SOURCES
    test/unit/test_complaint_base.cpp
    test/unit/test_complaint_coverage.cpp
    test/unit/test_complaint_customer.cpp
    test/unit/test_complaint_employee.cpp
    test/unit/test_integration_network.cpp
    test/unit/test_server.cpp
    test/unit/test_thread.cpp
)

add_executable(testClient ${TEST_CLIENT_SOURCES} ${CLIENT_PROD_SOURCES})
target_include_directories(testClient PRIVATE
    ${CMAKE_SOURCE_DIR}/Client
    ${CMAKE_SOURCE_DIR}/Client/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/test/support
    ${CMAKE_CURRENT_SOURCE_DIR}/test/unit
)
target_link_libraries(testClient PRIVATE GTest::gtest GTest::gtest_main ws2_32 oss_test_support)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(testClient PRIVATE
        "-Wl,--wrap=getch"
        "-Wl,--wrap=_getch"
        "-Wl,--wrap=system"
    )
endif()

add_test(NAME testClient COMMAND testClient)

add_executable(testServer ${TEST_SERVER_SOURCES} ${SERVER_PROD_SOURCES})

target_include_directories(testServer PRIVATE
    ${CMAKE_SOURCE_DIR}/Server
    ${CMAKE_SOURCE_DIR}/Server/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/test/support
    ${CMAKE_CURRENT_SOURCE_DIR}/test/unit
)

target_link_libraries(testServer PRIVATE
    GTest::gtest
    GTest::gtest_main
    ws2_32
    oss_test_support
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(testServer PRIVATE
        "-Wl,--wrap=getch"
        "-Wl,--wrap=_getch"
        "-Wl,--wrap=system"
    )
endif()

add_test(NAME testServer COMMAND testServer)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
        COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --object-directory ${CMAKE_BINARY_DIR}
                --filter "Client/" --filter "Server/"
                --xml ${CMAKE_BINARY_DIR}/coverage.xml
                --html-details ${CMAKE_BINARY_DIR}/coverage/index.html
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating coverage.xml and HTML report (open build/coverage/index.html)"
        VERBATIM
    )
endif()
