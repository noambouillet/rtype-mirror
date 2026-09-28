# R-Type: How the Game Works

A reference on the game itself: its rules, its entities (players, enemies, projectiles, bonuses), its levels and its bosses. It describes the 1987 arcade game and the gameplay elements the project subject asks for.

> Tags: **[Original]** = a feature of the 1987 arcade game. **[Required]** = a gameplay requirement from the project subject.

---

## 1. The game in a nutshell

R-Type (Irem, 1987) is a **horizontal shoot'em up**. The player pilots the **R-9 "Arrowhead"** fighter into the Bydo Empire, a race of bio-mechanical creatures (a design style inspired by H.R. Giger). The screen scrolls automatically to the right while the player dodges terrain, enemies and bullets and shoots back.

What defines the game:

- **Difficulty based on memorization**: enemies always come out in the same patterns.
- **Punishing death**: the player restarts from a checkpoint and loses all upgrades.
- **The Force Pod and the charged beam**, which give the game its tactical depth.
- **Terrain as a threat**: walls, ceilings and destructible tiles are part of the level design.
- **One boss per stage**, each with a specific weak point.
- **Eight stages**, then a second, harder loop.

---

## 2. Gameplay elements required by the project

| Element | Description |
|---|---|
| Starfield | Slow horizontal scrolling background with stars, planets… [Required] |
| Player movement | Arrow keys [Required] |
| Players | Up to four per game, each one distinguishable (color, sprite…) [Required] |
| Enemies | Bydo "slaves" [Required] |
| Missiles | Fired by players and by enemies [Required] |
| Spawning | Monsters appear randomly on the right of the screen [Required] |
| Time | Scrolling, enemy behavior and overall time flow do not depend on CPU speed [Required] |
| Sound | Basic sound design (music and effects) |

The official screen layout contains: the player (1), a monster (2), a monster that spawns a power-up on death (3), an enemy missile (4), a player missile (5), stage obstacles (6), destructible tiles (7) and the starfield background (8).

---

## 3. The player

### 3.1 The ship

The R-9 moves in four directions (and diagonals) within the screen. It has a small hitbox compared with the screen, and any contact with an enemy, an enemy bullet or the terrain is fatal [Original].

### 3.2 Normal shot

Pressing the fire button fires a small projectile going to the right. The rate of fire is limited, and holding the button does not fire faster.

### 3.3 Charged beam (Wave Cannon) [Original]

Holding the fire button fills a charge gauge. Releasing it fires a powerful beam.

- Three charge levels exist in the original game.
- A fully charged beam does much more damage and can hit several enemies at once.
- The trade-off: charging takes time, during which the player is exposed.

### 3.4 Death and checkpoints [Original]

When the player dies, they restart from a checkpoint with no weapon upgrades, no Force Pod, no Bits and no speed-ups. Recovering from a checkpoint with a bare ship is a core part of the challenge.

### 3.5 Multiplayer identity [Required]

Each of the four players has a distinct look, such as a different color or sprite, so everyone can tell who is who on screen.

---

## 4. The Force Pod and the Bits [Original]

### 4.1 Force Pod

An **invincible orb** obtained through a bonus. It is one of the most important elements of the game.

It can:

- **attach to the front** of the ship, adding firepower forward and shielding the nose,
- **attach to the back**, in which case it shoots backwards,
- be **detached** and fly independently, ahead of or behind the ship,
- be **recalled** to the ship,
- **block enemy bullets** while attached,
- **damage enemies on contact**,
- improve its firepower with further weapon bonuses (it can be leveled up twice).

Several levels are designed around it: for example, stage 3 is a long fight against a giant warship where the Force is needed to get past its defenses. On some bosses, the Force can be launched into the weak point and left there to deal continuous damage:

- **Dobkeratops** (stage 1): the Force can be lodged in its weak point.
- **Gomander** (stage 2): the Force can be placed on the globe that periodically appears on its top.

### 4.2 Bits

Bits are small invincible orbs that accompany the ship. They mostly stay in place relative to it, absorb enemy fire and destroy small enemies.

---

## 5. Enemies

### 5.1 Categories

| Category | Behavior |
|---|---|
| **Basic Bydos (slaves)** [Required] | Simple enemies: they fly in straight lines or waves, and may shoot |
| **Wave enemies** | Groups that arrive one after another along the same path |
| **Turrets** | Fixed to the ground or ceiling, firing at the player |
| **Power-up carriers (POW Armor)** [Original] | Float or hop over the scenery and release a power-up when destroyed |
| **Orb clusters** [Original] | Several orbs rotating around a central "master" orb; destroying the master destroys them all |
| **Snake-like enemies** | Chains of segments moving along a path |
| **Bosses** | One per stage, with specific weak points (see section 8) |

### 5.2 Movement patterns seen in the game

- straight line toward the left,
- sine wave (up and down while advancing),
- diagonal entries and zigzags,
- curved trajectories along an arc,
- chasing the player,
- staying fixed on the terrain (turrets) and scrolling out with it,
- entering the screen, stopping to shoot, then leaving.

### 5.3 Enemy attacks

- straight shots to the left,
- shots aimed at the player,
- bursts and spreads of several bullets,
- shots at irregular intervals.

### 5.4 Spawning [Required]

Monsters appear randomly at the right edge of the screen, then move across it. In the original game their appearance is scripted so that the same enemies always arrive at the same time; the project asks for random spawns.

---

## 6. Projectiles

| Projectile | Owner | Description |
|---|---|---|
| Player missile | Player | Fast, goes right, low damage |
| Charged beam | Player | High damage, can hit several enemies |
| Force Pod shots | Player | Extra firepower depending on the Force's level |
| Enemy bullet | Enemy | Straight or aimed at a player |
| Boss projectiles | Boss | Special patterns such as waves and rings |

