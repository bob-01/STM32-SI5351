## STM32-SI5351

* Портировал https://github.com/etherkit/Si5351Arduino под STM32

```C
  #include "si5351.h"

  si5351_init(&hi2c1, SI5351_BUS_BASE_ADDR, SI5351_CRYSTAL_LOAD_0PF, si5351_XTAL*1000000, si5351_FREQ_CORR);
  si5351_drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
  si5351_drive_strength(SI5351_CLK1, SI5351_DRIVE_8MA);
  si5351_drive_strength(SI5351_CLK2, SI5351_DRIVE_8MA);
  
  si5351_set_freq(10220000000, SI5351_CLK0);
  si5351_set_freq(10220000000, SI5351_CLK1);
  si5351_set_freq(10220000000, SI5351_CLK2);
```
