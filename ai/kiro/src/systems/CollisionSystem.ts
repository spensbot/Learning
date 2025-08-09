import { Ship } from '../objects/Ship';
import { Asteroid } from '../objects/Asteroid';
import { Projectile } from '../objects/Projectile';
import { ICollidable, Rectangle } from '../interfaces/ICollidable';
import { ScoringSystem } from './ScoringSystem';

/**
 * CollisionSystem handles all collision detection and response in the game.
 * It manages collisions between projectiles and asteroids, and between ship and asteroids.
 */
export class CollisionSystem {
    /**
     * Check and handle all collisions in the game
     * @param ship - The player's ship
     * @param asteroids - Array of active asteroids
     * @param scoringSystem - The scoring system to handle points and lives
     * @returns Array of new asteroids created from splitting
     */
    public static checkCollisions(ship: Ship, asteroids: Asteroid[], scoringSystem?: ScoringSystem): Asteroid[] {
        const newAsteroids: Asteroid[] = [];
        
        // Check projectile-asteroid collisions
        const splitAsteroids = this.checkProjectileAsteroidCollisions(ship, asteroids, scoringSystem);
        newAsteroids.push(...splitAsteroids);
        
        // Check ship-asteroid collisions
        this.checkShipAsteroidCollisions(ship, asteroids, scoringSystem);
        
        return newAsteroids;
    }

    /**
     * Check collisions between projectiles and asteroids
     * @param ship - The player's ship (to get projectiles)
     * @param asteroids - Array of active asteroids
     * @param scoringSystem - The scoring system to award points
     * @returns Array of new asteroids created from splitting
     */
    private static checkProjectileAsteroidCollisions(ship: Ship, asteroids: Asteroid[], scoringSystem?: ScoringSystem): Asteroid[] {
        const newAsteroids: Asteroid[] = [];
        const projectiles = ship.getProjectiles();

        for (const projectile of projectiles) {
            if (projectile.isObjectDestroyed()) {
                continue;
            }

            for (const asteroid of asteroids) {
                if (asteroid.isObjectDestroyed()) {
                    continue;
                }

                if (this.checkCollision(projectile, asteroid)) {
                    // Handle collision response and get any split asteroids
                    const splitAsteroids = this.handleProjectileAsteroidCollision(projectile, asteroid, scoringSystem);
                    newAsteroids.push(...splitAsteroids);
                    break; // Projectile can only hit one asteroid
                }
            }
        }
        
        return newAsteroids;
    }

    /**
     * Check collisions between ship and asteroids
     * @param ship - The player's ship
     * @param asteroids - Array of active asteroids
     * @param scoringSystem - The scoring system to handle life loss
     */
    private static checkShipAsteroidCollisions(ship: Ship, asteroids: Asteroid[], scoringSystem?: ScoringSystem): void {
        if (ship.isObjectDestroyed() || ship.isShipInvincible()) {
            return;
        }

        for (const asteroid of asteroids) {
            if (asteroid.isObjectDestroyed()) {
                continue;
            }

            if (this.checkCollision(ship, asteroid)) {
                // Handle collision response
                this.handleShipAsteroidCollision(ship, asteroid, scoringSystem);
                break; // Ship can only collide with one asteroid per frame
            }
        }
    }

    /**
     * Check if two collidable objects are colliding using bounding box collision
     * @param obj1 - First collidable object
     * @param obj2 - Second collidable object
     * @returns True if objects are colliding
     */
    private static checkCollision(obj1: ICollidable, obj2: ICollidable): boolean {
        const bounds1 = obj1.getCollisionBounds();
        const bounds2 = obj2.getCollisionBounds();

        return this.rectanglesIntersect(bounds1, bounds2);
    }

    /**
     * Check if two rectangles intersect
     * @param rect1 - First rectangle
     * @param rect2 - Second rectangle
     * @returns True if rectangles intersect
     */
    private static rectanglesIntersect(rect1: Rectangle, rect2: Rectangle): boolean {
        return !(rect1.x + rect1.width < rect2.x ||
                rect2.x + rect2.width < rect1.x ||
                rect1.y + rect1.height < rect2.y ||
                rect2.y + rect2.height < rect1.y);
    }

    /**
     * Handle collision between a projectile and an asteroid
     * @param projectile - The projectile that hit the asteroid
     * @param asteroid - The asteroid that was hit
     * @param scoringSystem - The scoring system to award points
     * @returns Array of new asteroids created from splitting
     */
    private static handleProjectileAsteroidCollision(projectile: Projectile, asteroid: Asteroid, scoringSystem?: ScoringSystem): Asteroid[] {
        // Award points for destroying the asteroid
        if (scoringSystem) {
            scoringSystem.awardAsteroidPoints(asteroid.getSize());
        }

        // Notify both objects of the collision
        projectile.onCollision(asteroid);
        asteroid.onCollision(projectile);

        // Destroy the projectile immediately
        if (!projectile.isObjectDestroyed()) {
            projectile.destroy();
        }

        // Split the asteroid before destroying it
        const splitAsteroids = asteroid.split();

        // Destroy the original asteroid
        if (!asteroid.isObjectDestroyed()) {
            asteroid.destroy();
        }

        console.log(`Projectile hit ${asteroid.getSize()} asteroid! Created ${splitAsteroids.length} smaller asteroids.`);
        
        return splitAsteroids;
    }

    /**
     * Handle collision between the ship and an asteroid
     * @param ship - The player's ship
     * @param asteroid - The asteroid that hit the ship
     * @param scoringSystem - The scoring system to handle life loss
     */
    private static handleShipAsteroidCollision(ship: Ship, asteroid: Asteroid, scoringSystem?: ScoringSystem): void {
        // Handle life loss through scoring system
        if (scoringSystem) {
            scoringSystem.handleShipCollision();
        }

        // Notify both objects of the collision
        ship.onCollision(asteroid);
        asteroid.onCollision(ship);

        // Destroy the asteroid that hit the ship
        if (!asteroid.isObjectDestroyed()) {
            asteroid.destroy();
        }

        console.log(`Ship collided with ${asteroid.getSize()} asteroid! Asteroid destroyed.`);
    }

    /**
     * Check collision between two objects using circular collision detection
     * This is more accurate for round objects like asteroids
     * @param obj1 - First collidable object with position
     * @param obj2 - Second collidable object with position
     * @param radius1 - Collision radius for first object
     * @param radius2 - Collision radius for second object
     * @returns True if objects are colliding
     */
    public static checkCircularCollision(
        obj1: { x: number; y: number },
        obj2: { x: number; y: number },
        radius1: number,
        radius2: number
    ): boolean {
        const dx = obj1.x - obj2.x;
        const dy = obj1.y - obj2.y;
        const distance = Math.sqrt(dx * dx + dy * dy);
        
        return distance < (radius1 + radius2);
    }

    /**
     * Get collision radius for an asteroid based on its size
     * @param asteroid - The asteroid to get radius for
     * @returns Collision radius in pixels
     */
    public static getAsteroidCollisionRadius(asteroid: Asteroid): number {
        switch (asteroid.getSize()) {
            case 'large':
                return 32;
            case 'medium':
                return 20;
            case 'small':
                return 12;
            default:
                return 32;
        }
    }

    /**
     * Get collision radius for the ship
     * @returns Ship collision radius in pixels
     */
    public static getShipCollisionRadius(): number {
        return 12; // Ship is roughly 24x24, so radius is 12
    }

    /**
     * Get collision radius for a projectile
     * @returns Projectile collision radius in pixels
     */
    public static getProjectileCollisionRadius(): number {
        return 2; // Projectile is 4x4, so radius is 2
    }
}