#include "plib_panel_sm.h"
#include "plib_panel_sm_follow.h"

// Static functions
static void PanelSM_Action_FollowON(PanelEntry_t *entry);
static void PanelSM_Action_FollowOFF(PanelEntry_t *entry);

// State table
const PanelSM_State_t followStateTable[] =
{
    [PANEL_SM_FOLLOW_IDLE] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_FOLLOW_ON}
        },
        .action = NULL,
        .numTransitions = 1
    },

    [PANEL_SM_FOLLOW_ON] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Released, PANEL_SM_FOLLOW_OFF}
        },
        .action = PanelSM_Action_FollowON,
        .numTransitions = 1
    },

    [PANEL_SM_FOLLOW_OFF] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_FOLLOW_ON}
        },
        .action = PanelSM_Action_FollowOFF,
        .numTransitions = 1
    }
};

// Public API
const PanelSM_State_t* PanelSM_Follow_GetTable()
{
    return followStateTable;
}

// Static functions
static void PanelSM_Action_FollowON(PanelEntry_t *entry)
{
    PanelEntry_SetAllLedsRelaysOn(entry);
    *entry->state = 1;
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}

static void PanelSM_Action_FollowOFF(PanelEntry_t *entry)
{
    PanelEntry_SetAllLedsRelaysOff(entry);
    *entry->state = 0;
    entry->hw->set_button_released_flag(entry->buttons[0], 0);
}
