#include "plib_panel_sm.h"
#include "plib_panel_sm_radio.h"

// Static functions
static void PanelSM_Action_RadioOFF(PanelEntry_t *entry);
static void PanelSM_Action_RadioOpt0(PanelEntry_t *entry);
static void PanelSM_Action_RadioOpt1(PanelEntry_t *entry);
static void PanelSM_Action_RadioOpt2(PanelEntry_t *entry);
static void PanelSM_Action_RadioOpt3(PanelEntry_t *entry);

// State table radio3
const PanelSM_State_t radio3StateTable[] =
{
    [PANEL_SM_RADIO_IDLE] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_RADIO_OPTION0},
            {PanelSM_Cond_isButton1Pressed, PANEL_SM_RADIO_OPTION1},
            {PanelSM_Cond_isButton2Pressed, PANEL_SM_RADIO_OPTION2}
        },
        .action = PanelSM_Action_RadioOFF,
        .numTransitions = 3
    },

    [PANEL_SM_RADIO_OPTION0] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_RADIO_IDLE},
            {PanelSM_Cond_isButton1Pressed, PANEL_SM_RADIO_OPTION1},
            {PanelSM_Cond_isButton2Pressed, PANEL_SM_RADIO_OPTION2}
        },
        .action = PanelSM_Action_RadioOpt0,
        .numTransitions = 3
    },

    [PANEL_SM_RADIO_OPTION1] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_RADIO_OPTION0},
            {PanelSM_Cond_isButton1Pressed, PANEL_SM_RADIO_IDLE},
            {PanelSM_Cond_isButton2Pressed, PANEL_SM_RADIO_OPTION2}
        },
        .action = PanelSM_Action_RadioOpt1,
        .numTransitions = 3
    },

    [PANEL_SM_RADIO_OPTION2] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_RADIO_OPTION0},
            {PanelSM_Cond_isButton1Pressed, PANEL_SM_RADIO_OPTION1},
            {PanelSM_Cond_isButton2Pressed, PANEL_SM_RADIO_IDLE}
        },
        .action = PanelSM_Action_RadioOpt2,
        .numTransitions = 3
    },
};

// State table radio4
const PanelSM_State_t radio4StateTable[] =
{
    [PANEL_SM_RADIO_IDLE] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_RADIO_OPTION0},
            {PanelSM_Cond_isButton1Pressed, PANEL_SM_RADIO_OPTION1},
            {PanelSM_Cond_isButton2Pressed, PANEL_SM_RADIO_OPTION2},
            {PanelSM_Cond_isButton3Pressed, PANEL_SM_RADIO_OPTION3}
        },
        .action = PanelSM_Action_RadioOFF,
        .numTransitions = 4
    },

    [PANEL_SM_RADIO_OPTION0] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_RADIO_IDLE},
            {PanelSM_Cond_isButton1Pressed, PANEL_SM_RADIO_OPTION1},
            {PanelSM_Cond_isButton2Pressed, PANEL_SM_RADIO_OPTION2},
            {PanelSM_Cond_isButton3Pressed, PANEL_SM_RADIO_OPTION3}
        },
        .action = PanelSM_Action_RadioOpt0,
        .numTransitions = 4
    },

    [PANEL_SM_RADIO_OPTION1] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_RADIO_OPTION0},
            {PanelSM_Cond_isButton1Pressed, PANEL_SM_RADIO_IDLE},
            {PanelSM_Cond_isButton2Pressed, PANEL_SM_RADIO_OPTION2},
            {PanelSM_Cond_isButton3Pressed, PANEL_SM_RADIO_OPTION3}
        },
        .action = PanelSM_Action_RadioOpt1,
        .numTransitions = 4
    },

    [PANEL_SM_RADIO_OPTION2] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_RADIO_OPTION0},
            {PanelSM_Cond_isButton1Pressed, PANEL_SM_RADIO_OPTION1},
            {PanelSM_Cond_isButton2Pressed, PANEL_SM_RADIO_IDLE},
            {PanelSM_Cond_isButton3Pressed, PANEL_SM_RADIO_OPTION3}
        },
        .action = PanelSM_Action_RadioOpt2,
        .numTransitions = 4
    },

    [PANEL_SM_RADIO_OPTION3] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PANEL_SM_RADIO_OPTION0},
            {PanelSM_Cond_isButton1Pressed, PANEL_SM_RADIO_OPTION1},
            {PanelSM_Cond_isButton2Pressed, PANEL_SM_RADIO_OPTION2},
            {PanelSM_Cond_isButton3Pressed, PANEL_SM_RADIO_IDLE}
        },
        .action = PanelSM_Action_RadioOpt3,
        .numTransitions = 4
    },
};

const PanelSM_State_t* PanelSM_Radio3_GetTable()
{
    return radio3StateTable;
}

const PanelSM_State_t* PanelSM_Radio4_GetTable()
{
    return radio4StateTable;
}

// Static functions
static void PanelSM_Action_RadioOFF(PanelEntry_t *entry)
{
    PanelEntry_SetToggleRadioState(entry, 0);
    for(uint8_t i = 0; i < entry->button_count; i++)
        entry->hw->set_button_pressed_flag(entry->buttons[i], 0);
}

static void PanelSM_Action_RadioOpt0(PanelEntry_t *entry)
{
    PanelEntry_SetToggleRadioState(entry, 1);
    for(uint8_t i = 0; i < entry->button_count; i++)
        entry->hw->set_button_pressed_flag(entry->buttons[i], 0);
}

static void PanelSM_Action_RadioOpt1(PanelEntry_t *entry)
{
    PanelEntry_SetToggleRadioState(entry, 2);
    for(uint8_t i = 0; i < entry->button_count; i++)
        entry->hw->set_button_pressed_flag(entry->buttons[i], 0);
}

static void PanelSM_Action_RadioOpt2(PanelEntry_t *entry)
{
    PanelEntry_SetToggleRadioState(entry, 3);
    for(uint8_t i = 0; i < entry->button_count; i++)
        entry->hw->set_button_pressed_flag(entry->buttons[i], 0);
}

static void PanelSM_Action_RadioOpt3(PanelEntry_t *entry)
{
    PanelEntry_SetToggleRadioState(entry, 4);
    for(uint8_t i = 0; i < entry->button_count; i++)
        entry->hw->set_button_pressed_flag(entry->buttons[i], 0);
}