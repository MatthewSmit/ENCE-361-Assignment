# group `yaw_api` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int32_t `[`GetTargetYawDegrees`](#group__yaw__api_1ga435932581c1f277dfe181c2728b65e83)`(void)`            | Get the target yaw in degrees.
`public void `[`SetTargetYawDegrees`](#group__yaw__api_1gabdb3b3a0d712763ac13a83c5c0e8864b)`(int32_t yaw)`            | Set the desired target yaw (degrees).
`public int32_t `[`GetTargetYaw`](#group__yaw__api_1ga8734cc39f5317745123f60ae67aa29be)`(void)`            | Get the target yaw.
`public void `[`SetTargetYaw`](#group__yaw__api_1ga513fa55c1d246a94a7108cf85f23d98f)`(int32_t yaw)`            | #### Parameters
`public void `[`YawControllerInit`](#group__yaw__api_1gab86837fccba07f66e1ab5d927a7b1be2)`(void)`            | Initialise the yaw controller.
`public void `[`PreloadYawController`](#group__yaw__api_1ga8aea286d2f1cfce0a3af66951884fed2)`(int32_t control,int32_t error)`            | #### Parameters
`public void `[`UpdateYawController`](#group__yaw__api_1ga78c3b56e9af57a40f5266b292006a302)`(uint32_t delta_t)`            | #### Parameters
`public void `[`TuneProportionalTailRotor`](#group__yaw__api_1ga96f25cf144fccb4efc3003607ad1e8c6)`(double gain)`            | Use at own risk.

## Members

#### `public int32_t `[`GetTargetYawDegrees`](#group__yaw__api_1ga435932581c1f277dfe181c2728b65e83)`(void)` 

Get the target yaw in degrees.

#### Returns
The target yaw in degrees.

#### `public void `[`SetTargetYawDegrees`](#group__yaw__api_1gabdb3b3a0d712763ac13a83c5c0e8864b)`(int32_t yaw)` 

Set the desired target yaw (degrees).

#### Parameters
* `yaw` The desired target yaw (degrees).

#### `public int32_t `[`GetTargetYaw`](#group__yaw__api_1ga8734cc39f5317745123f60ae67aa29be)`(void)` 

Get the target yaw.

#### Returns
the target yaw. 

**See also**: [yaw.h](#yaw_8h) for rotation unit.

#### `public void `[`SetTargetYaw`](#group__yaw__api_1ga513fa55c1d246a94a7108cf85f23d98f)`(int32_t yaw)` 

#### Parameters
* `yaw`

#### `public void `[`YawControllerInit`](#group__yaw__api_1gab86837fccba07f66e1ab5d927a7b1be2)`(void)` 

Initialise the yaw controller.

#### `public void `[`PreloadYawController`](#group__yaw__api_1ga8aea286d2f1cfce0a3af66951884fed2)`(int32_t control,int32_t error)` 

#### Parameters
* `control` 

* `error`

#### `public void `[`UpdateYawController`](#group__yaw__api_1ga78c3b56e9af57a40f5266b292006a302)`(uint32_t delta_t)` 

#### Parameters
* `delta_t`

#### `public void `[`TuneProportionalTailRotor`](#group__yaw__api_1ga96f25cf144fccb4efc3003607ad1e8c6)`(double gain)` 

Use at own risk.

#### Parameters
* `gain`

