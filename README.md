# Irregular Lunar Model (ILM) #

**Note: This software repository has been made available as an active workspace in support of on-going discussions 
during the 2024 ITU-R Study Group 3 meetings. The work presented here represents an example implementation of the
ILM. Software releases in this repository are best-effort attempts to augment conversation and support collaboration and 
data sharing in the context of NTIA's scientific culture that values Open Science and holds that openness in science 
is fundamental to scientific and research integrity. The contents of this repository, including any software 
releases, have not undergone ITS standard quality review procedures such as code-reviews and unit testing.**

This code repository contains a US implementation of the Irregular Lunar Model (ILM). ILM predicts terrestrial 
radiowave propagation for frequencies between 20 MHz and 20 GHz based on electromagnetic theory and empirical 
models developed by Anita Longley and Phil Rice. Propagation mechanisms considered include free space loss 
and diffraction. Specifically, ILM predicts attenuation as a function of distance (greater than 1 km), 
terminal heights, terrain effects, and the variability of the signal in location.

## Inputs ##

ILM can be called in either Area Prediction Mode or Point-to-Point Prediction Mode.  The below inputs 
are organized accordingly.

### Common Inputs ###

| Variable          | Type   | Units | Limits       | Description  |
|-------------------|--------|-------|--------------|--------------|
| `h_tx__meter`     | double | meter | 0.5 <= `h_tx__meter` <= 3000 | Structural height of the TX |
| `h_rx__meter`     | double | meter | 0.5 <= `h_rx__meter` <= 3000 | Structural height of the RX |
| `f__mhz`          | double | MHz   | 20 <= `f__mhz` <= 20000 | Frequency |
| `pol`             | int    |       | enum          | Polarization  <ul><li>0 = Horizontal</li><li>1 = Vertical</li></ul> |
| `epsilon`         | double |       | 1 < `epsilon` | Relative permittivity |
| `sigma`           | double | S/m   | 0 < `sigma`   | Conductivity |
| `p`               | double |       | 0 < `p` < 100 | Location variability |

### Point-to-Point Mode Specific Inputs ###

| Variable          | Type   | Units | Limits       | Description  |
|-------------------|--------|-------|--------------|--------------|
| `pfl`             | double[] | meter |              | Terrain profile data in PFL format, from TX to RX <ul><li>`pfl[0]` : Number of elevation points - 1</li><li>`pfl[1]` : Resolution, in meters</li><li>`pfl[i]` : Elevation above sea level, in meters</li></ul> |

### Area Mode Inputs ###

| Variable          | Type   | Units | Limits       | Description  |
|-------------------|--------|-------|--------------|--------------|
| `d__km`           | double | km    | 0 < `d__km`  | Path distance |
| `delta_h__meter`  | double | meter | 0 <= `delta_h__meter` | Terrain irregularity parameter |
| `tx_siting_criteria` | int |       | enum         | Siting criteria of TX <ul><li>0 = Mobile</li><li>1 = Fixed</li></ul>|
| `rx_siting_criteria` | int |       | enum         | Siting criteria of RX <ul><li>0 = Mobile</li><li>1 = Fixed</li></ul>|

## Outputs ##

| Variable      | Type   | Units | Description |
|---------------|--------|-------|-------------|
| `A__db`       | double | dB    | Basic transmission loss |
| `warnings`    | int    |       | Warning flags |

## Intermediate Values ##

Internal intermediate values can be extracted from ILM via functions that are suffixed with `_Ex`.  These 
functions will populate the `IntermediateValues` data structure with intermediate values from the ILM calculations.

| Variable         | Type      | Units       | Description |
|------------------|-----------|-------------|-------------|
| `theta_hzn`      | double[]  | radians     | Terminal horizon angles |
| `d_hzn__meter`   | double[]  | meter       | Terminal horizon distances |
| `h_e__meter`     | double[]  | meter       | Effective terminal heights |
| `delta_h__meter` | double    | meter       | Terrain irregularity parameter |
| `A_ref__db`      | double    | dB          | Reference attenuation |
| `A_fs__db`       | double    | dB          | Free space basic transmission loss |
| `d__km`          | double    | km          | Path distance |
| `mode`           | int       |             | Mode of propagation <ul><li>10 = Line of Sight</li><li>20 = Diffraction, Single Horizon</li><li>21 = Diffraction, Double Horizon</li></ul>|

## Error Codes and Warning Flags ##

ILM supports a defined list of error codes and warning flags.  A complete list can be found [here](ERRORS_AND_WARNINGS.md).

## Notes on Code Style ##

* In general, variables follow the naming convention in which a single underscore denotes a subscript 
(pseudo-LaTeX format), and where a double underscore is followed by the units, i.e. h_tx__meter.
* Variables are named to match their corresponding mathematical variables in the underlying references.
* Wherever possible, equation numbers and source documentation are provided.

## Configure and Build ##

### C++ Software ###

The software is designed to be built into a DLL (or corresponding library for non-Windows systems).  The source code 
can be built for any OS that supports the standard C++ libraries.

## References ##

* United States WP 3J input contribution [3J/26](https://www.itu.int/md/R23-WP3J-C-0026/en)

## Contact ##

For technical questions about ILM, contact Billy Kozma, wkozma@ntia.gov.
