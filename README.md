# ShooterGrenade_Proto

 - [Run the project](#run-the-project)
 - [Input mapping](#input-mapping)
 - [Main architecture and system configuration](#main-architecture-and-system-configuration)
 - [Problems encountered](#problems-encountered)
 - [Things to improve](#things-to-improve)

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


## Problems encountered


## Things to improve


