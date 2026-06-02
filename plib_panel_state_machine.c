#include "plib_panel_state_machine.h"

// Static functions: conditions
static uint8_t PanelSM_Cond_None(PanelEntry_t *entry);
static uint8_t PanelSM_Cond_isButton0Pressed(PanelEntry_t *entry);
static uint8_t PanelSM_Cond_isTimerFinished(PanelEntry_t *entry);

// Static functions: actions
static void PanelSM_Action_PulseON(PanelEntry_t *entry);
static void PanelSM_Action_PulseOFF(PanelEntry_t *entry);
static void PanelSM_Action_ToggleSimpleON(PanelEntry_t *entry);
static void PanelSM_Action_ToggleSimpleOFF(PanelEntry_t *entry);
static void PanelSM_Action_ToggleDoubleOpt0(PanelEntry_t *entry);
static void PanelSM_Action_ToggleDoubleOpt1(PanelEntry_t *entry);
static void PanelSM_Action_ToggleDoubleOFF(PanelEntry_t *entry);

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



// Public API
void SM_InitPanelEntry(PanelStateMachine_t *sm)
{
    switch (sm->entry.type)
    {
        case PANEL_SM_TYPE_PULSE:
            sm->states = pulseStateTable;
            break;
        
        case PANEL_SM_TYPE_TOGGLE_SIMPLE:
            sm->states = toggle1StateTable;
            break;
        
        case PANEL_SM_TYPE_TOGGLE_DOUBLE:
            sm->states = toggle2StateTable;
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

static uint8_t PanelSM_Cond_isButton0Pressed(PanelEntry_t *entry)
{
    return(entry->hw->get_button_pressed_flag(entry->buttons[0]));
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

static void PanelSM_Action_ToggleDoubleOpt0(PanelEntry_t *entry)
{
    PanelEntry_SetToggle2Leds(entry, 1);
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}

static void PanelSM_Action_ToggleDoubleOpt1(PanelEntry_t *entry)
{
    PanelEntry_SetToggle2Leds(entry, 2);
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}

static void PanelSM_Action_ToggleDoubleOFF(PanelEntry_t *entry)
{
    PanelEntry_SetToggle2Leds(entry, 0);
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}