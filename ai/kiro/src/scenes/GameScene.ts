import * as Phaser from 'phaser';
import { GameConfig } from '../config/GameConfig';
import { GameStateManager, UIManager } from '../managers';
import { ConfigLoader } from '../utils/ConfigLoader';
import { Ship } from '../objects/Ship';
import { Asteroid, AsteroidSize } from '../objects/Asteroid';
import { CollisionSystem } from '../systems/CollisionSystem';
import { ScoringSystem } from '../systems/ScoringSystem';
import { SpawningSystem } from '../systems/SpawningSystem';

export class GameScene extends Phaser.Scene {
    private gameConfig!: GameConfig;
    private gameStateManager!: GameStateManager;
    private uiManager!: UIManager;
    private configLoader!: ConfigLoader;
    private ship!: Ship;
    private asteroids: Asteroid[] = [];
    private scoringSystem!: ScoringSystem;
    private spawningSystem!: SpawningSystem;

    constructor() {
        super({ key: 'GameScene' });
    }

    preload(): void {
        // Set up loading progress
        this.load.on('progress', (value: number) => {
            console.log('Loading progress:', Math.round(value * 100) + '%');
        });

        this.load.on('complete', () => {
            console.log('All assets loaded');
        });

        // Get config loader instance
        this.configLoader = ConfigLoader.getInstance();

        // For now, we'll load the config synchronously in create()
        // In future tasks, we might add sprite assets here
    }

    create(): void {
        // Load game configuration
        this.gameConfig = this.configLoader.getConfig();

        // Initialize game state manager
        this.gameStateManager = new GameStateManager();

        // Initialize UI manager
        this.uiManager = new UIManager(this.gameStateManager);

        // Initialize scoring system
        this.scoringSystem = new ScoringSystem(this.gameStateManager, this.gameConfig);
        this.scoringSystem.setGameScene(this);

        // Initialize spawning system
        this.spawningSystem = new SpawningSystem(this, this.gameConfig, this.gameStateManager);

        // Set up physics world
        this.physics.world.setBounds(0, 0, this.scale.width, this.scale.height);

        // Set up screen wrapping - no bounds for wrapping behavior
        this.physics.world.checkCollision.up = false;
        this.physics.world.checkCollision.down = false;
        this.physics.world.checkCollision.left = false;
        this.physics.world.checkCollision.right = false;

        // Set background color to black (space)
        this.cameras.main.setBackgroundColor('#000000');

        // Create the player ship at the center of the screen
        this.ship = new Ship(this, this.scale.width / 2, this.scale.height / 2);
        this.ship.applyConfig(this.gameConfig);

        // Initialize the first wave of asteroids
        this.spawningSystem.initializeFirstWave();

        console.log('Game scene created successfully');
        console.log('Screen dimensions:', this.scale.width, 'x', this.scale.height);
        console.log('Game config loaded:', this.gameConfig);
    }

    override update(time: number, delta: number): void {
        // Main game loop - will be expanded in future tasks
        // For now, just ensure the game is running at 60fps

        if (this.gameStateManager.isPaused() || this.gameStateManager.isGameOver()) {
            return;
        }

        // Update ship (which also updates its projectiles)
        if (this.ship && !this.ship.isObjectDestroyed()) {
            this.ship.update(delta);
        }

        // Update asteroids
        this.updateAsteroids(delta);

        // Update spawning system for wave management
        this.spawningSystem.update(delta);

        // Check collisions between all game objects and handle asteroid splitting
        const newAsteroids = CollisionSystem.checkCollisions(this.ship, this.getAsteroids(), this.scoringSystem);
        
        // Add any new asteroids created from splitting
        if (newAsteroids.length > 0) {
            this.asteroids.push(...newAsteroids);
            console.log(`Added ${newAsteroids.length} new asteroids from splitting`);
        }

        // Update UI display
        this.uiManager.update();
    }

    // Utility method to get screen dimensions
    public getScreenBounds(): { width: number; height: number } {
        return {
            width: this.scale.width,
            height: this.scale.height
        };
    }

