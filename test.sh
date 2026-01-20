#!/bin/bash

# Banking Ledger System Test Script
# This demonstrates the application functionality

cd "$(dirname "$0")"

echo "================================"
echo "Banking Ledger System - Test Run"
echo "================================"
echo ""

# Check if binary exists
if [ ! -f "build/banking_ledger" ]; then
    echo "Error: banking_ledger binary not found!"
    echo "Please run: cmake .. && make in the build directory"
    exit 1
fi

echo "Testing: Create accounts, perform transfers, and display results"
echo ""

# Run the application with automated input
(
    # Menu selection 5: View All Accounts
    echo "5"
    sleep 0.5
    
    # Menu selection 2: Deposit funds
    echo "2"
    sleep 0.2
    echo "ACC001"
    sleep 0.2
    echo "200"
    sleep 0.5
    
    # Menu selection 4: Transfer
    echo "4"
    sleep 0.2
    echo "ACC001"
    sleep 0.2
    echo "ACC002"
    sleep 0.2
    echo "100"
    sleep 0.5
    
    # Menu selection 5: View All Accounts
    echo "5"
    sleep 0.5
    
    # Menu selection 6: View Account Statement
    echo "6"
    sleep 0.2
    echo "ACC001"
    sleep 0.5
    
    # Menu selection 7: View Transaction History
    echo "7"
    sleep 0.5
    
    # Menu selection 8: Test Rollback
    echo "8"
    sleep 0.2
    echo "ACC002"
    sleep 0.2
    echo "ACC003"
    sleep 0.2
    echo "150"
    sleep 0.2
    echo "y"
    sleep 0.5
    
    # Menu selection 5: View All Accounts (to verify rollback)
    echo "5"
    sleep 0.5
    
    # Exit
    echo "9"
    
) | ./build/banking_ledger

echo ""
echo "================================"
echo "Test completed successfully!"
echo "================================"
