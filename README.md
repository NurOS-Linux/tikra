# Tikra

A comprehensive multi-functional clock application for AetherDE desktop environment.

## Features

### 🕐 Clock
- Real-time clock display with seconds precision
- Current date and day display
- Large, easy-to-read digital display

### ⏱️ Stopwatch
- High precision timing (centiseconds)
- Start, stop, and reset functionality
- Lap recording with history
- Clean, professional interface

### ⏲️ Timer
- Customizable countdown timer (hours, minutes, seconds)
- Visual notification when timer expires
- Pause and resume functionality
- Input validation and user-friendly controls

### 🌍 World Clock
- Multiple timezone support (10 major cities)
- Real-time updates for all timezones
- Clean list view with city names and times
- Automatic date display for each location

### 🎨 Modern Design
- Built with modern GTK4 and libadwaita
- Adaptive interface with ViewSwitcher in header bar
- Responsive design that works on different screen sizes
- Follows GNOME Human Interface Guidelines
- Beautiful icons and typography

## Building

### Dependencies

- Vala >= 0.56.0
- GTK4 >= 4.10.0
- libadwaita >= 1.4.0
- Meson build system
- Blueprint compiler

### Build Instructions

```bash
meson setup build
meson compile -C build
```

### Installation

```bash
meson install -C build
```

## Running

After installation, you can run Tikra from your application menu or from the command line:

```bash
tikra
```

## License

This project is licensed under the GNU General Public License v3.0.

## Author

- **AnmiTaliDev** - *Initial work* - [GitHub](https://github.com/NurOS-Linux/tikra)
- Email: anmitali198@gmail.com

## Contributing

Bug reports and feature requests are welcome on the [GitHub issues page](https://github.com/NurOS-Linux/tikra/issues).

## Project

This application is part of the AetherDE project.