/*
 * Invader (c) 2019 Kavawuvi
 *
 * This program is free software under the GNU General Public License v3.0. See LICENSE for more information.
 */

#ifndef INVADER__TAG__HEK__CLASS__UI_WIDGET_DEFINITION_HPP
#define INVADER__TAG__HEK__CLASS__UI_WIDGET_DEFINITION_HPP

#include "hud_interface_types.hpp"

namespace Invader::HEK {
    enum UIGameDataInputReferenceFunction : TagEnum {
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_NULL,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_PLAYER_SETTINGS_MENU_UPDATE_DESC,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_UNUSED,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_PLAYLIST_SETTINGS_MENU_UPDATE_DESC,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_GAMETYPE_SELECT_MENU_UPDATE_DESC,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MULTIPLAYER_TYPE_MENU_UPDATE_DESC,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_SOLO_LEVEL_SELECT_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_DIFFICULTY_MENU_UPDATE_DESC,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_BUILD_NUMBER_TEXTBOX_ONLY,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_SERVER_LIST_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_NETWORK_PREGAME_STATUS_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_SPLITSCREEN_PREGAME_STATUS_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_NET_SPLITSCREEN_PREJOIN_PLAYERS,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_PROFILE_LIST_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION__3WIDE_PLAYER_PROFILE_LIST_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_PLYR_PROF_EDIT_SELECT_MENU_UPD8,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_PLAYER_PROFILE_SMALL_MENU_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_GAME_SETTINGS_LISTS_TEXT_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_SOLO_GAME_OBJECTIVE_TEXT,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_COLOR_PICKER_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_GAME_SETTINGS_LISTS_PIC_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MAIN_MENU_FAKE_ANIMATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_LEVEL_SELECT_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_GET_ACTIVE_PLYR_PROFILE_NAME,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_GET_EDIT_PLYR_PROFILE_NAME,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_GET_EDIT_GAME_SETTINGS_NAME,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_GET_ACTIVE_PLYR_PROFILE_COLOR,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_SET_TEXTBOX_MAP_NAME,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_SET_TEXTBOX_GAME_RULESET,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_SET_TEXTBOX_TEAMS_NOTEAMS,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_SET_TEXTBOX_SCORE_LIMIT,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_SET_TEXTBOX_SCORE_LIMIT_TYPE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_SET_BITMAP_FOR_MAP,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_SET_BITMAP_FOR_RULESET,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_SET_TEXTBOX,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_EDIT_PROFILE_SET_RULE_TEXT,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_SYSTEM_LINK_STATUS_CHECK,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_GAME_DIRECTIONS,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_TEAMS_NO_TEAMS_BITMAP_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_WARN_IF_DIFF_WILL_NUKE_SAVED_GAME,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_DIM_IF_NO_NET_CABLE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_PAUSE_GAME_SET_TEXTBOX_INVERTED,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_DIM_UNLESS_TWO_CONTROLLERS,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_CONTROLS_UPDATE_MENU,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_VIDEO_MENU_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_GAMESPY_SCREEN_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_COMMON_BUTTON_BAR_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_GAMEPAD_UPDATE_MENU,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_SERVER_SETTINGS_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_AUDIO_MENU_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_PROF_VEHICLES_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_SOLO_MAP_LIST_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_MP_MAP_LIST_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_GT_SELECT_LIST_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_GT_EDIT_LIST_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_LOAD_GAME_LIST_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_CHECKING_FOR_UPDATES,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_DIRECT_IP_CONNECT_UPDATE,
        UI_GAME_DATA_INPUT_REFERENCE_FUNCTION_NETWORK_SETTINGS_UPDATE,
    };
    enum UIEventType : TagEnum {
        UI_EVENT_TYPE_A_BUTTON,
        UI_EVENT_TYPE_B_BUTTON,
        UI_EVENT_TYPE_X_BUTTON,
        UI_EVENT_TYPE_Y_BUTTON,
        UI_EVENT_TYPE_BLACK_BUTTON,
        UI_EVENT_TYPE_WHITE_BUTTON,
        UI_EVENT_TYPE_LEFT_TRIGGER,
        UI_EVENT_TYPE_RIGHT_TRIGGER,
        UI_EVENT_TYPE_DPAD_UP,
        UI_EVENT_TYPE_DPAD_DOWN,
        UI_EVENT_TYPE_DPAD_LEFT,
        UI_EVENT_TYPE_DPAD_RIGHT,
        UI_EVENT_TYPE_START_BUTTON,
        UI_EVENT_TYPE_BACK_BUTTON,
        UI_EVENT_TYPE_LEFT_THUMB,
        UI_EVENT_TYPE_RIGHT_THUMB,
        UI_EVENT_TYPE_LEFT_ANALOG_STICK_UP,
        UI_EVENT_TYPE_LEFT_ANALOG_STICK_DOWN,
        UI_EVENT_TYPE_LEFT_ANALOG_STICK_LEFT,
        UI_EVENT_TYPE_LEFT_ANALOG_STICK_RIGHT,
        UI_EVENT_TYPE_LEFT_ANALOG_STICK_UP_1,
        UI_EVENT_TYPE_RIGHT_ANALOG_STICK_DOWN,
        UI_EVENT_TYPE_RIGHT_ANALOG_STICK_LEFT,
        UI_EVENT_TYPE_RIGHT_ANALOG_STICK_RIGHT,
        UI_EVENT_TYPE_CREATED,
        UI_EVENT_TYPE_DELETED,
        UI_EVENT_TYPE_GET_FOCUS,
        UI_EVENT_TYPE_LOSE_FOCUS,
        UI_EVENT_TYPE_LEFT_MOUSE,
        UI_EVENT_TYPE_MIDDLE_MOUSE,
        UI_EVENT_TYPE_RIGHT_MOUSE,
        UI_EVENT_TYPE_DOUBLE_CLICK,
        UI_EVENT_TYPE_CUSTOM_ACTIVATION,
        UI_EVENT_TYPE_POST_RENDER
    };
    enum UIEventHandlerReferenceFunction : TagEnum {
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NULL,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_LIST_GOTO_NEXT_ITEM,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_LIST_GOTO_PREVIOUS_ITEM,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_UNUSED,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_UNUSED_1,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_INITIALIZE_SP_LEVEL_LIST_SOLO,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_INITIALIZE_SP_LEVEL_LIST_COOP,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_DISPOSE_SP_LEVEL_LIST,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SOLO_LEVEL_SET_MAP,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SET_DIFFICULTY,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_START_NEW_GAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PAUSE_GAME_RESTART_AT_CHECKPOINT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PAUSE_GAME_RESTART_LEVEL,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PAUSE_GAME_RETURN_TO_MAIN_MENU,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CLEAR_MULTIPLAYER_PLAYER_JOINS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_JOIN_CONTROLLER_TO_MP_GAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_INITIALIZE_NET_GAME_SERVER_LIST,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_START_NETWORK_GAME_SERVER,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_DISPOSE_NET_GAME_SERVER_LIST,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SHUTDOWN_NETWORK_GAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NET_GAME_JOIN_FROM_SERVER_LIST,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SPLIT_SCREEN_GAME_INITIALIZE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_COOP_GAME_INITIALIZE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MAIN_MENU_INTIALIZE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_TYPE_MENU_INITIALIZE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PICK_PLAY_STAGE_FOR_QUICK_START,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_LEVEL_LIST_INITIALIZE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_LEVEL_LIST_DISPOSE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_LEVEL_SELECT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILES_LIST_INITIALIZE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILES_LIST_DISPOSE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_SET_FOR_GAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SWAP_PLAYER_TEAM,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NET_GAME_JOIN_PLAYER,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLAYER_PROFILE_LIST_INITIALIZE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLAYER_PROFILE_LIST_DISPOSE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION__3WIDE_PLYR_PROF_SET_FOR_GAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION__1WIDE_PLYR_PROF_SET_FOR_GAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_BEGIN_EDITING,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_END_EDITING,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_SET_GAME_ENGINE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_CHANGE_NAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_SET_CTF_RULES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_SET_KOTH_RULES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_SET_SLAYER_RULES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_SET_ODDBALL_RULES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_SET_RACING_RULES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_SET_PLAYER_OPTIONS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_SET_ITEM_OPTIONS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_SET_INDICATOR_OPTS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_INIT_GAME_ENGINE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_INIT_NAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_INIT_CTF_RULES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_INIT_KOTH_RULES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_INIT_SLAYER_RULES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_INIT_ODDBALL_RULES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_INIT_RACING_RULES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_INIT_PLAYER_OPTS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_INIT_ITEM_OPTIONS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_INIT_INDICATOR_OPTS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROFILE_SAVE_CHANGES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_COLOR_PICKER_MENU_INITIALIZE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_COLOR_PICKER_MENU_DISPOSE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_COLOR_PICKER_SELECT_COLOR,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLAYER_PROFILE_BEGIN_EDITING,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLAYER_PROFILE_END_EDITING,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLAYER_PROFILE_CHANGE_NAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLAYER_PROFILE_SAVE_CHANGES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLYR_PRF_INIT_CNTL_SETTINGS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLYR_PRF_INIT_ADV_CNTL_SET,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLYR_PRF_SAVE_CNTL_SETTINGS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLYR_PRF_SAVE_ADV_CNTL_SET,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_GAME_PLAYER_QUIT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MAIN_MENU_SWITCH_TO_SOLO_GAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_REQUEST_DEL_PLAYER_PROFILE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_REQUEST_DEL_PLAYLIST_PROFILE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_FINAL_DEL_PLAYER_PROFILE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_FINAL_DEL_PLAYLIST_PROFILE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CANCEL_PROFILE_DELETE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CREATE_EDIT_PLAYLIST_PROFILE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CREATE_EDIT_PLAYER_PROFILE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NET_GAME_SPEED_START,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NET_GAME_DELAY_START,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NET_SERVER_ACCEPT_CONX,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NET_SERVER_DEFER_START,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NET_SERVER_ALLOW_START,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_DISABLE_IF_NO_XDEMOS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_RUN_XDEMOS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SP_RESET_CONTROLLER_CHOICES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SP_SET_P1_CONTROLLER_CHOICE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SP_SET_P2_CONTROLLER_CHOICE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_ERROR_IF_NO_NETWORK_CONNECTION,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_START_SERVER_IF_NONE_ADVERTISED,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NET_GAME_UNJOIN_PLAYER,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CLOSE_IF_NOT_EDITING_PROFILE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_EXIT_TO_XBOX_DASHBOARD,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NEW_CAMPAIGN_CHOSEN,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NEW_CAMPAIGN_DECISION,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_POP_HISTORY_STACK_ONCE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_DIFFICULTY_MENU_INIT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_BEGIN_MUSIC_FADE_OUT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NEW_GAME_IF_NO_PLYR_PROFILES,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_EXIT_GRACEFULLY_TO_XBOX_DASHBOARD,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PAUSE_GAME_INVERT_PITCH,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_START_NEW_COOP_GAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PAUSE_GAME_INVERT_SPINNER_GET,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PAUSE_GAME_INVERT_SPINNER_SET,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MAIN_MENU_QUIT_GAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MOUSE__EMIT_ACCEPT_EVENT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MOUSE__EMIT_BACK_EVENT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MOUSE__EMIT_DPAD_LEFT_EVENT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MOUSE__EMIT_DPAD_RIGHT_EVENT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MOUSE_SPINNER_3WIDE_CLICK,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CONTROLS_SCREEN_INIT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_VIDEO_SCREEN_INIT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CONTROLS_BEGIN_BINDING,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMESPY_SCREEN_INIT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMESPY_SCREEN_DISPOSE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMESPY_SELECT_HEADER,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMESPY_SELECT_ITEM,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMESPY_SELECT_BUTTON,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLR_PROF_INIT_MOUSE_SET,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLR_PROF_CHANGE_MOUSE_SET,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLR_PROF_INIT_AUDIO_SET,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLR_PROF_CHANGE_AUDIO_SET,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLR_PROF_CHANGE_VIDEO_SET,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CONTROLS_SCREEN_DISPOSE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CONTROLS_SCREEN_CHANGE_SET,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MOUSE__EMIT_X_EVENT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMEPAD_SCREEN_INIT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMEPAD_SCREEN_DISPOSE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMEPAD_SCREEN_CHANGE_GAMEPADS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMEPAD_SCREEN_SELECT_ITEM,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MOUSE_SCREEN_DEFAULTS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_AUDIO_SCREEN_DEFAULTS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_VIDEO_SCREEN_DEFAULTS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CONTROLS_SCREEN_DEFAULTS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PROFILE_SET_EDIT_BEGIN,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PROFILE_MANAGER_DELETE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PROFILE_MANAGER_SELECT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMESPY_DISMISS_ERROR,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SERVER_SETTINGS_INIT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SS_EDIT_SERVER_NAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SS_EDIT_SERVER_PASSWORD,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SS_START_GAME,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_VIDEO_TEST_DIALOG_INIT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_VIDEO_TEST_DIALOG_DISPOSE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_VIDEO_TEST_DIALOG_ACCEPT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMESPY_DISMISS_FILTERS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMESPY_UPDATE_FILTER_SETTINGS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMESPY_BACK_HANDLER,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MOUSE_SPINNER_1WIDE_CLICK,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CONTROLS_BACK_HANDLER,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CONTROLS_ADVANCED_LAUNCH,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CONTROLS_ADVANCED_OK,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PAUSE_MENU_OPEN,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_GAME_OPTIONS_OPEN,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_CHOOSE_TEAM,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROF_INIT_VEHICLE_OPTIONS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROF_SAVE_VEHICLE_OPTIONS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SINGLE_PREV_CL_ITEM_ACTIVATED,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROF_INIT_TEAMPLAY_OPTIONS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_PROF_SAVE_TEAMPLAY_OPTIONS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_GAME_OPTIONS_CHOOSE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_EMIT_CUSTOM_ACTIVATION_EVENT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLR_PROF_CANCEL_AUDIO_SET,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLR_PROF_INIT_NETWORK_OPTIONS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_PLR_PROF_SAVE_NETWORK_OPTIONS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CREDITS_POST_RENDER,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_DIFFICULTY_ITEM_SELECT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CREDITS_INITIALIZE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CREDITS_DISPOSE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_GAMESPY_GET_PATCH,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_VIDEO_SCREEN_DISPOSE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CAMPAIGN_MENU_INIT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CAMPAIGN_MENU_CONTINUE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_LOAD_GAME_MENU_INIT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_LOAD_GAME_MENU_DISPOSE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_LOAD_GAME_MENU_ACTIVATED,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_SOLO_MENU_SAVE_CHECKPOINT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_MP_TYPE_SET_MODE,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CHECKING_FOR_UPDATES_OK,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_CHECKING_FOR_UPDATES_DISMISS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_DIRECT_IP_CONNECT_INIT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_DIRECT_IP_CONNECT_GO,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_DIRECT_IP_EDIT_FIELD,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NETWORK_SETTINGS_EDIT_A_PORT,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_NETWORK_SETTINGS_DEFAULTS,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_LOAD_GAME_MENU_DELETE_REQUEST,
        UI_EVENT_HANDLER_REFERENCE_FUNCTION_LOAD_GAME_MENU_DELETE_FINISH
    };
    enum UIReplaceFunction : TagEnum {
        UI_REPLACE_FUNCTION_NULL,
        UI_REPLACE_FUNCTION_WIDGET_S_CONTROLLER,
        UI_REPLACE_FUNCTION_BUILD_NUMBER,
        UI_REPLACE_FUNCTION_PID
    };
    enum UIWidgetType : TagEnum {
        UI_WIDGET_TYPE_CONTAINER,
        UI_WIDGET_TYPE_TEXT_BOX,
        UI_WIDGET_TYPE_SPINNER_LIST,
        UI_WIDGET_TYPE_COLUMN_LIST,
        UI_WIDGET_TYPE_GAME_MODEL_NOT_IMPLEMENTED,
        UI_WIDGET_TYPE_MOVIE_NOT_IMPLEMENTED,
        UI_WIDGET_TYPE_CUSTOM_NOT_IMPLEMENTED
    };
    enum UIControllerIndex : TagEnum {
        UI_CONTROLLER_INDEX_PLAYER_1,
        UI_CONTROLLER_INDEX_PLAYER_2,
        UI_CONTROLLER_INDEX_PLAYER_3,
        UI_CONTROLLER_INDEX_PLAYER_4,
        UI_CONTROLLER_INDEX_ANY_PLAYER
    };
    enum UIJustification : TagEnum {
        UI_JUSTIFICATION_LEFT_JUSTIFY,
        UI_JUSTIFICATION_RIGHT_JUSTIFY,
        UI_JUSTIFICATION_CENTER_JUSTIFY
    };

