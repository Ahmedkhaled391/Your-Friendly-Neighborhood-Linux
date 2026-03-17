# Your-Friendly-Neighborhood-Linux

## Project Idea
The project is a user-friendly Linux assistant designed to attract new users to Linux by simplifying system setup, configuration, and optimization tasks. It provides a menu-driven GUI that automates common actions, allowing beginners to customize and manage their Ubuntu system without needing prior experience with the terminal or Linux commands.

## Purpose
- Make Linux more approachable for beginners.
- Simplify system configuration and optimization tasks.
- Automate actions that are usually confusing or intimidating for new users, such as managing system updates, optimizing performance, and customizing the desktop environment.

## Key Features
- **System Optimization**: Clean unused files, manage updates and upgrades, enable firewall, and improve overall system performance.
- **Customization**: Adjust terminal settings, configure the dock, tweak desktop behaviors, and apply other beginner-friendly visual enhancements.
- **Convenience Scripts**: Provide simple scripts for common tasks, such as installing software, setting default applications, or enabling helpful system utilities.
- **User Guidance**: Assist new Linux users in performing actions that are typically done via the terminal, making the learning curve smoother.

## Technologies and Tools
- **Programming Language**: C
- **Scripting**: Bash
- **GUI**: GTK
- **Operating System**: Linux distributions (Ubuntu, with potential support for other popular distributions)

## Description
YFNL is a desktop app made in C to assist new users to Linux to do the basic commands like downloading packages, updating system, customizing their PC through a GUI instead of dealing with the terminal which will make things complicated for new Linux users.

## Installation
1. Clone the repository:
   ```
   git clone https://github.com/Ahmedkhaled391/Your-Friendly-Neighborhood-Linux.git
   cd Your-Friendly-Neighborhood-Linux
   ```

2. Install dependencies (GTK and build tools):
   ```
   sudo apt update
   sudo apt install build-essential libgtk-3-dev
   ```

3. Build the application:
   ```
   make
   ```

## Running the Program
After building, run the executable:
```
./yfnl
```

The GUI will open, allowing you to perform various system tasks through the menu-driven interface.
