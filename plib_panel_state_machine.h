#ifndef PLIB_PANEL_STATE_MACHINE_H
#define PLIB_PANEL_STATE_MACHINE_H

#include "plib_entry_panel.h"

typedef enum
{
    PANEL_SM_TYPE_PULSE
}PanelSMType_t;

typedef enum
{
    PANEL_SM_PULSE_IDLE,
    PANEL_SM_PULSE_ON,
}PanelSMPulse_t;

typedef uint8_t (*PanelSM_CondFunc)(PanelEntry_t *entry);
typedef void (*PanelSM_ActionFunc)(PanelEntry_t *entry);

typedef struct {
    PanelSM_CondFunc condition;
    PanelSM_ActionFunc action;
    uint8_t nextState;
}PanelSM_Transition_t;

typedef struct {
    const PanelSM_Transition_t *transitions;
    uint8_t numTransitions;
}PanelSM_State_t;

typedef struct {
    const PanelSM_State_t *states;
    uint8_t currentState;
    PanelEntry_t entry;
}PanelStateMachine_t;

// Public API
void SM_InitPanelEntry(PanelStateMachine_t *sm);
void SM_RunPanelEntry(PanelStateMachine_t *sm);

#endif
