# Firmware Tests

Unit tests for the Gumtar firmware are run on the host machine using PlatformIO's
native environment (no hardware required for logic tests).

## Running Tests

```bash
pio test -e native
```

## Test Coverage

- `ButtonManager`: debounce logic, justPressed/justReleased edge detection
- `WhammyManager`: rolling average, range mapping, deadzone behaviour
- `ConfigManager`: EEPROM magic byte check, default loading, round-trip save/load

Hardware-dependent tests (HID reports, LED output) are validated by flashing to
the target board and using the web app in status mode.
