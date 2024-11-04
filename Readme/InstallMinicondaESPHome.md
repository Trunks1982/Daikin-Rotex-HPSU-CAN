# InstallMinicondaESPHome.ps1

This PowerShell script automates the installation of Miniconda and ESPHome, and it allows you to start the ESPHome Dashboard easily.

## Prerequisites

- Windows Operating System
- PowerShell (typically installed by default on Windows)
- Internet access (to download Miniconda and ESPHome)

## Usage

To run the script, follow these steps:

1. Open **PowerShell as Administrator**.
2. Execute the following command, replacing `C:\Path\To\InstallMinicondaESPHome.ps1` with the actual path to the script:

   ```powershell
   PowerShell -ExecutionPolicy Bypass -File "C:\Path\To\InstallMinicondaESPHome.ps1"
   ```
## Features

- Option 1: Installs Miniconda and ESPHome if they are not already installed.
- Option 2: Upgrade ESPHome if a new version is available.
- Option 3: Starts the ESPHome Dashboard if Python and ESPHome are already installed.
- Option 4: Uninstall ESPHome and Miniconda.

## Notes

If you encounter any issues with script execution, ensure that your PowerShell execution policy allows for script execution. You can permanently set the execution policy by running:

   ```powershell
   Set-ExecutionPolicy -ExecutionPolicy Unrestricted -Scope Process
   ```
   
## License

This project is licensed under the MIT License - see the LICENSE file for details.
