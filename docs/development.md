# Development quick start
Download puckos-armv7 from Actions, extract it, then in Windows PowerShell run:
```powershell
.\tools\Deploy-PuckOS.ps1 -File .\puckos-diagnostics -Address YOUR_NEST_IP -Run
```
Use the existing Unfr0zen SSH key. Deployment stages a file, verifies SHA256 and activates it under /opt/puckos. This directory is a development choice, not a verified permanent storage contract. No boot changes. Diagnostics are read-only; optional --input observes event2/event3 without grabbing them. Hardware demo is experimental: deploy puckos-hardware-demo with -Run for a 30-second framebuffer test. Hold button two seconds to exit. Optional --sound enables a brief tone on press; test display first. Stock UI may overwrite the demo; no services are stopped. Framebuffer restoration is best effort. Keep the device on a trusted local network with internet access blocked at the router if possible.

Scratch storage disappeared between sessions; /opt/puckos worked across subsequent tests but reboot persistence is unverified.
