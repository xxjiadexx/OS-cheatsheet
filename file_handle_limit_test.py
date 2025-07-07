#!/usr/bin/env python3
"""
File Handle Limit Test
Tests the maximum number of file handles a single process can have open.

This script demonstrates system limits on open file descriptors.
"""

import os
import sys
import resource

def test_file_handle_limit():
    """Test maximum file handles that can be opened simultaneously"""
    
    print("🔍 Testing File Handle Limits")
    print("=" * 50)
    
    # Get current limits
    soft_limit, hard_limit = resource.getrlimit(resource.RLIMIT_NOFILE)
    print(f"📊 Current soft limit: {soft_limit}")
    print(f"📊 Current hard limit: {hard_limit}")
    print()
    
    # Try to increase soft limit to hard limit
    try:
        resource.setrlimit(resource.RLIMIT_NOFILE, (hard_limit, hard_limit))
        print(f"✅ Successfully increased soft limit to {hard_limit}")
    except:
        print(f"⚠️  Could not increase soft limit, using {soft_limit}")
    
    print("\n🧪 Starting file handle test...")
    print("Creating files and keeping them open...")
    
    i = 0
    files = []
    
    try:
        while True:
            try:
                # Create and open a file
                filename = f"test_file_{i}.txt"
                file_handle = open(filename, "w+")
                files.append(file_handle)
                
                # Write something to ensure it's a real file operation
                file_handle.write(f"Test file {i}\n")
                file_handle.flush()
                
                i += 1
                
                # Print progress every 100 files
                if i % 100 == 0:
                    print(f"📝 Opened {i} files...")
                    
            except OSError as e:
                print(f"\n❌ Error at file {i}: {e}")
                print(f"📈 Maximum file handles opened: {i}")
                break
                
    except KeyboardInterrupt:
        print(f"\n⏹️  Test interrupted at {i} files")
        
    finally:
        # Clean up: close all files and delete them
        print(f"\n🧹 Cleaning up {len(files)} files...")
        
        for j, file_handle in enumerate(files):
            try:
                file_handle.close()
                filename = f"test_file_{j}.txt"
                if os.path.exists(filename):
                    os.remove(filename)
            except:
                pass  # Ignore cleanup errors
                
        print("✅ Cleanup complete!")
        
    return i

def show_system_limits():
    """Display various system limits related to files"""
    print("\n📋 System Resource Limits:")
    print("=" * 50)
    
    limits = [
        (resource.RLIMIT_NOFILE, "Number of open files"),
        (resource.RLIMIT_FSIZE, "Maximum file size"),
        (resource.RLIMIT_NPROC, "Number of processes"),
    ]
    
    for limit_type, description in limits:
        try:
            soft, hard = resource.getrlimit(limit_type)
            print(f"{description}:")
            print(f"  Soft limit: {soft}")
            print(f"  Hard limit: {hard}")
            print()
        except:
            print(f"{description}: Not available on this system")

def main():
    """Main function to run the file handle test"""
    print("🔬 File Handle Limit Investigation")
    print("CSC1107 Operating Systems - File System Implementation")
    print("=" * 60)
    
    # Show initial system limits
    show_system_limits()
    
    # Ask user if they want to proceed
    response = input("⚠️  This test will create many temporary files. Continue? (y/N): ")
    if response.lower() != 'y':
        print("Test cancelled.")
        return
    
    # Run the test
    max_handles = test_file_handle_limit()
    
    # Summary
    print("\n📊 Test Results:")
    print("=" * 30)
    print(f"Maximum file handles opened: {max_handles}")
    
    # Get final limits for comparison
    soft_limit, hard_limit = resource.getrlimit(resource.RLIMIT_NOFILE)
    print(f"Process limit was: {soft_limit}")
    
    if max_handles < soft_limit:
        print("❗ Reached limit before system maximum - other factors involved")
        print("   (e.g., available memory, kernel limits, other open files)")
    
    print("\n💡 Key Insights:")
    print("• Each open file consumes a file descriptor")
    print("• File descriptors are limited per process")
    print("• System-wide limits also apply")
    print("• Real applications should close files when done!")

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"\n💥 Unexpected error: {e}")
        print("Test terminated.")
