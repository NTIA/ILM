# ILM Error Codes and Warning Flags

This document describes all error codes and warning flags used by the ITU-R SG3 Irregular Lunar Model (ILM) library.

## Return Codes

The ILM functions return integer codes to indicate success, warnings, or errors:

| Code | Constant | Description |
|------|----------|-------------|
| 0    | `SUCCESS` | Successful execution with no warnings |
| 1    | `SUCCESS_WITH_WARNINGS` | Successful execution but with warnings (check warnings parameter) |
| 1000+ | Various error codes | Function failed - see Error Codes section below |

## Error Codes (1000+)

All error codes are in the range 1000-1999. When an error occurs, the function returns immediately with the error code.

### Input Parameter Errors

| Code | Constant | Description | Valid Range |
|------|----------|-------------|-------------|
| 1000 | `ERROR__TX_TERMINAL_HEIGHT` | TX terminal height is out of range | 0.5 - 3000.0 meters |
| 1001 | `ERROR__RX_TERMINAL_HEIGHT` | RX terminal height is out of range | 0.5 - 3000.0 meters |
| 1002 | `ERROR__INVALID_PERCENTAGE` | Location percentage is out of range | 0 < p < 100 |
| 1004 | `ERROR__FREQUENCY` | Frequency is out of range | 20.0 - 20000.0 MHz |
| 1005 | `ERROR__POLARIZATION` | Invalid polarization value | 0 (horizontal) or 1 (vertical) |
| 1006 | `ERROR__EPSILON` | Relative permittivity is out of range | ≥ 1.0 |
| 1007 | `ERROR__SIGMA` | Conductivity is out of range | > 0.0 |
| 1009 | `ERROR__PATH_DISTANCE` | Path distance is out of range | Function-specific |
| 1010 | `ERROR__DELTA_H` | Terrain irregularity parameter is out of range | Function-specific |
| 1011 | `ERROR__TX_SITING_CRITERIA` | Invalid TX siting criteria | 0 (mobile) or 1 (fixed) |
| 1012 | `ERROR__RX_SITING_CRITERIA` | Invalid RX siting criteria | 0 (mobile) or 1 (fixed) |

### Calculation Errors

| Code | Constant | Description |
|------|----------|-------------|
| 1003 | `ERROR__REFRACTIVITY` | Refractivity is out of range |
| 1008 | `ERROR__GROUND_IMPEDANCE` | The imaginary portion of the complex impedance is larger than the real portion |

## Warning Flags (Bitwise)

Warning flags are combined using bitwise OR operations. Multiple warnings can be active simultaneously. The warnings parameter should be checked when the function returns `SUCCESS_WITH_WARNINGS`.

### Parameter Range Warnings

These warnings indicate parameters are near their operational limits but still within acceptable ranges:

| Flag | Constant | Description | Warning Range |
|------|----------|-------------|---------------|
| 0x0001 | `WARN__TX_TERMINAL_HEIGHT` | TX terminal height is near its limits | < 1.0 or > 1000.0 meters |
| 0x0002 | `WARN__RX_TERMINAL_HEIGHT` | RX terminal height is near its limits | < 1.0 or > 1000.0 meters |
| 0x0004 | `WARN__FREQUENCY` | Frequency is near its limits | < 40.0 or > 10000.0 MHz |

### Path Distance Warnings

| Flag | Constant | Description |
|------|----------|-------------|
| 0x0008 | `WARN__PATH_DISTANCE_TOO_BIG_1` | Path distance is near its upper limit |
| 0x0010 | `WARN__PATH_DISTANCE_TOO_BIG_2` | Path distance is large - care must be taken with result |
| 0x0020 | `WARN__PATH_DISTANCE_TOO_SMALL_1` | Path distance is near its lower limit |
| 0x0040 | `WARN__PATH_DISTANCE_TOO_SMALL_2` | Path distance is small - care must be taken with result |

### Horizon Angle Warnings

| Flag | Constant | Description |
|------|----------|-------------|
| 0x0080 | `WARN__TX_HORIZON_ANGLE` | TX horizon angle is large - small angle approximations could break down |
| 0x0100 | `WARN__RX_HORIZON_ANGLE` | RX horizon angle is large - small angle approximations could break down |

### Horizon Distance Warnings

