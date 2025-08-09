import * as Phaser from 'phaser';
import { GameObject } from './GameObject';
import { Projectile } from './Projectile';
import { GameScene } from '../scenes/GameScene';
import { GameConfig } from '../config/GameConfig';
import { ICollidable } from '../interfaces/ICollidable';

/**
 * Ship class representing the player's spaceship in the Asteroids game.
 * Handles movement, rotation, thrust physics, and input processing.
 */
export class Ship extends GameObject {
    private cursors!: Phaser.Types.Input.Keyboard.CursorKeys;
    private spaceKey!: Phaser.Input.Keyboard.Key;
    private thrustSpeed: number = 200;
    private rotationSpeed: number = 300;
    private maxSpeed: number = 400;
    private friction: number = 0.98;
    private isThrusting: boolean = false;
    private projectiles: Projectile[] = [];
    private maxProjectiles: number = 4;
    private lastShotTime: number = 0;
    private shotCooldown: number = 150; // milliseconds between shots
    
    // Invincibility and respawn properties
    private isInvincible: boolean = false;
    private invincibilityTimer: number = 0;
    private invincibilityTime: number = 2000; // milliseconds
    private blinkTimer: number = 0;
    private blinkInterval: number = 100; // milliseconds between blinks
    private isVisible: boolean = true;

    constructor(scene: GameScene, x: number, y: number) {
        // For now, use a simple geometric shape since we don't have sprite assets yet
        super(scene, x, y, '');

        // Create a simple triangle shape to represent the ship
        this.createShipGraphics();

        // Set up input handling
        this.setupInput();

        // Set initial rotation (pointing up)
        this.setRotation(-Math.PI / 2);
    }

    /**
     * Create a simple triangular graphic to represent the ship
     */
    private createShipGraphics(): void {
        // Create a graphics object for the ship shape
        const graphics = this.scene.add.graphics();
        graphics.fillStyle(0xffffff); // White color

        // Draw a proper triangle pointing upward
        graphics.beginPath();
        graphics.moveTo(0, 2); // Bottom Left (y, x)
        graphics.lineTo(4, 12);
        graphics.lineTo(0, 22);  // Bottom Right
        graphics.lineTo(24, 12);  // Top
        graphics.closePath();
        graphics.fillPath();

        // Convert to texture and apply to this sprite
        graphics.generateTexture('ship', 24, 24);
        this.setTexture('ship');
        graphics.destroy();

        // Set origin to center for proper rotation
        this.setOrigin(0.5, 0.5);
    }

    /**
     * Set up keyboard input handling
     */
    private setupInput(): void {
        // Get cursor keys for movement
        this.cursors = this.scene.input.keyboard!.createCursorKeys();
        
        // Get spacebar for shooting
        this.spaceKey = this.scene.input.keyboard!.addKey(Phaser.Input.Keyboard.KeyCodes.SPACE);
    }

    /**
     * Update method called every frame to handle input and physics
     * @param delta - Time elapsed since last frame in milliseconds
     */
    protected updateGameObject(delta: number): void {
        this.handleInput(delta);
        this.applyPhysics(delta);
        this.updateProjectiles(delta);
        this.updateInvincibility(delta);
    }

    /**
     * Handle keyboard input for ship movement
     * @param delta - Time elapsed since last frame in milliseconds
     */
    private handleInput(delta: number): void {
        const deltaSeconds = delta / 1000;

        // Handle rotation (left and right arrow keys)
        // Convert rotation speed from degrees per second to radians per second
        const rotationRadiansPerSecond = (this.rotationSpeed * Math.PI) / 180;

        if (this.cursors.left.isDown) {
            this.rotation -= rotationRadiansPerSecond * deltaSeconds;
        }
        if (this.cursors.right.isDown) {
            this.rotation += rotationRadiansPerSecond * deltaSeconds;
        }

        // Handle thrust (up arrow key)
        this.isThrusting = this.cursors.up.isDown;
        if (this.isThrusting) {
            this.applyThrust(deltaSeconds);
        }

        // Handle shooting (spacebar)
        if (this.spaceKey.isDown) {
            this.shoot();
        }
    }

    /**
     * Apply thrust force in the direction the ship is facing
     * @param deltaSeconds - Time elapsed since last frame in seconds
     */
    private applyThrust(deltaSeconds: number): void {
        // Calculate thrust direction based on current rotation
        const thrustX = Math.cos(this.rotation) * this.thrustSpeed * deltaSeconds;
        const thrustY = Math.sin(this.rotation) * this.thrustSpeed * deltaSeconds;

        // Add thrust to current velocity
        this.addVelocity(thrustX, thrustY);

        // Limit maximum speed
        const currentSpeed = this.getSpeed();
        if (currentSpeed > this.maxSpeed) {
            const scale = this.maxSpeed / currentSpeed;
            this.velocity.scale(scale);
        }
    }

    /**
     * Apply physics including friction and momentum
     * @param delta - Time elapsed since last frame in milliseconds
     */
    private applyPhysics(delta: number): void {
        // Apply friction to gradually slow down the ship
        this.velocity.scale(this.friction);

        // Stop very small velocities to prevent infinite drift
        if (this.velocity.length() < 0.1) {
            this.velocity.set(0, 0);
        }
    }

    /**
     * Handle collision with another object
     * @param other - The other object involved in the collision
     */
    public onCollision(other: ICollidable): void {
        // Ignore collisions during invincibility period
        if (this.isInvincible) {
            return;
        }
        
        console.log('Ship collision detected with:', other);
        // The actual respawn logic will be handled by the GameScene
        // This method is called by the collision system to notify of collision
    }

