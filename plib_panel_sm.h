#ifndef PLIB_PANEL_SM_H
#define PLIB_PANEL_SM_H

#include "plib_entry_panel.h"

typedef enum
{
    PANEL_SM_TYPE_PULSE,
    PANEL_SM_TYPE_DIFF_PULSE,
    PANEL_SM_TYPE_FOLLOW,
    PANEL_SM_TYPE_TOGGLE1,
    PANEL_SM_TYPE_TOGGLE2,
    PANEL_SM_TYPE_RADIO3,
    PANEL_SM_TYPE_RADIO4
}PanelSMType_t;

typedef enum
{
    PANEL_SM_PULSE_IDLE,
    PANEL_SM_PULSE_ON,
    PANEL_SM_PULSE_OFF,
}PanelSMPulse_t;

typedef enum
{
    PANEL_SM_DIFF_PULSE_IDLE,
    PANEL_SM_DIFF_PULSE_WAIT_VALID,
    PANEL_SM_DIFF_PULSE_ON,
    PANEL_SM_DIFF_PULSE_OFF,
}PanelSMDiffPulse_t;

typedef enum
{
    PANEL_SM_FOLLOW_IDLE,
    PANEL_SM_FOLLOW_ON,
    PANEL_SM_FOLLOW_OFF,
}PanelSMFollow_t;

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

typedef enum
{
    PANEL_SM_RADIO_IDLE,
    PANEL_SM_RADIO_OPTION0,
    PANEL_SM_RADIO_OPTION1,
    PANEL_SM_RADIO_OPTION2,
    PANEL_SM_RADIO_OPTION3
}PanelSMRadio_t;

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
void PanelSM_Init(PanelStateMachine_t *sm);
void PanelSM_InitList(PanelStateMachine_t *sm, uint8_t count, const PanelHW_t *hw);
void PanelSM_Run(PanelStateMachine_t *sm);
void PanelSM_RunList(PanelStateMachine_t *sm, uint8_t count);
void PanelSM_SetState(PanelStateMachine_t *sm, uint8_t state);
void PanelSM_SetAllEnable(PanelStateMachine_t *sm, uint8_t count, uint8_t state);

// Condition functions
uint8_t PanelSM_Cond_None(PanelEntry_t *entry);
uint8_t PanelSM_Cond_isValidated(PanelEntry_t *entry);
uint8_t PanelSM_Cond_isButton0Pressed(PanelEntry_t *entry);
uint8_t PanelSM_Cond_isButton1Pressed(PanelEntry_t *entry);
uint8_t PanelSM_Cond_isButton2Pressed(PanelEntry_t *entry);
uint8_t PanelSM_Cond_isButton3Pressed(PanelEntry_t *entry);
uint8_t PanelSM_Cond_isButton0Released(PanelEntry_t *entry);
uint8_t PanelSM_Cond_isTimerFinished(PanelEntry_t *entry);

#endif  // PLIB_PANEL_SM_H