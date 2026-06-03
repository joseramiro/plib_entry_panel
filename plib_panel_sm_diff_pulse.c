#include "plib_panel_sm.h"
#include "plib_panel_sm_diff_pulse.h"

// Static functions
static void PanelMS_Action_DiffPulseON(PanelEntry_t *entry);
static void PanelMS_Action_DiffPulseOFF(PanelEntry_t *entry);

// State table
const PanelSM_State_t diffPulseStateTable[] =
{
    [PANEL_SM_DIFF_PULSE_IDLE] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_DIFF_PULSE_WAIT_VALID}
        },
        .action = NULL,
        .numTransitions = 1
    },

    [PANEL_SM_DIFF_PULSE_WAIT_VALID] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isValidated, PANEL_SM_DIFF_PULSE_ON}
        },
        .action = NULL,
        .numTransitions = 1
    },

    [PANEL_SM_DIFF_PULSE_ON] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isTimerFinished, PANEL_SM_DIFF_PULSE_OFF}
        },
        .action = PanelMS_Action_DiffPulseON,
        .numTransitions = 1
    },

    [PANEL_SM_DIFF_PULSE_OFF] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_None, PANEL_SM_DIFF_PULSE_IDLE}
        },
        .action = PanelMS_Action_DiffPulseOFF,
        .numTransitions = 1
    }
};

// Public API
const PanelSM_State_t* PanelSM_DiffPulse_GetTable()
{
    return diffPulseStateTable;
}

// Static functions
static void PanelMS_Action_DiffPulseON(PanelEntry_t *entry)
{
    PanelEntry_SetAllLedsRelaysOn(entry);
    entry->hw->set_timer(entry->timer_id, PANEL_BUTTON_PULSE_TIMEOUT);
}

static void PanelMS_Action_DiffPulseOFF(PanelEntry_t *entry)
{
    PanelEntry_SetAllLedsRelaysOff(entry);
    PanelEntry_SetValidation(entry, 0);
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}