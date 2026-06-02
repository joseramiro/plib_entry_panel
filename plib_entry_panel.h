/**
 * @file plib_entry_panel.h
 * @brief Widgets pour panneau en C (uniquement logique)
 * @author Ramiro Najera
 * @version 1.0.0
 * @date 2026-05-18
 */

#ifndef PLIB_ENTRY_PANEL_H
#define PLIB_ENTRY_PANEL_H

#include <stdint.h>
#include <stddef.h>

#define PANEL_BUTTON_PULSE_TIMEOUT      1000
#define PANEL_LED_BLINK_TIMEOUT         500

// to be implemented in external lib
typedef enum
{
    ENTRY_TYPE_PULSE,
    ENTRY_TYPE_TOGGLE_SIMPLE,
    ENTRY_TYPE_TOGGLE_DOUBLE,
    ENTRY_TYPE_RADIO,
    ENTRY_TYPE_FOLLOW_INPUT,
}EntryType_t;

typedef struct
{
    void (*write_led)(uint16_t led, uint8_t value);
    void (*write_relay)(uint16_t relay, uint8_t value);
    void (*set_timer)(uint16_t timer_id, uint16_t value);
    uint8_t (*timer_finished)(uint16_t timer_id);
    void (*set_button_enabled)(uint16_t button, uint8_t value);
    uint8_t (*get_button_enabled)(uint16_t button);
    void (*set_button_pressed_flag)(uint16_t button, uint8_t value);
    uint8_t (*get_button_pressed_flag)(uint16_t button);
} PanelHW_t;

typedef struct PanelEntry
{
    uint8_t id;                 // id of entry (according to array in panel)
    EntryType_t type;           // type of entry (defines behaviour)
    const PanelHW_t *hw;        // hardware interface functions
    const uint16_t *buttons;    // list of buttons id
    uint8_t button_count;       // number of buttons
    const uint16_t *leds;       // list of leds id
    uint8_t led_count;          // number of leds
    const uint16_t *relays;     // list of relays id
    uint8_t relay_count;        // number of relays
    uint8_t *state;             // for toggle, radio and select functions
    uint16_t timer_id;          // if of timer for pulse and blink functions
    uint16_t blink_led;         // id of led to blink
    uint8_t blink_enabled;      // led blink enabled
    uint8_t blink_state;        // state of led blinking
}PanelEntry_t;

void PanelEntry_SetHW(PanelEntry_t *entry, const PanelHW_t *hw);

// Public API for PanelEntries
void PanelEntries_SetEnabled(PanelEntry_t *entries, uint8_t entry_count, uint8_t state);

// Public API for PanelEntry
void PanelEntry_SetEnabled(PanelEntry_t *entry, uint8_t state);
uint8_t PanelEntry_GetEnabled(PanelEntry_t *entry);
void PanelEntry_SetToggle2Leds(PanelEntry_t *entry, uint8_t state);
void PanelEntry_SetToggleRadio(PanelEntry_t *entry, uint8_t state);
void PanelEntry_SetAllLedsRelaysOn(PanelEntry_t *entry);
void PanelEntry_SetAllLedsRelaysOff(PanelEntry_t *entry);
void PanelEntry_OnToggleDouble(PanelEntry_t *entry, uint16_t btn);
void PanelEntry_OnRadio(PanelEntry_t *entry, uint16_t btn);

#endif  // PLIB_ENTRY_PANEL_H