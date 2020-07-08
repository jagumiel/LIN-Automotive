# LIN-Automotive
Projects and utilities using the LIN Automotive Bus


## Utilities
- **pid_calculator.c**

  Given a LIN ID this program calculates the Protected ID (PID) for the LIN 2.2 version.

- **checksum_calculator.c**

  Given a PID and a LIN data frame, this program calculates the checksum for the LIN 2.2 version.

- **full_frame_calculator.c**

  Given a LIN ID and a data frame, this program calculates the full frame to send using the LIN 2.2 version. This is useful to use it in a LIN project, where PIDs and checksums must be calculated to send the appropiate frame.

### How to compile?
Just as any C file: 
> gcc -o [output] [input.c]

## Projects
[todo]
