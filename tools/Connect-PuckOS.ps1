param([Parameter(Mandatory=$true)][string]$Address,[string]$Key="$env:USERPROFILE\.ssh\unfr0zen")
& ssh -i $Key "root@$Address"
