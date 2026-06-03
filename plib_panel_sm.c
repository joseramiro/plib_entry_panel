#include "plib_panel_sm.h"
#include "plib_panel_sm_diff_pulse.h"
#include "plib_panel_sm_pulse.h"
#include "plib_panel_sm_follow.h"
#include "plib_panel_sm_toggle1.h"
#include "plib_panel_sm_toggle2.h"
#include "plib_panel_sm_radio.h"

// Public API
void PanelSM_Init(PanelStateMachine_t *sm)
{
    switch (sm->entry.type)
    {
        case PANEL_SM_TYPE_PULSE:
            sm->states = PanelSM_Pulse_GetTable();
            break;
        
        case PANEL_SM_TYPE_DIFF_PULSE:
            sm->states = PanelSM_DiffPulse_GetTable();
            break;
        
        case PANEL_SM_TYPE_FOLLOW:
            sm->states = PanelSM_Follow_GetTable();
            break;
        
        case PANEL_SM_TYPE_TOGGLE1:
            sm->states = PanelSM_Toggle1_GetTable();
            break;
        
        case PANEL_SM_TYPE_TOGGLE2:
            sm->states = PanelSM_Toggle2_GetTable();
            break;
        
        case PANEL_SM_TYPE_RADIO3:
            sm->states = PanelSM_Radio3_GetTable();
            break;
        
        case PANEL_SM_TYPE_RADIO4:
            sm->states = PanelSM_Radio4_GetTable();
            break;
    
        default:
            break;
    }
    sm->currentState = 0;
    sm->entryFlag = 1;
}

void PanelSM_InitList(PanelStateMachine_t *sm, uint8_t count, const PanelHW_t *hw)
{
    for(uint8_t i = 0; i < count; i++)
    {
        PanelEntry_SetHW(&sm[i].entry, hw);
        PanelSM_Init(&sm[i]);
    }
}

void PanelSM_Run(PanelStateMachine_t *sm)
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
            PanelSM_SetState(sm, t->nextState);
            break;
        }
    }
}

void PanelSM_RunList(PanelStateMachine_t *sm, uint8_t count)
{
    for(uint8_t i = 0; i < count; i++)
    {
        PanelSM_Run(&sm[i]);
    }
}

void PanelSM_SetState(PanelStateMachine_t *sm, uint8_t state)
{
    sm->currentState = state;
    sm->entryFlag = 1;
}

void PanelSM_SetAllEnable(PanelStateMachine_t *sm, uint8_t count, uint8_t state)
{
    for(uint8_t i = 0; i < count; i++)
    {
        PanelEntry_SetEnabled(&sm[i].entry, state);
    }
}

// Condition functions
uint8_t PanelSM_Cond_None(PanelEntry_t *entry)
{
    return 1;
}

uint8_t PanelSM_Cond_isValidated(PanelEntry_t *entry)
{
    return PanelEntry_GetValidation(entry);
}

uint8_t PanelSM_Cond_isButton0Pressed(PanelEntry_t *entry)
{
    return(entry->hw->get_button_pressed_flag(entry->buttons[0]));
}

uint8_t PanelSM_Cond_isButton1Pressed(PanelEntry_t *entry)
{
    return(entry->hw->get_button_pressed_flag(entry->buttons[1]));
}

uint8_t PanelSM_Cond_isButton2Pressed(PanelEntry_t *entry)
{
    return(entry->hw->get_button_pressed_flag(entry->buttons[2]));
}

uint8_t PanelSM_Cond_isButton3Pressed(PanelEntry_t *entry)
{
    return(entry->hw->get_button_pressed_flag(entry->buttons[3]));
}

uint8_t PanelSM_Cond_isButton0Released(PanelEntry_t *entry)
{
    return(entry->hw->get_button_released_flag(entry->buttons[0]));
}

uint8_t PanelSM_Cond_isTimerFinished(PanelEntry_t *entry)
{
    return(entry->hw->timer_finished(entry->timer_id));
}