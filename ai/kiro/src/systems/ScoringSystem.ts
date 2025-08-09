import { GameStateManager } from '../managers/GameStateManager';
import { GameConfig } from '../config/GameConfig';
import { AsteroidSize } from '../objects/Asteroid';

// Forward declaration to avoid circular dependency
interface GameSceneInterface {
    respawnShip(): void;
}

/**
 * ScoringSystem manages all scoring logic, life management, and game over detection.
 * It handles point awards for asteroid destruction and life management for ship collisions.
 */
export class ScoringSystem {
    private gameStateManager: GameStateManager;
    private gameConfig: GameConfig;
    private gameScene?: GameSceneInterface;

    constructor(gameStateManager: GameStateManager, gameConfig: GameConfig) {
        this.gameStateManager = gameStateManager;
        this.gameConfig = gameConfig;
    }

    /**
     * Set the game scene reference for respawn functionality
     * @param gameScene - The game scene instance
     */
    public setGameScene(gameScene: GameSceneInterface): void {
        this.gameScene = gameScene;
    }

    /**
     * Award points for destroying an asteroid
     * @param asteroidSize - The size of the asteroid that was destroyed
     */
    public awardAsteroidPoints(asteroidSize: AsteroidSize): void {
        let points = 0;

        switch (asteroidSize) {
            case AsteroidSize.LARGE:
                points = this.gameConfig.scoring.asteroidPoints.large;
                break;
            case AsteroidSize.MEDIUM:
                points = this.gameConfig.scoring.asteroidPoints.medium;
                break;
            case AsteroidSize.SMALL:
                points = this.gameConfig.scoring.asteroidPoints.small;
                break;
            default:
                console.warn('Unknown asteroid size for scoring:', asteroidSize);
                return;
        }

        this.gameStateManager.updateScore(points);
        console.log(`Awarded ${points} points for destroying ${asteroidSize} asteroid. Total score: ${this.gameStateManager.getState().score}`);
        
        // UI will be updated automatically in the game loop
    }

    /**
     * Handle ship collision with asteroid - lose a life
     */
    public handleShipCollision(): void {
        const currentLives = this.gameStateManager.getState().lives;
        
        if (currentLives > 0) {
            this.gameStateManager.loseLife();
            const newLives = this.gameStateManager.getState().lives;
            
            console.log(`Ship destroyed! Lives remaining: ${newLives}`);
            
            // Check if game should end (GameStateManager handles setting isGameOver)
            if (newLives === 0) {
                this.handleGameOver();
            } else {
                // Respawn the ship if there are lives remaining
                this.respawnShip();
            }
        }
    }

    /**
     * Trigger ship respawn through the game scene
     */
    private respawnShip(): void {
        if (this.gameScene) {
            // Add a small delay before respawning to give player time to see the collision
            setTimeout(() => {
                if (this.gameScene) {
                    this.gameScene.respawnShip();
                }
            }, 500); // 500ms delay
        } else {
            console.warn('Cannot respawn ship: GameScene reference not set');
        }
    }

    /**
     * Handle game over state
     */
    private handleGameOver(): void {
        const finalScore = this.gameStateManager.getState().score;
        console.log(`Game Over! Final Score: ${finalScore}`);
        
        // GameStateManager already set isGameOver to true in loseLife()
        // UI will be updated automatically in the game loop
    }

    /**
     * Get the current score
     * @returns Current score value
     */
    public getCurrentScore(): number {
        return this.gameStateManager.getState().score;
    }

    /**
     * Get the current lives count
     * @returns Current lives remaining
     */
    public getCurrentLives(): number {
        return this.gameStateManager.getState().lives;
    }

    /**
     * Check if the game is over
     * @returns True if game is over (no lives remaining)
     */
    public isGameOver(): boolean {
        return this.gameStateManager.isGameOver();
    }

    /**
     * Reset the scoring system for a new game
     */
    public reset(): void {
        this.gameStateManager.reset();
        console.log('Scoring system reset for new game');
    }

    /**
     * Get scoring configuration for reference
     * @returns The scoring configuration object
     */
    public getScoringConfig() {
        return this.gameConfig.scoring;
    }
}