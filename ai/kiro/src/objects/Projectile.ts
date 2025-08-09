import * as Phaser from 'phaser';
import { GameObject } from './GameObject';
import { GameScene } from '../scenes/GameScene';
import { GameConfig } from '../config/GameConfig';
import { ICollidable } from '../interfaces/ICollidable';

/**
 * Projectile class representing bullets fired by the ship.
 * Handles linear movement, collision detection, and lifetime management.
 */
export class Projectile extends GameObject {
    private speed: number = 500;
    private lifetime: number = 2000; // milliseconds
    private timeAlive: number = 0;

    constructor(scene: GameScene, x: number, y: number, rotation: number) {
        // For now, use a simple geometric shape since we don't have sprite assets yet
        super(scene, x, y, '');

        // Create a simple circular graphic to represent the projectile
        this.createProjectileGraphics();

        // Set initial rotation and velocity
        this.setRotation(rotation);
        this.setInitialVelocity();
    }

    /**
     * Create a simple circular graphic to represent the projectile
     */
    private createProjectileGraphics(): void {
        // Create a graphics object for the projectile shape
        const graphics = this.scene.add.graphics();
        graphics.fillStyle(0xffffff); // White color

        // Draw a small circle
        graphics.fillCircle(0, 0, 2);

        // Convert to texture and apply to this sprite
        graphics.generateTexture('projectile', 4, 4);
        this.setTexture('projectile');
        graphics.destroy();

        // Set origin to center
        this.setOrigin(0.5, 0.5);
    }

    /**
     * Set initial velocity based on rotation
     */
    private setInitialVelocity(): void {
        const velocityX = Math.cos(this.rotation) * this.speed;
        const velocityY = Math.sin(this.rotation) * this.speed;
        this.setVelocity(velocityX, velocityY);
    }

    /**
     * Update method called every frame
     * @param delta - Time elapsed since last frame in milliseconds
     */
    protected updateGameObject(delta: number): void {
        // Update lifetime
        this.timeAlive += delta;

        // Check if projectile should be destroyed due to lifetime
        if (this.timeAlive >= this.lifetime) {
            this.destroy();
            return;
        }

        // Check if projectile is off-screen and should be destroyed
        this.checkScreenBounds();
    }

    /**
     * Check if projectile is off-screen and destroy it if so
     */
    private checkScreenBounds(): void {
        const bounds = this.gameScene.getScreenBounds();
        const buffer = 50; // Extra buffer to ensure projectile is completely off-screen

        if (this.x < -buffer || 
            this.x > bounds.width + buffer || 
            this.y < -buffer || 
            this.y > bounds.height + buffer) {
            this.destroy();
        }
    }

    /**
     * Handle collision with another object
     * @param other - The other object involved in the collision
     */
    public onCollision(other: ICollidable): void {
        // For now, just log the collision and destroy the projectile
        // In future tasks, this will handle asteroid destruction
        console.log('Projectile collision detected with:', other);
        this.destroy();
    }

    /**
     * Apply game configuration to the projectile
     * @param config - Game configuration object
     */
    public applyConfig(config: GameConfig): void {
        this.speed = config.projectile.speed;
        this.lifetime = config.projectile.lifetime;
    }

    /**
     * Get the age of this projectile in milliseconds
     * @returns Time alive in milliseconds
     */
    public getTimeAlive(): number {
        return this.timeAlive;
    }

    /**
     * Override screen wrapping to prevent projectiles from wrapping
     * Projectiles should be destroyed when they go off-screen
     */
    protected override handleScreenWrapping(): void {
        // Projectiles don't wrap around screen - they get destroyed instead
        // This is handled in checkScreenBounds()
    }
}