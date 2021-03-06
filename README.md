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

## Training Projects
Here you can find some projects I have developed to test the LIN protocol.
Right now I am using the Cypress CY8CKIT-042 + 026. At the end of this Readme.md you can find the compiling instructions.

- **LIN-Master-Slave-COM**

  First attempt to communicate with the LIN slave. 2 boards were used, one of them acts like master and the second one, as slave. The slave was programmed with a Cypress example. The project was only modified to change the LIN pinout. The master is based on a UART configuration. It writes the data frame on the TX line to be recognised by the LIN slave.
  
- **LIN-Master-Orders**

  Quite similar to the previous one. It has some preconfigured commands to change the led colors.

- **LIN-Internal-Light-Control**

  This project emulates the function of the internal light. It works with interruptions.
 
- **LIN-RGB_Multiple_Data**
  
   On this project, the user can introduce a numerical value through the PC (UART communication) and change the RGB LED colors of the development board. It is able to change colours individually or in group. For example, if you write "R217" it sets red led to that value (values are comprehended between 0-255). If you want to modify all the values, you can write "W018096123", which is the same to "R010", G"096" and "B123". When you press the SW2 button, Slave sends back it's value, and master sets the date on its' own LED. 
This project is interesting because it works with different frames with different lengths of data.

- **LIN-Sonceboz_Stepper_Motor_Test**

  This is a real project in a very early stage. The code just calibrates a stepper motor and assigns it 2 positions, so it can be rotating CW and CCW in a loop. It is the first step to make more complex designs and controls. All this is done using the LIN protocol.
  A second version has been done using capsense as an input to set the motor position.

  

### Compiling
To compile the projects you should use Cypress PSoC Creator 4.2 or higher. Just create a workspace and add the project or open the project files. The IDE is intuitive, so you just need a compatible board, check the pin configuration, compile it and program your development board.


