# group `flight_api` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void `[`FlightControllerInit`](#group__flight__api_1ga4394a6c257d73c65fc28dd3c0cebd578)`(void)`            | Initialise the flight controller module.
`public void `[`UpdateFlightMode`](#group__flight__api_1gab2bf648312a026f1a43e8b46041219f4)`()`            | U.
`public const char * `[`GetFlightMode`](#group__flight__api_1gacbea26128cb90a08dd9c29f40a47b512)`(void)`            | Get a string representation of the current flight mode.
`public void `[`PriorityTaskInit`](#group__flight__api_1ga7f2e026cf7eb5d211e8daa78277a4b7f)`(void)`            | Initialise the priority task sequencer.
`public void `[`PriorityTaskEnable`](#group__flight__api_1ga135a6914a2dfc136b398488316c409d6)`(void)`            | Enable the priority task sequencer.
`public void `[`PriorityTaskDisable`](#group__flight__api_1gaccb35ef73abc1e0e1f0d5c28f9b5e8d1)`(void)`            | Disable the priority task sequencer.

## Members

#### `public void `[`FlightControllerInit`](#group__flight__api_1ga4394a6c257d73c65fc28dd3c0cebd578)`(void)` 

Initialise the flight controller module.

#### `public void `[`UpdateFlightMode`](#group__flight__api_1gab2bf648312a026f1a43e8b46041219f4)`()` 

U.

#### `public const char * `[`GetFlightMode`](#group__flight__api_1gacbea26128cb90a08dd9c29f40a47b512)`(void)` 

Get a string representation of the current flight mode.

#### Returns
a string representing the flight mode

#### `public void `[`PriorityTaskInit`](#group__flight__api_1ga7f2e026cf7eb5d211e8daa78277a4b7f)`(void)` 

Initialise the priority task sequencer.

This timer handles updating of the pid controllers and height.

#### `public void `[`PriorityTaskEnable`](#group__flight__api_1ga135a6914a2dfc136b398488316c409d6)`(void)` 

Enable the priority task sequencer.

#### `public void `[`PriorityTaskDisable`](#group__flight__api_1gaccb35ef73abc1e0e1f0d5c28f9b5e8d1)`(void)` 

Disable the priority task sequencer.

