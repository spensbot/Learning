export interface GameState {
  score: number;
  lives: number;
  wave: number;
  isGameOver: boolean;
  isPaused: boolean;
  asteroidCount: number;
}