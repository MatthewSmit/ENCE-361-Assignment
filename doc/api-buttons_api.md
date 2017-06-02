# group `buttons_api` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`define `[`NUM_POLLS`](#group__buttons__api_1ga8d6a287942d41da4e609bd81f411c02d)            | Change button state only after NUM_POLLS consecutive readings have an opposite value.
`enum `[`@0`](#group__buttons__api_1ga06fc87d81c62e9abb8790b6e5713c55b)            | 
`public void `[`ButtonsInit`](#group__buttons__api_1gac479c9466379bee2e17d33edf3a7162c)`(void)`            | Initialise the buttons.
`public void `[`UpdateButtons`](#group__buttons__api_1ga652347f4782e34ffa799aa3fc3974ae4)`()`            | Update all of the buttons and their state.
`public uint8_t `[`NumPushes`](#group__buttons__api_1ga0f572456b5d8ec1761a0e38e50c0a99f)`(uint8_t button_name)`            | Gets the number of pushes for a given button and resets the push count.
`public void `[`ResetPushes`](#group__buttons__api_1ga0436c12de94d3de43aa55d212a42dddb)`(void)`            | Reset the push count for all buttons.

## Members

#### `define `[`NUM_POLLS`](#group__buttons__api_1ga8d6a287942d41da4e609bd81f411c02d) 

Change button state only after NUM_POLLS consecutive readings have an opposite value.

#### `enum `[`@0`](#group__buttons__api_1ga06fc87d81c62e9abb8790b6e5713c55b) 

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
BTN_UP            | 
BTN_DOWN            | 
BTN_LEFT            | 
BTN_RIGHT            | 
NUM_BUTTONS            | 

#### `public void `[`ButtonsInit`](#group__buttons__api_1gac479c9466379bee2e17d33edf3a7162c)`(void)` 

Initialise the buttons.

#### `public void `[`UpdateButtons`](#group__buttons__api_1ga652347f4782e34ffa799aa3fc3974ae4)`()` 

Update all of the buttons and their state.

#### `public uint8_t `[`NumPushes`](#group__buttons__api_1ga0f572456b5d8ec1761a0e38e50c0a99f)`(uint8_t button_name)` 

Gets the number of pushes for a given button and resets the push count.

#### Parameters
* `button_name` one of BUT_UP, BUT_DOWN, BUT_LEFT or BUT_RIGHT 

#### Returns
the number of pushes for the given button since last called

#### `public void `[`ResetPushes`](#group__buttons__api_1ga0436c12de94d3de43aa55d212a42dddb)`(void)` 

Reset the push count for all buttons.

