#!/bin/bash

# Function to detect the operating system
detect_os() {
    case "$(uname -s)" in
        Linux*)     os_type="Linux";;
        Darwin*)    os_type="macOS";;
        *)          os_type="unknown";;
    esac
}

# Define the installation directory
install_dir="$HOME/esphome_env"

# Function to install Python on Linux
install_python_linux() {
    echo "Installing Python 3 and necessary packages..."
    if command -v apt &> /dev/null; then
        sudo apt update && sudo apt install -y python3 python3-venv python3-pip
    elif command -v yum &> /dev/null; then
        sudo yum install -y python3 python3-venv python3-pip
    elif command -v dnf &> /dev/null; then
        sudo dnf install -y python3 python3-venv python3-pip
    elif command -v pacman &> /dev/null; then
        sudo pacman -Syu python python-venv python-pip
    else
        echo "No known package manager found. Please install Python 3 and venv manually."
        return 1
    fi
    return 0
}

# Function to check if Python 3 is installed
check_python_installed() {
    if command -v python3 &> /dev/null; then
        echo "Python 3 is installed."
        return 0
    else
        echo "Python 3 is not installed. Installing Python 3..."
        if [ "$os_type" = "Linux" ]; then
            install_python_linux
            return $?  # Return the result of the installation attempt
        else
            echo "Please install Python 3 to proceed."
            return 1
        fi
    fi
}

# Function to check if venv is installed
check_venv_installed() {
    if python3 -m venv --help &> /dev/null; then
        echo "venv is available."
        return 0
    else
        echo "venv module is not available. Please install it to proceed."
        return 1
    fi
}

# Function to install python3-venv on Debian/Ubuntu
install_python3_venv() {
    echo "Installing python3-venv..."
    if command -v apt &> /dev/null; then
        sudo apt install -y python3-venv
        return $?
    else
        echo "Cannot install python3-venv. Please install it manually."
        return 1
    fi
}

# Function to check if python3-venv is installed on Debian/Ubuntu
check_python3_venv_installed() {
    if dpkg -l | grep -q python3-venv; then
        echo "python3-venv is installed."
        return 0
    else
        echo "python3-venv is not installed. Installing it now..."
        install_python3_venv
        return $?  # Return the result of the installation attempt
    fi
}

# Function to check if the virtual environment exists
check_virtualenv_exists() {
    if [ -d "$install_dir" ]; then
        echo "Virtual environment exists."
        return 0
    else
        echo "Virtual environment does not exist."
        return 1
    fi
}

# Function to create a virtual environment for ESPHome
create_virtualenv() {
    echo "Creating a virtual environment for ESPHome..."
    python3 -m venv "$install_dir"
    if [ $? -eq 0 ]; then
        echo "Virtual environment created successfully at $install_dir."
        return 0
    else
        echo "Failed to create virtual environment. Please ensure you have python3-venv installed."
        return 1
    fi
}

# Function to check if ESPHome is installed in the virtual environment
check_esphome_installed() {
    if [ -f "$install_dir/bin/activate" ]; then
        source "$install_dir/bin/activate"
        if pip show esphome &> /dev/null; then
            echo "ESPHome is already installed in the virtual environment."
            return 0
        else
            echo "ESPHome is not installed in the virtual environment."
            return 1
        fi
    else
        echo "Virtual environment activation file does not exist."
        return 1
    fi
}

# Function to install ESPHome
install_esphome() {
    echo "Installing ESPHome in the virtual environment..."
    source "$install_dir/bin/activate"

    pip install --upgrade pip
    pip install esphome

    if [ $? -eq 0 ]; then
        echo "ESPHome installed successfully."
    else
        echo "ESPHome installation failed."
    fi
}

# Function to upgrade ESPHome if a new version is available
upgrade_esphome() {
    echo "Checking for ESPHome updates..."
    source "$install_dir/bin/activate"
    pip install --upgrade esphome
    if [ $? -eq 0 ]; then
        echo "ESPHome upgraded successfully."
    else
        echo "Failed to upgrade ESPHome."
    fi
}

# Function to start ESPHome Dashboard
start_esphome_dashboard() {
    echo "Starting ESPHome Dashboard..."
    source "$install_dir/bin/activate"
    esphome dashboard "$1"  # Pass the configuration directory
}

# Function to uninstall ESPHome
uninstall_esphome() {
    echo "Uninstalling ESPHome..."
    if [ -d "$install_dir" ]; then
        rm -rf "$install_dir"
        echo "ESPHome uninstalled successfully."
    else
        echo "ESPHome environment not found."
    fi
}

# Main script execution
detect_os
clear
echo "==============================="
echo "      ESPHome Manager          "
echo "==============================="
echo "1. Install ESPHome"
echo "2. Upgrade ESPHome"
echo "3. Start ESPHome Dashboard"
echo "4. Uninstall ESPHome"
echo "==============================="

# Prompt the user for a choice
read -p "Choose an option (1, 2, 3, or 4): " option

case $option in
    "1")
        # Check if Python 3 is installed
        if ! check_python_installed; then
            exit 1
        fi

        # Check if venv is available
        if ! check_venv_installed; then
            exit 1
        fi

        # Check if python3-venv is installed on Debian/Ubuntu systems
        if [ "$os_type" = "Linux" ]; then
            if ! check_python3_venv_installed; then
                exit 1
            fi
        fi

        # Check if the virtual environment exists
        if ! check_virtualenv_exists; then
            # Create the virtual environment only if it does not exist
            create_virtualenv
            if [ $? -ne 0 ]; then
                exit 1
            fi
        fi

        # Activate the virtual environment and check if ESPHome is installed
        source "$install_dir/bin/activate"
        if ! check_esphome_installed; then
            install_esphome
        else
            echo "Skipping ESPHome installation."
        fi
        ;;
    "2")
        # Check if the virtual environment exists
        if check_virtualenv_exists; then
            # Check if ESPHome is installed
            if check_esphome_installed; then
                upgrade_esphome
            else
                echo "ESPHome is not installed. Please install it first by choosing option 1."
            fi
        else
            echo "Please install ESPHome first by choosing option 1."
        fi
        ;;
    "3")
        # Check if the virtual environment exists
        if check_virtualenv_exists; then
            # Check if ESPHome is installed
            if check_esphome_installed; then
                read -p "Enter the path to your ESPHome configuration directory: " config_dir
                if [ -d "$config_dir" ]; then
                    start_esphome_dashboard "$config_dir"
                else
                    echo "The specified configuration directory does not exist. Please enter a valid directory."
                fi
            fi
        else
            echo "Please install ESPHome first by choosing option 1."
        fi
        ;;
    "4")
        # Uninstall ESPHome
        uninstall_esphome
        ;;
    *)
        echo "Invalid option selected. Please choose 1, 2, 3, or 4."
        ;;
esac
