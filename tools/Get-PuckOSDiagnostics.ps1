param([Parameter(Mandatory=$true)][string]$Address,[string]$Key="$env:USERPROFILE\.ssh\unfr0zen",[string]$Output="puckos-diagnostics.txt")
& ssh -T -i $Key "root@$Address" 'uname -r; id; cat /proc/cpuinfo /proc/meminfo /proc/mtd /proc/bus/input/devices; df -h; cat /sys/class/graphics/fb0/virtual_size /sys/class/graphics/fb0/bits_per_pixel /sys/class/graphics/fb0/stride; cat /sys/class/power_supply/*/uevent' | Out-File -Encoding utf8 $Output
if($LASTEXITCODE){throw 'SSH diagnostics failed'}
Write-Host "Saved $Output; review for private identifiers before sharing."
