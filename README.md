# stm32f429-fft-mbed

[![Build Status](https://travis-ci.org/yangosoft/stm32f429-fft-mbed.svg?branch=master)](https://travis-ci.org/yangosoft/stm32f429-fft-mbed)

FFT on audio captured by a mic and displayed on the LCD.

It uses  [KissFFT](https://github.com/mborgerding/kissfft)

This is a STM32F429I-DISC1 project using [mbed](https://os.mbed.com/docs/v5.10/tutorials/quick-start-offline.html).


## Build instructions

Download [ARM toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads). Please note: Ubuntu 18.04.01 has a broken ARM toolchain.

Set GCC_ARM in ``mbed_settings.py``

```
sudo apt install mercurial
mbed add mbed-os
mbed add http://os.mbed.com/teams/ST/code/TS_DISCO_F429ZI/
mbed add http://os.mbed.com/teams/ST/code/LCD_DISCO_F429ZI/
mbed add http://os.mbed.com/teams/ST/code/BSP_DISCO_F429ZI/
```

Edit ``debug.json``, ``develop.json`` and ``release.json`` in order to add ``std=c++14`` flags.


```
mbed compile --target DISCO_F429ZI --toolchain GCC_ARM -v
```
