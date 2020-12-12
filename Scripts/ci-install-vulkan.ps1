
$version = "1.2.154.1"

$n = 1
do {
    Invoke-WebRequest "https://sdk.lunarg.com/sdk/download/$version/windows/VulkanSDK-$version-Installer.exe" -OutFile VulkanSDK-Installer.exe
    if ($?) {
        break
    }
    $n++
    Start-Sleep -s 10
} while ($n -le 10)

.\VulkanSDK-Installer.exe /S

$Env:VULKAN_SDK="C:\VulkanSDK\$version"
