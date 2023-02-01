#include QMK_KEYBOARD_H

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // Send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD,
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

enum {
    TO1_TO2_TO3,
    J_CAPS,
    Q_ESC,

};

td_state_t cur_dance(qk_tap_dance_state_t *state);

void layer_finished(qk_tap_dance_state_t *state, void *user_data);
void layer_reset(qk_tap_dance_state_t *state, void *user_data);

// For the x tap dance. Put it here so it can be used in any keymap
// void x_finished(qk_tap_dance_state_t *state, void *user_data);
// void x_reset(qk_tap_dance_state_t *state, void *user_data);

// Have double tap TO1_TO3

// right pinkie 1U-D: Tab - move the /? to symbols layer
// right thumb : LCTR(TO(2))
// right thumb 1U-L: TO1_TO3 // thonk more about TO for the thumbs TO2_TO3?

// Put shift on left thumb
// left thumb 1u-R - MO(4) and tap is ENTER
// 


// LAYERS GO HERE 
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT_split_3x5_2(TD(Q_ESC), KC_C, KC_H, KC_P, KC_V, KC_K, KC_Y, KC_O, TD(J_CAPS), KC_BSPC, KC_R, KC_S, KC_N, KC_T, KC_G, KC_W, KC_U, KC_E, KC_A, KC_I, KC_X, LALT_T(KC_M), KC_L, KC_D, KC_B, KC_Z, KC_F, KC_COMM, KC_DOT, LGUI_T(KC_SLSH), MEH_T(KC_SPC), LSFT_T(KC_ENT), LCTL_T(KC_DEL), TD(TO1_TO2_TO3)),
	[1] = LAYOUT_split_3x5_2(KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_BSPC, KC_0, KC_LBRC, KC_LCBR, KC_LPRN, KC_NO, KC_QUOT, KC_UNDS, KC_PLUS, KC_GRV, KC_SCLN, KC_NUBS, LALT_T(KC_RBRC), KC_RCBR, KC_RPRN, KC_NO, KC_DQUO, KC_MINS, KC_EQL, KC_BSLS, KC_LGUI, TO(0), LSFT_T(KC_ENT), LCTL_T(KC_DEL), TD(TO1_TO2_TO3)),
	[2] = LAYOUT_split_3x5_2(KC_ESC, KC_WH_L, KC_MS_U, KC_WH_R, KC_WH_U, KC_PGUP, KC_HOME, KC_UP, KC_END, KC_BSPC, KC_TAB, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D, KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_NO, KC_LALT, KC_MS_R, KC_MS_L, KC_NO, KC_NO, KC_ACL0, KC_ACL1, KC_ACL2, KC_LGUI, TO(0), LSFT_T(KC_ENT), LCTL_T(KC_DEL), TD(TO1_TO2_TO3)),
	[3] = LAYOUT_split_3x5_2(KC_ESC, KC_BRIU, KC_VOLU, KC_MNXT, KC_MPLY, KC_F12, KC_F7, KC_F8, KC_F9, KC_BSPC, KC_TAB, KC_BRID, KC_VOLD, KC_MPRV, KC_MSTP, KC_F11, KC_F4, KC_F5, KC_F6, KC_NO, KC_NO, KC_LALT, KC_MUTE, KC_NO, KC_NO, KC_F10, KC_F1, KC_F2, KC_F3, KC_LGUI, TO(0), LSFT_T(KC_ENT), LCTL_T(KC_DEL), TD(TO1_TO2_TO3))
};

td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// BELOW IS AN EXAMPLE OF TAPDANCE FOR MULTIPLE OPTIONS:
// Tap
// Hold
// TapTap
// TapHold

// Create an instance of 'td_tap_t' for the 'x' tap dance.
// static td_tap_t xtap_state = {
//     .is_press_action = true,
//     .state = TD_NONE
// };

// void x_finished(qk_tap_dance_state_t *state, void *user_data) {
//     xtap_state.state = cur_dance(state);
//     switch (xtap_state.state) {
//         case TD_SINGLE_TAP: register_code(KC_X); break;
//         case TD_SINGLE_HOLD: register_code(KC_Y); break;
//         case TD_DOUBLE_TAP: register_code(KC_Z); break;
//         case TD_DOUBLE_HOLD: register_code(KC_A); break;
//         // Last case is for fast typing. Assuming your key is `f`:
//         // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
//         // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
//         case TD_DOUBLE_SINGLE_TAP: tap_code(KC_X); register_code(KC_X); break;
//         default: break;
//     }
// }

// void x_reset(qk_tap_dance_state_t *state, void *user_data) {
//     switch (xtap_state.state) {
//         case TD_SINGLE_TAP: unregister_code(KC_X); break;
//         case TD_SINGLE_HOLD: unregister_code(KC_Y); break;
//         case TD_DOUBLE_TAP: unregister_code(KC_Z); break;
//         case TD_DOUBLE_HOLD: unregister_code(KC_A); break;
//         case TD_DOUBLE_SINGLE_TAP: unregister_code(KC_X); break;
//         default: break;
//     }
//     xtap_state.state = TD_NONE;
// }

// qk_tap_dance_action_t tap_dance_actions[] = {
//     [X_Y_Z_A] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset)
// };

static td_tap_t layer_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void layer_finished(qk_tap_dance_state_t *state, void *user_data) {
    layer_tap_state.state = cur_dance(state);
    switch (layer_tap_state.state) {
        case TD_SINGLE_TAP:
            layer_on(1);
            break;
        case TD_DOUBLE_TAP:
            layer_on(2);
            break;
        case TD_TRIPLE_TAP:
            layer_on(3);
            break;
        default:
            break;
    }
}

void layer_reset(qk_tap_dance_state_t *state, void *user_data) {
    layer_tap_state.state = TD_NONE;
}

// Associate our tap dance key with its functionality
qk_tap_dance_action_t tap_dance_actions[] = {
    [TO1_TO2_TO3] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, layer_finished, layer_reset),
    [J_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_J, KC_CAPS),
    [Q_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_Q, KC_ESC),
};

// Set a long-ish tapping term for tap-dance keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 275;
        default:
            return TAPPING_TERM;
    }
}