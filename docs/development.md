# Development quick start
Download puckos-armv7 from Actions, extract it, then in Windows PowerShell run:
```powershell
.\tools\Deploy-PuckOS.ps1 -File .\puckos-diagnostics -Address YOUR_NEST_IP -Run
```
Use the existing Unfr0zen SSH key. Deployment stages a file, verifies SHA256 and activates it under /media/scratch/puckos. This directory is a development choice, not a verified permanent storage contract. No boot changes. Diagnostics are read-only; optional --input observes event2/event3 without grabbing them. Display drawing and beeper activation are deliberately deferred until driver behavior is verified. Keep the device on a trusted local network with internet access blocked at the router if possible.
