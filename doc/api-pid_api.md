# group `pid_api` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void `[`PidInit`](#group__pid__api_1ga252747f3b4f2241856f88641e104a785)`(`[`PidState`](#structPidState)` * state)`            | Initialise the pid controller.
`public void `[`PreloadPid`](#group__pid__api_1gaa22eb9d3cdfd214772d033a9fd4d5b3b)`(`[`PidState`](#structPidState)` * state,int32_t integral_preload)`            | Preload the integral component of the pid state with a postitve or negative error to reduce integration time.
`public int32_t `[`UpdatePid`](#group__pid__api_1gac67cc69c03ad452a9f640676e92be8e5)`(`[`PidState`](#structPidState)` * state,int32_t error,uint32_t delta_t,double proportional_gain,double integral_gain,double derivative_gain)`            | Update the pid controller loop.
`struct `[`PidState`](api-pid_api.md#structPidState) | A structure to accumulate the error and store the previous error for use by the pid controller.

## Members

#### `public void `[`PidInit`](#group__pid__api_1ga252747f3b4f2241856f88641e104a785)`(`[`PidState`](#structPidState)` * state)` 

Initialise the pid controller.

#### Parameters
* `state` The pid error state. 

**See also**: [PidState](#structPidState)

#### `public void `[`PreloadPid`](#group__pid__api_1gaa22eb9d3cdfd214772d033a9fd4d5b3b)`(`[`PidState`](#structPidState)` * state,int32_t integral_preload)` 

Preload the integral component of the pid state with a postitve or negative error to reduce integration time.

#### Parameters
* `state` The pid error state. 

* `integral_preload` The preload error.

#### `public int32_t `[`UpdatePid`](#group__pid__api_1gac67cc69c03ad452a9f640676e92be8e5)`(`[`PidState`](#structPidState)` * state,int32_t error,uint32_t delta_t,double proportional_gain,double integral_gain,double derivative_gain)` 

Update the pid controller loop.

#### Parameters
* `state` The pid error state. 

* `error` The current error. 

* `delta_t` The update period of the pid controller. 

* `proportional_gain` The proportional gain constant. 

* `integral_gain` The integral gain constant. 

* `derivative_gain` The derivative gain constant. 

#### Returns

# struct `PidState` 

A structure to accumulate the error and store the previous error for use by the pid controller.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int32_t `[`error_previous`](api-pid_api.md#structPidState_1ac18328849b77c6ff138e3f8326887942) | The previous error.
`public int32_t `[`error_integrated`](api-pid_api.md#structPidState_1a069073b6783c49fe6ba5eb8882884e0f) | The accumulated error.

## Members

#### `public int32_t `[`error_previous`](api-pid_api.md#structPidState_1ac18328849b77c6ff138e3f8326887942) 

The previous error.

#### `public int32_t `[`error_integrated`](api-pid_api.md#structPidState_1a069073b6783c49fe6ba5eb8882884e0f) 

The accumulated error.

