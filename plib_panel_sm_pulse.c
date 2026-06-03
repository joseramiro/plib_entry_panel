#include "plib_panel_sm.h"
#include "plib_panel_sm_pulse.h"

// Static functions
static void PanelSM_Action_PulseON(PanelEntry_t *entry);
static void PanelSM_Action_PulseOFF(PanelEntry_t *entry);

// State table
const PanelSM_State_t pulseStateTable[] =
{
    [PANEL_SM_PULSE_IDLE] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_PULSE_ON}
        },
        .action = NULL,
        .numTransitions = 1
    },

    [PANEL_SM_PULSE_ON] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isTimerFinished, PANEL_SM_PULSE_OFF}
        },
        .action = PanelSM_Action_PulseON,
        .numTransitions = 1
    },

    [PANEL_SM_PULSE_OFF] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_None, PANEL_SM_PULSE_IDLE}
        },
        .action = PanelSM_Action_PulseOFF,
        .numTransitions = 1
    }
};

// Public API
const PanelSM_State_t* PanelSM_Pulse_GetTable()
{
    return pulseStateTable;
}

// Static functions
static void PanelSM_Action_PulseON(PanelEntry_t *entry)
{
    PanelEntry_SetAllLedsRelaysOn(entry);
    entry->hw->set_timer(entry->timer_id, PANEL_BUTTON_PULSE_TIMEOUT);
}

static void PanelSM_Action_PulseOFF(PanelEntry_t *entry)
{
    PanelEntry_SetAllLedsRelaysOff(entry);
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}