    ENDIAN_TEMPLATE(EndianType) struct GameDataInputReference {
        EndianType<UIGameDataInputReferenceFunction> function;
        PAD(0x2);
        PAD(0x20);

        ENDIAN_TEMPLATE(NewType) operator GameDataInputReference<NewType>() const noexcept {
            GameDataInputReference<NewType> copy = {};
            COPY_THIS(function);
            return copy;
        }
    };
    static_assert(sizeof(GameDataInputReference<BigEndian>) == 0x24);

    struct EventHandlerReferencesFlags {
        std::uint32_t close_current_widget : 1;
        std::uint32_t close_other_widget : 1;
        std::uint32_t close_all_widgets : 1;
        std::uint32_t open_widget : 1;
        std::uint32_t reload_self : 1;
        std::uint32_t reload_other_widget : 1;
        std::uint32_t give_focus_to_widget : 1;
        std::uint32_t run_function : 1;
        std::uint32_t replace_self_w_widget : 1;
        std::uint32_t go_back_to_previous_widget : 1;
        std::uint32_t run_scenario_script : 1;
        std::uint32_t try_to_branch_on_failure : 1;
    };

    ENDIAN_TEMPLATE(EndianType) struct EventHandlerReference {
        EndianType<EventHandlerReferencesFlags> flags;
        EndianType<UIEventType> event_type;
        EndianType<UIEventHandlerReferenceFunction> function;
        TagDependency<EndianType> widget_tag; // ui_widget_definition
        TagDependency<EndianType> sound_effect; //sound
        TagString script;

        ENDIAN_TEMPLATE(NewType) operator EventHandlerReference<NewType>() const noexcept {
            EventHandlerReference<NewType> copy = {};
            COPY_THIS(flags);
            COPY_THIS(event_type);
            COPY_THIS(function);
            COPY_THIS(widget_tag);
            COPY_THIS(sound_effect);
            COPY_THIS(script);
            return copy;
        }
    };
    static_assert(sizeof(EventHandlerReference<BigEndian>) == 0x48);

