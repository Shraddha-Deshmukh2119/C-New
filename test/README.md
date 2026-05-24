# Unit Tests (Google Test)

Tests live under `test/unit/` and use the **Arrange → Act → Assert** style via `TEST_F(ShoppingSystemTest, Module_Scenario)`.

**119 tests** cover line and branch paths across Person, Customer, Employee, Admin, Complaints, Cash, Thread, Server, and Client.

| File | Role |
|------|------|
| `test_*_coverage.cpp` | Module flows (login, buy, admin menus, complaints) |
| `test_branch_targets.cpp` | Explicit true/false branch pairs |
| `test_admin_home_cases.cpp` | One test per `Admin::home` switch case |
| `test_person_deep_branches.cpp` | Multi-step Person/buy paths |
| `test_integration_network.cpp` | Real Winsock server/client (no mock thread) |

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

## Code coverage (~70% line + branch target)

Use **GCC/MinGW** with coverage flags:

```powershell
cmake -B build-cov -DBUILD_TESTS=ON -DENABLE_COVERAGE=ON -G "MinGW Makefiles"
cmake --build build-cov
.\build-cov\test\online_shopping_tests.exe
```

**One-command module report** (requires [gcovr](https://gcovr.com)):

```powershell
powershell -File test/scripts/report_coverage.ps1 -BuildDir build-cov
```

Measured with **gcovr** after `online_shopping_tests.exe` (MinGW + `-DENABLE_COVERAGE=ON`):

| Module | Line | Branch |
|--------|------|--------|
| Person.cpp | ~98% | ~57% |
| Admin.cpp | ~95% | ~57% |
| Customer.cpp | ~100% | ~62% |
| Employee.cpp | ~100% | ~59% |
| Complaint_Base.cpp | ~100% | ~57% |
| Complaint_E.cpp | ~100% | ~56% |
| Server.cpp | ~82% | ~48% |
| Thread.cpp | ~90% | ~55% |
| Client.cpp | ~100% line / ~50% branch | |
| **Overall** | **~96%** | **~56%** |

If your IDE shows **0% branch**, you are likely not using a **gcov/gcovr** build. MSVC/OpenCppCoverage needs a separate run configuration pointing at `build-cov/test/online_shopping_tests.exe`.

Branch % is lower than line % because every `if`/`switch` edge must execute **both** outcomes; socket error paths and `while (!eof)` loops add many compiler branches.

On **MSVC**, use [OpenCppCoverage](https://github.com/OpenCppCoverage/OpenCppCoverage) against `online_shopping_tests.exe` and exclude `googletest` and `test` folders.