| Flag | Constant | Description |
|------|----------|-------------|
| 0x0200 | `WARN__TX_HORIZON_DISTANCE_1` | TX horizon distance is less than 1/10 of the smooth earth horizon distance |
| 0x0400 | `WARN__RX_HORIZON_DISTANCE_1` | RX horizon distance is less than 1/10 of the smooth earth horizon distance |
| 0x0800 | `WARN__TX_HORIZON_DISTANCE_2` | TX horizon distance is greater than 3 times the smooth earth horizon distance |
| 0x1000 | `WARN__RX_HORIZON_DISTANCE_2` | RX horizon distance is greater than 3 times the smooth earth horizon distance |

## Usage Examples

### Checking for Errors

```cpp
double A_db;
long warnings = 0;
IntermediateValues interValues;

int result = PointToPoint_Ex(h_tx, h_rx, pfl, f_mhz, pol, epsilon, sigma, p, 
                            &A_db, &warnings, &interValues);

if (result != SUCCESS && result != SUCCESS_WITH_WARNINGS) {
    // Handle error
    switch (result) {
        case ERROR__TX_TERMINAL_HEIGHT:
            printf("Error: TX height must be between 0.5 and 3000.0 meters\n");
            break;
        case ERROR__FREQUENCY:
            printf("Error: Frequency must be between 20.0 and 20000.0 MHz\n");
            break;
        // ... handle other error codes
    }
    return result;
}
```

### Checking for Warnings

```cpp
if (result == SUCCESS_WITH_WARNINGS) {
    if (warnings & WARN__TX_TERMINAL_HEIGHT) {
        printf("Warning: TX height is near operational limits\n");
    }
    if (warnings & WARN__FREQUENCY) {
        printf("Warning: Frequency is near operational limits\n");
    }
    if (warnings & WARN__PATH_DISTANCE_TOO_BIG_2) {
        printf("Warning: Large path distance - results should be used with caution\n");
    }
    // ... check other warning flags
}
```

### Decoding All Active Warnings

```cpp
void printWarnings(long warnings) {
    if (warnings == NO_WARNINGS) {
        printf("No warnings\n");
        return;
    }
    
    printf("Active warnings:\n");
    if (warnings & WARN__TX_TERMINAL_HEIGHT) 
        printf("  - TX height near limits\n");
    if (warnings & WARN__RX_TERMINAL_HEIGHT) 
        printf("  - RX height near limits\n");
    if (warnings & WARN__FREQUENCY) 
        printf("  - Frequency near limits\n");
    if (warnings & WARN__PATH_DISTANCE_TOO_BIG_1) 
        printf("  - Path distance near upper limit\n");
    if (warnings & WARN__PATH_DISTANCE_TOO_BIG_2) 
        printf("  - Path distance large\n");
    if (warnings & WARN__PATH_DISTANCE_TOO_SMALL_1) 
        printf("  - Path distance near lower limit\n");
    if (warnings & WARN__PATH_DISTANCE_TOO_SMALL_2) 
        printf("  - Path distance small\n");
    if (warnings & WARN__TX_HORIZON_ANGLE) 
        printf("  - TX horizon angle large\n");
    if (warnings & WARN__RX_HORIZON_ANGLE) 
        printf("  - RX horizon angle large\n");
    if (warnings & WARN__TX_HORIZON_DISTANCE_1) 
        printf("  - TX horizon distance < 1/10 smooth earth\n");
    if (warnings & WARN__RX_HORIZON_DISTANCE_1) 
        printf("  - RX horizon distance < 1/10 smooth earth\n");
    if (warnings & WARN__TX_HORIZON_DISTANCE_2) 
        printf("  - TX horizon distance > 3x smooth earth\n");
    if (warnings & WARN__RX_HORIZON_DISTANCE_2) 
        printf("  - RX horizon distance > 3x smooth earth\n");
}
```

## Notes

1. **Error vs Warning Ranges**: Notice that warnings are triggered when parameters approach their limits, while errors occur when parameters exceed absolute bounds. For example:
   - TX Height Warning: < 1.0 or > 1000.0 meters
   - TX Height Error: < 0.5 or > 3000.0 meters

2. **Bitwise Warning Operations**: Multiple warnings can be active simultaneously. Use bitwise AND (`&`) to check for specific warnings.

3. **Function-Specific Validation**: Some parameters (like path distance and delta_h) have validation that depends on the specific function being called and may have different limits for different propagation scenarios.

4. **Polarization Values**: Only horizontal (0) and vertical (1) polarizations are supported.

5. **Siting Criteria**: Only mobile (0) and fixed (1) siting criteria are supported for Area_Ex function.

## See Also

- `src/include/Errors.h` - Error code definitions
- `src/include/Warnings.h` - Warning flag definitions  
- `src/include/Enums.h` - Enumeration constants
- `src/ValidateInputs.cpp` - Input validation implementation