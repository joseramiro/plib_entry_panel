#include "plib_panel_state_machine.h"

// Static functions: conditions
static uint8_t PanelSM_Cond_None(PanelEntry_t *entry);
static uint8_t PanelSM_Cond_isValidated(PanelEntry_t *entry);
static uint8_t PanelSM_Cond_isButton0Pressed(PanelEntry_t *entry);
static uint8_t PanelSM_Cond_isButton1Pressed(PanelEntry_t *entry);
static uint8_t PanelSM_Cond_isButton2Pressed(PanelEntry_t *entry);
static uint8_t PanelSM_Cond_isButton3Pressed(PanelEntry_t *entry);
static uint8_t PanelSM_Cond_isButton0Released(PanelEntry_t *entry);
static uint8_t PanelSM_Cond_isTimerFinished(PanelEntry_t *entry);

// Static functions: actions
static void PanelSM_Action_PulseON(PanelEntry_t *entry);
static void PanelSM_Action_PulseOFF(PanelEntry_t *entry);
static void PanelMS_Action_DiffPulseON(PanelEntry_t *entry);
static void PanelMS_Action_DiffPulseOFF(PanelEntry_t *entry);

static void PanelSM_Action_FollowON(PanelEntry_t *entry);
static void PanelSM_Action_FollowOFF(PanelEntry_t *entry);
static void PanelSM_Action_ToggleSimpleON(PanelEntry_t *entry);
static void PanelSM_Action_ToggleSimpleOFF(PanelEntry_t *entry);
static void PanelSM_Action_ToggleDoubleOFF(PanelEntry_t *entry);
static void PanelSM_Action_ToggleDoubleOpt0(PanelEntry_t *entry);
static void PanelSM_Action_ToggleDoubleOpt1(PanelEntry_t *entry);
static void PanelSM_Action_RadioOFF(PanelEntry_t *entry);
static void PanelSM_Action_RadioOpt0(PanelEntry_t *entry);
static void PanelSM_Action_RadioOpt1(PanelEntry_t *entry);
static void PanelSM_Action_RadioOpt2(PanelEntry_t *entry);
static void PanelSM_Action_RadioOpt3(PanelEntry_t *entry);

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


// Public API
void SM_InitPanelEntry(PanelStateMachine_t *sm)
{
    switch (sm->entry.type)
    {
        case PANEL_SM_TYPE_PULSE:
            sm->states = pulseStateTable;
            break;
        
        case PANEL_SM_TYPE_DIFF_PULSE:
            sm->states = diffPulseStateTable;
            break;
        
        case PANEL_SM_TYPE_FOLLOW:
            sm->states = followStateTable;
            break;
        
        case PANEL_SM_TYPE_TOGGLE1:
            sm->states = toggle1StateTable;
            break;
        
        case PANEL_SM_TYPE_TOGGLE2:
            sm->states = toggle2StateTable;
            break;
        
        case PANEL_SM_TYPE_RADIO3:
            sm->states = radio3StateTable;
            break;
        
        case PANEL_SM_TYPE_RADIO4:
            sm->states = radio4StateTable;
            break;
    
        default:
            break;
    }
    sm->currentState = 0;
    sm->entryFlag = 1;
}

void SM_RunPanelEntry(PanelStateMachine_t *sm)
{
    const PanelSM_State_t *s = &sm->states[sm->currentState];

    if(sm->entryFlag)
    {
        if(s->action)
        {
            s->action(&sm->entry);
            sm->entryFlag = 0;
        }
    }
    for (uint8_t i = 0; i < s->numTransitions; i++)
    {
        const PanelSM_Transition_t *t = &s->transitions[i];
        if (t->condition && t->condition(&sm->entry))
        {
            SM_SetState(sm, t->nextState);
            break;
        }
    }
}

void SM_SetState(PanelStateMachine_t *sm, uint8_t state)
{
    sm->currentState = state;
    sm->entryFlag = 1;
}

// Static functions: conditions
static uint8_t PanelSM_Cond_None(PanelEntry_t *entry)
{
    return 1;
}

static uint8_t PanelSM_Cond_isValidated(PanelEntry_t *entry)
{
    return PanelEntry_GetValidation(entry);
}

static uint8_t PanelSM_Cond_isButton0Pressed(PanelEntry_t *entry)
{
    return(entry->hw->get_button_pressed_flag(entry->buttons[0]));
}

static uint8_t PanelSM_Cond_isButton1Pressed(PanelEntry_t *entry)
{
    return(entry->hw->get_button_pressed_flag(entry->buttons[1]));
}

static uint8_t PanelSM_Cond_isButton2Pressed(PanelEntry_t *entry)
{
    return(entry->hw->get_button_pressed_flag(entry->buttons[2]));
}

static uint8_t PanelSM_Cond_isButton3Pressed(PanelEntry_t *entry)
{
    return(entry->hw->get_button_pressed_flag(entry->buttons[3]));
}

static uint8_t PanelSM_Cond_isButton0Released(PanelEntry_t *entry)
{
    return(entry->hw->get_button_released_flag(entry->buttons[0]));
}

static uint8_t PanelSM_Cond_isTimerFinished(PanelEntry_t *entry)
{
    return(entry->hw->timer_finished(entry->timer_id));
}

// Static functions: actions
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

static void PanelSM_Action_FollowON(PanelEntry_t *entry)
{
    PanelEntry_SetAllLedsRelaysOn(entry);
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}

static void PanelSM_Action_FollowOFF(PanelEntry_t *entry)
{
    PanelEntry_SetAllLedsRelaysOff(entry);
    entry->hw->set_button_released_flag(entry->buttons[0], 0);
}

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