#include "plib_panel_sm.h"
#include "plib_panel_sm_toggle1.h"

// Static functions
static void PanelSM_Action_ToggleSimpleON(PanelEntry_t *entry);
static void PanelSM_Action_ToggleSimpleOFF(PanelEntry_t *entry);

// State table
const PanelSM_State_t toggle1StateTable[] =
{
    [PANEL_SM_TOGGLE1_IDLE] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_TOGGLE1_ON}
        },
        .action = PanelSM_Action_ToggleSimpleOFF,
        .numTransitions = 1
    },

    [PANEL_SM_TOGGLE1_ON] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_TOGGLE1_IDLE}
        },
        .action = PanelSM_Action_ToggleSimpleON,
        .numTransitions = 1
    },
};

const PanelSM_State_t* PanelSM_Toggle1_GetTable()
{
    return toggle1StateTable;
}

// Static functions
static void PanelSM_Action_ToggleSimpleON(PanelEntry_t *entry)
{
    PanelEntry_SetAllLedsRelaysOn(entry);
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}

static void PanelSM_Action_ToggleSimpleOFF(PanelEntry_t *entry)
{
    PanelEntry_SetAllLedsRelaysOff(entry);
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}