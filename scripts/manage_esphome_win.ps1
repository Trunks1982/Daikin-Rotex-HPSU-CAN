# Define the URL for the Miniconda installer
$minicondaUrl = "https://repo.anaconda.com/miniconda/Miniconda3-latest-Windows-x86_64.exe"

# Define the installation directory
$installDir = "$env:UserProfile\Miniconda3"
$installerFileName = "Miniconda3-latest-Windows-x86_64.exe"
$installerFullPath = Join-Path -Path $PWD.Path -ChildPath $installerFileName  # Get full path for the installer

function Check-PythonInstallation {
    # Check if Miniconda's Python is installed
    $pythonPath = Join-Path -Path $installDir -ChildPath "python.exe"
    if (Test-Path $pythonPath) {
        try {
            $pythonVersionInfo = & $pythonPath --version 2>&1
            if ($pythonVersionInfo -match 'Python (\d+\.\d+\.\d+)') {
                Write-Host "Miniconda Python is installed: $pythonVersionInfo" -ForegroundColor Green
                return $true
            } else {
                Write-Host "Miniconda Python is not installed properly." -ForegroundColor Red
                return $false
            }
        } catch {
            Write-Host "Miniconda Python is not installed." -ForegroundColor Red
            return $false
        }
    } else {
        Write-Host "Miniconda Python is not installed." -ForegroundColor Red
        return $false
    }
}

function Check-ESPHomeInstallation {
    # Check if ESPHome is installed with the detected Python environment
    try {
        $esphomeVersion = & "$installDir\python.exe" -m pip show esphome | Select-String "Version:" | ForEach-Object { $_.ToString().Split(":")[1].Trim() }
        if ($esphomeVersion) {
            Write-Host "ESPHome is installed. Version: $esphomeVersion" -ForegroundColor Green
            return $esphomeVersion
        }
    } catch {
        Write-Host "ESPHome is not installed in the detected Python environment." -ForegroundColor Red
    }
    return $null
}

function Download-Miniconda {
    Write-Host "Downloading Miniconda installer..." -ForegroundColor Cyan
    try {
        $webClient = New-Object System.Net.WebClient
        $webClient.DownloadFile($minicondaUrl, $installerFullPath)
        Write-Host "Downloaded Miniconda installer to $installerFullPath" -ForegroundColor Green
        return $true
    } catch {
        Write-Host "Failed to download Miniconda. Please check your internet connection." -ForegroundColor Red
        return $false
    }
}

function Install-Miniconda {
    if (Download-Miniconda) {
        Write-Host "Installing Miniconda..." -ForegroundColor Cyan
        try {
            Start-Process -FilePath $installerFullPath -ArgumentList "/InstallationType=JustMe", "/RegisterPython=0", "/S", "/D=$installDir" -NoNewWindow -Wait
            Remove-Item $installerFullPath -Force
            & "$installDir\Scripts\conda.exe" init | Out-Null
            Start-Sleep -Seconds 2
            & "$installDir\Scripts\conda.exe" update conda -y | Out-Null
            & "$installDir\Scripts\conda.exe" install pip -y | Out-Null
            & "$installDir\Scripts\pip.exe" install esphome --no-warn-script-location | Out-Null
            Write-Host "`nMiniconda and esphome package installed successfully." -ForegroundColor Green
            Write-Host "`n!!! PLEASE RESTART POWERSHELL CONSOLE FOR PATH CHANGES TO TAKE EFFECT !!!" -ForegroundColor Red
        } catch {
            Write-Host "Installation of Miniconda failed. Please check for errors." -ForegroundColor Red
        }
    }
}

