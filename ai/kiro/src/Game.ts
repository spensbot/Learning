import * as Phaser from 'phaser';
import { GameScene } from './scenes';

export class Game {
  private game: Phaser.Game;

  constructor() {
    // Phaser game configuration
    const config: Phaser.Types.Core.GameConfig = {
      type: Phaser.AUTO, // Use WebGL if available, fallback to Canvas
      width: 800,
      height: 600,
      parent: 'game-container', // HTML element ID to attach the game
      backgroundColor: '#000000',
      
      // Physics configuration
      physics: {
        default: 'arcade',
        arcade: {
          gravity: { x: 0, y: 0 }, // No gravity in space
          debug: false // Set to true for debugging collision boxes
        }
      },

      // Scene configuration
      scene: [GameScene],

      // Scale configuration for responsive design
      scale: {
        mode: Phaser.Scale.FIT,
        autoCenter: Phaser.Scale.CENTER_BOTH,
        min: {
          width: 400,
          height: 300
        },
        max: {
          width: 1600,
          height: 1200
        }
      },

      // Input configuration
      input: {
        keyboard: true,
        mouse: false,
        touch: false
      },

      // Render configuration for smooth 60fps
      render: {
        antialias: true,
        pixelArt: false,
        roundPixels: false
      },

      // Audio configuration
      audio: {
        disableWebAudio: false
      }
    };

    // Initialize the Phaser game
    this.game = new Phaser.Game(config);

    // Handle window resize
    window.addEventListener('resize', () => {
      this.game.scale.refresh();
    });

    console.log('Asteroids Game initialized');
  }

  // Method to destroy the game instance
  public destroy(): void {
    if (this.game) {
      this.game.destroy(true);
    }
  }

  // Method to get the current scene
  public getCurrentScene(): GameScene | null {
    return this.game.scene.getScene('GameScene') as GameScene;
  }

  // Method to pause/resume the game
  public setPaused(paused: boolean): void {
    const scene = this.getCurrentScene();
    if (scene) {
      scene.getGameStateManager().setPaused(paused);
    }
  }
}