    ENDIAN_TEMPLATE(EndianType) struct SearchAndReplaceReference {
        TagString search_string;
        EndianType<UIReplaceFunction> replace_function;

        ENDIAN_TEMPLATE(NewType) operator SearchAndReplaceReference<NewType>() const noexcept {
            SearchAndReplaceReference<NewType> copy;
            COPY_THIS(search_string);
            COPY_THIS(replace_function);
            return copy;
        }
    };
    static_assert(sizeof(SearchAndReplaceReference<BigEndian>) == 0x22);

    struct ConditionalWidgetReferenceFlags {
        std::uint32_t load_if_event_handler_function_fails : 1;
    };

    ENDIAN_TEMPLATE(EndianType) struct ConditionalWidgetReference {
        TagDependency<EndianType> widget_tag; // ui_widget_definition
        TagString name;
        EndianType<ConditionalWidgetReferenceFlags> flags;
        EndianType<std::int16_t> custom_controller_index;
        PAD(0x1A);

        ENDIAN_TEMPLATE(NewType) operator ConditionalWidgetReference<NewType>() const noexcept {
            ConditionalWidgetReference<NewType> copy = {};
            COPY_THIS(widget_tag);
            COPY_THIS(name);
            COPY_THIS(flags);
            COPY_THIS(custom_controller_index);
            return copy;
        }
    };
    static_assert(sizeof(ConditionalWidgetReference<BigEndian>) == 0x50);

