# Game design

## General Information

CaptureTheBanana++ is a multiplayer PvP game, focused on fast pace combat to allow to carry a flag to the respective endscreen. After spawning in a random level, each team has to get passed the enemy team and conquer multiple rooms, while keeping the flag in their own ranks. Killing enemy players will gain you ground, but take care of them respawning in your way. Fighting only with your fist is not good enough for the game, so you can choose your preferred weapon from a variety spawning in each level. But be beware of the non-player controlled characters trying to interrupt your or your opponents run aswell.

## Goal of the game

The goal of the game is to carry the flag through multiple levels and reach a set goal.

## How to play

The game can be played either with just one players on one team or with multiple players per team. To add a player make sure you that you have enough controllers connected to your computer before starting the game. In the Main Menu every player can choose their desired team, displayed in the top right hand corner. After every player has choosen his team, one player can click the "Start Game"-Button and all player will spawn in a random selected level.

The teams will spawn at both sides of the screen, with a flag placed between them. To distinguish both teams the players shirts will be colored in a different color. The team reaching the flag first, has the opportunity to gain ground, with the other team trying to stop this by killing the flag carrier (Read [Movement and gaining ground](#movement-and-gaining-ground) for more information). When the flag has been dropped each team has the chance to grab it and gain ground for themself.

To kill a player from the opposing team you can either use your fists or pick up a weapon that is being spawned somewhere in the level. But not every weapon is the same! Read [Weapons](#weapons) for more information about their stats.

Concluding the chaos, there is a possibility for Bots to spawn in the Level to hamper the players. Read [Bots](#bots) for a List of all bots and their abilities

## Movement and gaining ground

To make progress in a Level, the player who currently is holding the flag has to fight his way towards his portal to enter the next level while the opposing team is trying to capture the flag by killing the flag carrier. The camera will always be focused on the flag carrier and will follow his movement. When a player leaves the camera area he will die and will be respawned on the side of the camera he is trying to defend, so that a defender will always spawn in front of the attacker.
When the flag is dropped into the void it will be spawned on a specific tile in the level and is up for grabbing by both teams.

## Weapons

| Weapon | Type/Range | Damage | Attack speed | Ammo    | Reload Delay |
| ------ | ---------- | ------ | ------------ | ------- | ------------ |
| Fist   | Melee/1    | 15-25  | 5ms          | inf.    | -            |
| Gun    | Ranged/50  | 8      | 200ms        | 30/180  | 1.5s         |

## Bots

| Bot    | Type/Range | Damage | HP   | Attack speed |
| ------ | ---------- | ------ | ---- | ------------ |
| UFO    | Melee/1    | 12-45  | 1    | -            |
| Zombie | Melee/1    | 10-18  | 1    | -            |

