# Junkpad — Config Guide

## Structure

```json
{
  "title": "My Config",
  "config": [ /* buttons */ ]
}
```

## Buttons

```json
{
  "id": "BTN_1",
  "label": "Launch OBS",
  "button_type": "momentary",
  "actions": [ /* actions */ ]
}
```

| Field | Description |
|---|---|
| `id` | Unique technical ID. Matches the firmware's physical button mapping — don't rename. |
| `label` | Display name, freely editable. |
| `button_type` | The type of button: `momentary`, `toggle`, or `analog`. |
| `actions` | List of actions to run. |

A `toggle` button needs two actions, one with `"is_on": true` and one with `"is_on": false`. A `momentary` button doesn't use `is_on`.

## Button types

**`momentary`** — push button, fires once per press

**`toggle`** — latching button, has `on`/`off` states

**`analog`** — sends a 0–100 percentage value whenever it changes (potentiometer, slider). The firmware does the normalization, so configs always work with a plain percentage — no raw range to worry about.

## Actions

**`hotkey`** — send a key combo
```json
{ "action_type": "hotkey", "keys": ["ctrl", "shift", "o"] }
```

**`launch_app`** — start a program
```json
{ "action_type": "launch_app", "path": "C:\\Program Files\\obs-studio\\bin\\64bit\\obs64.exe" }
```


**`obs_websocket`** — OBS command (requires OBS WebSocket enabled)
```json
{ "action_type": "obs_websocket", "request": "StartStream" }
```

**`analog_control`** — maps the button's percentage value to a target
```json
{ "action_type": "analog_control", "target": "system_volume" }
```
- **`target`** — what the percentage drives (e.g. `system_volume`, `app_volume`).
- The value is always 0–100, straight from the firmware — no scaling needed in the config.

> New action types must be documented here before merging.

## Examples

**Toggle button (start/stop stream):**
```json
{
  "id": "BTN_5",
  "label": "Streaming Mode",
  "button_type": "toggle",
  "actions": [
    { "is_on": true, "action_type": "obs_websocket", "request": "StartStream" },
    { "is_on": false, "action_type": "obs_websocket", "request": "StopStream" }
  ]
}
```

**Multiple actions on one press:**
```json
{
  "id": "BTN_3",
  "label": "Setup Stream",
  "button_type": "momentary",
  "actions": [
    { "action_type": "launch_app", "path": "C:\\...\\obs64.exe" },
    { "action_type": "hotkey", "keys": ["win", "d"] }
  ]
}
```

**Analog control (volume slider):**
```json
{
  "id": "POT_1",
  "label": "Master Volume",
  "button_type": "analog",
  "actions": [
    { "action_type": "analog_control", "target": "system_volume" }
  ]
}
```

## Common mistakes

- Windows paths need escaped backslashes: `"C:\\Program Files\\..."`
- `toggle` buttons need **both** `is_on: true` and `is_on: false` actions
- `id` must be unique and must match the firmware mapping — don't reuse or rename it
