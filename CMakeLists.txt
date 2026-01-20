cmake_minimum_required(VERSION 3.10)
project(BankingLedgerSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    main.cpp
    Account.cpp
    Transaction.cpp
    Ledger.cpp
    PersistenceManager.cpp
)

# Create the executable
add_executable(banking_ledger ${SOURCES})

# Compiler flags for better warnings
if(MSVC)
    target_compile_options(banking_ledger PRIVATE /W4)
else()
    target_compile_options(banking_ledger PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Link filesystem library (required for C++17 filesystem)
target_link_libraries(banking_ledger PRIVATE stdc++fs)
