import { GameStateManager } from './GameStateManager';
import { GameState } from '../interfaces/GameState';

/**
 * UIManager handles all user interface elements including score display,
 * lives counter, game over screen, and other UI components.
 */
export class UIManager {
    private gameStateManager: GameStateManager;
    private scoreElement: HTMLElement | null = null;
    private livesElement: HTMLElement | null = null;
    private gameOverElement: HTMLElement | null = null;
    private uiOverlay: HTMLElement | null = null;

    constructor(gameStateManager: GameStateManager) {
        this.gameStateManager = gameStateManager;
        this.initializeUI();
    }

    /**
     * Initialize all UI elements
     */
    private initializeUI(): void {
        this.createUIOverlay();
        this.createScoreDisplay();
        this.createLivesDisplay();
        this.createGameOverScreen();
        
        // Initial update to show starting values
        this.updateDisplay();
    }

    /**
     * Create the main UI overlay container
     */
    private createUIOverlay(): void {
        // Check if overlay already exists
        this.uiOverlay = document.getElementById('ui-overlay');
        
        if (!this.uiOverlay) {
            this.uiOverlay = document.createElement('div');
            this.uiOverlay.id = 'ui-overlay';
            document.body.appendChild(this.uiOverlay);
        }
    }

    /**
     * Create the score display element
     */
    private createScoreDisplay(): void {
        this.scoreElement = document.getElementById('score');
        
        if (!this.scoreElement) {
            this.scoreElement = document.createElement('div');
            this.scoreElement.id = 'score';
            this.uiOverlay?.appendChild(this.scoreElement);
        }
    }

    /**
     * Create the lives display element
     */
    private createLivesDisplay(): void {
        this.livesElement = document.getElementById('lives');
        
        if (!this.livesElement) {
            this.livesElement = document.createElement('div');
            this.livesElement.id = 'lives';
            this.uiOverlay?.appendChild(this.livesElement);
        }
    }

    /**
     * Create the game over screen
     */
    private createGameOverScreen(): void {
        this.gameOverElement = document.querySelector('.game-over');
        
        if (!this.gameOverElement) {
            this.gameOverElement = document.createElement('div');
            this.gameOverElement.className = 'game-over hidden';
            
            // Create game over content
            const title = document.createElement('h2');
            title.textContent = 'GAME OVER';
            
            const scoreText = document.createElement('p');
            scoreText.id = 'final-score';
            
            const restartButton = document.createElement('button');
            restartButton.textContent = 'Press SPACE to Restart';
            restartButton.id = 'restart-button';
            
            this.gameOverElement.appendChild(title);
            this.gameOverElement.appendChild(scoreText);
            this.gameOverElement.appendChild(restartButton);
            
            document.body.appendChild(this.gameOverElement);
        }
    }

    /**
     * Update all UI displays with current game state
     */
    public updateDisplay(): void {
        const state = this.gameStateManager.getState();
        
        this.updateScore(state.score);
        this.updateLives(state.lives);
        
        if (state.isGameOver) {
            this.showGameOver(state.score);
        } else {
            this.hideGameOver();
        }
    }

    /**
     * Update the score display
     * @param score - Current score value
     */
    public updateScore(score: number): void {
        if (this.scoreElement) {
            this.scoreElement.textContent = `SCORE: ${score.toLocaleString()}`;
        }
    }

    /**
     * Update the lives display with visual indicators
     * @param lives - Current number of lives
     */
    public updateLives(lives: number): void {
        if (this.livesElement) {
            // Create visual representation with ship symbols
            const shipSymbol = '◄'; // Simple ship representation
            const livesDisplay = Array(lives).fill(shipSymbol).join(' ');
            this.livesElement.textContent = `LIVES: ${livesDisplay}`;
        }
    }

    /**
     * Show the game over screen with final score
     * @param finalScore - The player's final score
     */
    public showGameOver(finalScore: number): void {
        if (this.gameOverElement) {
            const finalScoreElement = this.gameOverElement.querySelector('#final-score');
            if (finalScoreElement) {
                finalScoreElement.textContent = `Final Score: ${finalScore.toLocaleString()}`;
            }
            
            this.gameOverElement.classList.remove('hidden');
        }
    }

    /**
     * Hide the game over screen
     */
    public hideGameOver(): void {
        if (this.gameOverElement) {
            this.gameOverElement.classList.add('hidden');
        }
    }

    /**
     * Show a wave notification (for future use)
     * @param waveNumber - The current wave number
     */
    public showWaveNotification(waveNumber: number): void {
        // Create temporary wave notification
        const waveNotification = document.createElement('div');
        waveNotification.style.cssText = `
            position: fixed;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            font-size: 2rem;
            color: #fff;
            text-shadow: 0 0 10px #fff;
            z-index: 150;
            pointer-events: none;
        `;
        waveNotification.textContent = `WAVE ${waveNumber}`;
        
        document.body.appendChild(waveNotification);
        
        // Remove after 2 seconds
        setTimeout(() => {
            document.body.removeChild(waveNotification);
        }, 2000);
    }

    /**
     * Update UI in real-time (called from game loop)
     */
    public update(): void {
        // Check if game state has changed and update display accordingly
        const currentState = this.gameStateManager.getState();
        
        // Always update score and lives in case they changed
        this.updateScore(currentState.score);
        this.updateLives(currentState.lives);
        
        // Handle game over state
        if (currentState.isGameOver && this.gameOverElement?.classList.contains('hidden')) {
            this.showGameOver(currentState.score);
        } else if (!currentState.isGameOver && !this.gameOverElement?.classList.contains('hidden')) {
            this.hideGameOver();
        }
    }

    /**
     * Reset UI for a new game
     */
    public reset(): void {
        this.hideGameOver();
        this.updateDisplay();
    }

    /**
     * Cleanup UI elements when destroying the game
     */
    public destroy(): void {
        // Remove event listeners and clean up DOM elements if needed
        if (this.gameOverElement && this.gameOverElement.parentNode) {
            this.gameOverElement.parentNode.removeChild(this.gameOverElement);
        }
        
        if (this.uiOverlay && this.uiOverlay.parentNode) {
            this.uiOverlay.parentNode.removeChild(this.uiOverlay);
        }
    }
}