    // Utility method to wrap objects around screen edges
    public wrapAroundScreen(gameObject: Phaser.GameObjects.GameObject & { x: number; y: number }): void {
        const bounds = this.getScreenBounds();

        if (gameObject.x < 0) {
            gameObject.x = bounds.width;
        } else if (gameObject.x > bounds.width) {
            gameObject.x = 0;
        }

        if (gameObject.y < 0) {
            gameObject.y = bounds.height;
        } else if (gameObject.y > bounds.height) {
            gameObject.y = 0;
        }
    }

    // Getter methods for other classes to access managers
    public getGameConfig(): GameConfig {
        return this.gameConfig;
    }

    public getGameStateManager(): GameStateManager {
        return this.gameStateManager;
    }

    public getShip(): Ship {
        return this.ship;
    }

    public getAsteroids(): Asteroid[] {
        return this.asteroids.filter(asteroid => asteroid && !asteroid.isObjectDestroyed());
    }

    public getScoringSystem(): ScoringSystem {
        return this.scoringSystem;
    }

    public getUIManager(): UIManager {
        return this.uiManager;
    }

    public getSpawningSystem(): SpawningSystem {
        return this.spawningSystem;
    }

    /**
     * Add an asteroid to the scene (used by SpawningSystem)
     * @param asteroid - The asteroid to add
     */
    public addAsteroid(asteroid: Asteroid): void {
        this.asteroids.push(asteroid);
    }

    /**
     * Add multiple asteroids to the scene (used by SpawningSystem)
     * @param asteroids - Array of asteroids to add
     */
    public addAsteroids(asteroids: Asteroid[]): void {
        this.asteroids.push(...asteroids);
    }

    /**
     * Update all asteroids and remove destroyed ones
     * @param delta - Time elapsed since last frame in milliseconds
     */
    private updateAsteroids(delta: number): void {
        // Update all asteroids
        for (let i = 0; i < this.asteroids.length; i++) {
            const asteroid = this.asteroids[i];
            if (asteroid && !asteroid.isObjectDestroyed()) {
                asteroid.update(delta);
            }
        }

        // Remove destroyed asteroids
        this.asteroids = this.asteroids.filter(asteroid => asteroid && !asteroid.isObjectDestroyed());
    }

    /**
     * Respawn the ship at a safe location away from asteroids
     */
    public respawnShip(): void {
        const safePosition = this.findSafeSpawnPosition();
        this.ship.respawn(safePosition.x, safePosition.y);
        console.log(`Ship respawned at safe position: (${safePosition.x}, ${safePosition.y})`);
    }

    /**
     * Find a safe position to spawn the ship away from asteroids
     * @returns Safe spawn position
     */
    private findSafeSpawnPosition(): { x: number; y: number } {
        const centerX = this.scale.width / 2;
        const centerY = this.scale.height / 2;
        const safeRadius = this.gameConfig.game.safeSpawnRadius;
        
        // First try the center position
        if (this.isPositionSafe(centerX, centerY, safeRadius)) {
            return { x: centerX, y: centerY };
        }

        // If center is not safe, try several positions around the screen
        const attempts = 20;
        for (let i = 0; i < attempts; i++) {
            const angle = (i / attempts) * Math.PI * 2;
            const distance = Math.min(this.scale.width, this.scale.height) * 0.3;
            const x = centerX + Math.cos(angle) * distance;
            const y = centerY + Math.sin(angle) * distance;

            if (this.isPositionSafe(x, y, safeRadius)) {
                return { x, y };
            }
        }

        // If no safe position found, use center anyway (emergency fallback)
        console.warn('No safe spawn position found, using center position');
        return { x: centerX, y: centerY };
    }

    /**
     * Check if a position is safe from asteroids
     * @param x - X coordinate to check
     * @param y - Y coordinate to check
     * @param safeRadius - Minimum distance from asteroids
     * @returns True if position is safe
     */
    private isPositionSafe(x: number, y: number, safeRadius: number): boolean {
        const activeAsteroids = this.getAsteroids();
        
        for (const asteroid of activeAsteroids) {
            const distance = Math.sqrt(
                Math.pow(x - asteroid.x, 2) + Math.pow(y - asteroid.y, 2)
            );
            
            if (distance < safeRadius) {
                return false;
            }
        }
        
        return true;
    }
}