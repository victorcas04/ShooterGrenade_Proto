# ShooterGrenade_Proto : Add Frag Grenade to the existing Shooter Game Sample from UE4

 - [Prototype requisites](#prototype-requisites)
 - [Run the project](#run-the-project)
 - [Input mapping](#input-mapping)
 - [Main architecture and system configuration](#main-architecture-and-system-configuration)
 - [Problems encountered](#problems-encountered)
 - [Things to improve](#things-to-improve)
 - [Timeline](#timeline)

## Prototype requisites

 - User Windows 10
 - Latest UE4 version (4.27)
 - When a key is pressed, a Grenade is thrown(No Animation is needed)
 - No need to create a UI (A key press to trigger the action is fine)
 - Upon explosion, the grenade will damage characters around the explosion
 - A detailed, descriptive ReadMe/Document

## Run the project
 1. Double click on the `ShooterGrenade_Proto.uproject` file.
 2. When prompted to rebuild it, click `yes`.

## Input mapping

#### Inventory related

 - **Equip grenade** -> tap `G` when weapon is equipped
 - **Cycle through grenades** -> tag `G` when grenade is equipped
 - **Equip weapon again** -> hold `G` when grenade is equipped

#### Grenade related

 - **Show grenade trajectory until first impact** -> hold `Left Mouse Button` when grenade is equipped
 - **Cancel grenade throw** -> tap `Left Mouse Button` when grenade trajectory is shown
 - **Throw grenade** -> tap `Left Mouse Button` when grenade trajectory is shown and if it was not cancelled

#### Utils

 - **Spawn a bot near you** -> press `'` or `º` to open the command line, then type `spawnbot` and hit `ENTER`

## Main architecture and system configuration

NOTE: all default actors, components and systems already given by unreal's sample are not mentioned here (like the `WeaponConfig` struct or similar)

#### Inventory related

 - `UGrenadeManagerComp` -> component added to the player pawn
    - Manages all the grenade functionality related to the inventory management
    - System configuration
       - `TagComponentSpawnPoint` -> tag of the arrow component in the player pawn where the grenade will spawn
       - `TimeHoldToEquipWeapon` -> time player must hold `G` down to change back to weapons
       - `ArrayGrenadeClasses` -> grenades supported by this component. Those grenades will appear in the pawn's inventory
       - `CanShowTrajectory` -> if true: will shown a basic trajectory until the first hit with anything
       - `ShowTrajectory_BP(bool bShow)` -> implementation in blueprint of the trajectorey to show for faster iterations

 - `AShooterWeapon_Grenade` -> actor used from inventory by the pawn
    - Manages the grenade functionality while used as a weapon (being holded by the pawn).
    Which is mainly the trajectory configuration and weapon related inputs (fire, etc)
    - System configuration
       - `GrenadeClassToSpawn` -> actor class we will spawn when throwing the grenade
       - `TrajectoryData` -> has all the trajectory related information
          - `ThrowForce` -> force that will be added to the grenade
          - `ProjectileRadius` -> radius of the grenade, to check if can go throu holes or similar
          - `ActorsToIgnore` -> array of actors the grenade will go through
          - `ShowTraceDebug` -> if false: trajectory will not be displayed
          - `DelayRecalculateTrajectory` -> Time (in seconds) between each trajectory update
          - `SimFrequency` -> Time (in ms) between each step of the trajectory simulation (lower = better results; higher = better performance)
          - `MaxSimTime` -> maximum trajectory simulation time (in seconds) (if too short, will display only half the grenade path)
          - `Gravity` -> gravity value to apply to the grenade

#### Pure grenade related

- `AGrenade` -> actor spawned when a grenade is thrown (can be seen as the flying grenade)
    - Manages the physical related aspects of the grenade like the visual representation and the time before it explodes
    - System configuration
       - `GrenadeExplosionClassToSpawn` -> actor class we will spawn when the grenade explodes
       - `GrenadeData`
          - `ExplosionDelay` -> Setting this time to 0 means it will not explode automatically after a delay (useful for mines or similar grenades)
          - `DestroyGrenadeAfterExplosionDelay` -> Setting this time to 0 means the grenade will never be destroyed after explosion (useful for flares or similar grenades)
       - `GrenadeMesh` -> visual representation of the grenade thrown
       - `ExplodeGrenade_BP()` -> This is here so other team members can add vfx/sfx on blueprint (note that this is not the explosion related class, 
       so this function should be used for grenade related implementations when it explodes)

- `AGrenadeExplosion` -> actor spawned when the grenade explodes
    - Manages all the explosion related functionality like damage and FX
    - System configuration
       - `GrenadeExplosionData` -> all the explosion related data
          - `bCanDamageOwner` -> true if we can hurt ourselves with this explosion
          - `DamageType` -> unreal default damage config
          - `ExplosionDamage` -> damage caused by the initial explosion
          - `ExplosionRadius` -> radius of the explosion
          - `bExplosionPersists` -> true if we want the explosion to stay as a damaging area for a while
          - `DelayBeforeExplosionEnds` -> Time (in seconds) the explosion will persist (only available when `bExplosionPersists` is true)
          - `PersistDamage` -> damage caused by the persistent explosion area (only available when `bExplosionPersists` is true)
          - `bShowTriggerSphere` -> true if we want to see the trigger for debug
       - `ExplosionVFX_BP()` -> This is here so other team members can add VFX on blueprint for the explosion itself
       - `ExplosionSFX_BP()` -> This is here so other team members can add SFX on blueprint for the explosion itself
       - `ExplosionDamagedActor_BP(AActor* DamagedActor, float DamageApplied)` -> exposed function to blueprints in case any other team member want 
       to do something with it (like playing a sound of a soldier saying you hit someone)

#### Utils

I also needed to make a utility file `GrenadeTestHelpers` for some functions:
 - `GetValidatedComponentByClass(AActor* Target, TSubclassOf<UActorComponent> ComponentClass, GTestEObjectIsValid& OutResult)`
 -> gets the first component from an actor. Also returns Valid or NotValid
 - `GetValidatedComponentByClassWithTag(AActor* Target, TSubclassOf<UActorComponent> ComponentClass, FName Tag, GTestEObjectIsValid& OutResult)`
 -> gets the first component from an actor with a given tag. Also returns Valid or NotValid
 - `ExistsWallBetweenActors(AActor* OriginActor, AActor* TargetActor, ECollisionChannel TraceChannelToCheck = ECollisionChannel::ECC_EngineTraceChannel2)`
 -> checks if there is something between two actors given a collision channel

## Problems encountered

 - Unreal sample - weapon system: I wanted to use as much as possible of the already implemented systems given by unreal, but I though they were gonna be more flexible than they really were, so I ended up doing my own system, leaving the already existing one only for "normal" weapons. It was faster than trying to redesign the whole system.
 - Unreal sample - components: everything is on the character class in this sample. The first thing I was trying to do was to separate things into components or different actors, cause the original sample had everything in the main character class, and that made things harder cause there were a lot of coupling. I tried to make the new grenade system as decoupled as possible, but there were still some things I coudn't take out of there without making a big redesing work.
 - Trajectory: the main idea was to show the full grenade trajectory, with its bounces and all, and the save that trajectory to a variable the actual thrown grenade would use and follow, but it was gonna be way more harder and longer to develop than this new approach with the AddImpulse, and I needed this time to implement the whole system of grenade cycling and make things a little bit smoother.

## Things to improve

 - Trajectory: the trajectory shown while holding the left click button can be improved to include the movement of the grenade after the first bounce. Also, it seems like it has a little bit of an offset, so needs more tunning.
 - Grenade types: currently we have a basic grenade, that bounces and explodes after a while, but originally I wanted to create also a "sticky" grenade, so it would not bounce on walls for example. The architecture supports it and it's only a matter of creating a new type of grenade and add it to the inventory array, but didn't have time to implement that one.
 - FX: the grenade does not spawn any effect or sound when exploding, but the functions where they should be implemented are exposed to blueprints, so any tech artist or designer can do that.
 - Ammo pickups: had no time to implement the proper way to add ammo with current pickups, since the system is meant to be used with already spawned weapons all the time (like the "normal" weapons, which are spawned at the beginning of the game. But grenade weapons are only spawned when equipped, so I would need to rework that part a little bit, but it was not a requisite for the test, so I did not invest so much time in that.

## Timeline

 - Tuesday 17 and Wednesday 18 : **Setup**
   - Download sample project + setup git: 1h
   - What to do brainstorm + some definitions: 1h
   - Investigate unreal sample system + software design options: 4h

 - Thursday 19 : **First architecture**
   - Initial structure with base classes: 4h

 - Friday 20 : **Arch improvements**
   - Keep improving structure + add functionality: 4h

 - Sunday 22 : **Arch improvements**
   - Keep improving structure + add functionality: 4h

 - Monday 23 : **Trajectory**
   - Visual feedback + tune variables in editor: 2h
   - Trajectory: 4h

 - Tuesday 24 : **Finish Trajectory + Explosion**
   - Trajectory: 2h
   - Explosion: 2h
   - Bugfixing: 2h

 - Wednesday 25: **Finish Explosion + Readmen**
   - Explosion: 2h
   - Bugfixing: 2h
   - Readme: 2h

**Total development time** : **36h**
