# THIS FILE IS AUTOMATICALLY GENERATED
# Project: D:\Users\jagumiel\Documents\PSoC Creator\LIN-Master-Orders\LIN-Master-Orders.cydsn\LIN-Master-Orders.cyprj
# Date: Fri, 03 Jul 2020 11:39:38 GMT
#set_units -time ns
create_clock -name {UART_MASTER_SCBCLK(FFB)} -period 8666.6666666666661 -waveform {0 4333.33333333333} [list [get_pins {ClockBlock/ff_div_3}]]
create_clock -name {CyRouted1} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFClk} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySysClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {UART_MASTER_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 209 417} -nominal_period 8666.6666666666661 [list]


# Component constraints for D:\Users\jagumiel\Documents\PSoC Creator\LIN-Master-Orders\LIN-Master-Orders.cydsn\TopDesign\TopDesign.cysch
# Project: D:\Users\jagumiel\Documents\PSoC Creator\LIN-Master-Orders\LIN-Master-Orders.cydsn\LIN-Master-Orders.cyprj
# Date: Fri, 03 Jul 2020 11:39:33 GMT
