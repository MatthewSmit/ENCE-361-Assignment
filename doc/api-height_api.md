# group `height_api` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`define `[`FULL_SCALE_RANGE`](#group__height__api_1ga69d827366c566cff5e16aa731aac8282)            | The range of the height sensor for a 100% height reading.
`public int32_t `[`GetHeight`](#group__height__api_1gaf26778f87d9cbb1a05bd2bdcad8a0c84)`(void)`            | Get the current height.
`public int32_t `[`GetHeightPercentage`](#group__height__api_1ga9b2b31d9c0100134131c20e56388a6eb)`(void)`            | Get the current height as a percentage.
`public void `[`UpdateHeight`](#group__height__api_1gad3298901b988453044f12597c6b97979)`()`            | Trigger the current height reading to be updated.
`public void `[`HeightManagerInit`](#group__height__api_1gaf2143de3552ee67abf7ec971e9a6a4bf)`(void)`            | Initialise the height sensor peripherals and ports.
`public void `[`ZeroHeightTrigger`](#group__height__api_1ga093d4bb608e149801c2580bb938986fb)`(void)`            | Trigger a zero height reading to be used as a reference for subsequent height readings.

## Members

#### `define `[`FULL_SCALE_RANGE`](#group__height__api_1ga69d827366c566cff5e16aa731aac8282) 

The range of the height sensor for a 100% height reading.

This translates to a 0.8 V sensor range using the 12-bit ADC peripheral.

#### `public int32_t `[`GetHeight`](#group__height__api_1gaf26778f87d9cbb1a05bd2bdcad8a0c84)`(void)` 

Get the current height.

Retrieve the sensor reading after it has been offset the zeroed sensor reading.

#### Returns
The height.

#### `public int32_t `[`GetHeightPercentage`](#group__height__api_1ga9b2b31d9c0100134131c20e56388a6eb)`(void)` 

Get the current height as a percentage.

#### Returns
The height as a percentage.

#### `public void `[`UpdateHeight`](#group__height__api_1gad3298901b988453044f12597c6b97979)`()` 

Trigger the current height reading to be updated.

No longer used in favour of direct triggering via timer timeout flag.

#### `public void `[`HeightManagerInit`](#group__height__api_1gaf2143de3552ee67abf7ec971e9a6a4bf)`(void)` 

Initialise the height sensor peripherals and ports.

#### `public void `[`ZeroHeightTrigger`](#group__height__api_1ga093d4bb608e149801c2580bb938986fb)`(void)` 

Trigger a zero height reading to be used as a reference for subsequent height readings.

