# This file is for window power shell script
# Need to change LF to CRLF
# related packages: dos2unix, unix2dos
# written by Steve Jeong (steve@how2flow.net)

# Define the path to fwdn.exe
$fwdnExecutable = ".\fwdn.exe"

# Define the directory path
$directoryPath = "Z:\boot-firmware"

# Search for all files ending with fwdn.json and boot.json
$fwdnFiles = Get-ChildItem -Path $directoryPath -Filter "*fwdn.json"
$bootFiles = Get-ChildItem -Path $directoryPath -Filter "*boot.json"

# Execute fwdn.json files
foreach ($file in $fwdnFiles) {
    Write-Host "Executing file: $($file.FullName) (command: --fwdn)"
    & $fwdnExecutable --fwdn $file.FullName
}

# Execute boot.json files
foreach ($file in $bootFiles) {
    Write-Host "Executing file: $($file.FullName) (command: --write)"
    & $fwdnExecutable --write $file.FullName
}

# Execute additional command
$additionalFile = "Z:\boot-firmware\SD_Data.fai"
$storageType = "emmc"
$storageArea = "user"

Write-Host "Executing additional command: $additionalFile (command: --write --storage $storageType --area $storageArea)"
& $fwdnExecutable --write $additionalFile --storage $storageType --area $storageArea