function Start-ESPHomeDashboard {
    while ($true) {
        $configDir = Read-Host -Prompt "Enter the path to your ESPHome configuration directory"
        
        # Validate input
        if ([string]::IsNullOrWhiteSpace($configDir)) {
            Write-Host "You must enter a valid directory path. Please try again." -ForegroundColor Red
            continue
        }
        
        # Check if the path format is correct
        if ($configDir -notmatch '^[a-zA-Z]:\\.*') {
            Write-Host "The path format is invalid. Please use the format C:\path\to\directory." -ForegroundColor Red
            continue
        }

        # Check if the drive exists before creating the directory
        $driveLetter = [System.IO.Path]::GetPathRoot($configDir)
        if (-Not (Test-Path $driveLetter)) {
            Write-Host "The specified drive '$driveLetter' does not exist. Please enter a valid drive." -ForegroundColor Red
            continue
        }

        # Create the configuration directory if it does not exist
        if (-Not (Test-Path $configDir)) {
            New-Item -ItemType Directory -Path $configDir -Force | Out-Null
            Write-Host "Created configuration directory: $configDir" -ForegroundColor Green
        }

        # Attempt to locate the ESPHome executable
        $esphomePath = ""
        
        # Check common paths where esphome.exe might be located
        $possiblePaths = @(
            "$env:USERPROFILE\Miniconda3\Scripts\esphome.exe",
            "$env:USERPROFILE\AppData\Local\Programs\Python\Python*\Scripts\esphome.exe",
            "C:\Program Files\Python*\Scripts\esphome.exe"  # New path added here
        )

        foreach ($path in $possiblePaths) {
            $matchingPaths = Get-ChildItem -Path $path -ErrorAction SilentlyContinue
            if ($matchingPaths) {
                $esphomePath = $matchingPaths[0].FullName
                break
            }
        }

        if (-Not $esphomePath) {
            Write-Host "ESPHome executable not found in standard locations. Please check your installation." -ForegroundColor Red
            break
        }

        # Attempt to start ESPHome dashboard
        try {
            Write-Host "Starting ESPHome dashboard using: $esphomePath" -ForegroundColor Cyan
            $dashboardProcess = Start-Process -FilePath $esphomePath -ArgumentList "dashboard", $configDir -PassThru -ErrorAction Stop
            
            if ($dashboardProcess) {
                Write-Host "ESPHome dashboard started successfully at http://localhost:6052." -ForegroundColor Green
                break
            } else {
                Write-Host "Unknown error: ESPHome dashboard did not start. Check the configuration and try again." -ForegroundColor Red
            }
        } catch {
            Write-Host "Failed to start ESPHome dashboard due to error: $_" -ForegroundColor Red
        }
    }
}


function Uninstall-ESPHome {
    Write-Host "Uninstalling ESPHome..." -ForegroundColor Cyan
    if (Check-ESPHomeInstallation) {
        try {
            & "$installDir\python.exe" -m pip uninstall esphome -y
            Write-Host "ESPHome uninstalled successfully." -ForegroundColor Green
        } catch {
            Write-Host "Failed to uninstall ESPHome." -ForegroundColor Red
        }
    } else {
        Write-Host "ESPHome is not installed." -ForegroundColor Yellow
    }
}

function Uninstall-Miniconda {
    Write-Host "Uninstalling Miniconda..." -ForegroundColor Cyan
    if (Test-Path $installDir) {
        try {
            $uninstallerPath = Join-Path -Path $installDir -ChildPath "Uninstall-Miniconda.exe"
            if (Test-Path $uninstallerPath) {
                Start-Process -FilePath $uninstallerPath -ArgumentList "/S" -Wait
            } else {
                Remove-Item -Path $installDir -Recurse -Force
            }
            Write-Host "Miniconda uninstalled successfully." -ForegroundColor Green
        } catch {
            Write-Host "Failed to uninstall Miniconda." -ForegroundColor Red
        }
    } else {
        Write-Host "Miniconda is not installed." -ForegroundColor Yellow
    }
}

