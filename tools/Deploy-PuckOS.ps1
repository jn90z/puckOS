param([Parameter(Mandatory=$true)][string]$File,[Parameter(Mandatory=$true)][string]$Address,[string]$Key="$env:USERPROFILE\.ssh\unfr0zen",[switch]$Run)
$ErrorActionPreference='Stop'
if($Address -notmatch '^[0-9.]+$'){throw 'Use an IPv4 address'}
$File=(Resolve-Path -LiteralPath $File).Path
$Key=(Resolve-Path -LiteralPath $Key).Path
$name=[IO.Path]::GetFileName($File)
if($name -notmatch '^[a-zA-Z0-9._-]+$' -or $name -in '.', '..'){throw 'Unsafe filename'}
$nest="root@$Address"
$dir='/opt/puckos'
$dest="$dir/$name"
& ssh -T -i $Key $nest "mkdir -p $dir"
if($LASTEXITCODE){throw 'Cannot create deployment directory'}
# Use process streams rather than PowerShell text pipelines.
$p=New-Object System.Diagnostics.Process
$p.StartInfo.FileName=(Get-Command ssh.exe).Source
$p.StartInfo.Arguments="-T -i `"$Key`" $nest `"umask 077; cat > $dest.part`""
$p.StartInfo.UseShellExecute=$false
$p.StartInfo.RedirectStandardInput=$true
[void]$p.Start()
$s=[IO.File]::OpenRead($File)
try{$s.CopyTo($p.StandardInput.BaseStream)}finally{$s.Dispose();$p.StandardInput.Close()}
$p.WaitForExit()
if($p.ExitCode){throw 'Upload failed'}
$local=(Get-FileHash -LiteralPath $File -Algorithm SHA256).Hash
$result=& ssh -T -i $Key $nest "sha256sum $dest.part"
if($LASTEXITCODE -or ($result -split '\s+')[0] -ne $local){throw 'Checksum mismatch; staged file not activated'}
& ssh -T -i $Key $nest "chmod 700 $dest.part && mv -f $dest.part $dest"
if($LASTEXITCODE){throw 'Activation failed'}
Write-Host "Verified SHA256: $local"
if($Run){& ssh -T -i $Key $nest $dest;if($LASTEXITCODE){throw 'Remote execution failed'}}