    struct ChildWidgetReferenceFlags {
        std::uint32_t use_custom_controller_index : 1;
    };

    ENDIAN_TEMPLATE(EndianType) struct ChildWidgetReference {
        TagDependency<EndianType> widget_tag; // ui_widget_definition
        TagString name;
        EndianType<ChildWidgetReferenceFlags> flags;
        EndianType<std::int16_t> custom_controller_index;
        EndianType<std::int16_t> vertical_offset;
        EndianType<std::int16_t> horizontal_offset;
        PAD(0x16);

        ENDIAN_TEMPLATE(NewType) operator ChildWidgetReference<NewType>() const noexcept {
            ChildWidgetReference<NewType> copy = {};
            COPY_THIS(widget_tag);
            COPY_THIS(name);
            COPY_THIS(flags);
            COPY_THIS(custom_controller_index);
            COPY_THIS(vertical_offset);
            COPY_THIS(horizontal_offset);
            return copy;
        }
    };
    static_assert(sizeof(ChildWidgetReference<BigEndian>) == 0x50);

    struct UIWidgetDefinitionFlags {
        std::uint32_t pass_unhandled_events_to_focused_child : 1;
        std::uint32_t pause_game_time : 1;
        std::uint32_t flash_background_bitmap : 1;
        std::uint32_t dpad_up_down_tabs_thru_children : 1;
        std::uint32_t dpad_left_right_tabs_thru_children : 1;
        std::uint32_t dpad_up_down_tabs_thru_list_items : 1;
        std::uint32_t dpad_left_right_tabs_thru_list_items : 1;
        std::uint32_t dont_focus_a_specific_child_widget : 1;
        std::uint32_t pass_unhandled_events_to_all_children : 1;
        std::uint32_t render_regardless_of_controller_index : 1;
        std::uint32_t pass_handled_events_to_all_children : 1;
        std::uint32_t return_to_main_menu_if_no_history : 1;
        std::uint32_t always_use_tag_controller_index : 1;
        std::uint32_t always_use_nifty_render_fx : 1;
        std::uint32_t don_t_push_history : 1;
        std::uint32_t force_handle_mouse : 1;
    };

