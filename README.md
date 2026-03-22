# Your-Friendly-Neighborhood-Linux

## Project Idea

The project is a user-friendly Linux assistant designed to attract new users to Linux by simplifying system setup, configuration, and optimization tasks. It provides a menu-driven GUI that automates common actions, allowing beginners to customize and manage their Ubuntu system without needing prior experience with the terminal or Linux commands.

At a higher level, the application acts as a bridge between beginner expectations and everyday Linux maintenance. Instead of asking users to memorize commands or search through scattered settings, it brings common tasks together into a more approachable desktop tool.

## Purpose

- Make Linux more approachable for beginners.
- Simplify system configuration and optimization tasks.
- Automate actions that are usually confusing or intimidating for new users, such as managing system updates, optimizing performance, and customizing the desktop environment.

The broader goal is not only convenience, but also confidence. By turning frequent maintenance and setup tasks into guided actions, the project helps users become more comfortable with Linux over time.

## Key Features

- **System Optimization**: Clean unused files, manage updates and upgrades, and improve overall system performance.
- **Firewall Management**: Enable or disable the firewall through simple guided actions instead of manual terminal commands.
- **Backup Tools**: Support routine backup tasks to help users protect important data Automatically.
- **Customization**: Adjust terminal settings, configure the dock, tweak desktop behaviors, and apply other beginner-friendly visual enhancements.
- **Convenience Scripts**: Provide simple scripts for common tasks, such as installing software, enabling helpful system utilities, and handling routine maintenance.
- **Password Generator**: Allows users to generate random passwords, choose their length, and copy them easily.
- **User Guidance**: Assist new Linux users in performing actions that are typically done via the terminal, making the learning curve smoother.

Together, these features aim to present Linux administration in smaller, more understandable steps rather than exposing users

## Technologies and Tools

- **Programming Language**: C
- **Scripting**: Bash
- **GUI**: GTK
- **Operating System**: Linux distributions (Ubuntu, with potential support for other popular distributions)

## Installation

1. Clone the repository:

   ```
   git clone https://github.com/Ahmedkhaled391/Your-Friendly-Neighborhood-Linux.git
   cd Your-Friendly-Neighborhood-Linux
   ```

2. Install dependencies (GTK4 and build tools):

   ```
   sudo apt update
   sudo apt install libgtk-4-dev
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
