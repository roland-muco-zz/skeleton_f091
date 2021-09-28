# skeleton_f091

Skeleton application for `STMF091xx` including the bare minimum to get started. Using the `NUCLEO-F091RC` as a starting point, this skeleton helps to get things underway a.s.a.p.

The project can be imported in `STM32CubeIDE`.

The branch `LEAP` is used to create a very crude bootloader. It will jump to the application and flash the LED along the way. It is also for a particular board that uses a STM32F091CCTx.

- The LED is connected to PA15.
- The debug UART5 is connected to PB3 and PB4.
- The HSE clock used is 16MHz with MCO output on 8MHz

