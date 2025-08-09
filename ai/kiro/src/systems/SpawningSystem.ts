import { GameScene } from '../scenes/GameScene';
import { Asteroid, AsteroidSize } from '../objects/Asteroid';
import { GameConfig } from '../config/GameConfig';
import { GameStateManager } from '../managers/GameStateManager';

/**
 * SpawningSystem manages asteroid wave spawning, progression, and safe positioning.
 * It handles initial asteroid spawning, wave progression, and ensures asteroids spawn
 * at safe distances from the player ship.
 */
export class SpawningSystem {
    private gameScene: GameScene;
    private gameConfig: GameConfig;
    private gameStateManager: GameStateManager;
    private currentWave: number = 1;
    private isSpawning: boolean = false;
    private spawnDelayTimer: number = 0;
    private readonly SPAWN_DELAY_MS: number = 2000; // 2 second delay between waves

    constructor(gameScene: GameScene, gameConfig: GameConfig, gameStateManager: GameStateManager) {
        this.gameScene = gameScene;
        this.gameConfig = gameConfig;
        this.gameStateManager = gameStateManager;
    }

    /**
     * Initialize the first wave of asteroids at game start
     * Spawns 4-6 large asteroids at safe positions away from the ship
     */
    public initializeFirstWave(): void {
        console.log('Initializing first wave of asteroids...');
        
        const initialCount = this.gameConfig.game.initialAsteroidCount;
        const variationCount = Math.floor(Math.random() * 3); // 0-2 additional asteroids
        const totalAsteroids = initialCount + variationCount; // 4-6 asteroids
        
        this.currentWave = 1;
        this.gameStateManager.setWave(this.currentWave);
        
        this.spawnAsteroids(totalAsteroids, AsteroidSize.LARGE);
        
        console.log(`First wave spawned: ${totalAsteroids} large asteroids`);
    }

    /**
     * Check if a new wave should be spawned and handle wave progression
     * Should be called from the main game update loop
     * @param delta - Time elapsed since last frame in milliseconds
     */
    public update(delta: number): void {
        // Check if we need to spawn a new wave
        if (this.shouldSpawnNewWave()) {
            if (!this.isSpawning) {
                this.startWaveSpawnDelay();
            } else {
                this.updateSpawnDelay(delta);
            }
        }
    }

    /**
     * Check if a new wave should be spawned (all asteroids destroyed)
     * @returns True if new wave should be spawned
     */
    private shouldSpawnNewWave(): boolean {
        const activeAsteroids = this.gameScene.getAsteroids();
        return activeAsteroids.length === 0 && !this.gameStateManager.isGameOver();
    }

    /**
     * Start the delay timer before spawning the next wave
     */
    private startWaveSpawnDelay(): void {
        this.isSpawning = true;
        this.spawnDelayTimer = this.SPAWN_DELAY_MS;
        console.log(`All asteroids destroyed! Next wave in ${this.SPAWN_DELAY_MS / 1000} seconds...`);
    }

    /**
     * Update the spawn delay timer and spawn new wave when ready
     * @param delta - Time elapsed since last frame in milliseconds
     */
    private updateSpawnDelay(delta: number): void {
        this.spawnDelayTimer -= delta;
        
        if (this.spawnDelayTimer <= 0) {
            this.spawnNextWave();
            this.isSpawning = false;
        }
    }

    /**
     * Spawn the next wave with increased difficulty
     */
    private spawnNextWave(): void {
        this.currentWave++;
        this.gameStateManager.setWave(this.currentWave);
        
        // Calculate asteroid count for this wave
        const baseCount = this.gameConfig.game.initialAsteroidCount;
        const waveIncrement = this.gameConfig.game.waveIncrement;
        const additionalAsteroids = (this.currentWave - 1) * waveIncrement;
        const totalAsteroids = baseCount + additionalAsteroids;
        
        console.log(`Spawning wave ${this.currentWave} with ${totalAsteroids} asteroids`);
        
        this.spawnAsteroids(totalAsteroids, AsteroidSize.LARGE);
    }

    /**
     * Spawn a specified number of asteroids at safe positions
     * @param count - Number of asteroids to spawn
     * @param size - Size of asteroids to spawn
     */
    private spawnAsteroids(count: number, size: AsteroidSize): void {
        const spawnedAsteroids: Asteroid[] = [];
        const maxAttempts = 50; // Prevent infinite loops
        
        for (let i = 0; i < count; i++) {
            let attempts = 0;
            let spawnPosition: { x: number; y: number } | null = null;
            
            // Try to find a safe spawn position
            while (attempts < maxAttempts && !spawnPosition) {
                const candidatePosition = this.generateSpawnPosition();
                
                if (this.isSpawnPositionSafe(candidatePosition, spawnedAsteroids)) {
                    spawnPosition = candidatePosition;
                }
                
                attempts++;
            }
            
            // If we couldn't find a safe position, use the last candidate anyway
            if (!spawnPosition) {
                spawnPosition = this.generateSpawnPosition();
                console.warn(`Could not find safe spawn position for asteroid ${i + 1}, using fallback position`);
            }
            
            // Create and configure the asteroid
            const asteroid = new Asteroid(this.gameScene, spawnPosition.x, spawnPosition.y, size);
            asteroid.applyConfig(this.gameConfig);
            
            // Add to the scene
            this.gameScene.addAsteroid(asteroid);
            spawnedAsteroids.push(asteroid);
        }
        
        console.log(`Successfully spawned ${spawnedAsteroids.length} asteroids for wave ${this.currentWave}`);
    }

