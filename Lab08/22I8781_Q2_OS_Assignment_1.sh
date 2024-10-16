#!/bin/bash

# Initial Display: Name and Roll 
echo "22i-8781 Ibrahim AHmad"

# Function to create a new user with administrator privileges
create_user() {
    echo 'Operating System Assignment 1'
    read -p "Enter user: " username
    read -p "Enter password: " password

    # crreatign usier adn assigning name adn pswrd
    sudo useradd -m "$username" -p "$(openssl passwd -1 "$password")" -s /bin/bash
    # assign admin privalegs    
    sudo usermod -aG sudo "$username"
    #confirmation
    echo "User $username is created, and administrator privileges are assigned."
}

# Function to list all installed applications
list_installed_apps() {
    echo "Listing installed applications..."
    # show all installed apps and packages
    dpkg --list
}

# Function to install a application 
install_application() {
    read -p "Enter application name to install: " app_name
    echo "Installing $app_name..."
    sudo apt update
    sudo apt install -y "$app_name"
    echo "$app_name has been installed."
}

# Function to configure network settings
configure_network() {
    
    # Clear the existing IP on eth0 
    sudo ip addr flush dev eth0
    # set default 
    sudo ip addr add 10.0.0.1/24 dev eth0
    sudo ip route add default via 10.0.0.254

    # Update the DNS 
    echo "nameserver 8.8.8.8" | sudo tee /etc/resolv.conf > /dev/null
    #confirmation
    echo "Network configuration updated."
    echo "IP: 10.0.0.1, Mask: 255.255.255.0, Gateway: 10.0.0.254, DNS: 8.8.8.8"
}

# help information
display_help() {    
    echo "Usage: ./OS_Assignment_1.sh [OPTIONS]"
    echo "Options:"
    echo "  -uc       Create a new user with administrator privileges."
    echo "  -ld       List all installed applications."
    echo "  -ins      Install a specific application."
    echo "  -ipcon    Configure network settings (IP, Mask, Gateway, DNS)."
    echo "  -help     Display this help information."
}

case "$1" in 
    -uc)
        create_user
        ;;
    -ld)
        list_installed_apps
        ;;
    -ins)
        install_application
        ;;
    -ipcon)
        configure_network
        ;;
    -help)
        display_help
        ;;
    *)
        echo "Invalid option. Use -help for more information."
        ;;
esac