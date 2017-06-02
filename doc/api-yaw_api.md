# group `yaw_api` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`define `[`NUMBER_SLOTS`](#group__yaw__api_1ga11955ca482e9e323eebf0d1a701b1996)            | The number of slots in 360 degrees of rotation.
`define `[`YAW_FULL_ROTATION`](#group__yaw__api_1ga2f05011aeb1385dbb7363d09314ab642)            | The number of yaw updates in 360 degrees of rotation.
`public void `[`YawDetectionInit`](#group__yaw__api_1gab3cb00bee4d4de9d6074a250bd8f1074)`(void)`            | Initialises the yaw manager.
`public int32_t `[`GetYaw`](#group__yaw__api_1gadaaf694619119fa84d322a954e0e6036)`(void)`            | Get the current yaw.
`public int32_t `[`GetYawDegrees`](#group__yaw__api_1ga44afb9dcc9f3c83ec4964ff1e02f1654)`(void)`            | Get the current yaw.
`public int32_t `[`GetClosestYawRef`](#group__yaw__api_1ga9b958da6564c22a3327eb7798791591c)`(int32_t current_yaw)`            | Helper function to return the closest yaw such that the helicopter is facing towards the camera.
`public void `[`YawRefTrigger`](#group__yaw__api_1ga5a3d6aaf1269b785b0d63ba836da100e)`(void)`            | Triggers an interrupt to fire when the refernce yaw has been found.
`public bool `[`YawRefFound`](#group__yaw__api_1ga4b3ae61946ab9780a82f65c3979d8687)`(void)`            | Check if the reference yaw has been found.

## Members

#### `define `[`NUMBER_SLOTS`](#group__yaw__api_1ga11955ca482e9e323eebf0d1a701b1996) 

The number of slots in 360 degrees of rotation.

#### `define `[`YAW_FULL_ROTATION`](#group__yaw__api_1ga2f05011aeb1385dbb7363d09314ab642) 

The number of yaw updates in 360 degrees of rotation.

#### `public void `[`YawDetectionInit`](#group__yaw__api_1gab3cb00bee4d4de9d6074a250bd8f1074)`(void)` 

Initialises the yaw manager.

#### `public int32_t `[`GetYaw`](#group__yaw__api_1gadaaf694619119fa84d322a954e0e6036)`(void)` 

Get the current yaw.

#### Returns
the yaw (notches)

#### `public int32_t `[`GetYawDegrees`](#group__yaw__api_1ga44afb9dcc9f3c83ec4964ff1e02f1654)`(void)` 

Get the current yaw.

#### Returns
the yaw (degrees)

#### `public int32_t `[`GetClosestYawRef`](#group__yaw__api_1ga9b958da6564c22a3327eb7798791591c)`(int32_t current_yaw)` 

Helper function to return the closest yaw such that the helicopter is facing towards the camera.

#### Parameters
* `current_yaw` the current yaw 

#### Returns
the closest reference yaw

#### `public void `[`YawRefTrigger`](#group__yaw__api_1ga5a3d6aaf1269b785b0d63ba836da100e)`(void)` 

Triggers an interrupt to fire when the refernce yaw has been found.

#### `public bool `[`YawRefFound`](#group__yaw__api_1ga4b3ae61946ab9780a82f65c3979d8687)`(void)` 

Check if the reference yaw has been found.

#### Returns
true if the yaw reference has been found else false

