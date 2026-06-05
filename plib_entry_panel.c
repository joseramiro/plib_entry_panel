/**
 * @file plib_entry_panel.c
 * @brief Widgets pour panneau en C (uniquement logique)
 * @author Ramiro Najera
 * @version 1.0.0
 * @date 2026-05-18
 */

#include "plib_entry_panel.h"
#include "libs/common_c_libs/plib_data_struct.h"

// Static gpio functions
static void PanelEntry_WriteLed(const PanelEntry_t *entry, const uint16_t led, uint8_t value);
static void PanelEntry_WriteAllLeds(const PanelEntry_t *entry, uint8_t value);
static void PanelEntry_WriteRadioStateToLeds(const PanelEntry_t *entry, uint8_t state);
static void PanelEntry_WriteRelay(const PanelEntry_t *entry, const uint16_t relay, uint8_t value);
static void PanelEntry_WriteAllRelays(const PanelEntry_t *entry, uint8_t value);
static void PanelEntry_WriteRadioStateToRelays(const PanelEntry_t *entry, uint8_t state);

// PanelEntry public API
void PanelEntry_SetHW(PanelEntry_t *entry, const PanelHW_t *hw)
{
    entry->hw = hw;
}

void PanelEntry_SetValidation(PanelEntry_t *entry, uint8_t state)
{
    entry->validation = state;
}

void PanelEntry_StartPulse(PanelEntry_t *entry)
{
    if(entry->hw && entry->hw->set_button_pressed_flag && entry->buttons && entry->button_count > 0)
    {
        entry->hw->set_button_pressed_flag(entry->buttons[0], 1);
        PanelEntry_SetValidation(entry, 1);
    }
}

uint8_t PanelEntry_GetValidation(const PanelEntry_t *entry)
{
    return entry->validation;
}

void PanelEntry_SetEnabled(PanelEntry_t *entry, uint8_t state)
{
    if(entry->hw && entry->hw->set_button_enabled && entry->buttons && entry->button_count > 0)
    {
        for(uint8_t i = 0; i < entry->button_count; i++)
            entry->hw->set_button_enabled(entry->buttons[i], state);
    } 
}

uint8_t PanelEntry_GetEnabled(const PanelEntry_t *entry)
{
    if(entry->hw && entry->hw->get_button_enabled && entry->button_count > 0)
    {
        for(uint8_t i = 0; i < entry->button_count; i++)
        {
            if(!entry->hw->get_button_enabled(entry->buttons[i]))
                return 0;
        }
        return 1;
    }
    return 0;
}

void PanelEntry_SetAllLedsRelaysOn(PanelEntry_t *entry)
{
    if(entry->state)
    {
        *entry->state = 1;
        PanelEntry_WriteAllLeds(entry, 1);
        PanelEntry_WriteAllRelays(entry, 1);
    }    
}

void PanelEntry_SetAllLedsRelaysOff(PanelEntry_t *entry)
{
    if(entry->state)
    {
        *entry->state = 0;
        PanelEntry_WriteAllLeds(entry, 0);
        PanelEntry_WriteAllRelays(entry, 0);
    }
}

void PanelEntry_SetToggleDoubleState(PanelEntry_t *entry, uint8_t state)
{
    if(entry->state && entry->leds && entry->led_count >= 2)
    {
        *entry->state = state;

        switch (state)
        {
            // off state
            case 0:
                PanelEntry_WriteLed(entry, entry->leds[0], 0);
                PanelEntry_WriteLed(entry, entry->leds[1], 0);
                PanelEntry_WriteAllRelays(entry, 0);
                break;

            // option 0 selected
            case 1:
                PanelEntry_WriteLed(entry, entry->leds[0], 1);
                PanelEntry_WriteLed(entry, entry->leds[1], 0);
                PanelEntry_WriteAllRelays(entry, 0);
                break;

            // option 1 selected
            case 2:
                PanelEntry_WriteLed(entry, entry->leds[0], 0);
                PanelEntry_WriteLed(entry, entry->leds[1], 1);
                PanelEntry_WriteAllRelays(entry, 1);
                break;
        
            // off if invalid
            default:
                PanelEntry_WriteLed(entry, entry->leds[0], 0);
                PanelEntry_WriteLed(entry, entry->leds[1], 0);
                PanelEntry_WriteAllRelays(entry, 0);
                break;
        }
    }    
}

void PanelEntry_SetToggleRadioState(PanelEntry_t *entry, uint8_t state)
{
    if(entry->state && entry->leds && entry->led_count > 0)
    {
        *entry->state = state;
        PanelEntry_WriteRadioStateToLeds(entry, *entry->state);
        PanelEntry_WriteRadioStateToRelays(entry, *entry->state);
    }
}

// PanelEntries public API
void PanelEntries_SetEnabled(PanelEntry_t *entries, uint8_t entry_count, uint8_t state)
{
    for(uint8_t i = 0; i < entry_count; i++)
        PanelEntry_SetEnabled(&entries[i], state);
}

// Static functions
static void PanelEntry_WriteLed(const PanelEntry_t *entry, const uint16_t led, uint8_t value)
{
    if(entry->hw && entry->hw->write_led)
        entry->hw->write_led(led, value);
}

static void PanelEntry_WriteAllLeds(const PanelEntry_t *entry, uint8_t value)
{
    if(entry->leds && entry->led_count > 0)
    {
        for(uint8_t i = 0; i < entry->led_count; i++)
            PanelEntry_WriteLed(entry, entry->leds[i], value);
    }
}

static void PanelEntry_WriteRadioStateToLeds(const PanelEntry_t *entry, uint8_t state)
{
    if(entry->leds && entry->led_count > 0)
    {
        if(state == 0)
            for(uint8_t i = 0; i < entry->led_count; i++)
                PanelEntry_WriteLed(entry, entry->leds[i], 0);
        else
            for(uint8_t i = 0; i < entry->led_count; i++)
                PanelEntry_WriteLed(entry, entry->leds[i], (i == state -1));
    }
}

static void PanelEntry_WriteRelay(const PanelEntry_t *entry, const uint16_t relay, uint8_t value)
{
    if(entry->hw && entry->hw->write_relay)
        entry->hw->write_relay(relay, value);
}

static void PanelEntry_WriteAllRelays(const PanelEntry_t *entry, uint8_t value)
{
    if(entry->relays)
    {
        for(uint8_t i = 0; i < entry->relay_count; i++)
            PanelEntry_WriteRelay(entry, entry->relays[i], value);
    }
}

static void PanelEntry_WriteRadioStateToRelays(const PanelEntry_t *entry, uint8_t state)
{
    if(entry->relays)
    {
        if(state == 0)
            for(uint8_t i = 0; i < entry->relay_count; i++)
                PanelEntry_WriteRelay(entry, entry->relays[i], 0);
        else
            for(uint8_t i = 0; i < entry->relay_count; i++)
                PanelEntry_WriteRelay(entry, entry->relays[i], (i == state -1));
    }
}