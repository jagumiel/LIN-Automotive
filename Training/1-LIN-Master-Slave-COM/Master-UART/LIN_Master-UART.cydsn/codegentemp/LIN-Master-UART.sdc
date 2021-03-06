# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\jagumiel\OneDrive - TEKNIKER\Codigo\Cypress\1-Pruebas\1-LIN-Master-Slave\Master-UART\LIN-Master-UART.cydsn\LIN-Master-UART.cyprj
# Date: Wed, 08 Jul 2020 09:58:05 GMT
#set_units -time ns
create_clock -name {UART_MASTER_SCBCLK(FFB)} -period 8687.5 -waveform {0 4343.75} [list [get_pins {ClockBlock/ff_div_1}]]
create_clock -name {CyRouted1} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFClk} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyIMO} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFClk} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySysClk} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {UART_MASTER_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 417 835} [list]


# Component constraints for C:\Users\jagumiel\OneDrive - TEKNIKER\Codigo\Cypress\1-Pruebas\1-LIN-Master-Slave\Master-UART\LIN-Master-UART.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\jagumiel\OneDrive - TEKNIKER\Codigo\Cypress\1-Pruebas\1-LIN-Master-Slave\Master-UART\LIN-Master-UART.cydsn\LIN-Master-UART.cyprj
# Date: Wed, 08 Jul 2020 09:58:00 GMT
