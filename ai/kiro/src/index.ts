import './styles.css';
import { Game } from './Game';

// Main entry point for the Asteroids game
let game: Game;

console.log('Asteroids Game - TypeScript setup complete');

// Initialize the game once the DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    const loadingScreen = document.getElementById('loading-screen');
    
    // Initialize the Phaser game
    try {
        game = new Game();
        console.log('Game initialized successfully');
        
        // Hide loading screen after game is initialized
        if (loadingScreen) {
            setTimeout(() => {
                loadingScreen.style.display = 'none';
            }, 500);
        }
    } catch (error) {
        console.error('Failed to initialize game:', error);
        
        // Show error message if game fails to load
        if (loadingScreen) {
            loadingScreen.innerHTML = '<h2>Failed to load game</h2><p>Please refresh the page to try again.</p>';
        }
    }
});

// Handle page unload
window.addEventListener('beforeunload', () => {
    if (game) {
        game.destroy();
    }
});

// Handle visibility change (pause when tab is not active)
document.addEventListener('visibilitychange', () => {
    if (game) {
        game.setPaused(document.hidden);
    }
});