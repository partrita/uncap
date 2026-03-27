## 2025-03-27 - Prevent information leakage from uninitialized stack variables
**Vulnerability:** Information leakage from uninitialized stack variables in logging functions.
**Learning:** In C, local character arrays in functions (like `logKey`) are not automatically initialized to zero. If they are manipulated via functions like `strcpy` or `sprintf` but those functions don't completely fill the buffer (or depending on path, don't write to them at all), the uninitialized portions of the buffer retain old stack memory data. When these buffers are subsequently used or logged, they can inadvertently leak sensitive stack data, leading to an information disclosure vulnerability.
**Prevention:** Always initialize local character arrays and buffers to `{0}` upon declaration.
