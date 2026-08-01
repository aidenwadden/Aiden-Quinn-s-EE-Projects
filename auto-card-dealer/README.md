# Auto Card Dealer ("Electric Jacks")

A joystick-controlled automatic card dealing machine built around a single Arduino UNO. It rotates a "Lazy Susan" style platform to each seat and flicks a card out, supporting multiple card games for up to 4 players.

## How it works

- An XY joystick (VRx/VRy/SW) is used to navigate menus and select a game and player count.
- An FS90R continuous-rotation servo spins the Lazy Susan platform to index between seats.
- An SG90 micro servo flicks the card out to the selected seat.
- A 28BYJ-style stepper motor (2048 steps/rev) provides precise seat indexing for up to 5 seats.
- An optional 16x2 I2C LCD displays menus and game status.

## Supported games

- Blackjack (BJ)
- Poker
- Euchre

Each game supports up to 4 players, with the dealing sequence adjusted per game's rules.

## Hardware

- Arduino UNO
- XY joystick module (VRx, VRy, SW)
- FS90R continuous-rotation servo (platform rotation)
- SG90 micro servo (card flicking)
- 28BYJ-style stepper motor (seat indexing)
- 16x2 I2C LCD (optional)

## Files

- `arduino_uno.ino` - full control logic: joystick input, menu system, servo/stepper control, and game-specific dealing sequences. See the wiring diagram in the header comment for pin mappings.
