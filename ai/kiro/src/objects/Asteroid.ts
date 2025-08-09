import * as Phaser from 'phaser';
import { GameObject } from './GameObject';
import { GameScene } from '../scenes/GameScene';
import { GameConfig } from '../config/GameConfig';
import { ICollidable } from '../interfaces/ICollidable';

/**
 * Enum for asteroid sizes
 */
export enum AsteroidSize {
    LARGE = 'large',
    MEDIUM = 'medium',
    SMALL = 'small'
}

/**
 * Asteroid class representing space rocks that the player must destroy.
 * Handles movement, rotation, collision detection, and size management.
 */
export class Asteroid extends GameObject {
    private asteroidSize: AsteroidSize;
    private rotationSpeed: number;
    private baseSpeed: number;

    constructor(scene: GameScene, x: number, y: number, size: AsteroidSize = AsteroidSize.LARGE) {
        // For now, use a simple geometric shape since we don't have sprite assets yet
        super(scene, x, y, '');

        this.asteroidSize = size;
        this.rotationSpeed = 0;
        this.baseSpeed = 0;

        // Create asteroid graphics based on size
        this.createAsteroidGraphics();

        // Initialize random movement and rotation
        this.initializeMovement();
    }

    /**
     * Create a simple polygonal graphic to represent the asteroid
     */
    private createAsteroidGraphics(): void {
        const graphics = this.scene.add.graphics();
        graphics.lineStyle(2, 0xffffff); // White outline
        graphics.fillStyle(0x666666); // Gray fill

        let size: number;
        let textureKey: string;

        // Set size and texture key based on asteroid size
        switch (this.asteroidSize) {
            case AsteroidSize.LARGE:
                size = 32;
                textureKey = 'asteroid-large';
                break;
            case AsteroidSize.MEDIUM:
                size = 20;
                textureKey = 'asteroid-medium';
                break;
            case AsteroidSize.SMALL:
                size = 12;
                textureKey = 'asteroid-small';
                break;
            default:
                size = 32;
                textureKey = 'asteroid-large';
                break;
        }

        // Draw an irregular polygon to represent the asteroid
        const points: number[] = [];
        const numPoints = 8;
        const angleStep = (Math.PI * 2) / numPoints;

        for (let i = 0; i < numPoints; i++) {
            const angle = i * angleStep;
            // Add some randomness to make it look more like an asteroid
            const radiusVariation = 0.7 + Math.random() * 0.6; // Random between 0.7 and 1.3
            const radius = size * radiusVariation;
            
            const x = Math.cos(angle) * radius + size;
            const y = Math.sin(angle) * radius + size;
            
            points.push(x, y);
        }

        // Draw the polygon
        graphics.beginPath();
        graphics.moveTo(points[0]!, points[1]!);
        for (let i = 2; i < points.length; i += 2) {
            graphics.lineTo(points[i]!, points[i + 1]!);
        }
        graphics.closePath();
        graphics.fillPath();
        graphics.strokePath();

        // Convert to texture and apply to this sprite
        const textureSize = Math.ceil(size * 2.6); // Make texture large enough to contain the asteroid
        graphics.generateTexture(textureKey, textureSize, textureSize);
        this.setTexture(textureKey);
        graphics.destroy();

        // Set origin to center for proper rotation
        this.setOrigin(0.5, 0.5);
    }

    /**
     * Initialize random movement and rotation for the asteroid
     */
    private initializeMovement(): void {
        // Generate random direction (0 to 2π radians)
        const direction = Math.random() * Math.PI * 2;
        
        // Set velocity based on direction and speed for this size
        const velocityX = Math.cos(direction) * this.baseSpeed;
        const velocityY = Math.sin(direction) * this.baseSpeed;
        this.setVelocity(velocityX, velocityY);

        // Set random rotation speed (between -2 and 2 radians per second)
        this.rotationSpeed = (Math.random() - 0.5) * 4;
    }

    /**
     * Update method called every frame
     * @param delta - Time elapsed since last frame in milliseconds
     */
    protected updateGameObject(delta: number): void {
        const deltaSeconds = delta / 1000;

        // Apply rotation
        this.rotation += this.rotationSpeed * deltaSeconds;
    }

    /**
     * Handle collision with another object
     * @param other - The other object involved in the collision
     */
    public onCollision(other: ICollidable): void {
        // Log the collision for debugging
        console.log(`Asteroid (${this.asteroidSize}) collision detected with:`, other);
    }