Basic rules:
- A projectile hurts what it belongs against: player shots hurt enemies, enemy shots hurt players.
- A projectile disappears when it hits something or leaves the screen.
- Charged beams keep going after hitting an enemy.
- The Force Pod stops enemy bullets.

---

## 7. Bonuses (power-ups)

### 7.1 How they appear

In the original game, power-ups are released by **POW Armor** carriers when they are destroyed. These enemies float or hop over the scenery. Bonuses are also the third element of the official screen: a monster that spawns a power-up upon death.

### 7.2 Bonus types

| Bonus | Effect |
|---|---|
| Force Pod | Gives the player a Force Pod |
| Weapon upgrade | Improves or changes the ship's weapon (the original has several laser types) |
| Missiles | Adds an extra attack |
| Speed-up | Increases the ship's speed |
| Bits | Adds orbiting invincible orbs |

### 7.3 Rules

- Picking up a bonus is done by touching it with the ship.
- Weapon bonuses can be collected several times to level up the weapon and the Force.
- **All bonuses are lost when the player dies** [Original].

---

## 8. Levels and bosses

### 8.1 The eight stages of the original game

Names vary between versions and ports. The most common ones:

| Stage | Name | Boss | Notes |
|---|---|---|---|
| 1 | Encounter | **Dobkeratops** | A quick flight through space, then the interior of a Bydo ship. Enemies come from above and below, and orb clusters appear |
| 2 | Creature Cave | **Gomander** | An organic cave; the boss is a snake-like creature that enters and leaves shafts |
| 3 | Gigantic Warship | Warship (or its core) | The whole stage is a fight against a huge ship, ending at its bridge |
| 4 | Frontline Base | **Compiler** | A boss made of several parts that separate |
| 5 | The Den | Bellmite / Helmate (varies) | |
| 6 | Transport System | Yashoo (only in some versions) | |
| 7 | City in Ruins | Bronco / Blank (varies) | |
| 8 | The Bydo Empire's Star | Bydo (core) | Final boss |

After stage 8, the game restarts with a second loop featuring more aggressive enemies, for 16 stages in total.

### 8.2 How a level unfolds

A level is a fixed sequence:

1. The screen scrolls at a steady pace.
2. Enemies enter at set moments: individual monsters, waves, turrets on the terrain, power-up carriers.
3. Terrain sections force the player to find a safe route: corridors, ceilings and floors, destructible walls.
4. A mid-level checkpoint marks where the player restarts after dying.
5. The level ends with a boss fight.

Level themes differ: open space, bio-mechanical caves, enemy warships, bases, ruined cities, and the enemy's home star. Some levels add a twist, such as the long warship fight in stage 3.

### 8.3 Terrain

- **Solid walls, floors and ceilings**: touching them destroys the ship in the original game.
- **Destructible tiles**: they can be shot to open a path or remove a threat.
- Terrain is a large part of the difficulty, since moving safely through it is often the challenge.

### 8.4 Bosses

Bosses are the highlight of each stage:

- They are large, often composed of several parts.
- They have a **specific weak point** and attack with readable, repeatable patterns.
- They are usually a puzzle as much as a fight: the player has to learn where to stand and when to fire.

Examples:

| Boss | Description |
|---|---|
| **Dobkeratops** | A large red creature with a tail that snaps back and forth. Its weak point is a small green face/mouth in its midsection; two well-aimed beams into it are enough to kill it |
| **Gomander** | A snake-like boss coming out of and back into four shafts; hitting its red segments and its exposed globe damages it |
| **Warship** | The end of the giant warship level; the player must get in position and hit its exposed eye with a charged beam after it releases its projectiles |
| **Compiler** | A boss made of three pieces that start close together and then separate; the weak points can already be hit at the beginning of the fight |
| **Bydo (core)** | The last boss, at the heart of the Bydo Empire |

---

## 9. Collisions and damage

| Contact | Result |
|---|---|
| Player ↔ enemy | Player dies |
| Player ↔ enemy bullet | Player dies |
| Player ↔ terrain | Player dies |
| Player ↔ bonus | Bonus is picked up |
| Player missile ↔ enemy | Enemy takes damage, missile disappears |
| Charged beam ↔ enemy | Enemy takes damage, beam continues |
| Player missile ↔ destructible tile | Tile takes damage |
| Enemy bullet ↔ Force Pod | Bullet is blocked |
| Force Pod ↔ enemy | Enemy takes damage |

Enemies have varying amounts of health: basic Bydos die in one hit, while bosses need many. Destroying an enemy gives points.

---

## 10. Score, lives and game flow

- **Score**: each destroyed enemy gives points, and bosses give more.
- **Lives**: the player has a limited number of lives. Losing one means a return to the checkpoint without upgrades.
- **Game flow**: start, play through a stage, fight the boss, move to the next stage, until the last boss or until all lives are lost.
- **Difficulty**: the second loop after stage 8 brings more aggressive enemies.

---

## 11. Sound and visuals

- **Music**: a distinct theme for each stage and a boss theme.
- **Sound effects**: player shot, charge, charged beam, hits, explosions, bonus pickup, player death.
- **Visual style**: dark, bio-mechanical designs mixing flesh-like organic shapes with cold metal, and an animated starfield behind the action.

---

## 12. References

- Hardcore Gaming 101, R-Type article (level design, enemies, Dobkeratops)
- Arcade History, R-Type (Force Pod, charged beam)
- Shmups Wiki, R-Type and R-Type/Strategy (controls, stage breakdown, boss strategies)
- R-Type fan wikis (stage and boss lists; names vary by version)