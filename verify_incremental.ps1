$testDir = "test_incremental"
$dbFile = "test_incremental.db"
$cli = ".\build\cli\fo_cli.exe"

# Clean up
if (Test-Path $testDir) { Remove-Item -Recurse -Force $testDir }
if (Test-Path $dbFile) { Remove-Item -Force $dbFile }

# 1. Setup initial state
New-Item -ItemType Directory -Path $testDir | Out-Null
"content1" | Set-Content "$testDir\file1.txt"
"content2" | Set-Content "$testDir\file2.txt"
"content3" | Set-Content "$testDir\file3.txt"

Write-Host "--- Initial Scan ---"
& $cli scan $testDir "--db=$dbFile"

# 2. Modify state
# Move file1.txt -> moved_file1.txt (same content/mtime, different name)
Move-Item "$testDir\file1.txt" "$testDir\moved_file1.txt"

# Delete file2.txt
Remove-Item "$testDir\file2.txt"

# Create new file
"content4" | Set-Content "$testDir\new_file.txt"

Write-Host "`n--- Incremental Scan (with Prune) ---"
& $cli scan $testDir "--db=$dbFile" --incremental --prune

# 3. Verify
Write-Host "`n--- Verification ---"
& $cli scan $testDir "--db=$dbFile"