    /**
     * Generate a random spawn position at the edges of the screen
     * Asteroids spawn from the edges moving toward the center
     * @returns Random spawn position
     */
    private generateSpawnPosition(): { x: number; y: number } {
        const screenBounds = this.gameScene.getScreenBounds();
        const margin = 50; // Spawn slightly off-screen
        
        // Choose a random edge: 0=top, 1=right, 2=bottom, 3=left
        const edge = Math.floor(Math.random() * 4);
        
        let x: number, y: number;
        
        switch (edge) {
            case 0: // Top edge
                x = Math.random() * screenBounds.width;
                y = -margin;
                break;
            case 1: // Right edge
                x = screenBounds.width + margin;
                y = Math.random() * screenBounds.height;
                break;
            case 2: // Bottom edge
                x = Math.random() * screenBounds.width;
                y = screenBounds.height + margin;
                break;
            case 3: // Left edge
                x = -margin;
                y = Math.random() * screenBounds.height;
                break;
            default:
                x = screenBounds.width / 2;
                y = screenBounds.height / 2;
                break;
        }
        
        return { x, y };
    }

    /**
     * Check if a spawn position is safe (away from ship and other asteroids)
     * @param position - Position to check
     * @param existingAsteroids - Already spawned asteroids to avoid
     * @returns True if position is safe
     */
    private isSpawnPositionSafe(position: { x: number; y: number }, existingAsteroids: Asteroid[]): boolean {
        const safeRadius = this.gameConfig.game.safeSpawnRadius;
        const ship = this.gameScene.getShip();
        
        // Check distance from ship
        if (ship && !ship.isObjectDestroyed()) {
            const distanceFromShip = this.calculateDistance(position, { x: ship.x, y: ship.y });
            if (distanceFromShip < safeRadius) {
                return false;
            }
        }
        
        // Check distance from existing asteroids in this spawn batch
        for (const asteroid of existingAsteroids) {
            const distanceFromAsteroid = this.calculateDistance(position, { x: asteroid.x, y: asteroid.y });
            if (distanceFromAsteroid < safeRadius * 0.5) { // Asteroids can be closer to each other
                return false;
            }
        }
        
        // Check distance from already active asteroids
        const activeAsteroids = this.gameScene.getAsteroids();
        for (const asteroid of activeAsteroids) {
            if (!asteroid.isObjectDestroyed()) {
                const distanceFromAsteroid = this.calculateDistance(position, { x: asteroid.x, y: asteroid.y });
                if (distanceFromAsteroid < safeRadius * 0.5) {
                    return false;
                }
            }
        }
        
        return true;
    }

    /**
     * Calculate distance between two points
     * @param point1 - First point
     * @param point2 - Second point
     * @returns Distance between points
     */
    private calculateDistance(point1: { x: number; y: number }, point2: { x: number; y: number }): number {
        const dx = point1.x - point2.x;
        const dy = point1.y - point2.y;
        return Math.sqrt(dx * dx + dy * dy);
    }

    /**
     * Get the current wave number
     * @returns Current wave number
     */
    public getCurrentWave(): number {
        return this.currentWave;
    }

    /**
     * Check if the system is currently spawning a wave
     * @returns True if spawning is in progress
     */
    public isCurrentlySpawning(): boolean {
        return this.isSpawning;
    }

    /**
     * Get remaining time until next wave spawns
     * @returns Remaining spawn delay in milliseconds, or 0 if not spawning
     */
    public getSpawnDelayRemaining(): number {
        return this.isSpawning ? Math.max(0, this.spawnDelayTimer) : 0;
    }

    /**
     * Force spawn a new wave immediately (for testing or special events)
     * @param asteroidCount - Optional override for asteroid count
     */
    public forceSpawnWave(asteroidCount?: number): void {
        this.currentWave++;
        this.gameStateManager.setWave(this.currentWave);
        
        const count = asteroidCount || (this.gameConfig.game.initialAsteroidCount + (this.currentWave - 1) * this.gameConfig.game.waveIncrement);
        
        console.log(`Force spawning wave ${this.currentWave} with ${count} asteroids`);
        this.spawnAsteroids(count, AsteroidSize.LARGE);
        
        this.isSpawning = false;
        this.spawnDelayTimer = 0;
    }

    /**
     * Reset the spawning system for a new game
     */
    public reset(): void {
        this.currentWave = 0; // Will be set to 1 when first wave is initialized
        this.isSpawning = false;
        this.spawnDelayTimer = 0;
        console.log('SpawningSystem reset for new game');
    }
}