# group `height_controller` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void `[`SetTargetHeight`](#group__height__controller_1ga7e4b9e0553c6e4ff6f138525982b8a3a)`(uint32_t height)`            | Set the target height (%).
`public uint32_t `[`GetTargetHeight`](#group__height__controller_1ga2f9e7ec1d8f14d1b24e99f0d00942feb)`(void)`            | Get the target height (%).
`public void `[`HeightControllerInit`](#group__height__controller_1ga09a2f641e228f3777c3747e286436ff9)`(void)`            | Initialise the height controller.
`public void `[`PreloadHeightController`](#group__height__controller_1gaa87ff1b313c1da36eb19d9efaa9d3493)`(int32_t control,int32_t error)`            | Preload the integral component of the pid contoller so the Main rotor start of with.
`public void `[`UpdateHeightController`](#group__height__controller_1gaf92b7f65ace9826815b431fbe4ec82e8)`(uint32_t delta_t)`            | Update the height controller pid loop.
`public void `[`TuneProportionalMainRotor`](#group__height__controller_1gaf3c40224482eac21e7994279f07b0321)`(double gain)`            | Use at own risk.

## Members

#### `public void `[`SetTargetHeight`](#group__height__controller_1ga7e4b9e0553c6e4ff6f138525982b8a3a)`(uint32_t height)` 

Set the target height (%).

#### Parameters
* `height` The target height (%).

#### `public uint32_t `[`GetTargetHeight`](#group__height__controller_1ga2f9e7ec1d8f14d1b24e99f0d00942feb)`(void)` 

Get the target height (%).

#### Returns
The target height(%).

#### `public void `[`HeightControllerInit`](#group__height__controller_1ga09a2f641e228f3777c3747e286436ff9)`(void)` 

Initialise the height controller.

#### `public void `[`PreloadHeightController`](#group__height__controller_1gaa87ff1b313c1da36eb19d9efaa9d3493)`(int32_t control,int32_t error)` 

Preload the integral component of the pid contoller so the Main rotor start of with.

#### Parameters
* `control` power.

This was to improves rise time of the helicopter by boosting the Main rotor.

#### Parameters
* `control` The immediate control power desired by the Main rotor 

* `error` The absolute difference between current height and target height.

#### `public void `[`UpdateHeightController`](#group__height__controller_1gaf92b7f65ace9826815b431fbe4ec82e8)`(uint32_t delta_t)` 

Update the height controller pid loop.

#### Parameters
* `delta_t` The update period of the height controller.

#### `public void `[`TuneProportionalMainRotor`](#group__height__controller_1gaf3c40224482eac21e7994279f07b0321)`(double gain)` 

Use at own risk.

#### Parameters
* `gain`