    struct UIWidgetDefinitionFlags1 {
        std::uint32_t editable : 1;
        std::uint32_t password : 1;
        std::uint32_t flashing : 1;
        std::uint32_t don_t_do_that_weird_focus_test : 1;
    };

    struct UIWidgetDefinitionFlags2 {
        std::uint32_t list_items_generated_in_code : 1;
        std::uint32_t list_items_from_string_list_tag : 1;
        std::uint32_t list_items_only_one_tooltip : 1;
        std::uint32_t list_single_preview_no_scroll : 1;
    };

    ENDIAN_TEMPLATE(EndianType) struct UIWidgetDefinition {
        EndianType<UIWidgetType> widget_type;
        EndianType<UIControllerIndex> controller_index;
        TagString name;
        Rectangle2D<EndianType> bounds;
        EndianType<UIWidgetDefinitionFlags> flags;
        EndianType<std::int32_t> milliseconds_to_auto_close;
        EndianType<std::int32_t> milliseconds_auto_close_fade_time;
        TagDependency<EndianType> background_bitmap; // bitmap
        TagReflexive<EndianType, GameDataInputReference> game_data_inputs;
        TagReflexive<EndianType, EventHandlerReference> event_handlers;
        TagReflexive<EndianType, SearchAndReplaceReference> search_and_replace_functions;
        PAD(0x80);
        TagDependency<EndianType> text_label_unicode_strings_list; //unicode_string_list
        TagDependency<EndianType> text_font; // font
        ColorARGB<EndianType> text_color;
        EndianType<UIJustification> justification;
        EndianType<UIWidgetDefinitionFlags1> flags_1;
        PAD(0xC);
        EndianType<std::int16_t> string_list_index;
        EndianType<std::int16_t> horiz_offset;
        EndianType<std::int16_t> vert_offset;
        PAD(0x1A);
        PAD(0x2);
        EndianType<UIWidgetDefinitionFlags2> flags_2;
        TagDependency<EndianType> list_header_bitmap; // bitmap
        TagDependency<EndianType> list_footer_bitmap; // bitmap
        Rectangle2D<EndianType> header_bounds;
        Rectangle2D<EndianType> footer_bounds;
        PAD(0x20);
        TagDependency<EndianType> extended_description_widget; // ui_widget_definition
        PAD(0x20);
        PAD(0x100);
        TagReflexive<EndianType, ConditionalWidgetReference> conditional_widgets;
        PAD(0x80);
        PAD(0x80);
        TagReflexive<EndianType, ChildWidgetReference> child_widgets;

        ENDIAN_TEMPLATE(NewType) operator UIWidgetDefinition<NewType>() const noexcept {
            UIWidgetDefinition<NewType> copy = {};
            COPY_THIS(widget_type);
            COPY_THIS(controller_index);
            COPY_THIS(name);
            COPY_THIS(bounds);
            COPY_THIS(flags);
            COPY_THIS(milliseconds_to_auto_close);
            COPY_THIS(milliseconds_auto_close_fade_time);
            COPY_THIS(background_bitmap);
            COPY_THIS(game_data_inputs);
            COPY_THIS(event_handlers);
            COPY_THIS(search_and_replace_functions);
            COPY_THIS(text_label_unicode_strings_list);
            COPY_THIS(text_font);
            COPY_THIS(text_color);
            COPY_THIS(justification);
            COPY_THIS(flags_1);
            COPY_THIS(string_list_index);
            COPY_THIS(horiz_offset);
            COPY_THIS(vert_offset);
            COPY_THIS(flags_2);
            COPY_THIS(list_header_bitmap);
            COPY_THIS(list_footer_bitmap);
            COPY_THIS(header_bounds);
            COPY_THIS(footer_bounds);
            COPY_THIS(extended_description_widget);
            COPY_THIS(conditional_widgets);
            COPY_THIS(child_widgets);
            return copy;
        }
    };
    static_assert(sizeof(UIWidgetDefinition<BigEndian>) == 0x3EC);

    void compile_ui_widget_definition_tag(CompiledTag &compiled, const std::byte *data, std::size_t size);
}
#endif
