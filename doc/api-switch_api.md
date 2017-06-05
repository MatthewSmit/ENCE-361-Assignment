# group `switch_api` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`define `[`NUM_POLLS`](#group__switch__api_1ga8d6a287942d41da4e609bd81f411c02d)            | Change switch state only after NUM_POLLS consecutive readings have an opposite value.
`enum `[`@3`](#group__switch__api_1gabc6126af1d45847bc59afa0aa3216b04)            | 
`public void `[`SwitchInit`](#group__switch__api_1ga0f674ea092ae368ce3d994d179c91a24)`(void)`            | Initialise the switch.
`public void `[`UpdateSwitch`](#group__switch__api_1ga5d19f6f28cd200295d36a7f506867e2d)`()`            | Update the switch state.
`public uint8_t `[`GetSwitchEvent`](#group__switch__api_1gafb74642fec21ae48e96b4adcb0c60911)`(void)`            | Get the switch event.

## Members

#### `define `[`NUM_POLLS`](#group__switch__api_1ga8d6a287942d41da4e609bd81f411c02d) 

Change switch state only after NUM_POLLS consecutive readings have an opposite value.

#### `enum `[`@3`](#group__switch__api_1gabc6126af1d45847bc59afa0aa3216b04) 

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
SWITCH_DOWN            | 
SWITCH_UP            | 

#### `public void `[`SwitchInit`](#group__switch__api_1ga0f674ea092ae368ce3d994d179c91a24)`(void)` 

Initialise the switch.

#### `public void `[`UpdateSwitch`](#group__switch__api_1ga5d19f6f28cd200295d36a7f506867e2d)`()` 

Update the switch state.

#### `public uint8_t `[`GetSwitchEvent`](#group__switch__api_1gafb74642fec21ae48e96b4adcb0c60911)`(void)` 

Get the switch event.

#### Returns
DOWN or UP slide of the switch.

