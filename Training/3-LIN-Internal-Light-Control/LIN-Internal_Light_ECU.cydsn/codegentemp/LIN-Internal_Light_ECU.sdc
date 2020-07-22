# THIS FILE IS AUTOMATICALLY GENERATED
# Project: D:\Users\jagumiel\Documents\PSoC Creator\LIN-Automotive\3-LIN-Internal-Light-Control\LIN-Internal_Light_ECU.cydsn\LIN-Internal_Light_ECU.cyprj
# Date: Wed, 22 Jul 2020 06:10:17 GMT
#set_units -time ns
create_clock -name {UART_MASTER_SCBCLK(FFB)} -period 4333.333333333333 -waveform {0 2166.66666666667} [list [get_pins {ClockBlock/ff_div_3}]]
create_clock -name {CyRouted1} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFClk} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyIMO} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFClk} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySysClk} -period 20.833333333333332 -waveform {0 10.4166666666667} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {UART_MASTER_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 209 417} -nominal_period 4333.333333333333 [list]


# Component constraints for D:\Users\jagumiel\Documents\PSoC Creator\LIN-Automotive\3-LIN-Internal-Light-Control\LIN-Internal_Light_ECU.cydsn\TopDesign\TopDesign.cysch
# Project: D:\Users\jagumiel\Documents\PSoC Creator\LIN-Automotive\3-LIN-Internal-Light-Control\LIN-Internal_Light_ECU.cydsn\LIN-Internal_Light_ECU.cyprj
# Date: Wed, 22 Jul 2020 06:10:11 GMT