function Check-ForESPHomeUpdate {
    # Check for the latest ESPHome version
    try {
        $latestVersion = Invoke-RestMethod -Uri "https://pypi.org/pypi/esphome/json" | Select-Object -ExpandProperty info | Select-Object -ExpandProperty version
        $currentVersion = Check-ESPHomeInstallation
        if ($currentVersion -and [version]$latestVersion -gt [version]$currentVersion) {
            Write-Host "A new version of ESPHome is available: $latestVersion (current: $currentVersion)" -ForegroundColor Yellow
            return $latestVersion
        } elseif (-not $currentVersion) {
            Write-Host "ESPHome is not installed." -ForegroundColor Yellow
        } else {
            Write-Host "ESPHome is already up to date (version: $currentVersion)." -ForegroundColor Green
        }
    } catch {
        Write-Host "Failed to check for ESPHome updates. Error: $_" -ForegroundColor Red
    }
    return $null
}

function Upgrade-ESPHome {
    Write-Host "Upgrading ESPHome..." -ForegroundColor Cyan
    try {
        & "$installDir\python.exe" -m pip install --upgrade esphome | Out-Null
        if (Check-ESPHomeInstallation) {
            Write-Host "ESPHome upgraded successfully." -ForegroundColor Green
        } else {
            Write-Host "ESPHome upgrade failed." -ForegroundColor Red
        }
    } catch {
        Write-Host "Failed to upgrade ESPHome." -ForegroundColor Red
    }
}

# Main script execution
Clear-Host
Write-Host "===============================" -ForegroundColor Cyan
Write-Host "      ESPHome Manager          " -ForegroundColor Yellow
Write-Host "===============================" -ForegroundColor Cyan
Write-Host "1. Install ESPHome (and Miniconda if necessary)"
Write-Host "2. Upgrade ESPHome if a new version is available"
Write-Host "3. Start ESPHome Dashboard"
Write-Host "4. Uninstall ESPHome and Miniconda"
Write-Host "===============================" -ForegroundColor Cyan

$option = Read-Host -Prompt "Choose an option (1, 2, 3, or 4)"

switch ($option) {
    "1" {
        # Check if Python is installed
        if (Check-PythonInstallation) {
            # Python is installed, now check if ESPHome is installed
            if (Check-ESPHomeInstallation) {
                Write-Host "Both Miniconda Python and ESPHome are already installed." -ForegroundColor Green
            } else {
                Write-Host "ESPHome is not installed. Proceeding to install ESPHome using existing Python." -ForegroundColor Yellow
                # Use existing Python environment to install ESPHome
                & "$installDir\python.exe" -m pip install esphome | Out-Null
                if (Check-ESPHomeInstallation) {
                    Write-Host "ESPHome installed successfully." -ForegroundColor Green
                } else {
                    Write-Host "ESPHome installation failed." -ForegroundColor Red
                }
            }
        } else {
            # Python is not installed, proceed with Miniconda installation
            Write-Host "Miniconda Python is not installed. Proceeding to install Miniconda and ESPHome." -ForegroundColor Yellow
            Install-Miniconda  # Will install Miniconda and ESPHome
        }
    }
    "3" {
        # Start ESPHome Dashboard
        if (Check-PythonInstallation) {
            if (Check-ESPHomeInstallation) {
                Start-ESPHomeDashboard
            } else {
                Write-Host "ESPHome is not installed. Please install it first." -ForegroundColor Red
            }
        } else {
            Write-Host "Please ensure Miniconda Python is installed before starting the dashboard." -ForegroundColor Red
        }
    }
    "4" {
        # Uninstall ESPHome if installed
        Uninstall-ESPHome  
        
        # Uninstall Miniconda
        Uninstall-Miniconda
    }
    "2" {
        # Upgrade ESPHome if a new version is available
        $newVersion = Check-ForESPHomeUpdate
        if ($newVersion) {
            Upgrade-ESPHome
        }
    }
    default {
        Write-Host "Invalid option selected. Please choose 1, 2, 3, or 4." -ForegroundColor Red
    }
}