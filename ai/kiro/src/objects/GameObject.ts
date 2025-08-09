import * as Phaser from 'phaser';
import { IGameObject } from '../interfaces/IGameObject';
import { ICollidable, Rectangle } from '../interfaces/ICollidable';
import { IConfigurable } from '../interfaces/IConfigurable';
import { GameConfig } from '../config/GameConfig';
import { GameScene } from '../scenes/GameScene';

/**
 * Abstract base class for all game objects in the Asteroids game.
 * Provides common functionality for position, velocity, rotation, screen wrapping,
 * and basic physics management.
 */
export abstract class GameObject extends Phaser.GameObjects.Sprite implements IGameObject, ICollidable, IConfigurable {
    protected velocity: Phaser.Math.Vector2;
    protected gameScene: GameScene;
    protected gameConfig: GameConfig;
    protected isDestroyed: boolean = false;

    constructor(scene: GameScene, x: number, y: number, texture: string, frame?: string | number) {
        super(scene, x, y, texture, frame);
        
        this.gameScene = scene;
        this.gameConfig = scene.getGameConfig();
        this.velocity = new Phaser.Math.Vector2(0, 0);
        
        // Add to scene and enable physics
        scene.add.existing(this);
        scene.physics.add.existing(this);
        
        // Apply initial configuration
        this.applyConfig(this.gameConfig);
    }

    /**
     * Update method called every frame
     * @param delta - Time elapsed since last frame in milliseconds
     */
    public override update(delta: number): void {
        if (this.isDestroyed) {
            return;
        }

        // Apply velocity to position
        this.x += this.velocity.x * (delta / 1000);
        this.y += this.velocity.y * (delta / 1000);

        // Handle screen wrapping
        this.handleScreenWrapping();

        // Call child-specific update logic
        this.updateGameObject(delta);
    }

    /**
     * Abstract method for child classes to implement their specific update logic
     * @param delta - Time elapsed since last frame in milliseconds
     */
    protected abstract updateGameObject(delta: number): void;

    /**
     * Get current position as a simple object
     * @returns Object with x and y coordinates
     */
    public getPosition(): { x: number; y: number } {
        return { x: this.x, y: this.y };
    }

    /**
     * Get current velocity as a simple object
     * @returns Object with x and y velocity components
     */
    public getVelocity(): { x: number; y: number } {
        return { x: this.velocity.x, y: this.velocity.y };
    }

    /**
     * Set velocity with x and y components
     * @param x - X velocity component
     * @param y - Y velocity component
     */
    public setVelocity(x: number, y: number): void {
        this.velocity.set(x, y);
    }

    /**
     * Add to current velocity
     * @param x - X velocity to add
     * @param y - Y velocity to add
     */
    public addVelocity(x: number, y: number): void {
        this.velocity.x += x;
        this.velocity.y += y;
    }

    /**
     * Get velocity magnitude (speed)
     * @returns Current speed
     */
    public getSpeed(): number {
        return this.velocity.length();
    }

    /**
     * Handle screen wrapping - objects that go off one edge appear on the opposite edge
     */
    protected handleScreenWrapping(): void {
        const bounds = this.gameScene.getScreenBounds();
        const buffer = Math.max(this.width, this.height) / 2;

        // Wrap horizontally
        if (this.x < -buffer) {
            this.x = bounds.width + buffer;
        } else if (this.x > bounds.width + buffer) {
            this.x = -buffer;
        }

        // Wrap vertically
        if (this.y < -buffer) {
            this.y = bounds.height + buffer;
        } else if (this.y > bounds.height + buffer) {
            this.y = -buffer;
        }
    }

    /**
     * Get bounding rectangle for collision detection
     * @returns Rectangle representing the object's bounds
     */
    public getCollisionBounds(): Rectangle {
        return {
            x: this.x - this.width / 2,
            y: this.y - this.height / 2,
            width: this.width,
            height: this.height
        };
    }

    /**
     * Handle collision with another collidable object
     * @param other - The other object involved in the collision
     */
    public abstract onCollision(other: ICollidable): void;

    /**
     * Apply game configuration to this object
     * @param config - Game configuration object
     */
    public abstract applyConfig(config: GameConfig): void;

    /**
     * Destroy this game object and clean up resources
     */
    public override destroy(): void {
        if (this.isDestroyed) {
            return;
        }

        this.isDestroyed = true;
        
        // Remove from physics world
        if (this.body && (this.body as Phaser.Physics.Arcade.Body | Phaser.Physics.Arcade.StaticBody)) {
            this.gameScene.physics.world.remove(this.body as Phaser.Physics.Arcade.Body | Phaser.Physics.Arcade.StaticBody);
        }
        
        // Destroy the Phaser sprite
        super.destroy();
    }

    /**
     * Check if this object is destroyed
     * @returns True if the object has been destroyed
     */
    public isObjectDestroyed(): boolean {
        return this.isDestroyed;
    }

    /**
     * Get the game scene reference
     * @returns The game scene this object belongs to
     */
    protected getGameScene(): GameScene {
        return this.gameScene;
    }

    /**
     * Get the current game configuration
     * @returns The current game configuration
     */
    protected getGameConfig(): GameConfig {
        return this.gameConfig;
    }
}