    /**
     * Apply game configuration to the asteroid
     * @param config - Game configuration object
     */
    public applyConfig(config: GameConfig): void {
        // Set speed based on asteroid size
        switch (this.asteroidSize) {
            case AsteroidSize.LARGE:
                this.baseSpeed = config.asteroid.speeds.large;
                break;
            case AsteroidSize.MEDIUM:
                this.baseSpeed = config.asteroid.speeds.medium;
                break;
            case AsteroidSize.SMALL:
                this.baseSpeed = config.asteroid.speeds.small;
                break;
        }

        // Re-initialize movement with new speed if already created
        if (this.baseSpeed > 0) {
            this.initializeMovement();
        }
    }

    /**
     * Get the size of this asteroid
     * @returns The asteroid size enum value
     */
    public getSize(): AsteroidSize {
        return this.asteroidSize;
    }

    /**
     * Get the base speed of this asteroid
     * @returns The base movement speed
     */
    public getBaseSpeed(): number {
        return this.baseSpeed;
    }

    /**
     * Get the rotation speed of this asteroid
     * @returns The rotation speed in radians per second
     */
    public getRotationSpeed(): number {
        return this.rotationSpeed;
    }

    /**
     * Set a new random movement direction while keeping the same speed
     */
    public randomizeMovement(): void {
        const direction = Math.random() * Math.PI * 2;
        const velocityX = Math.cos(direction) * this.baseSpeed;
        const velocityY = Math.sin(direction) * this.baseSpeed;
        this.setVelocity(velocityX, velocityY);
    }

    /**
     * Split this asteroid into smaller asteroids
     * @returns Array of new smaller asteroids, or empty array if this is a small asteroid
     */
    public split(): Asteroid[] {
        const splitAsteroids: Asteroid[] = [];
        
        // Small asteroids don't split - they are completely destroyed
        if (this.asteroidSize === AsteroidSize.SMALL) {
            return splitAsteroids;
        }

        // Determine the new size after splitting
        let newSize: AsteroidSize;
        if (this.asteroidSize === AsteroidSize.LARGE) {
            newSize = AsteroidSize.MEDIUM;
        } else if (this.asteroidSize === AsteroidSize.MEDIUM) {
            newSize = AsteroidSize.SMALL;
        } else {
            return splitAsteroids; // Should not happen, but safety check
        }

        // Get split count from config (2-3 asteroids)
        const config = this.gameConfig;
        const minSplit = config.asteroid.splitCount.min;
        const maxSplit = config.asteroid.splitCount.max;
        const splitCount = Math.floor(Math.random() * (maxSplit - minSplit + 1)) + minSplit;

        // Create the split asteroids
        for (let i = 0; i < splitCount; i++) {
            const newAsteroid = new Asteroid(this.gameScene, this.x, this.y, newSize);
            newAsteroid.applyConfig(config);
            
            // Give each split asteroid a different movement direction
            // Spread them out in different directions from the original
            const baseAngle = Math.random() * Math.PI * 2;
            const angleOffset = (i / splitCount) * Math.PI * 2;
            const direction = baseAngle + angleOffset;
            
            // Add some randomness to the direction
            const directionVariation = (Math.random() - 0.5) * Math.PI * 0.5; // ±45 degrees
            const finalDirection = direction + directionVariation;
            
            // Set velocity based on the new direction and size-appropriate speed
            let speed: number;
            switch (newSize) {
                case AsteroidSize.MEDIUM:
                    speed = config.asteroid.speeds.medium;
                    break;
                case AsteroidSize.SMALL:
                    speed = config.asteroid.speeds.small;
                    break;
                default:
                    speed = config.asteroid.speeds.large;
                    break;
            }
            
            const velocityX = Math.cos(finalDirection) * speed;
            const velocityY = Math.sin(finalDirection) * speed;
            newAsteroid.setVelocity(velocityX, velocityY);
            
            splitAsteroids.push(newAsteroid);
        }

        return splitAsteroids;
    }

    /**
     * Create a new asteroid of a smaller size at the current position
     * Used for asteroid splitting mechanics
     * @param newSize - The size of the new asteroid
     * @returns A new asteroid instance
     */
    public createSmallerAsteroid(newSize: AsteroidSize): Asteroid {
        const newAsteroid = new Asteroid(this.gameScene, this.x, this.y, newSize);
        newAsteroid.applyConfig(this.gameConfig);
        return newAsteroid;
    }
}