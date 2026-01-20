#!/bin/bash
# Quick demo of the Banking Ledger System

cd "$(dirname "$0")/build"

echo "Running a quick demo of the Banking Ledger System..."
echo "This will show account creation, deposits, transfers, and rollback testing."
echo ""

# Run with input commands
(
    # Show all accounts
    echo "5"
    sleep 1
    
    # Deposit to ACC001
    echo "2"
    sleep 0.3
    echo "ACC001"
    sleep 0.3
    echo "150"
    sleep 1
    
    # Transfer from ACC001 to ACC002
    echo "4"
    sleep 0.3
    echo "ACC001"
    sleep 0.3
    echo "ACC002"
    sleep 0.3
    echo "75"
    sleep 1
    
    # View ACC001 statement
    echo "6"
    sleep 0.3
    echo "ACC001"
    sleep 1
    
    # Exit
    echo "9"
    
) | ./banking_ledger

echo ""
echo "Demo completed!"
