/**
 * @file plib_entry_panel.c
 * @brief Widgets pour panneau en C (uniquement logique)
 * @author Ramiro Najera
 * @version 1.0.0
 * @date 2026-05-18
 */

#include "plib_entry_panel.h"

// Static gpio functions
static void PanelEntry_WriteLed(const PanelEntry_t *entry, const uint16_t led, uint8_t value);
static void PanelEntry_WriteAllLeds(const PanelEntry_t *entry, uint8_t value);
static void PanelEntry_WriteStateToToogle2Leds(const PanelEntry_t *entry, uint8_t state);
static void PanelEntry_WriteStateToRadioLeds(const PanelEntry_t *entry, uint8_t state);
static void PanelEntry_WriteRelay(const PanelEntry_t *entry, const uint16_t relay, uint8_t value);
static void PanelEntry_WriteAllRelays(const PanelEntry_t *entry, uint8_t value);
static void PanelEntry_WriteStateToToggle2Relays(const PanelEntry_t *entry, uint8_t state);
static void PanelEntry_WriteStateToRadioRelays(const PanelEntry_t *entry, uint8_t state);

// Public API
void PanelEntries_Init(PanelEntry_t *entries, uint8_t entry_count, const PanelHW_t *hw)
{
    for(uint8_t i = 0; i < entry_count; i++)
        entries[i].hw = hw;
}

// Public API for PanelEntries
void PanelEntries_ManageEntryChange(PanelEntry_t *entries, uint8_t entry_count, uint16_t btn)
{
    //if(g_panelHW == NULL)
    //    return;
    
    for (uint8_t i = 0; i < entry_count; i++)
    {
        PanelEntry_t *entry = &entries[i];
        
        if (!PanelEntry_GetEnabled(entry) || !entry->on_button)
            continue;

        for (uint8_t b = 0; b < entry->button_count; b++)
        {
            if (entry->buttons[b] == btn)
            {
                entry->on_button(entry, btn);
                return;
            }
        }
    }
}

void PanelEntries_Update(PanelEntry_t *entries, uint8_t entry_count)
{
    //if(g_panelHW == NULL)
    //    return;

    for (uint8_t i = 0; i < entry_count; i++)
    {
        PanelEntry_t *entry = &entries[i];

        // Blink feature
        /*
        if (entry->blink_enabled && g_panelHW->timer_finished(entry->timer_id))
        {
           PanelEntry_UpdateBlink(entry);
        }
        */
        
        if (!PanelEntry_GetEnabled(entry))
            continue;

        // Normal entry update
        if (entry->on_update)
            entry->on_update(entry);
    }
}

void PanelEntries_SetEnabled(PanelEntry_t *entries, uint8_t entry_count, uint8_t state)
{
    for(uint8_t i = 0; i < entry_count; i++)
        PanelEntry_SetEnabled(&entries[i], state);
}

void PanelEntries_SetAllLeds(PanelEntry_t *entries, uint8_t entry_count, uint8_t state)
{
    for(uint8_t i = 0; i < entry_count; i++)
        PanelEntry_WriteAllLeds(&entries[i], state);
}

void PanelEntries_SetAllRelays(PanelEntry_t *entries, uint8_t entry_count, uint8_t state)
{
    for(uint8_t i = 0; i < entry_count; i++)
        PanelEntry_WriteAllRelays(&entries[i], state);
}


// Public API for PanelEntry
void PanelEntry_SetEnabled(PanelEntry_t *entry, uint8_t state)
{
    if(entry->hw->set_button_enabled)
    {
        for(uint8_t i = 0; i < entry->button_count; i++)
            entry->hw->set_button_enabled(entry->buttons[i], state);
    } 
}