    /**
     * Shoot a projectile in the direction the ship is facing
     */
    private shoot(): void {
        const currentTime = this.scene.time.now;
        
        // Check cooldown to prevent rapid firing
        if (currentTime - this.lastShotTime < this.shotCooldown) {
            return;
        }

        // Check if we've reached the maximum number of projectiles
        if (this.projectiles.length >= this.maxProjectiles) {
            return;
        }

        // Calculate projectile spawn position (slightly in front of ship)
        const spawnDistance = 15; // Distance in front of ship
        const spawnX = this.x + Math.cos(this.rotation) * spawnDistance;
        const spawnY = this.y + Math.sin(this.rotation) * spawnDistance;

        // Create new projectile
        const projectile = new Projectile(this.gameScene, spawnX, spawnY, this.rotation);
        projectile.applyConfig(this.gameConfig);
        
        // Add to projectiles array
        this.projectiles.push(projectile);

        // Update last shot time
        this.lastShotTime = currentTime;

        console.log(`Shot fired! Projectiles: ${this.projectiles.length}/${this.maxProjectiles}`);
    }

    /**
     * Update all projectiles and remove destroyed ones
     * @param delta - Time elapsed since last frame in milliseconds
     */
    private updateProjectiles(delta: number): void {
        // Update all projectiles
        for (let i = 0; i < this.projectiles.length; i++) {
            const projectile = this.projectiles[i];
            if (projectile && !projectile.isObjectDestroyed()) {
                projectile.update(delta);
            }
        }

        // Remove destroyed projectiles
        this.projectiles = this.projectiles.filter(projectile => projectile && !projectile.isObjectDestroyed());
    }

    /**
     * Get all active projectiles fired by this ship
     * @returns Array of active projectiles
     */
    public getProjectiles(): Projectile[] {
        return this.projectiles.filter(projectile => projectile && !projectile.isObjectDestroyed());
    }

    /**
     * Apply game configuration to the ship
     * @param config - Game configuration object
     */
    public applyConfig(config: GameConfig): void {
        this.thrustSpeed = config.ship.thrustSpeed;
        this.rotationSpeed = config.ship.rotationSpeed;
        this.maxSpeed = config.ship.maxSpeed;
        this.friction = config.ship.friction;
        this.maxProjectiles = config.projectile.maxCount;
        this.invincibilityTime = config.ship.invincibilityTime;
    }

    /**
     * Get whether the ship is currently thrusting
     * @returns True if the ship is thrusting
     */
    public isShipThrusting(): boolean {
        return this.isThrusting;
    }

    /**
     * Reset ship to initial state (used for respawning)
     * @param x - X position to reset to
     * @param y - Y position to reset to
     */
    public reset(x: number, y: number): void {
        this.setPosition(x, y);
        this.setRotation(-Math.PI / 2); // Point upward
        this.setVelocity(0, 0);
        this.isThrusting = false;
        
        // Clear all projectiles
        this.projectiles.forEach(projectile => {
            if (projectile) {
                projectile.destroy();
            }
        });
        this.projectiles = [];
        this.lastShotTime = 0;
    }

    /**
     * Respawn the ship with invincibility period
     * @param x - X position to respawn at
     * @param y - Y position to respawn at
     */
    public respawn(x: number, y: number): void {
        this.reset(x, y);
        this.startInvincibility();
        console.log(`Ship respawned at (${x}, ${y}) with ${this.invincibilityTime}ms invincibility`);
    }

    /**
     * Start invincibility period with visual feedback
     */
    private startInvincibility(): void {
        this.isInvincible = true;
        this.invincibilityTimer = this.invincibilityTime;
        this.blinkTimer = 0;
        this.isVisible = true;
        this.setVisible(true);
    }

    /**
     * Update invincibility state and visual feedback
     * @param delta - Time elapsed since last frame in milliseconds
     */
    private updateInvincibility(delta: number): void {
        if (!this.isInvincible) {
            return;
        }

        // Update invincibility timer
        this.invincibilityTimer -= delta;
        
        // Update blink timer for visual feedback
        this.blinkTimer += delta;
        
        // Toggle visibility for blinking effect
        if (this.blinkTimer >= this.blinkInterval) {
            this.isVisible = !this.isVisible;
            this.setVisible(this.isVisible);
            this.blinkTimer = 0;
        }

        // End invincibility when timer expires
        if (this.invincibilityTimer <= 0) {
            this.endInvincibility();
        }
    }

    /**
     * End invincibility period and restore normal visibility
     */
    private endInvincibility(): void {
        this.isInvincible = false;
        this.invincibilityTimer = 0;
        this.blinkTimer = 0;
        this.isVisible = true;
        this.setVisible(true);
        console.log('Ship invincibility ended');
    }

    /**
     * Check if the ship is currently invincible
     * @returns True if the ship is invincible
     */
    public isShipInvincible(): boolean {
        return this.isInvincible;
    }

    /**
     * Get remaining invincibility time in milliseconds
     * @returns Remaining invincibility time
     */
    public getRemainingInvincibilityTime(): number {
        return Math.max(0, this.invincibilityTimer);
    }

    /**
     * Override destroy to clean up projectiles
     */
    public override destroy(): void {
        // Destroy all projectiles
        this.projectiles.forEach(projectile => {
            if (projectile) {
                projectile.destroy();
            }
        });
        this.projectiles = [];
        
        super.destroy();
    }
}