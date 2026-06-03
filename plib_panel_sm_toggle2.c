#include "plib_panel_sm.h"
#include "plib_panel_sm_toggle2.h"

// Static functions
static void PanelSM_Action_ToggleDoubleOFF(PanelEntry_t *entry);
static void PanelSM_Action_ToggleDoubleOpt0(PanelEntry_t *entry);
static void PanelSM_Action_ToggleDoubleOpt1(PanelEntry_t *entry);

// State table
const PanelSM_State_t toggle2StateTable[] =
{
    [PANEL_SM_TOGGLE2_IDLE] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_TOGGLE2_OPTION0}
        },
        .action = PanelSM_Action_ToggleDoubleOFF,
        .numTransitions = 1
    },

    [PANEL_SM_TOGGLE2_OPTION0] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_TOGGLE2_OPTION1}
        },
        .action = PanelSM_Action_ToggleDoubleOpt0,
        .numTransitions = 1
    },

    [PANEL_SM_TOGGLE2_OPTION1] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_TOGGLE2_OPTION0}
        },
        .action = PanelSM_Action_ToggleDoubleOpt1,
        .numTransitions = 1
    },
};

// Public API
const PanelSM_State_t* PanelSM_Toggle2_GetTable()
{
    return toggle2StateTable;
}

// Static functions
static void PanelSM_Action_ToggleDoubleOFF(PanelEntry_t *entry)
{
    PanelEntry_SetToggleDoubleState(entry, 0);
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}

static void PanelSM_Action_ToggleDoubleOpt0(PanelEntry_t *entry)
{
    PanelEntry_SetToggleDoubleState(entry, 1);
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}

static void PanelSM_Action_ToggleDoubleOpt1(PanelEntry_t *entry)
{
    PanelEntry_SetToggleDoubleState(entry, 2);
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}