uint8_t PanelEntry_GetEnabled(PanelEntry_t *entry)
{
    if(entry->hw->get_button_enabled)
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

void PanelEntry_GeneratePulse(PanelEntry_t *entry)
{
    PanelEntry_OnPulse(entry, 0);   // button param is unused
}

void PanelEntry_SetToggle1Led(PanelEntry_t *entry, uint8_t state)
{
    if(entry->state == NULL || entry->led_count == 0)
        return;

    *entry->state = state;
    PanelEntry_WriteAllLeds(entry, *entry->state);
    PanelEntry_WriteAllRelays(entry, *entry->state);
}

void PanelEntry_SetToggle2Leds(PanelEntry_t *entry, uint8_t state)
{
    if(entry->state == NULL || entry->led_count == 0)
        return;

    *entry->state = state;
    PanelEntry_WriteStateToToogle2Leds(entry, *entry->state);
    PanelEntry_WriteStateToToggle2Relays(entry, *entry->state);
}

void PanelEntry_SetToggleRadio(PanelEntry_t *entry, uint8_t state)
{
    if(entry->state == NULL || entry->led_count == 0)
        return;

    *entry->state = state;
    PanelEntry_WriteStateToRadioLeds(entry, *entry->state);
    PanelEntry_WriteStateToRadioRelays(entry, *entry->state);
}

// Functions by entry type
void PanelEntry_OnPulse(PanelEntry_t *entry, uint16_t btn)
{
    
    if(entry->hw->set_timer)
    {
        *entry->state = 1;
        PanelEntry_WriteAllLeds(entry, 1);
        PanelEntry_WriteAllRelays(entry, 1);
        entry->hw->set_timer(entry->timer_id, PANEL_BUTTON_PULSE_TIMEOUT);
    }
    
}

void PanelEntry_UpdatePulse(PanelEntry_t *entry)
{
    if(entry->hw->timer_finished)
    {
        if(entry->hw->timer_finished(entry->timer_id) && entry->state)
        {
            *entry->state = 0;
            PanelEntry_WriteAllLeds(entry, 0);
            PanelEntry_WriteAllRelays(entry, 0);
        }
    }
    
}

void PanelEntry_OnToggleSimple(PanelEntry_t *entry, uint16_t btn)
{
    if(entry->state == NULL || entry->led_count == 0)
        return;
    
    *entry->state ^= 1;
    PanelEntry_WriteAllLeds(entry, *entry->state);
    PanelEntry_WriteAllRelays(entry, *entry->state);
}

void PanelEntry_OnToggleDouble(PanelEntry_t *entry, uint16_t btn)
{
    if(entry->state == NULL || entry->led_count == 0)
        return;

    *entry->state ^= 1;
    PanelEntry_WriteStateToToogle2Leds(entry, *entry->state);
    PanelEntry_WriteStateToToggle2Relays(entry, *entry->state);
}

void PanelEntry_OnRadio(PanelEntry_t *entry, uint16_t btn)
{
    // find which index was pressed
    for(uint8_t i = 0; i < entry->button_count; i++)
    {
        if(entry->buttons[i] == btn)
        {
            if(*entry->state == i + 1)
                *entry->state = 0;
            else
                *entry->state = i + 1;
            PanelEntry_WriteStateToRadioLeds(entry, *entry->state);
            PanelEntry_WriteStateToRadioRelays(entry, *entry->state);
            return;
        }
    }
}









// Statis functions

static void PanelEntry_WriteLed(const PanelEntry_t *entry, const uint16_t led, uint8_t value)
{
    if(entry->hw->write_led)
        entry->hw->write_led(led, value);
}

static void PanelEntry_WriteAllLeds(const PanelEntry_t *entry, uint8_t value)
{
    if(entry->leds == NULL)
        return;
    // set all leds state
    for(uint8_t i = 0; i < entry->led_count; i++)
        PanelEntry_WriteLed(entry, entry->leds[i], value);
}

static void PanelEntry_WriteStateToToogle2Leds(const PanelEntry_t *entry, uint8_t state)
{
    if(entry->leds == NULL)
        return;
    
    if(state == 0)
    {
        PanelEntry_WriteLed(entry, entry->leds[0], 1);
        PanelEntry_WriteLed(entry, entry->leds[1], 0);
    }
    else
    {
        PanelEntry_WriteLed(entry, entry->leds[0], 0);
        PanelEntry_WriteLed(entry, entry->leds[1], 1);
    }
}

static void PanelEntry_WriteStateToRadioLeds(const PanelEntry_t *entry, uint8_t state)
{
    if(entry->leds == NULL)
        return;
    // set only 1 led from entry according to state
    if(state == 0)
        for(uint8_t i = 0; i < entry->led_count; i++)
            PanelEntry_WriteLed(entry, entry->leds[i], 0);
    else
        for(uint8_t i = 0; i < entry->led_count; i++)
            PanelEntry_WriteLed(entry, entry->leds[i], (i == state -1));
}

static void PanelEntry_WriteRelay(const PanelEntry_t *entry, const uint16_t relay, uint8_t value)
{
    if(entry->hw->write_relay)
        entry->hw->write_relay(relay, value);
}

static void PanelEntry_WriteAllRelays(const PanelEntry_t *entry, uint8_t value)
{
    if(entry->relays == NULL)
        return;
    // set all relays state
    for(uint8_t i = 0; i < entry->relay_count; i++)
        PanelEntry_WriteRelay(entry, entry->relays[i], value);
}

static void PanelEntry_WriteStateToToggle2Relays(const PanelEntry_t *entry, uint8_t state)
{
    if(entry->relays == NULL)
        return;
    
    if(state == 0)
    {
        PanelEntry_WriteRelay(entry, entry->relays[0], 0);
    }
    else
    {
        PanelEntry_WriteRelay(entry, entry->relays[0], 1);
    }
}

static void PanelEntry_WriteStateToRadioRelays(const PanelEntry_t *entry, uint8_t state)
{
    if(entry->relays == NULL)
        return;
    // set only 1 relay from entry according to state
    if(state == 0)
        for(uint8_t i = 0; i < entry->relay_count; i++)
            PanelEntry_WriteRelay(entry, entry->relays[i], 0);
    else
        for(uint8_t i = 0; i < entry->relay_count; i++)
            PanelEntry_WriteRelay(entry, entry->relays[i], (i == state -1));
}