#ifndef PLIB_PANEL_STATE_MACHINE_H
#define PLIB_PANEL_STATE_MACHINE_H

#include "plib_entry_panel.h"

typedef enum
{
    PANEL_SM_TYPE_PULSE,
    PANEL_SM_TYPE_TOGGLE_SIMPLE,
    PANEL_SM_TYPE_TOGGLE_DOUBLE,
}PanelSMType_t;

typedef enum
{
    PANEL_SM_PULSE_IDLE,
    PANEL_SM_PULSE_ON,
    PANEL_SM_PULSE_OFF,
}PanelSMPulse_t;

typedef enum
{
    PANEL_SM_TOGGLE1_IDLE,
    PANEL_SM_TOGGLE1_ON,
}PanelSMToggleSimple_t;

typedef enum
{
    PANEL_SM_TOGGLE2_IDLE,
    PANEL_SM_TOGGLE2_OPTION0,
    PANEL_SM_TOGGLE2_OPTION1,
}PanelSMToggleDouble_t;

typedef uint8_t (*PanelSM_CondFunc)(PanelEntry_t *entry);
typedef void (*PanelSM_ActionFunc)(PanelEntry_t *entry);

typedef struct {
    PanelSM_CondFunc condition;
    uint8_t nextState;
}PanelSM_Transition_t;

typedef struct {
    const PanelSM_Transition_t *transitions;
    uint8_t numTransitions;
    PanelSM_ActionFunc action;
}PanelSM_State_t;

typedef struct {
    const PanelSM_State_t *states;
    uint8_t currentState;
    uint8_t entryFlag;
    PanelEntry_t entry;
}PanelStateMachine_t;

// Public API
void SM_InitPanelEntry(PanelStateMachine_t *sm);
void SM_RunPanelEntry(PanelStateMachine_t *sm);
void SM_SetState(PanelStateMachine_t *sm, uint8_t state);

#endif
