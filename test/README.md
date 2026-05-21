# Unit Tests (Google Test)

Tests live under `test/unit/` and use the **Arrange → Act → Assert** style via `TEST_F(ShoppingSystemTest, Module_Scenario)`.

## Modules covered

| Module | Test file |
|--------|-----------|
| Cash | `test_cash.cpp` |
| Complaint_Base | `test_complaint_base.cpp` |
| Complaint_C (customer) | `test_complaint_customer.cpp` |
| Complaint_E (employee) | `test_complaint_employee.cpp` |
| Thread | `test_thread.cpp` |
| Server | `test_server.cpp` |
| Person | `test_person.cpp` |
| Customer | `test_customer.cpp` |
| Employee | `test_employee.cpp` |
| Admin | `test_admin.cpp` |
| Client | `test_client.cpp` |

Networking I/O is simulated with `Thread::enableTestMode()` (compile flag `ONLINE_SHOPPING_UNIT_TEST`).

## Build and run (Windows + MinGW or MSVC)

```powershell
cd C:\Users\Prathemesh\Desktop\Cpp\online-shopping-system
cmake -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Or run the binary directly:

```powershell
.\build\test\online_shopping_tests.exe
```

## Code coverage (~60% line/branch target)

Use **GCC/MinGW** with coverage flags:

```powershell
cmake -B build-cov -DBUILD_TESTS=ON -DENABLE_COVERAGE=ON -G "MinGW Makefiles"
cmake --build build-cov
cd build-cov
ctest --output-on-failure
```

Generate HTML report (install `lcov` / `genhtml`, e.g. via MSYS2):

```bash
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*/googletest/*' '*/test/*' --output-file coverage.filtered.info
genhtml coverage.filtered.info --output-directory coverage-html
```

On **MSVC**, use [OpenCppCoverage](https://github.com/OpenCppCoverage/OpenCppCoverage) against `online_shopping_tests.exe` and exclude `googletest` and `test` folders.

### Tips to raise coverage

- Add more `queueRecv` sequences for unvisited `switch` branches (Admin stock `FALSE` paths, buy insufficient balance, employee default menu).
- Run tests from the `build/test/runtime` working directory (CTest sets this automatically).
