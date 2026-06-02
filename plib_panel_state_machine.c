#include "plib_panel_state_machine.h"

// Static functions: conditions
static uint8_t PanelSM_Cond_isButton0Pressed(PanelEntry_t *entry);
static uint8_t PanelSM_Cond_isTimerFinished(PanelEntry_t *entry);

// Static functions: actions
static void PanelSM_Action_PulseON(PanelEntry_t *entry);
static void PanelSM_Action_PulseOFF(PanelEntry_t *entry);

const PanelSM_State_t pulseStateTable[] =
{
    [PANEL_SM_PULSE_IDLE] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isButton0Pressed, PanelSM_Action_PulseON, PANEL_SM_PULSE_ON}
        },
        .numTransitions = 1
    },

    [PANEL_SM_PULSE_ON] = {
        .transitions = (PanelSM_Transition_t[]){
            {PanelSM_Cond_isTimerFinished, PanelSM_Action_PulseOFF, PANEL_SM_PULSE_IDLE}
        },
        .numTransitions = 1
    }
};

// Public API
void SM_InitPanelEntry(PanelStateMachine_t *sm)
{
    switch (sm->entry.type)
    {
        case PANEL_SM_TYPE_PULSE:
            sm->states = pulseStateTable;
            break;
    
        default:
            break;
    }
    sm->currentState = 0;
}

void SM_RunPanelEntry(PanelStateMachine_t *sm)
{
    const PanelSM_State_t *s = &sm->states[sm->currentState];

    for (uint8_t i = 0; i < s->numTransitions; i++)
    {
        const PanelSM_Transition_t *t = &s->transitions[i];

        if (t->condition && t->condition(&sm->entry))
        {
            if (t->action)
                t->action(&sm->entry);

            sm->currentState = t->nextState;
            break;
        }
    }
}

// Static functions: conditions
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
    PanelEntry_PulseOn(entry);
    entry->hw->set_timer(entry->timer_id, PANEL_BUTTON_PULSE_TIMEOUT);
}

static void PanelSM_Action_PulseOFF(PanelEntry_t *entry)
{
    PanelEntry_PulseOff(entry);
    entry->hw->set_button_pressed_flag(entry->buttons[0], 0);
}